#ifndef DALIZX0_H
#define DALIZX0_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class DaliZX0
{
public:
    DaliZX0();

#define BUFFER_SIZE 65536  /* must be > MAX_OFFSET */
#define INITIAL_OFFSET 1

#define FALSE 0
#define TRUE 1
#define DALI_BITS_LEFT 0
#define DALI_ELIAS_LE 1

typedef struct ctx {
    unsigned char *packed_data;
    unsigned char *reencoded_data;
    unsigned char *unpacked_data;
    size_t packed_index;
    size_t packed_size;
    int packed_bit_mask;
    int packed_bit_value;
    size_t reencoded_index;
    int reencoded_bit_mask;
    int reencoded_bit_value;
    int reencoded_bit_index;
    size_t unpacked_index;
    size_t unpacked_size;
    int inplace;

    char *output_name;
    char *input_name;
    char *prefix_name;
    char *clamped_name;

    int cbm;
    int cbm_orig_addr;
    int cbm_packed_addr;
    int cbm_range_from;
    int cbm_range_to;
    int cbm_relocate_packed_addr;
    int cbm_relocate_origin_addr;
    int cbm_prefix_from;

    int sfx;
    int sfx_addr;
    int sfx_01;
    int sfx_cli;
    int sfx_small;
    int sfx_size;
    char *sfx_code;
    int lz_bits;
} ctx;


static int read_number(char* arg, char* argname, int limit) {
    int number;
    if (arg != NULL && arg[0] == '$') number = strtoul(arg + 1, NULL, 16);
    else if (arg != NULL && arg[0] == '0' && arg[1] == 'x') number = strtoul(arg + 2, NULL, 16);
    else if (arg != NULL && arg[0] >= '0' && arg[0] <= '9') number = strtoul(arg, NULL, 10);
    else {
        fprintf(stderr, "Error: no valid number given for argument %s (given value is: '%s')\n", argname, arg);
        exit(1);
    }
    if (number < 0 || number > limit) {
        fprintf(stderr, "Error: Number '%s' out of range (0 - 65536)\n", arg);
        exit(1);
    }
    return number;
}

static void file_write_byte(int byte, FILE *ofp) {
    if (fputc(byte, ofp) != byte) {
        fprintf(stderr, "Error: Cannot write output file\n");
        perror("fputc");
        exit(1);
    }
    return;
}

static inline unsigned bit_size(unsigned value) {
#   ifdef __GNUC__
//    enum { WORD_BITS = sizeof(unsigned) * CHAR_BIT };

    return ((sizeof(unsigned) * 8 - 1) ^ __builtin_clz(value));
#   else
    signed bits = -1;

    do
        ++bits;
    while(value >>= 1);

    return bits;
#   endif
}

void write_reencoded_byte(ctx* ctx, int value) {
    ctx->reencoded_data[ctx->reencoded_index++] = value;
}

void write_reencoded_bit(ctx* ctx, int value) {
    if (!(ctx->reencoded_bit_mask & 255)) {
        if (DALI_BITS_LEFT == 1) {
            ctx->reencoded_bit_mask = 0x80;
        } else {
            ctx->reencoded_bit_mask = 0x1;
        }
        /* remember position of bit-buffer */
        ctx->reencoded_bit_index = ctx->reencoded_index;
        ctx->lz_bits++;
        write_reencoded_byte(ctx, 0);
    }
    if (value)
        ctx->reencoded_data[ctx->reencoded_bit_index] |= ctx->reencoded_bit_mask;
    if (DALI_BITS_LEFT == 1) {
        ctx->reencoded_bit_mask >>= 1;
    } else {
        ctx->reencoded_bit_mask <<= 1;
    }
}

void write_reencoded_interlaced_elias_gamma(ctx* ctx, int value, int skip) {
    int bits = bit_size(value);
    int i;

    for (i = 2; i <= value; i <<= 1);
    i >>= 1;

    if (DALI_ELIAS_LE) {
        if (bits >= 8) {
            /* change bit-order, send LSB first */
            /* remove preceeding 1 first */
            value = value & ((0xffff ^ i));
            /* move LSB bits to the beginning */
            value = (value >> 8) | ((value & 0xff) << (bits - 8));
        }
    }

    while ((i >>= 1) > 0) {
        if (!skip) write_reencoded_bit(ctx, 0);
        skip = 0;
        write_reencoded_bit(ctx, ((value & i) > 0));
    }
    if (!skip) write_reencoded_bit(ctx, 1);
}

int read_byte(ctx* ctx) {
    return ctx->packed_data[ctx->packed_index++];
}

int read_bit(ctx* ctx) {
    if ((ctx->packed_bit_mask >>= 1) == 0) {
        ctx->packed_bit_mask = 0x80;
        ctx->packed_bit_value = read_byte(ctx);
    }
    return (ctx->packed_bit_value & ctx->packed_bit_mask) != 0;
}

int read_interlaced_elias_gamma(ctx* ctx, int inverted, int skip) {
    int value = 1;
    /* skip first read bit if skip != 0 */
    while (skip || !read_bit(ctx)) {
        skip = 0;
        value = (value << 1) | (read_bit(ctx) ^ inverted);
    }
    return value;
}

void save_reencoded_stream(ctx* ctx) {
}

void copy_inplace_literal(ctx* ctx) {
    int i;
    for (i = ctx->unpacked_index; i < ctx->unpacked_size; i++) {
        ctx->reencoded_data[ctx->packed_index] = ctx->unpacked_data[i];
        ctx->packed_index++;
    }
}

void encode_literal(ctx* ctx, int length, int first) {
    int i;
    if (!first) write_reencoded_bit(ctx, 0);
    write_reencoded_interlaced_elias_gamma(ctx, length, 0);
    for (i = 0; i < length; i++) {
        write_reencoded_byte(ctx, ctx->unpacked_data[ctx->unpacked_index + i]);
    }
}

void encode_rep(ctx* ctx, int length) {
    write_reencoded_bit(ctx, 0);
    write_reencoded_interlaced_elias_gamma(ctx, length, 0);
}

void encode_match(ctx* ctx, int length, int offset) {
    write_reencoded_bit(ctx, 1);
    write_reencoded_interlaced_elias_gamma(ctx, ((offset - 1) >> 7) + 1, 0);
    write_reencoded_byte(ctx, (((offset - 1) & 0x7f) << 1) | (length == 2));
    write_reencoded_interlaced_elias_gamma(ctx, length - 1, 1);
}

void reencode_packed_stream(ctx* ctx) {
    int last_offset = INITIAL_OFFSET;
    int length;
    int overwrite;

    int bit, byte;
    int i;

    int safe_input_index = 0;
    int safe_output_index = 0;

    int first = 1;

    ctx->packed_index = 0;
    ctx->packed_bit_mask = 0;

    ctx->reencoded_index = 0;
    ctx->reencoded_bit_mask = 0;
    ctx->reencoded_bit_value = 0;
    ctx->reencoded_bit_index = 0;

    ctx->packed_index = 0;
    ctx->unpacked_index = 0;

    ctx->packed_bit_mask = 0;
    ctx->lz_bits = 0;

    while (1) {
        /* literal */
        length = read_interlaced_elias_gamma(ctx, FALSE, 0);
        for (i = 0; i < length; i++) read_byte(ctx);
        encode_literal(ctx, length, first);
        first = 0;

        ctx->unpacked_index += length;

        overwrite = (ctx->unpacked_index) - (ctx->unpacked_size - ctx->packed_size + ctx->packed_index);
        /* literal would overwrite packed src */
        if (ctx->inplace && overwrite >= 0) {
            /* go back to previous index */
            ctx->unpacked_index = safe_input_index;
            ctx->packed_index = safe_output_index;
            copy_inplace_literal(ctx);
            return;
        }
        /* do remember last safe position */
        safe_input_index = ctx->unpacked_index;
        safe_output_index = ctx->packed_index;

        /* copy from new or last offset? */
        bit = read_bit(ctx);
        if (!bit) {
            /* copy from last_offset */
            length = read_interlaced_elias_gamma(ctx, FALSE, 0);
            encode_rep(ctx, length);

            ctx->unpacked_index += length;

            overwrite = (ctx->unpacked_index) - (ctx->unpacked_size - ctx->packed_size + ctx->packed_index);
            /* rep would overwrite packed src */
            if (ctx->inplace && overwrite >= 0) {
                copy_inplace_literal(ctx);
                return;
            }
            safe_input_index = ctx->unpacked_index;
            safe_output_index = ctx->packed_index;

            bit = read_bit(ctx);
        }

        while (bit) {
            /* copy from new_offset */
            last_offset = read_interlaced_elias_gamma(ctx, TRUE, 0);
            if (last_offset == 256) {
                if (!ctx->inplace) {
                    write_reencoded_bit(ctx, 1);
                    write_reencoded_interlaced_elias_gamma(ctx, last_offset, 0);
                }
                return;
            }
            byte = read_byte(ctx);
            if (byte & 1) length = 2;
            else length = read_interlaced_elias_gamma(ctx, FALSE, 1) + 1;
            last_offset = (last_offset << 7) - (byte >> 1);
            encode_match(ctx, length, last_offset);

            ctx->unpacked_index += length;

            overwrite = (ctx->unpacked_index) - (ctx->unpacked_size - ctx->packed_size + ctx->packed_index);
            /* rep would overwrite packed src */
            if (ctx->inplace && overwrite >= 0) {
                copy_inplace_literal(ctx);
                return;
            }
            safe_input_index = ctx->unpacked_index;
            safe_output_index = ctx->packed_index;

            bit = read_bit(ctx);
        }
    }
}

void write_reencoded_stream(ctx* ctx) {
    FILE *fp = NULL;
    /* write reencoded output file */
    fp = fopen(ctx->output_name, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot create reencoded file (%s)\n", ctx->output_name);
        exit(1);
    }

        if (ctx->cbm_relocate_origin_addr >= 0) {
            ctx->cbm_orig_addr = ctx->cbm_relocate_origin_addr;
            ctx->cbm = TRUE;
        }

        if (ctx->inplace) {
            ctx->cbm_packed_addr = ctx->cbm_range_to - ctx->packed_index - 2;
        } else {
            if (ctx->cbm_relocate_packed_addr >= 0) {
                ctx->cbm_packed_addr = ctx->cbm_relocate_packed_addr;
            } else {
                ctx->cbm_packed_addr = ctx->cbm_orig_addr;
            }
        }


        if (ctx->cbm) {
            printf("original: $%04x-$%04lx ($%04lx) 100%%\n", ctx->cbm_orig_addr, ctx->cbm_orig_addr + ctx->unpacked_size, ctx->unpacked_size);
            printf("packed:   $%04x-$%04lx ($%04lx) %3.2f%%\n", ctx->cbm_packed_addr, ctx->cbm_packed_addr + ctx->packed_index + 2, ctx->packed_index + 2, ((float)(ctx->packed_index) / (float)(ctx->unpacked_size) * 100.0));
            if ((ctx->cbm_packed_addr >= 0xd000 && ctx->cbm_packed_addr < 0xe000) || (ctx->cbm_packed_addr < 0xd000 && ctx->cbm_packed_addr + ctx->packed_index + 2 > 0xd000)) {
                fprintf(stderr, "Warning: Packed file lies in I/O-range from $d000-$dfff\n");
            }

            /* little endian */
            file_write_byte(ctx->cbm_packed_addr & 255, fp);
            file_write_byte((ctx->cbm_packed_addr >> 8) & 255, fp);

            /* big endian, as read backwards by depacker */
            file_write_byte((ctx->cbm_orig_addr >> 8) & 255, fp);
            file_write_byte(ctx->cbm_orig_addr & 255, fp);
        } else {
            printf("original: $%04x-$%04lx ($%04lx) 100%%\n", 0, ctx->unpacked_size, ctx->unpacked_size);
            printf("packed:   $%04x-$%04lx ($%04lx) %3.2f%%\n", 0, ctx->packed_index, ctx->packed_index, ((float)(ctx->packed_index) / (float)(ctx->unpacked_size) * 100.0));
        }


    if (fwrite(ctx->reencoded_data, sizeof(char), ctx->packed_index, fp) != ctx->packed_index) {
        fprintf(stderr, "Error: Cannot write output file\n");
        exit(1);
    }
    fclose(fp);
}

void do_reencode(ctx* ctx) {
    char tmp_name[] = "dict-XXXXXX";
    char src_name[] = "src-XXXXXX";
    unsigned char *dict_data = NULL;
    int dict_size = 0;
    char *salvador_argv[5];
    int salvador_argc = 0;
    int dict_temp = FALSE;
    FILE *dfp = NULL;
    FILE *sfp = NULL;
    FILE* ufp = NULL;
    FILE* pfp = NULL;


    /* determine output filename */
    if (ctx->output_name == NULL) {
        ctx->output_name = (char *)malloc(strlen(ctx->input_name) + 4);
        strcpy(ctx->output_name, ctx->input_name);
        strcat(ctx->output_name, ".lz");
        printf("output name: %s\n", ctx->output_name);
    }

    /* allocate buffers */
    ctx->packed_data = (unsigned char *)malloc(BUFFER_SIZE);
    ctx->unpacked_data = (unsigned char *)malloc(BUFFER_SIZE + 2);
    ctx->reencoded_data = (unsigned char *)malloc(BUFFER_SIZE);

    if (!ctx->packed_data || !ctx->unpacked_data || !ctx->reencoded_data) {
        fprintf(stderr, "Error: Insufficient memory\n");
        exit(1);
    }

    /* load unpacked file */
    ufp = fopen(ctx->input_name, "rb");
    if (!ufp) {
        fprintf(stderr, "Error: Cannot access input file\n");
        exit(1);
    }
    ctx->unpacked_size = fread(ctx->unpacked_data, sizeof(char), BUFFER_SIZE + 2, ufp);
    fclose(ufp);

    /* cbm address handling */
    if (ctx->cbm_relocate_origin_addr >= 0) {
        ctx->cbm_orig_addr = ctx->cbm_relocate_origin_addr;
    } else {
        ctx->cbm_orig_addr = ctx->unpacked_data[0] + (ctx->unpacked_data[1] << 8);
    }

    if (ctx->cbm) {
      ctx->unpacked_data += 2;
      ctx->unpacked_size -= 2;
    }

    /* take care of range (--from --to) */
    if (ctx->cbm_range_from < 0) ctx->cbm_range_from = ctx->cbm_orig_addr;
    if (ctx->cbm_range_to < 0) ctx->cbm_range_to = ctx->cbm_orig_addr + ctx->unpacked_size;

    if ((ctx->cbm_range_to - ctx->cbm_orig_addr) > ctx->unpacked_size) {
        ctx->cbm_range_to = ctx->unpacked_size + ctx->cbm_orig_addr;
        fprintf(stderr, "Warning: File ends at $%04x, adopting --to value\n", ctx->cbm_range_to);
    }
    ctx->unpacked_size = (ctx->cbm_range_to - ctx->cbm_orig_addr);

    /* if range is below start_address, adopt range */
    if (ctx->cbm_range_from < ctx->cbm_orig_addr) {
        ctx->cbm_range_from = ctx->cbm_orig_addr;
        fprintf(stderr, "Warning: File starts at $%04x, adopting --from value\n", ctx->cbm_range_from);
    }
    if (ctx->cbm_range_from > ctx->cbm_range_to) {
        fprintf(stderr, "Error: --from beyond fileend ($%04x - $%04x)\n", ctx->cbm_range_from, ctx->cbm_range_to);
        exit(1);
    }

    /* setup dict lengths and position */
    if (ctx->cbm_prefix_from >= 0) {
        if (ctx->cbm_range_from < 0) {
            fprintf(stderr, "Error: Dict is zero size (use --from)\n");
            exit(1);
        }
        else if (ctx->cbm_prefix_from >= ctx->cbm_range_from) {
            fprintf(stderr, "Error: --from must be greater than --prefix-from\n");
            exit(1);
        }
        if (ctx->cbm_range_from >= 0 && ctx->cbm_range_from - ctx->cbm_prefix_from > 32640) {
            //ctx->cbm_prefix_from = ctx->cbm_range_from - 32640;
            fprintf(stderr, "Info: --prefix-from  exceeds max offset, not all bytes can be used\n");
        }
        /* if range is below start_address, adopt range */
        if (ctx->cbm_prefix_from < ctx->cbm_orig_addr) {
            ctx->cbm_prefix_from = ctx->cbm_orig_addr;
            fprintf(stderr, "Warning: File starts at $%04x, adopting --prefix-from value\n", ctx->cbm_prefix_from);
        }
        dict_data = ctx->unpacked_data + ctx->cbm_prefix_from - ctx->cbm_orig_addr;
        dict_size = ctx->cbm_range_from - ctx->cbm_prefix_from;
    }

    /* load file from start_pos on only, so skip bytes on input */
    ctx->unpacked_data += (ctx->cbm_range_from - ctx->cbm_orig_addr);
    /* also adopt ctx->unpacked_size */
    ctx->unpacked_size -= (ctx->cbm_range_from - ctx->cbm_orig_addr);
    /* and set up new load-address */
    ctx->cbm_orig_addr = ctx->cbm_range_from;

    if (ctx->unpacked_size <= 0) {
        fprintf(stderr, "Error: Input too small\n");
        exit(1);
    }

    printf("Compressing from $%04x to $%04x = $%04lx bytes\n", ctx->cbm_range_from, ctx->cbm_range_to, ctx->unpacked_size);

    if (ctx->cbm_relocate_packed_addr >= 0 || ctx->sfx) {
        ctx->inplace = FALSE;
    }

    /* write clamped raw data */
    ctx->clamped_name = (char*)malloc(sizeof(src_name));
    strcpy(ctx->clamped_name, src_name);
    sfp = fdopen(mkstemp(ctx->clamped_name),"wb");
    if (!sfp) {
        fprintf(stderr, "Error: Cannot create clamped file %s\n", ctx->clamped_name);
        exit(1);
    }
    if (ctx->unpacked_size != 0) {
        if (fwrite(ctx->unpacked_data, sizeof(char), ctx->unpacked_size, sfp) != ctx->unpacked_size) {
            fprintf(stderr, "Error: Cannot write clamped file\n");
            perror("fwrite");
            exit(1);
        }
    }
    fclose(sfp);

    /* ctreate temp file for dict */
    if (ctx->cbm_prefix_from >= 0) {
        if (ctx->prefix_name == NULL) {
            ctx->prefix_name = (char*)malloc(sizeof(tmp_name));
            strcpy(ctx->prefix_name, tmp_name);
            dfp = fdopen(mkstemp(ctx->prefix_name),"wb");
            printf("using prefix: $%04x - $%04x\n", ctx->cbm_prefix_from, ctx->cbm_prefix_from + dict_size);
            if (!dfp) {
                fprintf(stderr, "Error: Cannot create dict file %s\n", ctx->prefix_name);
                exit(1);
            }
            if (!dict_data || fwrite(dict_data, sizeof(char), dict_size, dfp) != dict_size) {
                fprintf(stderr, "Error: Cannot write dict file %s\n", ctx->prefix_name);
                remove(ctx->prefix_name);
                exit(1);
            }
            dict_temp = TRUE;
            fclose(dfp);
        }
    }

    /* compress data with salvador */
    salvador_argv[salvador_argc++] = "salvador";
    if (ctx->prefix_name) {
        salvador_argv[salvador_argc++] = "-D";
        salvador_argv[salvador_argc++] = ctx->prefix_name;
    }
    salvador_argv[salvador_argc++] = ctx->clamped_name;
    salvador_argv[salvador_argc++] = ctx->output_name;
    salvador_main(salvador_argc, salvador_argv);

    /* delete dict */
    if (dict_temp) remove(ctx->prefix_name);
    /* remove clamped */
    remove(ctx->clamped_name);

    /* read packed data */
    pfp = fopen(ctx->output_name, "rb");
    if (!pfp) {
        fprintf(stderr, "Error: Cannot access input file\n");
        exit(1);
    }
    ctx->packed_size = fread(ctx->packed_data, sizeof(char), BUFFER_SIZE, pfp);
    fclose(pfp);

    /* determine size without eof-marker -> remove 18 bits, either 2 byte or three byte depending on position of last bitpair */
    if (ctx->packed_data[ctx->packed_size - 1] & 0x80) ctx->packed_size -= 3;
    else ctx->packed_size -= 2;

    reencode_packed_stream(ctx);

    if (ctx->packed_index + 2 > ctx->unpacked_size) {
        fprintf(stderr, "Error: Packed file larger than original\n");
        exit(1);
    }

    //printf("control-bytes: $%04x\n", ctx->lz_bits);

    write_reencoded_stream(ctx);
    return;
}

int main(int argc, char *argv[]) {
    int i;

    ctx ctx = { 0 };

    ctx.output_name = NULL;
    ctx.input_name = NULL;
    ctx.prefix_name = NULL;

    ctx.inplace = TRUE;

    ctx.cbm = TRUE;
    ctx.cbm_orig_addr = 0;
    ctx.cbm_packed_addr = 0;
    ctx.cbm_range_from = -1;
    ctx.cbm_range_to = -1;
    ctx.cbm_relocate_packed_addr = -1;
    ctx.cbm_relocate_origin_addr = -1;
    ctx.cbm_prefix_from = -1;

    ctx.sfx = FALSE;
    ctx.sfx_addr = -1;
    ctx.sfx_01 = -1;
    ctx.sfx_cli = FALSE;
    ctx.sfx_small = FALSE;
    ctx.sfx_code = NULL;

    for (i = 1; i < argc; i++) {
        if (!strncmp(argv[i], "-", 1) || !strncmp(argv[i], "--", 2)) {
            if (!strcmp(argv[i], "--binfile")) {
                ctx.cbm = FALSE;
            } else if (!strcmp(argv[i], "--prefix-from")) {
                ctx.cbm_prefix_from = read_number(argv[i + 1], argv[i], 65536);
                i++;
            } else if (!strcmp(argv[i], "--prefix-file")) {
                i++;
                ctx.prefix_name = argv[i];
            } else if (!strcmp(argv[i], "--no-inplace")) {
                ctx.inplace = FALSE;
            } else if (!strcmp(argv[i], "--small")) {
                ctx.sfx_small = TRUE;
            } else if (!strcmp(argv[i], "--relocate-packed")) {
                ctx.cbm_relocate_packed_addr = read_number(argv[i + 1], argv[i], 65536);
                i++;
            } else if (!strcmp(argv[i], "--relocate-origin")) {
                ctx.cbm_relocate_origin_addr = read_number(argv[i + 1], argv[i], 65536);
                i++;
            } else if (!strcmp(argv[i], "--from")) {
                ctx.cbm_range_from = read_number(argv[i + 1], argv[i], 65536);
                i++;
            } else if (!strcmp(argv[i], "--to")) {
                ctx.cbm_range_to = read_number(argv[i + 1], argv[i], 65536);
                i++;
            } else if (!strcmp(argv[i], "--01")) {
                ctx.sfx_01 = read_number(argv[i + 1], argv[i], 256);
                i++;
            } else if (!strcmp(argv[i], "--cli")) {
                ctx.sfx_cli = TRUE;
            } else if (!strcmp(argv[i], "--sfx")) {
                ctx.sfx_addr = read_number(argv[i + 1], argv[i], 65536);
                i++;
                ctx.sfx = TRUE;
                ctx.inplace = FALSE;
            } else if (!strcmp(argv[i], "-o")) {
                i++;
                ctx.output_name = argv[i];
            } else {
                fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
                exit(1);
            }
        } else if (i == argc - 1) {
            ctx.input_name = argv[i];
        } else {
            fprintf(stderr, "Error: Unknown option %s\n", argv[i]);
            exit(1);
        }
    }

    printf("dali v0.3 - a zx0-reencoder for bitfire by Tobias Bindhammer\n");
    printf("underlying zx0-packer salvador by Emmanuel Marty\n");

    if (argc == 1) {
        fprintf(stderr, "Usage: %s [options] input\n"
                        "  -o [filename]               Set output filename.\n"
                        "  --sfx [num]                 Create a c64 compatible sfx-executable.\n"
                        "  --01 [num]                  Set 01 to [num] after sfx.\n"
                        "  --cli [num]                 Do a CLI after sfx, default is SEI.\n"
                        "  --small                     Use a very small depacker that fits into zeropage, but --01 and --cli are ignored and it trashes zeropage (!)\n"
                        "  --no-inplace                Disable inplace-decompression.\n"
                        "  --binfile                   Input file is a raw binary without load-address.\n"
                        "  --from [num]                Compress file from [num] on.\n"
                        "  --to [num]                  Compress file until position [num].\n"
                        "  --prefix-from [num]         Use preceeding data from [num] on as dictionary (in combination with --from).\n"
                        "  --prefix-file [file]        Use preceeding data from [file] as dictionary.\n"
                        "  --relocate-packed [num]     Relocate packed data to desired address [num] (resulting file can't de decompressed inplace!)\n"
                        "  --relocate-origin [num]     Set load-address of source file to [num] prior to compression. If used on bin-files, load-address and depack-target is prepended on output.\n"
                        ,argv[0]);
        exit(1);
    }

    if (!ctx.sfx && ctx.sfx_small) {
        fprintf(stderr, "Info: No sfx, ignoring --small option\n");
    }
    if (!ctx.sfx && ctx.sfx_01 >= 0) {
        fprintf(stderr, "Info: No sfx, ignoring --01 option\n");
    }
    if (!ctx.sfx && ctx.sfx_cli) {
        fprintf(stderr, "Info: No sfx, ignoring --cli option\n");
    }

    if (ctx.input_name == NULL) {
        fprintf(stderr, "Error: No input-filename given\n");
        exit(1);
    }

    do_reencode(&ctx);
    return 0;
}

};



#endif // DALIZX0_H
