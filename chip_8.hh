// File "chip_8.hh" created July 2019
// Copyright (c) Lion Kortlepel 2019
#ifndef CHIP_8_CHIP_8_HH
#define CHIP_8_CHIP_8_HH


class Chip8
{
private:
    unsigned short  opcode;         // opcode has 2 bytes
    unsigned char   memory[4096];   // 4k memory
    unsigned char   V[16];          // general purpose registers V0-VE
    unsigned short  I;              // index register
    unsigned short  pc;             // program counter
    unsigned char   gfx[64 * 32];   // graphics (pixel on / pixel off)
    unsigned char   delay_timer;    // 
    unsigned char   sound_timer;    // sounds buzzer when it reaches zero 
    unsigned short  stack[16];      // stack
    unsigned short  sp;      // stack pointer
    unsigned char   key[16];
public:
    void initialize ();
};


/*
 * Memory map:
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 */

#endif //CHIP_8_CHIP_8_HH
