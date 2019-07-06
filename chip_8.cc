// File "chip_8.cc" created July 2019
// Copyright (c) Lion Kortlepel 2019
#include "chip_8.hh"
#include <cstring>

unsigned char chip_8_fontset[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

void Chip8::initialize ()
{
    pc      = 0x0200; // program counter starts at 0x200 (decimal 512)
    opcode  = 0x0000; // reset opcode
    I       = 0x0000; // reset index register
    sp      = 0x0000; // reset stack pointer
    
    // clear display
    memset (gfx, 0x00, sizeof (gfx));
    // clear stack
    memset (stack, 0x0000, sizeof (stack));
    // clear registers V0-VF
    memset (V, 0x00, sizeof (V));
    // clear memory 
    memset (memory, 0x00, sizeof (memory));
    
    // load fontset
    for (int i = 0; i < 80; ++i)
    {
        memory[i] = chip_8_fontset[i];
    }
    
    // reset timers
    delay_timer = 0x00;
    sound_timer = 0x00;
}

void Chip8::load_program (const char* program_name)
{
    FILE* file = fopen (program_name, "rb");
    unsigned char buffer[0x1000 - 0x200] = { 0 }; // program buffer
    fread (buffer, 1, 0x1000 - 0x200, file); // 0x1000 - 0x200 is max size
    fclose (file);
    
    // copy into memory
    for (int i = 0; i < 0x1000 - 0x200; ++i)
    {
        memory[i + 0x200] = buffer[i];
    }
}

void Chip8::emulate_cycle ()
{
    // fetch opcode
    // merge / combine two consecutive bytes
    opcode = memory[pc] << 8 | memory[pc+1];
    
    printf ("→ opcode: 0x%X\n", opcode);
    
    // decode opcode
    switch (opcode & 0xF000)
    {
        case 0x0000: // 0x0NNN
            switch (opcode & 0x000F)
            {
                case 0x0000: // 0x00E0 clear screen
                    memset (gfx, 0x0, sizeof (gfx));
                    pc += 2;
                    printf ("Clear screen buffer");
                    break;
                case 0x000E: // 0x00EE return from subroutine
                    pc = stack[sp-1];
                    --sp;
                    pc += 2; // advance, otherwise infinite loop
                    printf ("Return from subroutine: sp: 0x%X, pc: 0x%X\n", sp, pc);
                    break;
            }
            break;
        case 0x3000: // 0x3XNN if(VX == NN) skip next instruction
            printf ("Skip one instruction if V[0x%X] == 0x%X\n", (opcode & 0x0F00) >> 8, (opcode & 0x00FF));
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                pc += 2; // skip one instruction
                printf ("   TRUE\n");
            }
            else
            {
                printf ("   FALSE\n");
            }
            pc += 2; // normal advancement in program
            break;
        case 0x6000: // 0x6XNN set VX to NN
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            printf ("Set V[0x%X] to 0x%X\n", (opcode & 0x0F00) >> 8, 
                opcode & 0x00FF);
            pc += 2;
            break;
        case 0xA000: // 0xANNN movi: move NNN into I
            I = opcode & 0x0FFF; // move last 12 bits into I
            printf ("Move 0x%X into I\n", I);
            pc += 0x2; // advance by 2 bytes (size of opcode)
            break;
        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007: // FX07 Sets VX to value of delay_timer
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                    printf ("Set V[0x%X] to delay_timer (0x%X)\n", (opcode & 0x0F00) >> 8, delay_timer);
                    break;
                case 0x0015: // FX15 Sets delay_timer to X
                    delay_timer = (opcode & 0x0F00) >> 8;
                    pc += 2;
                    printf ("Set delay_timer to 0x%X\n", delay_timer);
                    break;
                case 0x0029: // FX29 Sets I to the location of the sprite for the character &VX.
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc += 2;
                    printf ("I = V[0x%X] * 5 |=> 0x%X\n", (opcode & 0x0F00) >> 8, I);
                    break;
                case 0x0033: // FX33 Binary coded decimal (BCD)
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    pc += 2;
                    printf ("Binary coded decimal of 0x%X\n", (opcode & 0x0F00) >> 8);
                    break;
                case 0x0065: // FX65 Fills V0 to VX with values from memory from I->.
                    for (unsigned short i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                        // "<=" because X is inclusive
                    {
                        V[i] = memory[I + i]; // TODO untested
                    }
                    printf ("Filled V0-V%X with values from memory range 0x%X-0x%X\n", (opcode & 0x0F00) >> 8, I, I + ((opcode & 0x0F00) >> 8));
                    pc += 2;
                    break;
            }
            break;
        case 0x2000: // 0x2NNN call subroutine at address NNN
            stack[sp] = pc; // store program counter in stack
            ++sp; // advance stack pointer
            pc = opcode & 0x0FFF; // "jump" to subroutine at NNN
            printf ("Call subroutine at 0x%X (from 0x%X)\n", pc, stack[sp - 1]);
            break;
        case 0x7000: // 0x7XNN Add NN to VX
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
            printf ("Add 0x%X to V[0x%X] (no carry)\n", opcode & 0x00FF, (opcode & 0x0F00) >> 8);
            break;
        case 0x8000: // 0x8NNN
            switch (opcode & 0x000F)
            {
                case 0x0004: // 0x8XY4 add VY (V[Y]) to VX (V[X])
                {
                    // set carry flag if operation result > 0xFF (255)
                    printf ("V[0x%X] += V[0x%X]", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
                    if (V[(opcode & 0x0F00) >> 8] > 
                        0xFF - V[(opcode & 0x00F0) >> 4])
                    {
                        V[0xF] = 0x1;
                        printf ("CARRY\n");
                    }
                    else
                    {
                        V[0xF] = 0x0;
                    }
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    break;
                }
            }
            pc += 2;
            break;
        case 0xD000: // 0xDXYN draw
        {
            /*
             * Draws a sprite at coordinate (VX, VY) that has a width of 8 
             * pixels and a height of N pixels. 
             * 
             * Each row of 8 pixels is read as bit-coded starting from memory 
             * location I; 
             * 
             * I value doesn’t change after the execution of this instruction. 
             * 
             * VF is set to 1 if any screen pixels are flipped from set to unset
             * when the sprite is drawn, and to 0 if that doesn’t happen.
             */
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short n_rows = opcode & 0x000F;
            unsigned short pixel;
            
            V[0xF] = 0;
            
            for (unsigned short row = 0; row < n_rows; ++row)
            {
                pixel = memory[I + row];
                for (unsigned short bits = 0; bits < 8; ++bits)
                {
                    if (pixel & (0b10000000 >> bits))
                    {
                        if (gfx[x + bits + ((y + row) * 64)] == 0x1)
                            V[0xF] = 0x1;
                        gfx[x + bits + ((y + row) * 64)] ^= 0x1;
                    }
                }
            }
            
            draw_flag = true;
            pc += 2;
            
            printf ("draw call\n");
            
            break;
        }
        default:
            printf ("Unknown opcode / not implemented: 0x%X\n", opcode);
            break;
    }
    
    // update timers
    if (delay_timer > 0)
    {
        --delay_timer;
    }
    
    if (sound_timer > 0)
    {
        if (sound_timer == 1)
        {
            printf ("BEEP!\n");
        }
        --sound_timer;
    }
}
