# chip_8 (CHIP-8)
My implementation attempt of a chip-8 emulator. [Wikipedia: CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)

All opcodes are implemented and have been tested manually.

All CHIP-8 programs you can find on the internet should run on it. 

## Notes

You might have to adjust the [`window->setTargetFramerate (...)`](https://github.com/lionkor/chip_8/blob/master/main.cpp#L30) to set your preferred clock speed (60 would be "specification").

## Troubleshooting

If anything doesn't work as intended, especially if you notice it as the author of a CHIP-8 program, feel free to open an issue about it. 

#### "I'm getting `Unknown opcode / not implemented: 0xXXXX`"

If this happens, it means that the emulator has reached an opcode that it does not recognize. This might happen if the emulator runs into your programs data, as opposed to instructions, or if you used an invalid opcode. 

This emulator does not support [`Super Chip-48` instructions](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.2), but feel free to implement them yourself. Otherwise I might add them in the future if the "need" arises.

#### "My program is stuck in an infinite loop"

In this case it's important to note that this emulator will jump back to the start if it encounters `0x0000`, as a mechanism to never actually quit. This means all programs (unless they fill the memory completely with instructions) will behave as if they have a `0x1200` (`JP 0x200`) at the end.

`0xFx0A` (`LD Vx`) will always cause "infinite loop"-like behaviour in the stdout logging. Due to the way this instruction is implemented, it will not advance the program counter (`Chip8::pc`) until a key has been pressed. This is practically an infinite loop. This achieves exactly what the instruction should do, and as such this behaviour can be ignored. Feel free to remove the `printf` statements to reduce the log spam in this instruction.

#### "My program is not working without error"

If anything doesn't work as intended, the best course of action is to step through the emulator running with a debugger and to look at the stdout log messages. These will give you some information about what the emulator tries to do, and is the easiest way to identify problems, together with a debugger (so you can check out the actual values of register and memory at runtime).

Otherwise it's always helpful to carefully read the [Wikipedia Article](https://en.wikipedia.org/wiki/CHIP-8) and [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM). 

Keep in mind that the registers are `V[0x0]` to `V[0xF]`, although `V[0xF]` will be set by instructions like `0x8xy4`, and so probably shouldn't be used.

#### "I tried everything and I can't figure out why it's not working"

In the rare case that you actually care about a CHIP-8 program, you can always send me an email (email address on my profile) and we'll figure it out.
