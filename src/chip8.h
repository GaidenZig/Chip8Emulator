#include <cstdint>


class Chip8{
    public:
    Chip8();
    bool LoadRom(char const* filename);
    void EmulationCycle();

    void cpuNULL(); //nothing to do, use this to fill memory adresses
    void opcodes_table_0();
    void opcodes_table_8();
    void opcodes_table_E();
    void opcodes_table_F();

    void ( Chip8::*Table_opcodes[0xF + 1] )() = {&Chip8::cpuNULL};
    void ( Chip8::*Table_0xxx_opcodes[0xE +1])() = {&Chip8::cpuNULL};
    void ( Chip8::*Table_8xxx_opcodes[0xE +1])() = {&Chip8::cpuNULL};
    void ( Chip8::*Table_Exxx_opcodes[0xA1 +1])() = {&Chip8::cpuNULL};
    void ( Chip8::*Table_Fxxx_opcodes[0x65 +1])() = {&Chip8::cpuNULL};

    void OPC_0NNN();
    void OPC_00E0();
    void OPC_00EE();
    void OPC_1NNN();
    void OPC_2NNN();
    void OPC_3XNN();
    void OPC_4XNN();
    void OPC_5XY0();
    void OPC_6XNN();
    void OPC_7XNN();
    void OPC_8XY0();
    void OPC_8XY1();
    void OPC_8XY2();
    void OPC_8XY3();
    void OPC_8XY4();
    void OPC_8XY5();
    void OPC_8XY6();
    void OPC_8XY7();
    void OPC_8XYE();
    void OPC_9XY0();
    void OPC_ANNN();
    void OPC_BNNN();
    void OPC_CXNN();
    void OPC_DXYN();
    void OPC_EX9E();
    void OPC_EXA1();
    void OPC_FX07();
    void OPC_FX0A();
    void OPC_FX15();
    void OPC_FX18();
    void OPC_FX1E();
    void OPC_FX29();
    void OPC_FX33();
    void OPC_FX55();
    void OPC_FX65();

    uint8_t V[16]{}; //general pourpose registers from v[0x0] to v[0xf], wathout for "f"
    uint8_t memory[4096]{};
    uint16_t i; //index register
    uint16_t programCounter;
    uint16_t stack[16]{};
    uint8_t stackPointer{};
    uint32_t gfx[64*32]{}; //the screen resolution 64 x 32 holding 1 or 0 for each pixel
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};
    uint8_t keypad[16];
    uint16_t opcode;

    private:
};


