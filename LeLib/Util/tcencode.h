#ifndef TCENCODE_H
#define TCENCODE_H

#include <QString>
#include <QStringList>
#include <QVector>

class OptToken {
    int cost, length,offset;
    QVector<uchar> data;
//    OptToken* next = nullptr;



//    OptToken(int pcost, QVector<uchar> pdata, OptToken* pnext, int plength, int poffset):cost(pcost), data(pdata), next(pnext), length(plength), offset(poffset) {
  //  }
/*
    void repr() {
        QString dr = "";
        if (data.count()<5) {
            int last = data.length()-1;
            int cnt = 0;
            for (auto& s:data) {
                dr+=QString::number(s,16);
                if (cnt!=data.length()-1)
                    dr+=" ";
            }
        }
        else
        {

        }
    }*/
/*
        def __repr__(self):
            if len(self.data) < 5:
                dr = " ".join(['{:02x}'.format(j) for j in self.data])
            else:
                dr = "{} bytes".format(len(self.data))
            return "OptToken({},[{}],{},{},{})".format(self.cost, dr, self.next, self.length, self.offset)
*/
};

class CDataChunk {
    int addr = 0;
    QByteArray data;
    CDataChunk* se = nullptr;
    CDataChunk(int paddr, QByteArray pdata, CDataChunk* pse=nullptr) {
            addr=paddr;
            data=pdata;
            se=pse;
    }

    CDataChunk* ending_at(int ea, QByteArray data, CDataChunk* se=nullptr) {
            return new CDataChunk(ea-data.length(), data, se);
    }
    int end_addr() {
        return addr + data.size();
    }

/*
        def ext(self):
            return "0x{:04x}-0x{:04x}".format(self.addr,self.end_addr())

        def split_at(self,addr):
            assert(self.addr<=addr)
            assert(addr<=self.end_addr())
            sp=addr-self.addr
            lower=CDataChunk(self.addr, self.data[:sp])
            upper=CDataChunk(addr, self.data[sp:])
            return lower,upper

        def extend(self, addend):
            assert(self.end_addr()==addend.addr)
            self.data.extend(addend.data)
            */

};


class CCRuncher {
    QByteArray output_bytes, input_chunk;
    QByteArray remainder = 0;
    QByteArray data;

    void crunch(QByteArray d) {
        output_bytes = QByteArray();
        output_bytes.resize(3);
        output_bytes.fill(0);
        //[0, 0, 0]  # leave space for target address, and final byte

        input_chunk = d;

        data.append(input_chunk[input_chunk.size()-1]);
//        self.data = input_chunk.data[:-1]
        remainder.append(input_chunk[input_chunk.size()-1]);
//        self.remainder = input_chunk.data[-1:]
//        if (data.size()==0)
  //          return output_token_list(QVector<OptToken>());

//        if self.greedy:
  //          return self.crunch_greedy()
    //    else:
    //      return crunch_optimal();
    }/*
    void output_token_list(QVector<OptToken> tokens){
        j = tokens.size();
        int watermark = j;
        if (inPlace) {
            // Scan the token list from the end back.
            // Whenever compressed remainder is equal or longer in length to raw remainder,
            // set that token to start of raw data.
            int raw_bytes_after_token_j = 0;
            int comp_bytes_after_token_j = 0;
            int raw_bytes_after_watermark = 0;
            while (j > 0) {
                j -= 1;
                raw_bytes_after_token_j += tokens[j].size();
                comp_bytes_after_token_j += tokens[j].data.length();
                if (raw_bytes_after_token_j <= comp_bytes_after_token_j) {
                    watermark = j;
                    raw_bytes_after_watermark += raw_bytes_after_token_j;
                    raw_bytes_after_token_j = 0;
                    comp_bytes_after_token_j = 0;
                }
            }
   //     for t in tokens[:watermark]:
     //       self.stats.log_token(t)
     //       output_bytes.append(itertools.chain.from_iterable(x.data for x in tokens[:watermark]))
        for (int i=0;i<watermark;i++) {
            output_bytes.append(tokens[i].data);
        }
        if ( (raw_bytes_after_watermark > 0)) {
            remainder = data[-raw_bytes_after_watermark:] + remainder;
        }
//        if (remainder.size()>1)
  //          self.stats.log_raw(len(self.remainder)-1)

        output_bytes[0] = remainder[0];
        output_bytes[1] = (input_chunk.addr - 1) % 256;
        output_bytes[2] = (input_chunk.addr - 1); // 256
        //self.stats.log_terminator()
        remainder[0] = 0;  # terminator for compressed data
        output_bytes.append(remainder);
        remainder.clear();
//        self.remainder = None
        return output_bytes;


    }
    */
};


class TCEncode
{
public:
    TCEncode();
    bool inPlace = true;


    void Encode(QString in, QString out);
    void crunch_optimal() {
/*
        longestCopy    = self.longestCopy
        maxPairOffset  = self.maxPairOffset
        maxOffset      = self.maxOffset
        longestLiteral = self.longestLiteral
        data = self.data

        l = len(data)
        last_seen = defaultdict(lambda: -(2 ** 17))  # address key was last seen starting at

        # cfile[j] contains the tail of a list that in turn contains the
        # cheapest representation of the first j bytes of the file
        # data containst the bytes that must be added to the stream to
        # cover the bytes between that token and its predecessor
        cfile = [self.OptToken(0, None, None, 0, 0)]
        best = None

        for j in range(1, l + 1):
            copy_candidates = [tuple(data[j - i:j]) for i in range(2, longestCopy + 1) if
                               j - i >= 0]  # find all the tuples that end at this point
            best = self.OptToken(inf, None, None, 0, 0)

            for k in copy_candidates:
                mra = last_seen[k]
                length = len(k)
                start_addr = j - length
                assert (length > 1)
                offset = j - mra
                if offset < maxOffset:
                    nb = [1.01, 2.012][length > 2 or offset > maxPairOffset]
                    cost = cfile[start_addr].cost + nb
                    if cost < best.cost:
                        token = self.encode_copy(length, offset)
                        if token[0] != 0:
                            best = self.OptToken(cost, token, start_addr, length=length, offset=offset)
                            assert ((mra - length) < j)

            for length in range(1, longestLiteral + 1):
                start_addr = j - length
                if start_addr >= 0:
                    cost = cfile[start_addr].cost + length + 1.01
                    if cost < best.cost:
                        literal = data[start_addr:j]
                        best = self.OptToken(cost, self.encode_literal(literal), start_addr, length=len(literal),
                                             offset=0)
                        assert (len(best.data) == length + 1)
                        assert (start_addr < j)

            cfile.append(best)
            for k in copy_candidates:
                last_seen[k] = j

        assert best is not None
        tokens = [best]
        while best.next != 0:
            best = cfile[best.next]
            tokens.append(best)
        tokens.reverse()
        return self.output_token_list(tokens)
    }
*/
    }
/*
    void level_crunch(op, QByteArray pinput_chunk):
        output_bytes = op.crunch(input_chunk, args.inPlace)

        if args.inPlace:
            la = input_chunk.end_addr() - len(output_bytes)
        elif args.endAddr is not None:
            la = args.endAddr - len(output_bytes)
        else:
            la = args.startAddr

        output_chunk=CDataChunk(la, output_bytes)

        if args.paramFile:
            print("dcSrc=$%04x" % (output_chunk.addr,), file=args.paramFile)
            args.paramFile.close()

        return output_chunk
*/

};

#endif // TCENCODE_H
