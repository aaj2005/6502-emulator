#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;


//memory
struct Mem{

    static constexpr u32 MAX_MEM = 1024 *64;
    Byte Data[MAX_MEM];

    void Initialise(){
        for(u32 i =0; i< MAX_MEM; i++){
            Data[i] = 0;
        }
    }

    // read 1 byte
    Byte operator[](u32 Address) const{

        // assert here Address is < MAX_MEM
        return Data[Address];
    }

    // write 1 byte
    Byte& operator[](u32 Address){

        // assert here Address is < MAX_MEM
        return Data[Address];
    }


};


struct CPU{

    

    Word PC; // Program Counter
    Word SP; // Stack Pointer


    Byte A, X, Y; // registers (Accumulator, Index Register X, Index Register Y)


    // status flags -- bitfields (saves memory so that each flag uses one bit rather a whole byte)
    Byte C : 1; // Carry Flags
    Byte Z : 1; // Zero Flag
    Byte I : 1; // Interrupt Disable
    Byte D : 1; // Decimal Mode
    Byte B : 1; // Break Command
    Byte V : 1; // Overflow Flag
    Byte N : 1; // Negative Flag


    //turn on CPU
    void Reset(Mem& memory){
        PC = 0xFFFC; // initialise PC address
        SP  = 0x0100; // initialise stack pointer address
        C = Z = I = B = V = N = D = 0; // initialse flags to 0 
        A = X = Y = 0; // initialise registers to 0

        memory.Initialise();

    }

    Byte FetchByte(u32& Cycles, Mem& memory){
        Byte Data = memory[PC]; //get data from PC
        PC++; //Increment counter
        Cycles--; //used up one cycle
        return Data;
    }


    //fetch byte but without incrementing PC
    Byte ReadByte(u32& Cycles, Byte Address, Mem& memory){
        Byte Data = memory[Address]; //get data from PC
        Cycles--; //used up one cycle
        return Data;
    }

    //opcodes
    static constexpr Byte 
        INS_LDA_IM =0xA9, // Load Accumulator with Immediate
        INS_LDA_ZP = 0xA5 // Load Accumulator with Zero Page (first 256 bytes of memory) 
        ;

    void LDAStatus(){
        Z = (A == 0); // zero flag is set if A == 0
        N = (A & 0b10000000) > 0; // negative flag set if bit 7 of Acc is set
    }

    // Cycles: number of cycles needed to execute some instruction
    void Execute(u32 Cycles, Mem& memory){
        
        while( Cycles > 0){
            //fetch next instruction
            Byte Ins = FetchByte( Cycles, memory ); // 1 clock cycle
            switch (Ins){
                
                // executes the load accumulator with immediate instruction  
                case INS_LDA_IM:{
                    // byte retrieved comes immediately after instruction opcode
                    Byte Value = FetchByte (Cycles, memory); // 1 clock cycle
                    A = Value; // set accumulator to fetched value
                    LDAStatus(); // default LDA flag setting
                }break;


                // execute the load immediate with zero page instruction
                case INS_LDA_ZP:{
                    Byte ZeroPageAddress = FetchByte (Cycles, memory); // 1 clock cycle
                    A = ReadByte(Cycles, ZeroPageAddress, memory); // 1 clock cycle 
                    LDAStatus(); // default LDA flag setting

                }break;

                default:
                {
                    printf("Instruction not handeled %d", Ins);
                }break;
            
            
            }
        }

    }

};

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