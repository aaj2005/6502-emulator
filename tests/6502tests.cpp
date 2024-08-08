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

TEST_F( M6502Test1, LDAImmediateCanLoadAValueIntoARegister ){
    
    //  given: 
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    // end - inline a little program

    // when:
    cpu.Execute(2,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x84);

}


TEST_F( M6502Test1, LDAZeroPageCanLoadAValueIntoARegister ){
    
    //  given: 
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x37;
    // end - inline a little program

    // when:
    cpu.Execute(3,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);

}

TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoARegister ){
    
    //  given: 
    cpu.X = 5;
    // start - inline a little program
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x42;
    mem[0x0047] = 0x37;
    // end - inline a little program

    // when:
    cpu.Execute(4,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);

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
