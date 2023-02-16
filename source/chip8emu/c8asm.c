/*
CHIP8 assembler.
Mostly based on the syntax of "Cowgod's Chip-8 Technical Reference v1.0",
by Thomas P. Greene, http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
Hexadecimal constants can be written as
*/
/* CHIP-8 Assembler. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>


#include "c8asm.h"

#define TOK_SIZE    64

#define MAX_DEFS    256
#define MAX_LOOKUP  256

typedef enum {
    SYM_END,
    SYM_IDENTIFIER,
    SYM_INSTRUCTION,
    SYM_REGISTER,
    SYM_NUMBER,
    SYM_I,
    SYM_DT,
    SYM_ST,
    SYM_K,
    SYM_F,
    SYM_B,
    SYM_HF,
    SYM_R,
    SYM_DEFINE,
    SYM_OFFSET,
    SYM_DB,
    SYM_DW,
} SYMBOL;
/* List of instruction names. */
static const char *inst_names[] = {
    "add",
    "and",
    "call",
    "cls",
    "drw",
    "jp",
    "ld",
    "or",
    "ret",
    "rnd",
    "se",
    "shl",
    "shr",
    "sknp",
    "skp",
    "sne",
    "sub",
    "subn",
    "sys",
    "xor",
    "scd",
    "scr",
    "scl",
    "exit",
    "low",
    "high",
};

//static int sym;
typedef struct {
    const char * in;
    const char * last;
    SYMBOL sym;
    int linenum;
    char token[TOK_SIZE];
} Stepper;


/* Generated instructions before binary output */
static struct {
    uint8_t byte;
    char *label;
    int linenum;
} program[TOTAL_RAM];

static uint16_t next_instr; /* Address of next instruction */
static uint16_t max_instr;  /* Largest instruction address for output */

/* Lookup table for labels for JP and CALL instructions */
static struct {
    char *label;
    uint16_t addr;
} lookup[MAX_LOOKUP];
static int n_lookup;

/* Lookup table for DEFINE identifier value statements */
static struct {
    char *name;
    SYMBOL type;
    char *value;
} defs[MAX_DEFS];
static int n_defs;

static void exit_error(const char *msg, ...) {
    char buffer[MAX_MESSAGE_TEXT];
    if(msg) {
        va_list arg;
        va_start (arg, msg);
        vsnprintf (buffer, MAX_MESSAGE_TEXT - 1, msg, arg);
        va_end (arg);
        c8_message("%s", buffer);
    }
}

static void emit(const Stepper * stepper, uint16_t inst) {
    if(next_instr >= TOTAL_RAM)
        exit_error("error: program too large\n");
    program[next_instr].linenum = stepper->linenum;
    program[next_instr++].byte = inst >> 8;
    program[next_instr].linenum = stepper->linenum;
    program[next_instr++].byte = inst & 0xFF;
    if(next_instr > max_instr)
        max_instr = next_instr;
}

static void emit_l(const Stepper * stepper, uint16_t inst, const char *label) {
    if(next_instr == TOTAL_RAM)
        exit_error("error: program too large\n");
    program[next_instr].linenum = stepper->linenum;
    program[next_instr].byte = inst >> 8;
    program[next_instr].label = strdup(label);
    next_instr++;
    program[next_instr].linenum = stepper->linenum;
    program[next_instr].byte = 0;
    next_instr++;
    if(next_instr > max_instr)
        max_instr = next_instr;
}

static void emit_b(const Stepper * stepper, uint8_t byte) {
    if(next_instr >= TOTAL_RAM)
        exit_error("error: program too large\n");
    program[next_instr].linenum = stepper->linenum;
    program[next_instr++].byte = byte;
    if(next_instr > max_instr)
        max_instr = next_instr;
}

static void add_label(const char *label, const int linenum) {
    int i;
    if(n_lookup == MAX_LOOKUP)
        exit_error("error: too many entries in lookup\n");
    for(i = 0; i < n_lookup; i++)
        if(!strcmp(lookup[i].label, label))
            exit_error("error:%d: duplicate label '%s'\n", linenum, label);
    lookup[n_lookup].label = strdup(label);
    lookup[n_lookup].addr = next_instr;
    n_lookup++;
}

static void add_definition(const Stepper * stepper, char *name) {
    if(n_defs == MAX_DEFS)
        exit_error("error:%d: too many definitions\n", stepper->linenum);
    defs[n_defs].name = name;
    defs[n_defs].type = stepper->sym;
    defs[n_defs].value = strdup(stepper->token);
    n_defs++;
}

static int nextsym(Stepper * stepper) {
    /* TODO: Ought to guard against buffer overruns in tok, but not today. */
    char *tok = stepper->token;

    stepper->sym = SYM_END;
    *tok = '\0';

scan_start:
    while(isspace(*stepper->in)) {
        if(*stepper->in == '\n')
            stepper->linenum++;
        stepper->in++;
    }

    stepper->last=stepper->in;
    if(!*stepper->in)
        return SYM_END;

    if(*stepper->in == ';') {
        while(*stepper->in && *stepper->in != '\n')
            stepper->in++;
        goto scan_start;
    }
    

    if(isalpha(*stepper->in)) {
        int i;
        while(isalnum(*stepper->in) || *stepper->in == '_')
            *tok++ = tolower(*stepper->in++);
        *tok = '\0';
        for(i = 0; i < (sizeof inst_names)/(sizeof inst_names[0]); i++)
            if(!strcmp(inst_names[i], stepper->token)) {
                stepper->sym = SYM_INSTRUCTION;
                break;
            }
        /* see http://stackoverflow.com/a/15824981/115589
        if(bsearch(stepper->token, inst_names, (sizeof inst_names)/(sizeof inst_names[0]), sizeof inst_names[0], myStrCmp)) {
            sym = SYM_INSTRUCTION;
        } */
        if(stepper->sym != SYM_INSTRUCTION) {
            if(stepper->token[0] == 'v' && isxdigit(stepper->token[1]) && !stepper->token[2])
                stepper->sym = SYM_REGISTER;
            else if(!strcmp(stepper->token, "i"))
                stepper->sym = SYM_I;
            else if(!strcmp(stepper->token, "dt"))
                stepper->sym = SYM_DT;
            else if(!strcmp(stepper->token, "st"))
                stepper->sym = SYM_ST;
            else if(!strcmp(stepper->token, "k"))
                stepper->sym = SYM_K;
            else if(!strcmp(stepper->token, "f"))
                stepper->sym = SYM_F;
            else if(!strcmp(stepper->token, "b"))
                stepper->sym = SYM_B;
            else if(!strcmp(stepper->token, "hf"))
                stepper->sym = SYM_HF;
            else if(!strcmp(stepper->token, "r"))
                stepper->sym = SYM_R;
            else if(!strcmp(stepper->token, "define"))
                stepper->sym = SYM_DEFINE;
            else if(!strcmp(stepper->token, "offset"))
                stepper->sym = SYM_OFFSET;
            else if(!strcmp(stepper->token, "db"))
                stepper->sym = SYM_DB;
            else if(!strcmp(stepper->token, "dw"))
                stepper->sym = SYM_DW;
            else {
                stepper->sym = SYM_IDENTIFIER;
                for(i = 0; i < n_defs; i++) {
                    if(!strcmp(defs[i].name, stepper->token)) {
                        stepper->sym = defs[i].type;
                        strcpy(stepper->token, defs[i].value);
                        break;
                    }
                }
            }
        }
    } else if(isdigit(*stepper->in) || *stepper->in=='-' || *stepper->in=='+') {
        *tok++ = *stepper->in++;
        while(isdigit(*stepper->in))
            *tok++ = *stepper->in++;
        if(isalnum(*stepper->in))
            exit_error("error:%d: invalid number\n", stepper->linenum);
        *tok = '\0';
        stepper->sym = SYM_NUMBER;
    } else if(*stepper->in == '#') {
        stepper->in++;
        while(isxdigit(*stepper->in))
            *tok++ = *stepper->in++;
        if(isalnum(*stepper->in))
            exit_error("error:%d: invalid #hex number\n", stepper->linenum);
        *tok = '\0';
        long x = strtol(stepper->token, NULL, 16);
        sprintf(stepper->token, "%ld", x);
        stepper->sym = SYM_NUMBER;
    } else if(*stepper->in == '%') {
        stepper->in++;
        while(strchr("01",*stepper->in))
            *tok++ = *stepper->in++;
        if(isalnum(*stepper->in))
            exit_error("error:%d: invalid %%bin number\n", stepper->linenum);
        *tok = '\0';
        long x = strtol(stepper->token, NULL, 2);
        sprintf(stepper->token, "%ld", x);
        stepper->sym = SYM_NUMBER;
    } else {
        stepper->token[0] = *stepper->in;
        stepper->token[1] = '\0';
        stepper->sym = *stepper->in++;
    }

    return stepper->sym;
}




void expect(Stepper * stepper, int what) {
    SYMBOL sym = nextsym(stepper); 
    if(sym != what) 
        exit_error("error:%d: '%c'%d expected\n", stepper->linenum, what,sym); 
    nextsym(stepper);
}

static int get_register(const Stepper * stepper) {
    int reg = stepper->token[1];
    if(stepper->sym != SYM_REGISTER)
        exit_error("error:%d: register expected\n", stepper->linenum);
    assert(isxdigit(reg));
    if(reg >= 'a') {
        reg = reg - 'a' + 0xA;
    } else {
        reg -= '0';
    }
    assert(reg >= 0 && reg <= 0xF);
    return reg;
}

static int get_addr(const Stepper * stepper) {
    int a = atoi(stepper->token);
    if(a < 0 || a > 0xFFF)
        exit_error("error:%d: invalid addr %d (%03X)\n", stepper->linenum, a, a);
    return a;
}

static int get_byte(const Stepper * stepper) {
    int a = atoi(stepper->token);
    if(a < -128 || a > 0xFF)
        exit_error("error:%d: invalid byte value %d (%02X)\n", stepper->linenum, a, a);
    return a&0xff;
}

static int get_word(const Stepper * stepper) {
    int a = atoi(stepper->token);
    if(a < 0 || a > 0xFFFF)
        exit_error("error:%d: invalid word value %d (%04X)\n", stepper->linenum, a, a);
    return a;
}
char c8_message_text[1024];



int c8_assemble(const char *text) {
//    c8_reset();
    static Stepper stepper;
    int i, j, regx = -1, regy = 0;
    stepper.in = text;
    c8_message_text[0]=0;
    if(c8_verbose) c8_message("Assembling...\n");

    next_instr = 512;
    max_instr = 0;

    stepper.linenum = 1;
    stepper.last = NULL;

    n_lookup = 0;
    n_defs = 0;

    memset(program, 0, sizeof program);

    nextsym(&stepper);
    while(stepper.sym != SYM_END) {
        //c8_message("%d %d %s\n", stepper.linenum, stepper.sym, stepper->token);
        if(stepper.sym == SYM_DEFINE) {
            nextsym(&stepper);
            char *name;
            /* "Identifier expected" may also mean that the name has
                already been used, eg. if aaa is already defined as 123
                then define aaa 456 looks like define 123 456 */
            if(stepper.sym != SYM_IDENTIFIER)
                exit_error("error:%d: identifier expected, found %s\n", stepper.linenum, stepper.token);
            name = strdup(stepper.token);
            nextsym(&stepper);
            if(stepper.sym != SYM_NUMBER && stepper.sym != SYM_REGISTER)
                exit_error("error:%d: value expected\n", stepper.linenum);
            add_definition(&stepper, name);
            nextsym(&stepper);
        } else if(stepper.sym == SYM_OFFSET) {
            nextsym(&stepper);
            if(stepper.sym != SYM_NUMBER)
                exit_error("error:%d: offset expected\n", stepper.linenum);
            next_instr = get_addr(&stepper);
            nextsym(&stepper);
        } else if(stepper.sym == SYM_DB) {
            do {
                nextsym(&stepper);
                if(stepper.sym == SYM_END)
                    break;
                if(stepper.sym != SYM_NUMBER)
                    exit_error("error:%d: byte value expected\n", stepper.linenum);
                emit_b(&stepper,get_byte(&stepper));
                nextsym(&stepper);
            } while(stepper.sym == ',');
        } else if(stepper.sym == SYM_DW) {
            do {
                nextsym(&stepper);
                if(stepper.sym == SYM_END)
                    break;
                if(stepper.sym != SYM_NUMBER)
                    exit_error("error:%d: byte value expected\n", stepper.linenum);
                //emit_b(get_byte(&stepper));
                uint16_t word = get_word(&stepper);
                emit_b(&stepper, word >> 8);
                emit_b(&stepper,word & 0xFF);
                nextsym(&stepper);
            } while(stepper.sym == ',');
        } else if(stepper.sym == SYM_IDENTIFIER) {
            add_label(stepper.token, stepper.linenum);
            expect(&stepper, ':');
        } else if(stepper.sym == SYM_INSTRUCTION) {
            if(!strcmp("cls", stepper.token))
                emit(&stepper, 0x00E0);
            else if(!strcmp("ret", stepper.token))
                emit(&stepper, 0x00EE);
            else if(!strcmp("jp", stepper.token)) {
                nextsym(&stepper);
                if(stepper.sym == SYM_IDENTIFIER)
                    emit_l(&stepper, 0x1000, stepper.token);
                else if(stepper.sym == SYM_REGISTER) {
                    if(strcmp(stepper.token, "v0"))
                        exit_error("error:%d: JP applies to V0 only\n", stepper.linenum);
                    expect(&stepper, ',');
                    if(stepper.sym == SYM_IDENTIFIER)
                        emit_l(&stepper, 0xB000, stepper.token);
                    else
                        emit(&stepper, 0xB000 | get_addr(&stepper));
                } else
                    emit(&stepper, 0x1000 | get_addr(&stepper));
            } else if(!strcmp("call", stepper.token)) {
                nextsym(&stepper);
                if(stepper.sym == SYM_IDENTIFIER)
                    emit_l(&stepper, 0x2000, stepper.token);
                else {
                    if(stepper.sym != SYM_NUMBER)
                        exit_error("error:%d: address expected", stepper.linenum);
                    emit(&stepper, 0x2000 | get_addr(&stepper));
                }
            } else if(!strcmp("se", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                if(stepper.sym == SYM_NUMBER)
                    emit(&stepper, 0x3000 | (regx << 8) | get_byte(&stepper));
                else if(stepper.sym == SYM_REGISTER) {
                    regy = get_register(&stepper);
                    emit(&stepper, 0x5000 | (regx << 8) | (regy << 4));
                } else
                    exit_error("error:%d: operand expected\n", stepper.linenum);
            } else if(!strcmp("sne", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                if(stepper.sym == SYM_NUMBER) {
                    emit(&stepper, 0x4000 | (regx << 8) | get_byte(&stepper));
                } else if(stepper.sym == SYM_REGISTER) {
                    regy = get_register(&stepper);
                    emit(&stepper, 0x9000 | (regx << 8) | (regy << 4));
                } else
                    exit_error("error:%d: operand expected\n", stepper.linenum);
            } else if(!strcmp("add", stepper.token)) {
                nextsym(&stepper);
                if(stepper.sym == SYM_I) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF01E | (get_register(&stepper) << 8));
                } else {
                    regx = get_register(&stepper);
                    expect(&stepper, ',');
                    if(stepper.sym == SYM_NUMBER) {
                        emit(&stepper, 0x7000 | (regx << 8) | get_byte(&stepper));
                    } else if(stepper.sym == SYM_REGISTER) {
                        regy = get_register(&stepper);
                        emit(&stepper, 0x8004 | (regx << 8) | (regy << 4));
                    } else
                        exit_error("error:%d: operand expected\n", stepper.linenum);
                }
            } else if(!strcmp("ld", stepper.token)) {
                nextsym(&stepper);
                if(stepper.sym == SYM_I) {
                    expect(&stepper, ',');
                    if(stepper.sym == SYM_IDENTIFIER)
                        emit_l(&stepper, 0xA000, stepper.token);
                    else
                        emit(&stepper, 0xA000 | get_addr(&stepper));
                } else if(stepper.sym == SYM_DT) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF015 | (get_register(&stepper) << 8));
                } else if(stepper.sym == SYM_ST) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF018 | (get_register(&stepper) << 8));
                } else if(stepper.sym == SYM_F) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF029 | (get_register(&stepper) << 8));
                } else if(stepper.sym == SYM_B) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF033 | (get_register(&stepper) << 8));
                } else if(stepper.sym == '[') {
                    if(nextsym(&stepper) != SYM_I || nextsym(&stepper) != ']')
                        exit_error("error:%d: [I] expected\n", stepper.linenum);
                    if(nextsym(&stepper) != ',')
                        exit_error("error:%d: ',' expected\n", stepper.linenum);
                    nextsym(&stepper);
                    emit(&stepper, 0xF055 | (get_register(&stepper) << 8));
                } else if(stepper.sym == SYM_HF) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF030 | (get_register(&stepper) << 8));
                } else if(stepper.sym == SYM_R) {
                    expect(&stepper, ',');
                    emit(&stepper, 0xF075 | (get_register(&stepper) << 8));
                } else {
                    regx = get_register(&stepper);
                    expect(&stepper, ',');
                    if(stepper.sym == SYM_NUMBER)
                        emit(&stepper, 0x6000 | (regx << 8) | get_byte(&stepper));
                    else if(stepper.sym == SYM_REGISTER) {
                        regy = get_register(&stepper);
                        emit(&stepper, 0x8000 | (regx << 8) | (regy << 4));
                    } else if(stepper.sym == SYM_DT)
                        emit(&stepper, 0xF007 | (regx << 8));
                    else if(stepper.sym == SYM_K)
                        emit(&stepper, 0xF00A | (regx << 8));
                    else if(stepper.sym == '[') {
                        if(nextsym(&stepper) != SYM_I || nextsym(&stepper) != ']')
                            exit_error("error:%d: [I] expected\n", stepper.linenum);
                        emit(&stepper, 0xF065 | (regx << 8));
                    } else if(stepper.sym == SYM_R) {
                        emit(&stepper, 0xF085 | (regx << 8));
                    } else
                        exit_error("error:%d: operand expected\n", stepper.linenum);
                }
            } else if(!strcmp("or", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                emit(&stepper, 0x8001 | (regx << 8) | (regy << 4));
            } else if(!strcmp("and", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                emit(&stepper, 0x8002 | (regx << 8) | (regy << 4));
            } else if(!strcmp("xor", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                emit(&stepper, 0x8003 | (regx << 8) | (regy << 4));
            } else if(!strcmp("sub", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                emit(&stepper, 0x8005 | (regx << 8) | (regy << 4));
            } else if(!strcmp("shr", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                nextsym(&stepper);
                if(stepper.sym == ',') {
                    nextsym(&stepper);
                    regy = get_register(&stepper);
                } else
                    stepper.in=stepper.last;
                emit(&stepper, 0x8006 | (regx << 8) | (regy << 4));
            } else if(!strcmp("subn", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                emit(&stepper, 0x8007 | (regx << 8) | (regy << 4));
            } else if(!strcmp("shl", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                nextsym(&stepper);
                if(stepper.sym == ',') {
                    nextsym(&stepper);
                    regy = get_register(&stepper);
                } else
                    stepper.in=stepper.last;
                emit(&stepper, 0x800E | (regx << 8) | (regy << 4));
            } else if(!strcmp("rnd", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                if(stepper.sym != SYM_NUMBER)
                    exit_error("error:%d: operand expected\n", stepper.linenum);
                emit(&stepper, 0xC000 | (regx << 8) | get_byte(&stepper));
            }  else if(!strcmp("drw", stepper.token)) {
                nextsym(&stepper);
                regx = get_register(&stepper);
                expect(&stepper, ',');
                regy = get_register(&stepper);
                expect(&stepper, ',');
                int nib = get_byte(&stepper);
                if(nib < 0 || nib > 0xF)
                    exit_error("error:%d: invalid value %d\n", stepper.linenum, nib);
                emit(&stepper, 0xD000 | (regx << 8) | (regy << 4) | nib);
            } else if(!strcmp("skp", stepper.token)) {
                nextsym(&stepper);
                emit(&stepper, 0xE09E | (get_register(&stepper) << 8));
            } else if(!strcmp("sknp", stepper.token)) {
                nextsym(&stepper);
                emit(&stepper, 0xE0A1 | (get_register(&stepper) << 8));
            } else if(!strcmp("scd", stepper.token)) {
                nextsym(&stepper);
                int nib = get_byte(&stepper);
                if(nib < 0 || nib > 0xF)
                    exit_error("error:%d: invalid value %d\n", stepper.linenum, nib);
                emit(&stepper, 0x00C0 | nib);
            } else if(!strcmp("scr", stepper.token)) {
                emit(&stepper, 0x00FB);
            } else if(!strcmp("scl", stepper.token)) {
                emit(&stepper, 0x00FC);
            } else if(!strcmp("exit", stepper.token)) {
                emit(&stepper, 0x00FD);
            } else if(!strcmp("low", stepper.token)) {
                emit(&stepper, 0x00FE);
            } else if(!strcmp("high", stepper.token)) {
                emit(&stepper, 0x00FF);
            } else if(!strcmp("sys", stepper.token)) {
#if 1
                /* SYS is not supported in modern emulators */
                exit_error("error:%d: SYS support is disabled\n", stepper.linenum);
#else
                nextsym(&stepper);
                emit(0x0000 | get_addr(&stepper));
#endif
            }

            nextsym(&stepper);
        } else
            exit_error("error:%d: unexpected token [%d]: '%s'\n", stepper.linenum, stepper.sym, stepper.token);
    }

    if(c8_verbose) c8_message("Resolving labels...\n");
    size_t n = PROG_OFFSET;
    for(i = PROG_OFFSET; i < max_instr; i++) {
        if(program[i].label) {
            for(j = 0; j < n_lookup; j++) {
                if(!strcmp(lookup[j].label, program[i].label)) {
                    assert(lookup[j].addr <= 0xFFF);
                    program[i].byte |= (lookup[j].addr >> 8);
                    program[i + 1].byte = lookup[j].addr & 0xFF;
                    free(program[i].label);
                    program[i].label = NULL;
                    break;
                }
            }
            if(program[i].label)
                exit_error("error:%d: unresolved label '%s'\n", program[i].linenum, program[i].label);
        }
        if(c8_verbose > 1) {
            if(!(i & 0x01))
                c8_message("%03X: %02X", i, program[i].byte);
            else
                c8_message("%02X\n", program[i].byte);
        }

        c8_set(n++, program[i].byte);
    }
    if(c8_verbose > 1 && i & 0x01)
        c8_message("\n");

    if(c8_verbose) c8_message("Assembled; %d bytes.\n", max_instr - PROG_OFFSET);

    for(i = 0; i < n_lookup; i++) {
        free(lookup[i].label);
    }
    for(i = 0; i < n_defs; i++) {
        free(defs[i].name);
        free(defs[i].value);
    }

    return 1;
}
static uint8_t RAM[TOTAL_RAM]; /* Interpreter RAM */

int c8_verbose = 0;
int c8_message(const char *msg, ...) {
    if(msg) {
        va_list arg;
        va_start (arg, msg);
        vsnprintf (c8_message_text, MAX_MESSAGE_TEXT-1, msg, arg);
        va_end (arg);
    }


}

int c8_save_file(const char *fname) {
    uint16_t n = c8_prog_size();
    size_t len = n - PROG_OFFSET;
    FILE *f = fopen(fname, "wb");
    if(!f)
        return 0;
    if(fwrite(RAM + PROG_OFFSET, 1, len, f) != len)
        return 0;
    fclose(f);
    return len;
}

void c8_set(uint16_t addr, uint8_t byte) {
    assert(addr < TOTAL_RAM);
    RAM[addr] = byte;
}

uint16_t c8_prog_size() {
    uint16_t n;
    for(n = TOTAL_RAM - 1; n > PROG_OFFSET && RAM[n] == 0; n--);
    if(++n & 0x1) // Fix for #4
        return n + 1;
    return n;
}
