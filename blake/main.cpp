//
// Created by kranya on 19.12.2019.
//
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <stdio.h>
#include <cstring>

using namespace std;

string globalMessage;
string smallMessage;

int permutations[10][16]={
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
        {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
        {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
        {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
        {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
        {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
        {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
        {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
        {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
        {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0}};

unsigned int h[8]={
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
};

unsigned int c[16]={
        0x243F6A88,
        0x85A308D3,
        0x13198A2E,
        0x03707344,
        0xA4093822,
        0x299F31D0,
        0x082EFA98,
        0xEC4E6C89,
        0x452821E6,
        0x38D01377,
        0xBE5466CF,
        0x34E90C6C,
        0xC0AC29B7,
        0xC97C50DD,
        0x3F84D5B5,
        0xB5470917
};

unsigned int s[4] = {
        s[0] = 0x0,
        s[1] = 0x0,
        s[2] = 0x0,
        s[3] = 0x0
};

int t[2]={0,0};

unsigned long int h0 = h[0];
unsigned long int h1 = h[1];
unsigned long int h2 = h[2];
unsigned long int h3 = h[3];
unsigned long int h4 = h[4];
unsigned long int h5 = h[5];
unsigned long int h6 = h[6];
unsigned long int h7 = h[7];

unsigned long int v0 = h0;
unsigned long int v1 = h1;
unsigned long int v2 = h2;
unsigned long int v3 = h3;
unsigned long int v4 = h4;
unsigned long int v5 = h5;
unsigned long int v6 = h6;
unsigned long int v7 = h7;

unsigned long int v8 = s[0]^c[0];
unsigned long int v9 = s[1]^c[1];
unsigned long int v10 = s[2]^c[2];
unsigned long int v11 = s[3]^c[3];
unsigned long int v12 = t[0]^c[4];
unsigned long int v13 = t[0]^c[5];
unsigned long int v14 = t[1]^c[6];
unsigned long int v15 = t[1]^c[7];


unsigned long int v[4][4] = {
        {v0, v1, v2,  v3},
        {v4, v5, v6,  v7},
        {v8, v9, v10, v11},
        {v12,v13,v14, v15}
};

string convert_int_hex(int n)
{
    string result;
    stringstream ss;
    ss << hex << n;
    ss >> result;
    return result;
}

void padding(string & m, int l, string hex)
{
    int i;
    int blockSum = 0;
    if(((l*4-447)%512==0) && l!=0)
        m = m + '9';
    else
        m = m + '8';

    for(i=0;i<l*4;i=i+4)
    {
        if((((i-447)%512)==0) && i!=0) blockSum++;
    }

    blockSum++;

    for(i = m.size()+1; i <= (blockSum)*128-17; i++)
    {
        m = m + '0';
    }

    if(((l*4-447)%512==0) && l!=0)
        m = m;
    else
        m = m + '1';

    for(i = (blockSum)*128-16; i <= (blockSum)*128-hex.size()-1; i++)
    {
        m = m + '0';
    }

    m = m + hex;

}


unsigned int rotl(unsigned long int x,int n)
{
    unsigned int y = (x >> n) | (x << (32 - n));
    return y;
}


unsigned int MESSAGEELECTION_TO_INT(string m, int i)
{
    string temp;
    temp.assign(m, i*8, 8);
    return strtoul(temp.c_str(), NULL, 16);
}


void G(int r, int i, string s)
{
    unsigned int A, B, C, D;
    if (i == 0)		{A = v[0][0]; B = v[1][0]; C = v[2][0]; D = v[3][0];}
    if (i == 1)		{A = v[0][1]; B = v[1][1]; C = v[2][1]; D = v[3][1];}
    if (i == 2)		{A = v[0][2]; B = v[1][2]; C = v[2][2]; D = v[3][2];}
    if (i == 3)		{A = v[0][3]; B = v[1][3]; C = v[2][3]; D = v[3][3];}
    if (i == 4)		{A = v[0][0]; B = v[1][1]; C = v[2][2]; D = v[3][3];}
    if (i == 5)		{A = v[0][1]; B = v[1][2]; C = v[2][3]; D = v[3][0];}
    if (i == 6)		{A = v[0][2]; B = v[1][3]; C = v[2][0]; D = v[3][1];}
    if (i == 7)		{A = v[0][3]; B = v[1][0]; C = v[2][1]; D = v[3][2];}
/*=========================================================================*/
    A = A + B + (MESSAGEELECTION_TO_INT(smallMessage, permutations[r%10][2*i])^c[permutations[r%10][(2*i)+1]]);
    D = rotl((D ^ A), 16);
    C = C + D;
    B = rotl((B^C), 12);
    A = A + B + (MESSAGEELECTION_TO_INT(smallMessage, permutations[r%10][2*i+1])^c[permutations[r%10][(2*i)]]);
    D = rotl((D^A), 8);
    C = C + D;
    B = rotl((B^C), 7);
/*=========================================================================*/
    if (i == 0)		{v[0][0] = A; v[1][0] = B; v[2][0] = C; v[3][0] = D;}
    if (i == 1)		{v[0][1] = A; v[1][1] = B; v[2][1] = C; v[3][1] = D;}
    if (i == 2)		{v[0][2] = A; v[1][2] = B; v[2][2] = C; v[3][2] = D;}
    if (i == 3)		{v[0][3] = A; v[1][3] = B; v[2][3] = C; v[3][3] = D;}
    if (i == 4)		{v[0][0] = A; v[1][1] = B; v[2][2] = C; v[3][3] = D;}
    if (i == 5)		{v[0][1] = A; v[1][2] = B; v[2][3] = C; v[3][0] = D;}
    if (i == 6)		{v[0][2] = A; v[1][3] = B; v[2][0] = C; v[3][1] = D;}
    if (i == 7)		{v[0][3] = A; v[1][0] = B; v[2][1] = C; v[3][2] = D;}
}


int main(int argc, char *argv[])
{
    int messageLenght;
    string hexMessageLenght;

    if(argc==1)
    {
        messageLenght=0;
    }
    else
    {
        globalMessage = argv[1];
        messageLenght = globalMessage.size();
    }

    hexMessageLenght = convert_int_hex(messageLenght*4);
    padding(globalMessage, messageLenght, hexMessageLenght);

    int blocks = 1;
    int i, blockZero, t[2];

    t[1] = 0;
    t[0] = 0;

    for(i=0;i<=messageLenght*4;i=i+4)
    {
        if(((i%((blocks-1)*512+448))==0) && (i!=0))
            blocks++;
    }

    if(((messageLenght*4)%512>444) && ((messageLenght*4)%512<512))
        blockZero = 1;
    else
        blockZero=0;

    for(i=1;i<=blocks;i++)
    {
        if(i < blocks-1)
            t[0]=i*512;
        if(i == blocks-1)
        {
            if(blockZero==1)
                t[0] = t[0]+(messageLenght*4)%512;
            else
                t[0] = t[0]+512;
        }
        if(i==blocks)
        {
            if(blockZero==1)
                t[0] = 0;
            else
                t[0] = t[0]+(messageLenght*4)%512;
        }

        smallMessage.assign(globalMessage, (i-1)*128, 128);

        v[3][0] = t[0]^c[4]; v[3][1] = t[0]^c[5]; v[3][2] = t[1]^c[6]; v[3][3] = t[1]^c[7];
        v[2][0] = s[0]^c[0]; v[2][1] = s[1]^c[1]; v[2][2] = s[2]^c[2]; v[2][3] = s[3]^c[3];


        int j,k;

        for(j=0; j<14; j++)
        {
            for(k=0; k<8; k++)
            {
                G(j,k,smallMessage);
            }
        }



        h0=h0^s[0]^v[0][0]^v[2][0];
        h1=h1^s[1]^v[0][1]^v[2][1];
        h2=h2^s[2]^v[0][2]^v[2][2];
        h3=h3^s[3]^v[0][3]^v[2][3];
        h4=h4^s[0]^v[1][0]^v[3][0];
        h5=h5^s[1]^v[1][1]^v[3][1];
        h6=h6^s[2]^v[1][2]^v[3][2];
        h7=h7^s[3]^v[1][3]^v[3][3];

        v[0][0]=h0; v[0][1]=h1; v[0][2]=h2; v[0][3]=h3;
        v[1][0]=h4; v[1][1]=h5; v[1][2]=h6; v[1][3]=h7;


    }

    printf("%0.8x",h0);
    printf("%0.8x",h1);
    printf("%0.8x",h2);
    printf("%0.8x",h3);
    printf("%0.8x",h4);
    printf("%0.8x",h5);
    printf("%0.8x",h6);
    printf("%0.8x",h7);

    return 0;
}
