// File "chip_8.hh" created July 2019
// Copyright (c) Lion Kortlepel 2019
#ifndef CHIP_8_CHIP_8_HH
#define CHIP_8_CHIP_8_HH


#include <string>

class Chip8
{
public:
    unsigned char memory[0x1000];   // 4k memory (4096 == 0x1000)
    unsigned short opcode;      // opcode has 2 bytes
    unsigned char V[16];        // general purpose registers V0-VE and carry VF
    unsigned short I;           // index register
    unsigned short pc;          // program counter
    unsigned char gfx[64 * 32]; // graphics (pixel on / pixel off)
    unsigned char delay_timer;  // 60 HZ timer
    unsigned char sound_timer;  // 60 HZ timer, sounds buzzer when it reaches zero 
    unsigned short stack[16];   // stack
    unsigned short sp;          // stack pointer
    unsigned char key[16];      // keypad (HEX)
    bool draw_flag;             // draw flag (redraw needed)
    bool halt_flag;             // halt flag, program halted while true
    
    void initialize ();
    void load_program (const char* program_name);
    void emulate_cycle ();
};


/*
 * Memory map:
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 */

#endif //CHIP_8_CHIP_8_HH
