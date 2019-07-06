#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include "chip_8.hh"

Chip8 chip8;

void beep ()
{
    sf::SoundBuffer buffer;
    buffer.loadFromFile("beep.wav");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
}

int main (int argc, char* argv[])
{
    chip8.initialize ();
    chip8.beep_fn = beep;
    chip8.load_program ("invaders.c8");
    
    sf::RenderWindow* window = new sf::RenderWindow { sf::VideoMode (64*16, 32*16), "CHIP-8" };
    sf::Event event;
    
    window->clear ();
    window->display ();
    
    window->setFramerateLimit (1200); // 60Hz Clock
    
    sf::VertexArray varray = sf::VertexArray (sf::PrimitiveType::Quads, 64 * 32 * 4);
    
    std::size_t index = 0;
    
    while (window->isOpen ())
    {        
        chip8.emulate_cycle ();
        
        if (chip8.draw_flag)
        {
            //window->clear ();
            //varray.clear ();
            
            index = 0;
            for (int x = 0; x < 64; ++x)
            for (int y = 0; y < 32; ++y)
            {
                sf::Color color = chip8.gfx[y * 64 + x] == 0 ? sf::Color::Black : sf::Color::White;
                varray[index + 0] = sf::Vertex ({ x * 16.0f,         y * 16.0f       }, color);
                varray[index + 1] = sf::Vertex ({ x * 16.0f + 16,    y * 16.0f       }, color);
                varray[index + 2] = sf::Vertex ({ x * 16.0f + 16,    y * 16.0f + 16  }, color);
                varray[index + 3] = sf::Vertex ({ x * 16.0f,         y * 16.0f + 16  }, color);
                index += 4;
            }
            
            window->draw (varray);
            window->display ();
        }
        
        for (int i = 0; i < sizeof (chip8.key); ++i)
        {
            if (chip8.key[i] != 0)
                printf ("   0x%X: 0x%X\n", i, chip8.key[i]);
        }

        //printf ("\n\n\nI: 0x%X\n", chip8.I);
        //printf ("pc: 0x%X\n", chip8.pc);
        //printf ("delay_timer: 0x%X\n", chip8.delay_timer);
        //printf ("sound_timer: 0x%X\n", chip8.sound_timer);
        //printf ("sp: 0x%X\n", chip8.sp);
        //printf ("draw_flag: 0x%X\n", chip8.draw_flag);
        
        // clear all keys
        memset (chip8.key, 0x0, sizeof (chip8.key));
        
        while (window->pollEvent (event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window->close ();
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Num1:
                            chip8.key[0x1] = 0x1;
                            chip8.beep_fn ();
                            break;
                        case sf::Keyboard::Num2:
                            chip8.key[0x2] = 0x1;
                            break;
                        case sf::Keyboard::Num3:
                            chip8.key[0x3] = 0x1;
                            break;
                        case sf::Keyboard::Num4:
                            chip8.key[0xC] = 0x1;
                            break;
                        case sf::Keyboard::Q:
                            chip8.key[0x4] = 0x1;
                            break;
                        case sf::Keyboard::W:
                            chip8.key[0x5] = 0x1;
                            break;
                        case sf::Keyboard::E:
                            chip8.key[0x6] = 0x1;
                            break;
                        case sf::Keyboard::R:
                            chip8.key[0xD] = 0x1;
                            break;
                        case sf::Keyboard::A:
                            chip8.key[0x7] = 0x1;
                            break;
                        case sf::Keyboard::S:
                            chip8.key[0x8] = 0x1;
                            break;
                        case sf::Keyboard::D:
                            chip8.key[0x9] = 0x1;
                            break;
                        case sf::Keyboard::F:
                            chip8.key[0xE] = 0x1;
                            break;
                        case sf::Keyboard::Y: // QWERTZ
                        case sf::Keyboard::Z: // QWERTY
                            chip8.key[0xA] = 0x1;
                            break;
                        case sf::Keyboard::X:
                            chip8.key[0x0] = 0x1;
                            break;
                        case sf::Keyboard::C:
                            chip8.key[0xB] = 0x1;
                            break;
                        case sf::Keyboard::V:
                            chip8.key[0xF] = 0x1;
                            break;
                    }
                    break;
            }
        }
    }
    
    return 0;
}
