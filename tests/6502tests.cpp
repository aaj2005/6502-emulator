#include <gtest/gtest.h>
#include "main_6502.h"

class M6502Test1 : public testing::Test{
    public:
        Mem mem;
        CPU cpu;

        
        virtual void SetUp(){
            cpu.Reset(mem);
        }

        virtual void TearDown(){

        }
};

TEST_F( M6502Test1, RunALittleInlineProgram){
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    // end - inline a little program
    cpu.Execute(9,mem);

}


#if 0
#include "main_6502.h"

int main(){
    
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    cpu.Execute(3,mem);
    return 0;
}

#endif
