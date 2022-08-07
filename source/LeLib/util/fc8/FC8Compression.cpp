#include "FC8Compression.h"
#include "../util.h"

uint32_t FC8::GetUInt32(const uint8_t *in)
{
    return ((uint32_t)in[0]) << 24 |
                                ((uint32_t)in[1]) << 16 |
                                                     ((uint32_t)in[2]) << 8 |
                                                                          ((uint32_t)in[3]);
}

void FC8::SetUInt32(uint8_t *in, uint32_t val)
{
    in[0] = val >> 24;
    in[1] = val >> 16;
    in[2] = val >> 8;
    in[3] = val;
}

search_accel_t *FC8::SearchAccel_Create()
{
    search_accel_t *self;

    /* Allocate memory for the sarch tab object */
    self = (search_accel_t *)malloc(sizeof(search_accel_t));
    if (!self)
        return (search_accel_t*) 0;

    /* Backchain linked lists. Total size is one pointer for each entry in the
           sliding window. Each entry is a pointer to the previous instance of the same
           3-byte sequence that appears at that point in the sliding window. The end
           of this chain may point to older instances that are no longer within the
           sliding window, so the search function must check for this and terminate. */
    self->backchain = (unsigned char**)calloc(_FC8_WINDOW_SIZE, sizeof(unsigned char *));
    if (!self->backchain)
    {
        free(self);
        return (search_accel_t*) 0;
    }

    /* Most recent occurrence lookup table. For 3 byte keys, 256 ^ 3 = 16 meg
           of entries are required. Each entry is a pointer to the most recent
           occurence of that 3-byte key sequence in the input string, looking backwards
           from the current position. */
    self->mostRecent = (unsigned char**)calloc(16L*1024*1024, sizeof(unsigned char *));
    if (!self->mostRecent)
    {
        free(self->backchain);
        free(self);
        return (search_accel_t*) 0;
    }

    return self;
}

void FC8::SearchAccel_Destroy(search_accel_t *self)
{
    if (!self)
        return;

    free(self->mostRecent);
    free(self->backchain);
    free(self);
}

void FC8::UpdateLastPos(search_accel_t *sa, const uint8_t *first, uint8_t *pos)
{
    uint32_t key = (((uint32_t)pos[0]) << 16) | (((uint32_t)pos[1]) << 8) | ((uint32_t)pos[2]);

    sa->backchain[(pos - first) & (_FC8_WINDOW_SIZE-1)] = sa->mostRecent[key];
    sa->mostRecent[key] = pos;
}

uint32_t FC8::GetCompressedSizeForMatch(uint32_t dist, uint32_t length)
{
    // BR0 = 01baaaaa  offset aaaaa, length b+3
    // BR1 = 10bbbaaa'aaaaaaaa   offset aaa'aaaaaaaa, length bbb+3
    // BR2 = 11bbbbba'aaaaaaaa'aaaaaaaa   offset a'aaaaaaaa'aaaaaaaa, length LUT[bbbbb]

    // fits in B0?
    if (dist <= 31 && length <= 4)
    {
        return 1;
    }
    // fits in B1?
    else if (dist <= 0x7FF && length <= 10)
    {
        return 2;
    }
    // fits in B2?
    else if (dist <= 0x1FFFF && length <= 256)
    {
        return 3;
    }

    return 0xFFFFFFFF;
}

uint32_t FC8::FindMatch(search_accel_t *sa, const uint8_t *inputStart, const uint8_t *inputEnd, const uint8_t *curPos, uint8_t symbolCost, uint32_t *matchOffset)
{
    uint32_t matchLength, bestLength = 2, dist, preMatch, maxMatches, win, bestWin = 0;
    uint8_t *prevPos, *curPtr, *prevPtr, *minPos, *endStr;

    *matchOffset = 0;

    /* Minimum search position */
    if ((uint32_t)(curPos - inputStart) >= _FC8_WINDOW_SIZE)
        minPos = (uint8_t*)(curPos - _FC8_WINDOW_SIZE);
    else
        minPos = (uint8_t*)inputStart;

    /* Search string end */
    endStr = (uint8_t*)(curPos + _FC8_MAX_MATCH_LENGTH);
    if (endStr > inputEnd)
        endStr = (uint8_t*)inputEnd;

    /* Previous search position */
    prevPos = sa->backchain[(curPos - inputStart) & (_FC8_WINDOW_SIZE - 1)];

    /* Pre-matched by the acceleration structure */
    preMatch = 3;

    /* Main search loop */
    maxMatches = _FC8_MAX_MATCHES;
    while (prevPos && (prevPos > minPos) && (maxMatches--))
    {
        /* If we don't have a match at bestLength, don't even bother... */
        if (curPos[bestLength] == prevPos[bestLength])
        {
            /* Calculate maximum match length for this offset */
            curPtr = (uint8_t*)curPos + preMatch;
            prevPtr = prevPos + preMatch;
            while (curPtr < endStr && *curPtr == *prevPtr)
            {
                ++curPtr;
                ++prevPtr;
            }
            matchLength = curPtr - curPos;

            /* Quantize length */
            matchLength = _FC8_LENGTH_QUANT_LUT[matchLength];

            dist = (uint32_t)(curPos - prevPos);

            /* Get actual compression win for this match */
            win = matchLength + symbolCost - 1 - GetCompressedSizeForMatch(dist, matchLength);

            /* Best win so far? */
            if (win > bestWin)
            {
                bestWin = win;
                *matchOffset = dist;
                bestLength = matchLength;

                /* Did we find a match that was good enough, or did we reach
                        the end of the buffer (no longer match is possible)? */
                if ((matchLength >= _FC8_MAX_MATCH_LENGTH) || (curPtr >= endStr))
                    break;
            }
        }

        /* Previous search position */
        prevPos = sa->backchain[(prevPos - inputStart) & (_FC8_WINDOW_SIZE - 1)];
    }

    /* Did we get a match that would actually compress? */
    if (bestWin > 0)
        return bestLength;
    else
        return 0;
}

uint32_t FC8::Encode(const uint8_t *in, uint32_t insize, uint8_t *out, uint32_t outsize)
{
    uint8_t *src, *inEnd, *dst, *outEnd, symbol;
    uint32_t compressedSize, backrefSize;
    uint32_t length, offset = 0, symbolCost = 0, i;
    uint8_t* pRunLengthByte = NULL;
    uint32_t literalRunLength = 0;
    search_accel_t *sa = (search_accel_t*) 0;

    /* Check arguments */
    if ((!in) || (!out) || (outsize < (FC8_HEADER_SIZE + insize)))
        goto fail;

    /* Initialize search accelerator */
    sa = SearchAccel_Create();
    if (!sa)
        goto fail;

    /* Initialize the byte streams */
    src = (uint8_t *)in;
    inEnd = ((uint8_t *)in) + insize;
    dst = out + FC8_HEADER_SIZE;
    outEnd = out + outsize;

    /* Main compression loop */
    while (src < inEnd)
    {
        /* Get current symbol (don't increment, yet) */
        symbol = *src;

        // are there at least three bytes remaining?
        if (inEnd - src >= 3)
        {
            /* What's the cost for this symbol if we do not compress */
            symbolCost = literalRunLength == 0 ? 2 : 1;

            /* Update search accelerator */
            UpdateLastPos(sa, in, src);
        }
        else
        {
            length = 0;
        }

        /* Find best history match for this position in the input buffer */
        length = FindMatch(sa, in, inEnd, src, symbolCost, &offset);

        if (length > 0)
        {
            if (literalRunLength)
            {
                // terminate the previous literal run, if any
                *pRunLengthByte = literalRunLength-1;
                literalRunLength = 0;
            }

            // find the compressed size of this (offset,length) backref in the new compression scheme
            backrefSize = GetCompressedSizeForMatch(offset, length);
            if (backrefSize > 3)
                goto fail;

            // LIT = 00aaaaaa  next aaaaaa+1 bytes are literals
            // BR0 = 01baaaaa  offset aaaaa, length b+3
            // BR1 = 10bbbaaa'aaaaaaaa   offset aaa'aaaaaaaa, length bbb+3
            // BR2 = 11bbbbba'aaaaaaaa'aaaaaaaa   offset a'aaaaaaaa'aaaaaaaa, length lookup_table[bbbbb]
            // EOF = 01x00000   end of file
            if (backrefSize == 1)
            {
                *dst++ = (uint8_t)(0x40 | offset | ((length-3)<<5));
            }
            else if (backrefSize == 2)
            {
                *dst++ = (uint8_t)(0x80 | ((length-3)<<3) | (offset >> 8));
                *dst++ = (uint8_t)(offset);
            }
            else if (backrefSize == 3)
            {
                *dst++ = (uint8_t)(0xC0 | (_FC8_LENGTH_ENCODE_LUT[length]<<1) | (offset >> 16));
                *dst++ = (uint8_t)(offset >> 8);
                *dst++ = (uint8_t)(offset);
            }

            /* Skip ahead (and update search accelerator)... */
            for (i = 1; i < length; ++i)
                UpdateLastPos(sa, in, src + i);
            src += length;
        }
        else
        {
            // literal
            if (literalRunLength == 0)
            {
                pRunLengthByte = dst;
                *dst++; // skip a byte for the run length
            }

            // output the literal
            if (dst >= outEnd) goto overflow;
            *dst++ = symbol;
            src++;
            literalRunLength++;

            // terminate the run if literal run length has reached max
            if (literalRunLength == _FC8_LONGEST_LITERAL_RUN)
            {
                *pRunLengthByte = literalRunLength-1;
                literalRunLength = 0;
            }
        }
    }

    if (literalRunLength)
    {
        // terminate the final literal run, if any
        *pRunLengthByte = literalRunLength-1;
    }

    // insert EOF
    *dst++ = 0x40;

    /* Free resources */
    SearchAccel_Destroy(sa);

    compressedSize = dst - out;

    /* Set header data */
    out[0] = 'F';
    out[1] = 'C';
    out[2] = '8';
    out[3] = '_';

    SetUInt32(out + FC8_DECODED_SIZE_OFFSET, insize);
    /* Return size of compressed buffer */
    return compressedSize;


overflow:
    /* Free resources */
    SearchAccel_Destroy(sa);

    /* Return size of compressed buffer */
    return 0;


fail:
    /* Exit routine for failure situations */
    if (sa)
        SearchAccel_Destroy(sa);
    return 0;
}

unsigned int FC8::Encode(QString fileIn, QString fileOut)
{
    QByteArray in = Util::loadBinaryFile(fileIn);
    if (QFile::exists(fileOut))
        QFile::remove(fileOut);

    int max = 1024*1024;

    uint8_t* data = new uint8_t[max];
    uint8_t* dataIn = new uint8_t[in.length()];

    for (int i=0;i<in.length();i++)
        dataIn[i] = in[i];

    int size = Encode(dataIn,in.length(),data,max);
    QByteArray out;
    for (int i=0;i<size;i++)
        out.append(data[i]);
    Util::SaveByteArray(out,fileOut);
//    qDebug() << "Compression ratio = " <<QString::number((1.0-size/(float)in.count())*100) +"% with size "+QString::number(size);
    delete[] data;
    delete[] dataIn;
    return size;
}

