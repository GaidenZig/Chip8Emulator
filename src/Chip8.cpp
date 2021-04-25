#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "chip8.h"

using std::cout;

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS =0x50;

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}


uint8_t fontset[FONTSET_SIZE] =
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

Chip8::Chip8(){
    programCounter = START_ADDRESS;

    Table_opcodes[0x0]= &Chip8::opcodes_table_0;
    Table_opcodes[0x1]= &Chip8::OPC_1NNN;
    Table_opcodes[0x2]= &Chip8::OPC_2NNN;
    Table_opcodes[0x3]= &Chip8::OPC_3XNN;
    Table_opcodes[0x4]= &Chip8::OPC_4XNN;
    Table_opcodes[0x5]= &Chip8::OPC_5XY0;
    Table_opcodes[0x6]= &Chip8::OPC_6XNN;
    Table_opcodes[0x7]= &Chip8::OPC_7XNN;
    Table_opcodes[0x8]= &Chip8::opcodes_table_8;
    Table_opcodes[0x9]= &Chip8::OPC_9XY0;
    Table_opcodes[0xA]= &Chip8::OPC_ANNN;
    Table_opcodes[0xB]= &Chip8::OPC_BNNN;
    Table_opcodes[0xC]= &Chip8::OPC_CXNN;
    Table_opcodes[0xD]= &Chip8::OPC_DXYN;
    Table_opcodes[0xE]= &Chip8::opcodes_table_E;
    Table_opcodes[0xF]= &Chip8::opcodes_table_F;

    Table_0xxx_opcodes[0x0]= &Chip8::OPC_00E0;
    Table_0xxx_opcodes[0xE]= &Chip8::OPC_00EE;

    Table_8xxx_opcodes[0x0]= &Chip8::OPC_8XY0;
    Table_8xxx_opcodes[0x1]= &Chip8::OPC_8XY1;
    Table_8xxx_opcodes[0x2]= &Chip8::OPC_8XY2;
    Table_8xxx_opcodes[0x3]= &Chip8::OPC_8XY3;
    Table_8xxx_opcodes[0x4]= &Chip8::OPC_8XY4;
    Table_8xxx_opcodes[0x5]= &Chip8::OPC_8XY5;
    Table_8xxx_opcodes[0x6]= &Chip8::OPC_8XY6;
    Table_8xxx_opcodes[0x7]= &Chip8::OPC_8XY7;
    Table_8xxx_opcodes[0xE]= &Chip8::OPC_8XYE;

    Table_Exxx_opcodes[0x9E]= &Chip8::OPC_EX9E;
    Table_Exxx_opcodes[0xA1]= &Chip8::OPC_EXA1;

    Table_Fxxx_opcodes[0x07]= &Chip8::OPC_FX07;
    Table_Fxxx_opcodes[0x0A]= &Chip8::OPC_FX0A;
    Table_Fxxx_opcodes[0x15]= &Chip8::OPC_FX15;
    Table_Fxxx_opcodes[0x18]= &Chip8::OPC_FX18;
    Table_Fxxx_opcodes[0x1E]= &Chip8::OPC_FX1E;
    Table_Fxxx_opcodes[0x29]= &Chip8::OPC_FX29;
    Table_Fxxx_opcodes[0x33]= &Chip8::OPC_FX33;
    Table_Fxxx_opcodes[0x55]= &Chip8::OPC_FX55;
    Table_Fxxx_opcodes[0x65]= &Chip8::OPC_FX65;


    //Load Fonts in memory
    for(unsigned int i = 0; i < FONTSET_SIZE; ++i ){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }
}

bool Chip8::LoadRom(char const* filename){
    
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(file.is_open()){
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for(long i = 0; i < size; ++i){
            // std::cout << "Opcode: " << hex(buffer[i])<< " | ";
            // std::system("pause");
            memory[START_ADDRESS +i] =buffer[i];
        }

        delete[] buffer;
    }else{
        return false;
    }

	return true;
}

void Chip8::cpuNULL(){
    //Look mom i'm doing nothing hehe :v
}

void Chip8::opcodes_table_0(){
    if (opcode < 0x00E0 || opcode > 0x00EE)
    {
        std::cout << "it's using the 0NNN opcode...\n";
    }else{

        ((*this).*(Table_0xxx_opcodes[(opcode & 0x000Fu)]))();
    }

}

void Chip8::opcodes_table_8(){
    ((*this).*(Table_8xxx_opcodes[(opcode & 0x000Fu)]))();
}

void Chip8::opcodes_table_E(){
    ((*this).*(Table_Exxx_opcodes[(opcode & 0x00FFu)]))();
}

void Chip8::opcodes_table_F(){
    ((*this).*(Table_Fxxx_opcodes[(opcode & 0x00FFu)]))();
}

void Chip8::OPC_00E0(){
    memset(gfx,0,sizeof(gfx));
    // for( long i = 0; i < (sizeof(gfx)/sizeof(*gfx)); ++i ){
    //     gfx[i] = 0;
    // }
}

void Chip8::OPC_00EE(){
    --stackPointer;
    programCounter = stack[stackPointer];
}

void Chip8::OPC_1NNN(){
    programCounter = opcode & 0x0FFFu;
}

void Chip8::OPC_2NNN(){
    stack[stackPointer] = programCounter;
    stackPointer++;
    programCounter = opcode & 0x0FFFu;
}

void Chip8::OPC_3XNN(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t NN = opcode & 0x00FF;
    
    if( V[x] == NN){
        programCounter += 2;
    }
}

void Chip8::OPC_4XNN(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t NN = opcode & 0x00FF;

    if( V[x] != NN){
        programCounter += 2;
    }
}

void Chip8::OPC_5XY0(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if( V[x] == V[y]){
        programCounter += 2;
    }
}

void Chip8::OPC_6XNN(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t NN = opcode & 0x00FF;

    V[x] = NN;
}

void Chip8::OPC_7XNN(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t NN = opcode & 0x00FF;

    V[x] = V[x] +   NN;
}

void Chip8::OPC_8XY0(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[y];
}

void Chip8::OPC_8XY1(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] | V[y];
}

void Chip8::OPC_8XY2(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] & V[y];
}

void Chip8::OPC_8XY3(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] ^ V[y];
}

void Chip8::OPC_8XY4(){
    V[0xF] = 0;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sum =V[x] + V[y];

    if(sum > 255){
        V[0xF] = 1;
    }

    V[x] = sum & 0xFF; //this will directly restrict the memory used for the register location to a 1byte number(255).
}

void Chip8::OPC_8XY5(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint16_t sub = V[x] - V[y];

    if(V[x] > V[y]){
        V[0xF] = 1;
    }else{
        V[0xF] = 0;
    }
    
    V[x] = sub;
}

void Chip8::OPC_8XY6(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x1;

    V[x] = V[x] >> 1; // " x>>1 == x/2, (o_o') it's completly new to me... "
}

// i don't know wich one of those two are gona work correctly so i commented this one because of its ambiguous description in
// Matthew Mikolay's blog.

// void Chip8::OPC_8XY6(){
//     uint8_t x = (opcode & 0x0F00) >> 8;  
//     uint8_t y = (opcode & 0x00F0) >> 4;

//     V[0xF] = V[y] & 0x1;
//     V[x] = V[y] >> 1;
// }

void Chip8::OPC_8XY7(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0)  >> 4;
    uint16_t sub = V[y] - V[x];

    if(V[y] > V[x]){
        V[0xF] = 1;
    }else{
        V[0xF] = 0;
    }
    
    V[x] = sub;
}

void Chip8::OPC_8XYE(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[0xF] = (V[x] & 0x80) >> 7;
    V[x] = V[x] << 1; // yeah... left-shiting it is equivalent to multiply it by 2, a brand new world is opening in front of my eyes (o_o')
}

//void Chip8::OPC_8XYE(){}

void Chip8::OPC_9XY0(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    if(V[x] != V[y]){
        programCounter += 2;
    }
}

void Chip8::OPC_ANNN(){
    uint16_t memAdress = opcode & 0x0FFF;
    i = memAdress;
}

void Chip8::OPC_BNNN(){
    programCounter = (opcode & 0x0FFF) + V[0];
}

void Chip8::OPC_CXNN(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t NN = opcode & 0x00FF;

    V[x] = (rand() % 0xFF) & NN;
}

void Chip8::OPC_DXYN(){ //PENDING CORRECTIONS...
    uint8_t x = V[(opcode & 0x0F00) >> 8];
    uint8_t y = V[(opcode & 0x00F0) >> 4];
    uint8_t nBytes = opcode & 0x000F;
	uint8_t xPos = x;uint8_t nxPos;
	uint8_t yPos = y;uint8_t nyPos;

	V[0xF] = 0;

	for (unsigned int row = 0; row < nBytes; ++row)
	{
		uint8_t spriteByte = memory[i + row];

		for (unsigned int col = 0; col < 8; ++col)
		{
			uint8_t spritePixel = spriteByte & (0x80u >> col);
            nxPos = xPos + col;
            nyPos = yPos + row;
            nxPos = nxPos%64;
            nyPos = nyPos%32;
			uint32_t* screenPixel = &gfx[(nyPos) * 64 + (nxPos)];

			// Sprite pixel is on
			if (spritePixel)
			{
				// Screen pixel also on - collision
				if (*screenPixel == 0xFFFFFFFF)
				{
					V[0xF] = 1;
				}

				// Effectively XOR with the sprite pixel
				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}

    // uint8_t pixel;

    // V[0xF] = 0; 
    // for (size_t ySpritePixel = 0; ySpritePixel < nBytes; ySpritePixel++){
    //     pixel = memory[i + ySpritePixel];
    //     for (size_t xSpritePixel = 0; xSpritePixel < 8; xSpritePixel++){
    //         if(((pixel & (0x80 >> xSpritePixel)))){
    //             if(*(&gfx[((x + xSpritePixel) + ((y + ySpritePixel) * 64))] ) == 1){
    //                 V[0xF] == 1;
    //             }
    //             *(&gfx[((x + xSpritePixel) + ((y + ySpritePixel) * 64))]) ^= 1;
    //         }
    //     }
        
    // }
}

void Chip8::OPC_EX9E(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = V[x];

    if(keypad[key]){
        programCounter +=2;
    }
}

void Chip8::OPC_EXA1(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t key = V[x];

    if(!keypad[key]){
        programCounter +=2;
    }
}

void Chip8::OPC_FX07(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[x] = delayTimer;
}

void Chip8::OPC_FX0A(){
    // uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	// if (keypad[0])
	// {
	// 	V[Vx] = 0;
	// }
	// else if (keypad[1])
	// {
	// 	V[Vx] = 1;
	// }
	// else if (keypad[2])
	// {
	// 	V[Vx] = 2;
	// }
	// else if (keypad[3])
	// {
	// 	V[Vx] = 3;
	// }
	// else if (keypad[4])
	// {
	// 	V[Vx] = 4;
	// }
	// else if (keypad[5])
	// {
	// 	V[Vx] = 5;
	// }
	// else if (keypad[6])
	// {
	// 	V[Vx] = 6;
	// }
	// else if (keypad[7])
	// {
	// 	V[Vx] = 7;
	// }
	// else if (keypad[8])
	// {
	// 	V[Vx] = 8;
	// }
	// else if (keypad[9])
	// {
	// 	V[Vx] = 9;
	// }
	// else if (keypad[10])
	// {
	// 	V[Vx] = 10;
	// }
	// else if (keypad[11])
	// {
	// 	V[Vx] = 11;
	// }
	// else if (keypad[12])
	// {
	// 	V[Vx] = 12;
	// }
	// else if (keypad[13])
	// {
	// 	V[Vx] = 13;
	// }
	// else if (keypad[14])
	// {
	// 	V[Vx] = 14;
	// }
	// else if (keypad[15])
	// {
	// 	V[Vx] = 15;
	// }
	// else
	// {
	// 	programCounter -= 2;
	// }

    uint8_t x = (opcode & 0x0F00) >> 8;
    bool keyPressed = false;
    
    for (size_t c = 0; c < 16; c++){
        if (keypad[c]){
            V[x] = c;
            keyPressed = true;
            break;
        }
    }

    if (!keyPressed){
        programCounter -= 2;
    }
}

void Chip8::OPC_FX15(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    delayTimer = V[x];
}

void Chip8::OPC_FX18(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    soundTimer = V[x];
}

void Chip8::OPC_FX1E(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    i = V[x] + i;
}

void Chip8::OPC_FX29(){
    uint8_t x = (opcode & 0x0F00) >> 8;
    i = FONTSET_START_ADDRESS + (5 * V[x]); 
// just think about this, x must be a single hexadecimal digit that means it have to be from 0 - F,
// so if we want to star to found the fonts that are stored in memory we have to use "FONTSET_START_ADDRESS",
// then we know that each one of the fonts in chip8 are represent with a group of 5 bytes in "fontset" and then
// they are stored in memory one by one, so that means that each font of chip8 can be found on memory every 5 slot
// in "memory[]" array, but starting the count from an offset equal of "FONTSET_START_ADDRESS".
// and then this opcode just set i to the "address" of the font equal to x, so if we want to do this, we just can 
// set i to the first slot in memory[] that thanks to the following 4 slots, represents the font settled in "x" (line 2 in the function).
}

void Chip8::OPC_FX33(){
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = V[Vx];

	// Ones-place
	memory[i + 2] = value % 10;
	value /= 10;

	// Tens-place
	memory[i + 1] = value % 10;
	value /= 10;

	// Hundreds-place
	memory[i] = value % 10;

//     uint8_t x = (opcode & 0x0F00) >> 8;
//     uint8_t Vx = V[x];
    
// // "x | 0 " gives x truncated.
//     memory[i+2] = Vx % 10;
//     memory[i+1] = ((Vx/10) | 0) % 10;
//     memory[i] = (((Vx/10) | 0) / 10) | 0;
}

void Chip8::OPC_FX55(){
    uint8_t x = (opcode & 0x0F00) >> 8;

    for (long c = 0; c <= x; c++){
        memory[i + c] = V[c];
    }
}

void Chip8::OPC_FX65(){
    uint8_t x = (opcode & 0x0F00) >> 8;

    for (long c = 0; c <= x; c++){
        V[c] = memory[i + c];
    }
    
}


void Chip8::EmulationCycle(){
    //Fetch phase
    opcode = (memory[programCounter] << 8) | memory[programCounter + 1];
    programCounter += 2;
    if(opcode == 0x13DC){
        std::cout<<"stop here... you're violating the law"<<std::endl;
    }
    //Decode phase        
    ((*this).*(Table_opcodes[(opcode & 0xF000u) >> 12u]))();

    //Update timers
    if(delayTimer > 0){
        --delayTimer;
    }
    if(soundTimer > 0){
        --soundTimer;
    }

}
