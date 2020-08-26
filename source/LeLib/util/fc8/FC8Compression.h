/*
* FC8 compression by Steve Chamberlin, 2016
* Some concepts and code derived from liblzg by Marcus Geelnard, 2010
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*
* Modified to fit TRSE by Leuat
*/


#ifndef FC8_H
#define FC8_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <QString>
#include <QFile>
#include <QByteArray>

#define _FC8_MAX_MATCH_LENGTH 256
#define _FC8_WINDOW_SIZE (128L*1024)
#define _FC8_MAX_MATCHES (128L*1024)
#define _FC8_LONGEST_LITERAL_RUN 64




#define FC8_HEADER_SIZE 8
#define FC8_DECODED_SIZE_OFFSET 4

// for FC8b block format header
#define FC8_BLOCK_HEADER_SIZE 12
#define FC8_BLOCK_SIZE_OFFSET 8

typedef struct {
    uint8_t **backchain;
    uint8_t **mostRecent;
} search_accel_t;

/* LUT for encoding the copy length parameter */
const uint8_t _FC8_LENGTH_ENCODE_LUT[257] = {
    255,255,255,0,1,2,3,4,5,6,7,8,9,10,11,12,         /* 0 - 15 */
    13,14,15,16,17,18,19,20,21,22,23,24,25,26,26,26, /* 16 - 31 */
    26,26,26,27,27,27,27,27,27,27,27,27,27,27,27,27, /* 32 - 47 */
    28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28, /* 48 - 63 */
    28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29, /* 64 - 79 */
    29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29, /* 80 - 95 */
    29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29, /* 96 - 111 */
    29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29, /* 112 - 127 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 128 - 143 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 144 - 159 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 160 - 175 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 176 - 191 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 192 - 207 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 208 - 223 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 224 - 239 */
    30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30, /* 240 - 255 */
    31                                               /* 256 */
};

/* LUT for decoding the copy length parameter */
const uint16_t _FC8_LENGTH_DECODE_LUT[32] = {
    3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,35,48,72,128,256
};

/* LUT for quantizing the match length parameter */
const uint16_t _FC8_LENGTH_QUANT_LUT[257] = {
    0,0,0,3,4,5,6,7,8,9,10,11,12,13,14,15,           /* 0 - 15 */
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,29,29, /* 16 - 31 */
    29,29,29,35,35,35,35,35,35,35,35,35,35,35,35,35, /* 32 - 47 */
    48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48, /* 48 - 63 */
    48,48,48,48,48,48,48,48,72,72,72,72,72,72,72,72, /* 64 - 79 */
    72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, /* 80 - 95 */
    72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, /* 96 - 111 */
    72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72, /* 112 - 127 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 128 - 143 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 144 - 159 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 160 - 175 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 176 - 191 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 192 - 207 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 208 - 223 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 224 - 239 */
    128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128, /* 240 - 255 */
    256                                                              /* 256 */
};



class FC8 {
private:
    uint32_t GetUInt32(const uint8_t *in);

    void SetUInt32(uint8_t *in, uint32_t val);

    search_accel_t* SearchAccel_Create();

    void SearchAccel_Destroy(search_accel_t *self);

    void UpdateLastPos(search_accel_t *sa, const uint8_t *first, uint8_t *pos);

    uint32_t GetCompressedSizeForMatch(uint32_t dist, uint32_t length);

    uint32_t FindMatch(search_accel_t *sa, const uint8_t *inputStart, const uint8_t *inputEnd, const uint8_t *curPos, uint8_t symbolCost, uint32_t *matchOffset);

    uint32_t Encode(const uint8_t *in, uint32_t insize, uint8_t *out, uint32_t outsize);

public:

    unsigned int Encode(QString fileIn, QString fileOut);


};


#endif

