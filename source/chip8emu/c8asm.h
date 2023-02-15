/** chip8.h
 *  =======
 *
 * ![toc]
 * ## Introduction
 *
 * This header provides the public API for the core of the CHIP-8 toolkit.
 *
 * The core of the CHIP-8 interpreter is written to be platform independent.
 * To use this core, one needs to use this API to create
 * a platform-specific _implementation_. This _implementation_ should do the
 * following:
 *
 * * Step through the instructions in the interpreter; see `c8_step()`.
 * * Draw the graphics; The resolution is obtained through `c8_resolution()`
 *     and then the individual pixel states are read through `c8_get_pixel()`
 * * Tell the interpreter about the state of the keyboard; It should call
 *     `c8_key_down()` and `c8_key_up()` when the state of the keyboard
 *     changes.
 * * Tell the intepreter about every 60Hz timer tick; see `c8_60hz_tick()`.
 * * Play sound. Since the sound is just a buzzer, you may wish to skip this;
 *     See `c8_sound()`.
 *
 * The header also includes an API for the CHIP-8 assembler and a disassembler.
 * The syntax is based on **Cowgod's Chip-8 Technical Reference v1.0**
 * by Thomas P. Greene available at
 * <http://devernay.free.fr/hacks/chip8/C8TECH10.HTM>
 *
 * ## License
 * This code is licensed under the [Apache license version 2](http://www.apache.org/licenses/LICENSE-2.0):
 *
 * ```
 *    Copyright 2015-2016 Werner Stoop
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * ```
 */

/** ## Definitions */
/** `#define TOTAL_RAM 4096`  \
 * Maximum addressable memory in the interpreter.
 */
#define TOTAL_RAM 4096

/** `#define PROG_OFFSET	512`  \
 * Offset of the program in RAM. Should be 512, but
 * apparently there are some computers where this is `0x600` [1].
 */
#define PROG_OFFSET	512

/** `#define MAX_MESSAGE_TEXT	128`  \
 * Size of the buffer used internally by `c8_message()`
 */
#define MAX_MESSAGE_TEXT	128

/** `extern int c8_verbose;`  \
 * Set to non-zero to turn on verbose mode.
 *
 * The higher the value, the more verbose the output.
 */

extern int c8_verbose;

/** ## Interpreter */

/** `void c8_reset();`  \
 * Resets the state of the interpreter so that a new program
 * can be executed.
 */
void c8_reset();

/** `void c8_step();`  \
 * Steps through a single instruction in the interpreter.
 *
 * This function forms the core of the interpreter.
 */
void c8_step();

/** `int c8_ended();`  \
 * Returns true if the interpreter has ended.
 *
 * The interpreter has ended if a **00FD** instruction has been encountered.
 *
 * The **00FD** instruction is actually SuperChip specific.
 */
int c8_ended();

/** `int c8_waitkey();`  \
 * Returns true if the interpreter is waiting for keyboard input.
 *
 * The **Fx0A** instruction is the one that waits for a specific key to be pressed.
 */
int c8_waitkey();

/** ## Debugging */

/** `uint8_t c8_get(uint16_t addr);`  \
 * Gets the value of a byte at a specific address `addr` in
 * the interpreter's RAM.
 */
uint8_t c8_get(uint16_t addr);

/** `void c8_set(uint16_t addr, uint8_t byte);`  \
 * Sets the value of the `byte` at a specific address `addr` in
 * the interpreter's RAM.
 */
void c8_set(uint16_t addr, uint8_t byte);

/** `uint16_t c8_opcode(uint16_t addr);`  \
 * Gets the opcode at a specific address `addr` in the interpreter's RAM.
 */
uint16_t c8_opcode(uint16_t addr);

/** `uint16_t c8_get_pc();`  \
 * Gets the current address pointed to by the interpreter's program
 * counter (PC).
 *
 */
uint16_t c8_get_pc();

/** `uint16_t c8_prog_size();`  \
 * Gets the size of the program in the interpreter's RAM.
 *
 * It basically just search for the last non-zero byte in RAM.
 */
uint16_t c8_prog_size();

/** `uint8_t c8_get_reg(uint8_t r);`  \
 * Gets the value of the register `Vr` where `0` <= `r` <= `F`.
 */
uint8_t c8_get_reg(uint8_t r);

/** `int (*c8_rand)();`  \
 * Points to the function that should be used to generate
 * random numbers for the **Cxkk** instruction.
 *
 * The default value points to `rand()` in the standard library.
 * This implies that `srand()` should be called at the
 * start of the program.
 */
extern int (*c8_rand)();

/** ## Graphics
 * The _implementation_ should provide a platform specific way for the interpreter
 * core to draw its graphics.
 *
 */

/** `int c8_screen_updated();`  \
 * Returns true if the last instruction executed by `c8_step()` changed the graphics,
 * in which case the display should be updated.
 */
int c8_screen_updated();

/** `int c8_resolution(int *w, int *h);`  \
 * Loads the current resolution of the interpreter into `w` and `h`.
 *
 * The interpreter will be in either the normal CHIP-8 64x32 resolution or
 * in the SuperChip-specific 128x64 "high" resolution mode.
 *
 * It returns 1 if the interpreter is in high resolution mode, 0 otherwise.
 */
int c8_resolution(int *w, int *h);

/** `int c8_get_pixel(int x, int y);`  \
 * Gets the status of the pixel at (x,y).
 *
 * Returns 1 if the pixel is set - i.e. it should be drawn in the foreground colour.
 *
 * Returns 0 if the pixel is cleared - i.e. it should be drawn in the background colour.
 */
int c8_get_pixel(int x, int y);

/** ## Keyboard routines
 * The _implementation_ should use these functions to tell the interpreter
 * about changes in the keyboard state:
 */

/** `void c8_key_down(uint8_t k);`  \
 * Sets the state of key `k` to pressed.
 */
void c8_key_down(uint8_t k);

/** `void c8_key_up(uint8_t k);`  \
 * Sets the state of key `k` to released.
 */
void c8_key_up(uint8_t k);

/** ## Timer and sound functions
 * CHIP-8 has a 60Hz timer that updates a delay timer and a sound timer
 * register. The _implementation_ needs to tell the interpreter about these
 * 60Hz ticks.
 */

/** `void c8_60hz_tick();`  \
 * Executes a CHIP-8 60Hz timer tick.
 *
 * This decrements the delay and sound timers if they are non-zero.
 *
 * The _implementation_ should call this function 60 times per second.
 */
void c8_60hz_tick();

/** `int c8_sound();`  \
 * Returns true if the sound timer is non-zero and sound should be played.
 *
 * CHIP-8 sounds use a single tone over which programs have no control.
 */
int c8_sound();

/** ## I/O Routines
 * The toolkit provides several functions to save
 * and load CHIP-8 programs to and from disk.
 */

/** `size_t c8_load_program(uint8_t program[], size_t n);`  \
 * Loads a program's bytes (of length `n`) into the interpreter's RAM.
 * It returns the number of bytes loaded.
 */
size_t c8_load_program(uint8_t program[], size_t n);

/** `int c8_load_file(const char *fname);`  \
 * Loads a CHIP-8 file from disk into the interpreter's RAM.
 *
 * Returns the number of bytes read, 0 on error.
 */
int c8_load_file(const char *fname);

/** `int c8_save_file(const char *fname);`  \
 * Writes the contents of the interpreter's RAM to a file.
 *
 * This is typically done after `c8_assemble()` to write the
 * final program to disk.
 *
 * Returns the number of bytes written on success, 0 on failure.
 */
int c8_save_file(const char *fname);

/** `char *c8_load_txt(const char *fname);`  \
 * Utility function that loads a text file.
 *
 * It `malloc()`s a buffer large enough to hold the entire file
 * that needs to be `free()`ed afterwards.
 *
 * Returns the buffer, or `NULL` on error.
 */
char *c8_load_txt(const char *fname);

/** ## Output and Error handling */

/** `extern int (*c8_puts)(const char* s);`  \
 * Pointer to a function that outputs text messages.
 *
 * The default implementation wraps around `fputs()`
 * and writes to `stdout`; change it if output needs
 * to be done differently.
 */
extern int (*c8_puts)(const char* s);

/** `int c8_message(const char *msg, ...);`  \
 * Outputs a formatted message.
 *
 * It has the same symantics as `printf()` and
 * calls `c8_puts()` to output a message.
 *
 * Returns the value of the `c8_puts()` call.
 */
int c8_message(const char *msg, ...);

/** `extern char c8_message_text[];`  \
 * The internal buffer used by `c8_message()`.
 */
char c8_message_text[1024];

/** ## Assembler */

/** `int c8_assemble(const char *text);`  \
 * Assembles a block of text into the interpreter's RAM.
 *
 * The assembled program can be written to a file using `c8_save_file()`.
 *
 * `c8_load_txt()` is provided as a utility function to load
 * a text file that can be assembled.
 *
 * See `asmmain.c` for an example of a program that uses this function.
 */
int c8_assemble(const char *text);
int c8_save_file(const char *fname);


/** ## Disassembler */

/** `void c8_disasm();`  \
 * Disassembles the program currently in the interpreter's RAM.
 *
 * The output is written through `c8_puts()`.
 *
 * See `dasmmain.c` for an example of a program that uses this function.
 */
void c8_disasm();
