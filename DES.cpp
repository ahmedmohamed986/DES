#include <iostream>
#include <string>
#ifdef __GNUC__
#define __rdtsc __builtin_ia32_rdtsc
#else
#include<intrin.h>
#endif
using namespace std;
typedef unsigned long long u64;

u64 HextoDec(string);
void encrypt(string,string,int);
u64 calcInitialPermutation(u64 text);
u64 calculateEfficientKey(u64 keyDecimal);
u64 RotateShiftLeft(u64 keyDecimal,int n);
u64 calcPermutation(u64 data);
u64 expand(u64 data);
u64 calcSBox(u64 data);
u64 calcFinalPermutation(u64 output);
int main(int argc, char **argv)
{
    string programName = argv[0];
    string operation = argv[1];
    string text = argv[2];
    string key = argv[3];
    if(operation == "encrypt")
    {
      encrypt(text,key,1);
    }
    else if(operation == "decrypt")
    {
       encrypt(text,key,0);
    }

    return 0;
}
u64 HextoDec(string data)
{
    u64 val=0;
    for(int i = 0;i < 16;i++)
    {
        if(data[i] >= '0' && data[i] <= '9')
        {
            val |= (u64)(data[i]-'0')<<((15-i)<<2);
        }
        else if(data[i] >= 'A' && data[i] <= 'F')
        {
            val |= (u64)(data[i]-'A'+10)<<((15-i)<<2);
        }
        else if(data[i] >= 'a' && data[i] <= 'f')
        {
            val |= (u64)(data[i]-'a'+10)<<((15-i)<<2);
        }
    }
    return val;
}
u64 calcInitialPermutation(u64 text)
{
    u64 IP = 0;
    char Permutation [64] = {58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
    for(int i = 0;i < 64;i++)
    {
        IP |= (text>>(64-Permutation[i])&1)<<64-(i+1); 
    }
    return IP;
}
u64 calculateEfficientKey(u64 keyDecimal)
{
    u64 efficientKey = 0;
    char Permutation [56] = {57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
    for(int i = 0;i < 56;i++)
    {
        efficientKey |= (keyDecimal>>(64-Permutation[i])&1)<<(56-(i+1)); 
    }
    return efficientKey;
}
u64 RotateShiftLeft(u64 keyDecimal,int n)
{
    keyDecimal = ((keyDecimal << n) | (keyDecimal >>(28-n))) & 0x000000000FFFFFFF;
    return keyDecimal;
}

u64 calculatePermutationChoice2(u64 key)
{
    u64 out = 0;
    char Permutation[48] = {14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
    for(int i = 0;i < 48;i++)
    {
        out |= (key>>(56-Permutation[i])&1)<<(48-(i+1)); 
    }
    return out;
}
u64 expand(u64 data)
{
    u64 out = 0;
    char Permutation[48] = {32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
     for(int i = 0;i < 48;i++)
    {
        out |= (data>>(32-Permutation[i])&1)<<(48-(i+1)); 
    }
    return out;
}
u64 calcSBox(u64 data)
{
    char sBoxes[8][64] = {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9},{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }};
    u64 out = 0;
    u64 index = 0;
    for(int i = 0;i < 8;i++)
    {
        index = data>>(7-i)*6 & 0x3F;
        index = index>>1&15|(index&1)<<4|index&0x020;
        out |= sBoxes[i][index]<<(7-i)*4;
    }
    return out;
}

u64 calcPermutation(u64 data)
{
    u64 out = 0;
    char Permutation[32] = { 16, 7, 20, 21,29, 12, 28, 17,1, 15, 23, 26,5, 18, 31, 10,2, 8, 24, 14,32, 27, 3, 9,19, 13, 30, 6,22, 11, 4, 25 };
    for(int i = 0;i < 32;i++)
    {
        out |= (data>>(32-Permutation[i])&1)<<(32-(i+1)); 
    }
    return out;
}
u64 calcFinalPermutation(u64 output)
{
    u64 out = 0;
    char Permutation[64] = { 40, 8, 48, 16, 56, 24, 64, 32,39, 7, 47, 15, 55, 23, 63, 31,38, 6, 46, 14, 54, 22, 62, 30,37, 5, 45, 13, 53, 21, 61, 29,36, 4, 44, 12, 52, 20, 60, 28,35, 3, 43, 11, 51, 19, 59, 27,34, 2, 42, 10, 50, 18, 58, 26,33, 1, 41, 9, 49, 17, 57, 25 };
    for(int i = 0;i < 64;i++)
    {
        out |= (output>>(64-Permutation[i])&1)<<(64-(i+1)); 
    }
    return out;
}

void encrypt(string text,string key,int encrypt)
{
    //Converting to u64
    u64 textDecimal = HextoDec(text);
    u64 keyDecimal = HextoDec(key);
    
    //Calculating efficient key & keys generation
    keyDecimal = calculateEfficientKey(keyDecimal);
    u64 C0 = (keyDecimal & 0x00FFFFFFF0000000)>>28;
    u64 D0 = keyDecimal & 0x000000000FFFFFFF;
    
    //RotateShiftLeft
    u64 keys[16];
    int iteration = 0;
    u64 Cn = 0;
    u64 Dn = 0;
    
    while(iteration < 16)
    {
        if(iteration == 0 | iteration == 1 | iteration == 8 | iteration == 15)
        {
            Cn = RotateShiftLeft(C0,1);
            Dn = RotateShiftLeft(D0,1);
            keys[iteration] = (Cn << 28) | Dn;
        }
        else
        {
            Cn = RotateShiftLeft(C0,2);
            Dn = RotateShiftLeft(D0,2);
            keys[iteration] = (Cn << 28) | Dn;
        }
         C0 = Cn;
         D0 = Dn;
        iteration++;
    }
    iteration = 0;
      while(iteration < 16)
      {
        keys[iteration] = calculatePermutationChoice2(keys[iteration]);
        iteration++;
      }

    iteration = 0;
    u64 L0 = 0;
    u64 R0 = 0;
    long long t1 = __rdtsc();
    u64 IP = calcInitialPermutation(textDecimal);
    L0 = (IP & 0xFFFFFFFF00000000)>>32;
    R0 = IP & 0x00000000FFFFFFFF;

    while(iteration < 16)
    {
    u64 temp = R0;
    R0 = expand(R0);
    if(encrypt == 1)
    {
     R0 = R0^keys[iteration];
    }
    else
    {
     R0 = R0^keys[15-iteration];
    }
    R0 = calcSBox(R0);
    R0 = calcPermutation(R0);
    R0 = R0^L0;
    L0 = temp;
    iteration++;
    }
    u64 output = L0 | R0<<32;
    output = calcFinalPermutation(output);
    long long t2 = __rdtsc();
    printf("Cipher: %016llX\n",output);
    printf("Cycles: %lld\n",t2-t1);
    
    
}