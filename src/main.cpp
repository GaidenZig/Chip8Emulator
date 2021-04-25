#include "Chip8.h"
#include "Layout.h"
#include <stdio.h>
#include <chrono>
#include <iostream>


int main(int argc, char** argv){
    // if(argc != 4 ){
    //     std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
    //     std::exit(EXIT_FAILURE);
    // }
	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Layout layout("CHIP-8 Emulator", 64*videoScale, 32*videoScale, 64, 32);

    Chip8 chip8;
	if(chip8.LoadRom(romFilename)){
		int videoPitch = sizeof(chip8.gfx[0]) * 64;
		auto lastCycleTime = std::chrono::high_resolution_clock::now();
		bool quit = false;
		// uint32_t pixels[2048];

		while (!quit){ 
			quit = layout.ProcessInput(chip8.keypad);
			auto currentTime = std::chrono::high_resolution_clock::now();
			float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

			if(dt > cycleDelay){
				lastCycleTime = currentTime;
				chip8.EmulationCycle();

				// for (int i = 0; i < 2048; ++i){
				// 	uint8_t pixel = chip8.gfx[i];
				// 	pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
				// }
				// layout.Update(pixels,videoPitch);
				layout.Update(chip8.gfx,videoPitch);
			}
		}

    }else{

		std::exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}