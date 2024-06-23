#include <stdio.h>
#include <stdlib.h>

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

};

int main(){
    
    Mem mem;
    CPU cpu;

    cpu.Reset(mem);

    return 0;
}