#include <gtest/gtest.h>
#include "main_6502.h"

class M6502Test1 : public testing::Test{
    protected:
        Mem mem;
        CPU cpu;

        
        virtual void SetUp(){
            cpu.Reset(mem);
        }

        virtual void TearDown(){

        }
};


static void verifyUnchangedFlagsFromLDA(const CPU& cpu, const CPU& CPUCopy){
    EXPECT_EQ(cpu.C, CPUCopy.C);
    EXPECT_EQ(cpu.I, CPUCopy.I);
    EXPECT_EQ(cpu.D, CPUCopy.D);
    EXPECT_EQ(cpu.B, CPUCopy.B);
    EXPECT_EQ(cpu.V, CPUCopy.V );
}

TEST_F( M6502Test1, CPUDoesNothingWhen0CyclesExecuted ){
    
    // given:
    constexpr s32 NUM_CYCLES = 0;
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES,mem);
    
    // then 
    EXPECT_EQ(CyclesUsed, 0);

}


TEST_F( M6502Test1, CPUCanExecuteMoreCyclesThanRequestedIfRequiredByInstruction ){
    
    //  given: 
    
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 0;

    // when:
    s32 CyclesUsed = cpu.Execute(2,mem);

    

    // then:
    EXPECT_EQ(CyclesUsed, 2);
}

TEST_F( M6502Test1, ExecutingBadInstructionDoesNotPutUsInInfiniteLoop ){
    
    //  given: 
    mem[0xFFFC] = 0x0; // invalid instruction/opcode
    mem[0xFFFD] = 0x0;
    CPU CPUCopy = cpu;
    constexpr s32 NUM_CYCLES = 2;

    // when:
    s32 CyclesUsed = cpu.Execute(NUM_CYCLES,mem);

    

    // then:
    EXPECT_EQ(CyclesUsed, NUM_CYCLES);
}


TEST_F( M6502Test1, LDAImmediateCanLoadAValueIntoARegister ){
    
    //  given: 
    
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x84;
    

    // when:
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(2,mem);

    

    // then:
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_EQ(CyclesUsed, 2);
    EXPECT_FALSE(cpu.Z);
    EXPECT_TRUE(cpu.N);
    verifyUnchangedFlagsFromLDA(cpu, CPUCopy);

}


TEST_F( M6502Test1, LDAZeroPageCanLoadAValueIntoARegister ){
    
    //  given: 
    
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x37;
    

    // when:
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(3,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, 3);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    verifyUnchangedFlagsFromLDA(cpu, CPUCopy);


}



TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoARegister ){
    
    //  given: 
    cpu.X = 5;
    
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x42;
    mem[0x0047] = 0x37;
    

    // when:
    CPU CPUCopy = cpu;
    s32 CyclesUsed = cpu.Execute(4,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, 4);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    verifyUnchangedFlagsFromLDA(cpu, CPUCopy);
    

}


TEST_F( M6502Test1, LDAZeroPageXCanLoadAValueIntoARegisterWhenItWraps ){
    
    //  given: 
    cpu.X = 0xFF;
    
    mem[0xFFFC] = CPU::INS_LDA_ZPX;
    mem[0xFFFD] = 0x80;
    mem[0x007F] = 0x37;
    

    // when:
    cpu.Execute(4,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);

}

TEST_F( M6502Test1, LDAAbsoluteCanLoadAValueIntoTheRegister ){
    
    //  given: 
    cpu.X = 0xFF;
    
    mem[0xFFFC] = CPU::INS_LDA_ABS;
    mem[0xFFFD] = 0x80;
    mem[0xFFFE] = 0x44; // 0x4480
    mem[0x4480] = 0x37; // 0x4480
    constexpr s32 EXPECTED_CYCLES = 4;    
    CPU CPUCopy = cpu;

    // when:
    s32 CyclesUsed = cpu.Execute(4,mem);

    // then:
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
    EXPECT_FALSE(cpu.Z);
    EXPECT_FALSE(cpu.N);
    verifyUnchangedFlagsFromLDA(cpu, CPUCopy);

}



#if 0
#include "main_6502.h"

int main(){
    
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    cpu.Execute(3,mem);
    return 0;
}

#endif
