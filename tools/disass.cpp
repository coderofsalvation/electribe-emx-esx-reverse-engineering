#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>

#define b0000 0x0
#define b0001 0x1
#define b0010 0x2
#define b0011 0x3
#define b0100 0x4
#define b0101 0x5
#define b0110 0x6
#define b0111 0x7
#define b1000 0x8
#define b1001 0x9
#define b1010 0xA
#define b1011 0xB
#define b1100 0xC
#define b1101 0xD
#define b1110 0xE
#define b1111 0xF

using namespace std;


int convertIntMain(unsigned int number, int nbrNibble, bool Signed = true)
{
    number &= 0xFFFFFFFF>>4*(8-nbrNibble);
	if(Signed && number&(0x8<<((nbrNibble-1)*4))) //if negatif
    {
        number |= (0xFFFFFFFF<<4*(nbrNibble))&0xFFFFFFFF;
    }
    return (int)number;
}
int convertInt2NibbleS(int number){
    return convertIntMain(number, 2, true);}
int convertInt2NibbleU(int number){
    return convertIntMain(number, 2, false);}
int convertInt3NibbleS(int number){
    return convertIntMain(number, 3, true);}
int convertInt8NibbleS(int number){
    return convertIntMain(number, 8, true);}

string convertHexMain(unsigned int number, int nbrNibble, bool Signed = true, int multiple = 1)
{
	stringstream ss;
	if(Signed && number&(0x8<<((nbrNibble-1)*4))) //if negatif
    {
        ss << '-';
        number = (~number)+1;
        /* number *= multiple;
        ss << hex << number;
        return ss.str();
        /*
        string retour = ss.str();
        if(retour.length() <= (retour.length()-nbrNibble+1))
            return retour;
        else
            return retour.substr(retour.length()-(nbrNibble+1),(nbrNibble+1));
    }
	else
    {
        number &= 0xFFFFFFFF>>4*(8-nbrNibble);
        number *= multiple;
        ss << hex << number;
        return ss.str();*/
        /*
        string retour = ss.str();
        if(retour.length() <= (retour.length()-nbrNibble))
            return retour;
        else
            return retour.substr(retour.length()-(nbrNibble),(nbrNibble));
        */
    }
        number &= 0xFFFFFFFF>>4*(8-nbrNibble);
        number *= multiple;
        ss << hex << number;
        return ss.str();

}

string convertHexNibbleS(int number,int multiple=1){
    return convertHexMain(number, 1, true, multiple);}
string convertHexNibbleU(int number,int multiple=1){
    return convertHexMain(number, 1, false, multiple);}

string convertHex2NibbleS(int number,int multiple=1){
    return convertHexMain(number, 2, true, multiple);}
string convertHex2NibbleU(int number,int multiple=1){
    return convertHexMain(number, 2, false, multiple);}

string convertHex3NibbleS(int number,int multiple=1){
    return convertHexMain(number, 3, true, multiple);}
string convertHex3NibbleU(int number,int multiple=1){
    return convertHexMain(number, 3, false, multiple);}

string convertHex8NibbleS(int number,int multiple=1){
    return convertHexMain(number, 8, true, multiple);}
string convertHex8NibbleU(int number,int multiple=1){
    return convertHexMain(number, 8, false, multiple);}



string convertInt(int number, int base = 10)
{
	stringstream ss;
	if(base == 16)
		ss << hex << number;
	else
		ss << number;
	return ss.str();
}


int main(int argc, char **argv)
{
	cout << "SuperH Disassembleur v1.01" << endl;
	cout << "	by Ziqumu for Planet-casio.com and friends" << endl;
	//recuperation des parametres
		if(argc <= 1)
		{
			cerr << "Please give a filename in parameter !" << endl;
			cout << endl << "Press any key to continue." << endl;
			cin.ignore( numeric_limits<streamsize>::max(), '\n' );
			return 0;
		}
		string outputFilename;
		if(argc >= 3)
			outputFilename = argv[2];
		else
			outputFilename = "output.txt";
//Ouverture des fichier
    ifstream fichier(argv[1], ios::in | ios::binary);
    ofstream outFile(outputFilename.c_str());
    if(fichier)
    {
        cout << "Processing.." << endl;
		char buffer[2];
		string temp="";
		unsigned int offset = 0x0;
		unsigned char nibble[4];
		while(fichier.read(buffer, 2))
		{
				nibble[0] = ((unsigned)buffer[0]); nibble[0] >>= 4;
				nibble[1] = ((unsigned)buffer[0]); nibble[1] &= 0xF;
				nibble[2] = ((unsigned)buffer[1]); nibble[2] >>= 4;
				nibble[3] = ((unsigned)buffer[1]); nibble[3] &= 0xF;
			//Interpretation
			string interpretation="";
			switch(nibble[0])
			{
				case b0000:
					switch(nibble[3])
					{
						case b1000:
							switch(nibble[2]) {
								case b0000: interpretation = "clrt";	break;
								case b0001: interpretation = "sett";	break;
								case b0010: interpretation = "clrmac";	break;
								case b0011: interpretation = "ldtlb";	break;
								case b0100: interpretation = "clrs";	break;
								case b0101: interpretation = "sets";	break;
							} break;
						case b1001:
							switch(nibble[2]) {
								case b0000: interpretation = "nop";	break;
								case b0001: interpretation = "div0u";	break;
								case b0010: interpretation = "movt  r" +  convertInt(nibble[1]);	break;
							} break;
						case b1011:
							switch(nibble[2]) {
								case b0000: interpretation = "rts";	break;
								case b0001: interpretation = "sleep";	break;
								case b0010: interpretation = "rte";	break;
							} break;
						case b0010:
							switch(nibble[2]){
								case b0000: interpretation = "stc  sr, r" + convertInt(nibble[1]);	break; //0000nnnn00000010
								case b0001: interpretation = "stc  gbr, r" + convertInt(nibble[1]);	break; //0000nnnn00010010
								case b0010: interpretation = "stc  vbr, r" + convertInt(nibble[1]);	break; //0000nnnn00100010
								case b0011: interpretation = "stc  ssr, r" + convertInt(nibble[1]);	break; //0000nnnn00110010
								case b0100: interpretation = "stc  spc, r" + convertInt(nibble[1]);	break; //0000nnnn01000010
								case b1000: interpretation = "stc  r0_bank, r" + convertInt(nibble[1]);	break; //0000nnnn10000010
								case b1001: interpretation = "stc  r1_bank, r" + convertInt(nibble[1]);	break; //0000nnnn10010010
								case b1010: interpretation = "stc  r2_bank, r" + convertInt(nibble[1]);	break; //0000nnnn10100010
								case b1011: interpretation = "stc  r3_bank, r" + convertInt(nibble[1]);	break; //0000nnnn10110010
								case b1100: interpretation = "stc  r4_bank, r" + convertInt(nibble[1]);	break; //0000nnnn11000010
								case b1101: interpretation = "stc  r5_bank, r" + convertInt(nibble[1]);	break; //0000nnnn11010010
								case b1110: interpretation = "stc  r6_bank, r" + convertInt(nibble[1]);	break; //0000nnnn11100010
								case b1111: interpretation = "stc  r7_bank, r" + convertInt(nibble[1]);	break; //0000nnnn11110010
							} break;
						case b0011:
							switch(nibble[2]){
								case b0000: interpretation = "bsrf  r" + convertInt(nibble[1]);	break; //0000nnnn00000011
								case b0010: interpretation = "braf  r" + convertInt(nibble[1]);	break; //0000nnnn00100011
								case b1000: interpretation = "pref  @r" + convertInt(nibble[1]);	break; //0000nnnn10000011
							}break;
						case b1010:
							switch(nibble[2]) {
							case b0000: interpretation = "sts  mach, r" + convertInt(nibble[1]);	break; //0000nnnn00001010
							case b0001: interpretation = "sts  macl, r" + convertInt(nibble[1]);	break; //0000nnnn00011010
							case b0010: interpretation = "sts  pr, r" + convertInt(nibble[1]);	break; //0000nnnn00101010
							} break;
						case b0100: interpretation = "mov.b  r" + convertInt(nibble[2]) + ", @(r0,r" + convertInt(nibble[1]) + ")"; break; //0000nnnnmmmm0100
						case b0101: interpretation = "mov.w  r" + convertInt(nibble[2]) + ", @(r0,r" + convertInt(nibble[1]) + ")"; break; //0000nnnnmmmm0101
						case b0110: interpretation = "mov.l  r" + convertInt(nibble[2]) + ", @(r0,r" + convertInt(nibble[1]) + ")"; break; //0000nnnnmmmm0110

						case b0111: interpretation = "mul.l  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0000nnnnmmmm0111

						case b1100: interpretation = "mov.b  @(r0,r" + convertInt(nibble[2]) + "), r" + convertInt(nibble[1]); break; //0000nnnnmmmm1100
						case b1101: interpretation = "mov.w  @(r0,r" + convertInt(nibble[2]) + "), r" + convertInt(nibble[1]); break; //0000nnnnmmmm1101
						case b1110: interpretation = "mov.l  @(r0,r" + convertInt(nibble[2]) + "), r" + convertInt(nibble[1]); break; //0000nnnnmmmm1110
						case b1111: interpretation = "mac.l  @r" + convertInt(nibble[2]) + "+, @r" + convertInt(nibble[1]) + "+"; break; //0000nnnnmmmm1111
					} break;
				case b0001: interpretation = "mov.l  r" + convertInt(nibble[2]) + ", @(h'" + convertHexNibbleU(nibble[3], 4) + ",r" + convertInt(nibble[1]) + ")"; break; //0001nnnnmmmmdddd
				case b0010:
					switch(nibble[3]) {
						case b0000: interpretation = "mov.b  r" + convertInt(nibble[2]) + ", @r" + convertInt(nibble[1]); break; //0010nnnnmmmm0000
						case b0001: interpretation = "mov.w  r" + convertInt(nibble[2]) + ", @r" + convertInt(nibble[1]); break; //0010nnnnmmmm0001
						case b0010: interpretation = "mov.l  r" + convertInt(nibble[2]) + ", @r" + convertInt(nibble[1]); break; //0010nnnnmmmm0010
						case b0100: interpretation = "mov.b  r" + convertInt(nibble[2]) + ", @-r" + convertInt(nibble[1]); break; //0010nnnnmmmm0100
						case b0101: interpretation = "mov.w  r" + convertInt(nibble[2]) + ", @-r" + convertInt(nibble[1]); break;//0010nnnnmmmm0101
						case b0110: interpretation = "mov.l  r" + convertInt(nibble[2]) + ", @-r" + convertInt(nibble[1]); break; //0010nnnnmmmm0110
						case b0111: interpretation = "div0s  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm0111
						case b1000: interpretation = "tst  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break;  //0010nnnnmmmm1000
						case b1001: interpretation = "and  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1001
						case b1010: interpretation = "xor  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1010
						case b1011: interpretation = "or  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1011
						case b1100: interpretation = "cmp/str  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1100
						case b1101: interpretation = "xtrct  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1101
						case b1110: interpretation = "mulu.w  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1110
						case b1111: interpretation = "muls.w  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0010nnnnmmmm1111
					} break;
				case b0011:
					switch(nibble[3]) {
						case b0000: interpretation = "cmp/eq  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0000
						case b0010: interpretation = "cmp/hs  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0010
						case b0011: interpretation = "cmp/ge  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0011
						case b0100: interpretation = "div1  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0100
						case b0101: interpretation = "dmulu.l  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0101
						case b0110: interpretation = "cmp/hi  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0110
						case b0111: interpretation = "cmp/gt  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm0111
						case b1000: interpretation = "sub  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1000
						case b1010: interpretation = "subc  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1010
						case b1011: interpretation = "subv  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1011
						case b1100: interpretation = "add  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1100
						case b1101: interpretation = "dmuls.l  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1101
						case b1110: interpretation = "addc  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1110
						case b1111: interpretation = "addv  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0011nnnnmmmm1111
					} break;
				case b0100:
					switch(nibble[3]) {
						case b0110:
							switch(nibble[2]) {
								case b0000: interpretation = "lds.l  @r" + convertInt(nibble[1]) + "+, mach";	break; //0100mmmm00000110
								case b0001: interpretation = "lds.l  @r" + convertInt(nibble[1]) + "+, macl";	break; //0100mmmm00010110
								case b0010: interpretation = "lds.l  @r" + convertInt(nibble[1]) + "+, pr";	break; //0100mmmm00100110
							} break;
						case b0111:
							switch(nibble[2]){
								case b0000: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, sr"; break; //0100mmmm00000111
								case b0001: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, gbr"; break; //0100mmmm00010111
								case b0010: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, vbr"; break; //0100mmmm00100111
								case b0011: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, ssr"; break; //0100mmmm00110111
								case b0100: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, spc"; break; //0100mmmm01000111
								case b1000: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r0_bank"; break; //0100mmmm10000111
								case b1001: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r1_bank"; break; //0100mmmm10010111
								case b1010: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r2_bank"; break; //0100mmmm10100111
								case b1011: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r3_bank"; break; //0100mmmm10110111
								case b1100: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r4_bank"; break; //0100mmmm11000111
								case b1101: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r5_bank"; break; //0100mmmm11010111
								case b1110: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r6_bank"; break; //0100mmmm11100111
								case b1111: interpretation = "ldc.l  @r" + convertInt(nibble[1]) + "+, r7_bank"; break; //0100mmmm11110111
							} break;
						case b1010:
							switch(nibble[2]) {
								case b0000: interpretation = "lds  r" + convertInt(nibble[1]) + ", mach"; break; //0100mmmm00001010
								case b0001: interpretation = "lds  r" + convertInt(nibble[1]) + ", macl"; break; //0100mmmm00011010
								case b0010: interpretation = "lds  r" + convertInt(nibble[1]) + ", pr"; break; //0100mmmm00101010
							} break;
						case b1110:
							switch(nibble[2]) {
								case b0000: interpretation = "ldc  r" + convertInt(nibble[1]) + ", sr"; break; //0100mmmm00001110
								case b0001: interpretation = "ldc  r" + convertInt(nibble[1]) + ", gbr"; break; //0100mmmm00011110
								case b0010: interpretation = "ldc  r" + convertInt(nibble[1]) + ", vbr"; break; //0100mmmm00101110
								case b0011: interpretation = "ldc  r" + convertInt(nibble[1]) + ", ssr"; break; //0100mmmm00111110
								case b0100: interpretation = "ldc  r" + convertInt(nibble[1]) + ", spc"; break; //0100mmmm01001110
								case b1000: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r0_bank"; break; //0100mmmm10001110
								case b1001: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r1_bank"; break; //0100mmmm10011110
								case b1010: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r2_bank"; break; //0100mmmm10101110
								case b1011: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r3_bank"; break; //0100mmmm10111110
								case b1100: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r4_bank"; break; //0100mmmm11001110
								case b1101: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r5_bank"; break; //0100mmmm11011110
								case b1110: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r6_bank"; break; //0100mmmm11101110
								case b1111: interpretation = "ldc  r" + convertInt(nibble[1]) + ", r7_bank"; break; //0100mmmm11111110
							} break;
						case b0100:
							switch(nibble[2]) {
								case b0000: interpretation = "rotl  r" + convertInt(nibble[1]); break; //0100nnnn00000100
								case b0010: interpretation = "rotcl  r" + convertInt(nibble[1]); break; //0100nnnn00100100
							} break;
						case b0000:
							switch(nibble[2]) {
								case b0000: interpretation = "shll  r" + convertInt(nibble[1]); break; //0100nnnn00000000
								case b0001: interpretation = "dt  r" + convertInt(nibble[1]); break; //0100nnnn00010000
								case b0010: interpretation = "shal  r" + convertInt(nibble[1]); break; //0100nnnn00100000
							} break;
						case b0001:
							switch(nibble[2]) {
								case b0000: interpretation = "shlr  r" + convertInt(nibble[1]); break; //0100nnnn00000001
								case b0001: interpretation = "cmp/pz  r" + convertInt(nibble[1]); break; //0100nnnn00010001
								case b0010: interpretation = "shar  r" + convertInt(nibble[1]); break; //0100nnnn00100001
							} break;
						case b0010:
							switch(nibble[2]) {
								case b0000: interpretation = "sts.l  mach, @-r" + convertInt(nibble[1]); break; //0100nnnn00000010
								case b0001: interpretation = "sts.l  macl, @-r" + convertInt(nibble[1]); break; //0100nnnn00010010
								case b0010: interpretation = "sts.l  pr, @-r" + convertInt(nibble[1]); break; //0100nnnn00100010
							} break;
						case b0011:
							switch(nibble[2]) {
								case b0000: interpretation = "stc.l  sr, @-r" + convertInt(nibble[1]); break; //0100nnnn00000011
								case b0001: interpretation = "stc.l  gbr, @-r" + convertInt(nibble[1]); break; //0100nnnn00010011
								case b0010: interpretation = "stc.l  vbr, @-r" + convertInt(nibble[1]); break; //0100nnnn00100011
								case b0011: interpretation = "stc.l  ssr, @-r" + convertInt(nibble[1]); break; //0100nnnn00110011
								case b0100: interpretation = "stc.l  spc, @-r" + convertInt(nibble[1]);; break; //0100nnnn01000011
								case b1000: interpretation = "stc.l  r0_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn10000011
								case b1001: interpretation = "stc.l  r1_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn10010011
								case b1010: interpretation = "stc.l  r2_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn10100011
								case b1011: interpretation = "stc.l  r3_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn10110011
								case b1100: interpretation = "stc.l  r4_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn11000011
								case b1101: interpretation = "stc.l  r5_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn11010011
								case b1110: interpretation = "stc.l  r6_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn11100011
								case b1111: interpretation = "stc.l  r7_bank, @-r" + convertInt(nibble[1]); break; //0100nnnn11110011
							} break;
						case b0101:
							switch(nibble[2]) {
								case b0000: interpretation = "rotr  r" + convertInt(nibble[1]); break; //0100nnnn00000101
								case b0001: interpretation = "cmp/pl  r" + convertInt(nibble[1]); break; //0100nnnn00010101
								case b0010: interpretation = "rotcr  r" + convertInt(nibble[1]); break; //0100nnnn00100101
							} break;
						case b1000:
							switch(nibble[2]) {
								case b0000: interpretation = "shll2  r" + convertInt(nibble[1]); break; //0100nnnn00001000
								case b0001: interpretation = "shll8  r" + convertInt(nibble[1]); break; //0100nnnn00011000
								case b0010: interpretation = "shll16  r" + convertInt(nibble[1]); break; //0100nnnn00101000
							} break;
						case b1001:
							switch(nibble[2]) {
								case b0000: interpretation = "shlr2  r" + convertInt(nibble[1]); break; //0100nnnn00001001
								case b0001: interpretation = "shlr8  r" + convertInt(nibble[1]); break; //0100nnnn00011001
								case b0010: interpretation = "shlr16  r" + convertInt(nibble[1]); break; //0100nnnn00101001
							} break;
						case b1011:
							switch(nibble[2]) {
								case b0000: interpretation = "jsr  @r" + convertInt(nibble[1]); break; //0100nnnn00001011
								case b0001: interpretation = "tas.b  @r" + convertInt(nibble[1]); break; //0100nnnn00011011
								case b0010: interpretation = "jmp  @r" + convertInt(nibble[1]);	break;
							} break;
						case b1100: interpretation = "shad  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0100nnnnmmmm1100
						case b1101: interpretation = "shld  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0100nnnnmmmm1101
						case b1111: interpretation = "mac.w  @r" + convertInt(nibble[2]) + "+, @r" + convertInt(nibble[1]) + "+"; break; //0100nnnnmmmm1111
					} break;
				case b0101: interpretation = "mov.l  @(h'" + convertHexNibbleU(nibble[3], 4) + ",r" + convertInt(nibble[2]) + "), r" + convertInt(nibble[1]); break; //0101nnnnmmmmdddd
				case b0110:
					switch(nibble[3]) {
						case b0000: interpretation = "mov.b  @r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm0000
						case b0001: interpretation = "mov.w  @r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm0001
						case b0010: interpretation = "mov.l  @r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm0010

						case b0011: interpretation = "mov  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm0011
						case b0100: interpretation = "mov.b  @r" + convertInt(nibble[2]) + "+, r" + convertInt(nibble[1]); break; //0110nnnnmmmm0100
						case b0101: interpretation = "mov.w  @r" + convertInt(nibble[2]) + "+, r" + convertInt(nibble[1]); break; //0110nnnnmmmm0101
						case b0110: interpretation = "mov.l  @r" + convertInt(nibble[2]) + "+, r" + convertInt(nibble[1]); break; //0110nnnnmmmm0110
						case b0111: interpretation = "not  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm0111
						case b1000: interpretation = "swap.b  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1000
						case b1001: interpretation = "swap.w  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1001
						case b1010: interpretation = "negc  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1010
						case b1011: interpretation = "neg  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1011
						case b1100: interpretation = "extu.b  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1100
						case b1101: interpretation = "extu.w  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1101
						case b1110: interpretation = "exts.b  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1110
						case b1111: interpretation = "exts.w  r" + convertInt(nibble[2]) + ", r" + convertInt(nibble[1]); break; //0110nnnnmmmm1111
					} break;
				case b0111:
					temp = convertInt(buffer[1], 16);
					if(temp.length() >= 2)
						interpretation = "add  #h'" + temp.substr(temp.length()-2) + ", r" + convertInt(nibble[1]);
					break; //0111nnnniiiiiiii
				case b1000:
					switch(nibble[1]) {
						case b0000: interpretation = "mov.b  r0, @(h'" + convertHexNibbleU(nibble[3]) + ",r" + convertInt(nibble[2]) + ")"; break; //10000000nnnndddd
						case b0001: interpretation = "mov.w  r0, @(h'" + convertHexNibbleU(nibble[3], 2) + ",r" + convertInt(nibble[2]) + ")"; break; //10000001nnnndddd
						case b0100: interpretation = "mov.b  @(h'" + convertHexNibbleU(nibble[3]) + ",r" + convertInt(nibble[2]) + "), r0"; break; //10000100mmmmdddd
						case b0101: interpretation = "mov.w  @(h'" + convertHexNibbleU(nibble[3], 2) + ",r" + convertInt(nibble[2]) + "), r0"; break; //10000101mmmmdddd
						case b1000: interpretation = "cmp/eq  #h'" + convertHex8NibbleU(buffer[1]) + ", r0"; break; //10001000iiiiiiii
						case b1001: interpretation = "bt  h'" + convertHex2NibbleS(((nibble[2]<<4)+nibble[3]),2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt2NibbleS((nibble[2]<<4)+nibble[3])) + ")" ; break; //10001001dddddddd
						case b1011: interpretation = "bf  h'" + convertHex2NibbleS(((nibble[2]<<4)+nibble[3]),2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt2NibbleS((nibble[2]<<4)+nibble[3])) + ")" ; break; //10001011dddddddd
						case b1101: interpretation = "bt/s  h'" + convertHex2NibbleS(((nibble[2]<<4)+nibble[3]),2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt2NibbleS((nibble[2]<<4)+nibble[3])) + ")" ; break; //10001101dddddddd
						case b1111: interpretation = "bf/s  h'" + convertHex2NibbleS(((nibble[2]<<4)+nibble[3]),2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt2NibbleS((nibble[2]<<4)+nibble[3])) + ")" ; break; //10001111dddddddd
					} break;
				case b1001: interpretation = "mov.w  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 2) + ",pc), r" + convertInt(nibble[1]) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt2NibbleU((nibble[2]<<4)+nibble[3])) + ")" ;break; //1001nnnndddddddd
				case b1010: interpretation = "bra  h'" + convertHex3NibbleS((nibble[1]<<8)+(nibble[2]<<4)+nibble[3], 2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt3NibbleS((nibble[1]<<8)+(nibble[2]<<4)+nibble[3])) + ")" ; break;  //1010dddddddddddd
				case b1011: interpretation = "bsr  h'" + convertHex3NibbleS((nibble[1]<<8)+(nibble[2]<<4)+nibble[3], 2) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%2) + 2*convertInt3NibbleS((nibble[1]<<8)+(nibble[2]<<4)+nibble[3])) + ")" ; break;
				case b1100:
					switch(nibble[1]) {
						case b0000: interpretation = "mov.b  r0, @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3]) + ",gbr)"; break; //11000000dddddddd
						case b0001: interpretation = "mov.w  r0, @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 2) + ",gbr)"; break; //11000001dddddddd
						case b0010: interpretation = "mov.l  r0, @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 4) + ",gbr)"; break; //11000010dddddddd
						case b0011: interpretation = "trapa  #h'" + convertHex2NibbleU(buffer[1]); break; //11000011iiiiiiii
						case b0100: interpretation = "mov.b  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3]) + ",gbr), r0"; break; //11000100dddddddd
						case b0101: interpretation = "mov.w  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 2) + ",gbr), r0"; break; //11000101dddddddd
						case b0110: interpretation = "mov.l  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 4) + ",gbr), r0"; break; //11000110dddddddd
						case b0111: interpretation = "mova  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 4) + ",pc), r0" + "					;h'" + convertHex8NibbleU(offset + 4 -(offset%4) + 4*convertInt2NibbleU((nibble[2]<<4)+nibble[3])) ; break; //11000111dddddddd
						case b1000: interpretation = "tst  #h'" + convertHex2NibbleU(buffer[1]) + ", r0"; break; //11001000iiiiiiii
						case b1001: interpretation = "and  #h'" + convertHex2NibbleU(buffer[1]) + ", r0"; break; //11001001iiiiiiii
						case b1010: interpretation = "xor  #h'" + convertHex2NibbleU(buffer[1]) + ", r0"; break; //11001010iiiiiiii
						case b1011: interpretation = "or  #h'" + convertHex2NibbleU(buffer[1]) + ", r0"; break; //11001011iiiiiiii
						case b1100: interpretation = "tst.b  #h'" + convertHex2NibbleU(buffer[1]) + ", @(r0,gbr)"; break; //11001100iiiiiiii
						case b1101: interpretation = "and.b  #h'" + convertHex2NibbleU(buffer[1]) + ", @(r0,gbr)"; break; //11001101iiiiiiii
						case b1110: interpretation = "xor.b  #h'" + convertHex2NibbleU(buffer[1]) + ", @(r0,gbr)"; break; //11001110iiiiiiii
						case b1111: interpretation = "or.b  #h'" + convertHex2NibbleU(buffer[1]) + ", @(r0,gbr)"; break; //11001111iiiiiiii
					} break;
				case b1101: interpretation = "mov.l  @(h'" + convertHex2NibbleU((nibble[2]<<4)+nibble[3], 4) + ",pc), r" + convertInt(nibble[1]) + "					;@(h'" + convertHex8NibbleU(offset + 4 -(offset%4) + 4*convertInt2NibbleU((nibble[2]<<4)+nibble[3])) + ")" ;break; //1101nnnndddddddd
				case b1110: interpretation = "mov  #h'" + convertHex8NibbleU(buffer[1]) + ", r" + convertInt(nibble[1]); break; //1110nnnniiiiiiii
			}
			//insertion dans le fichier
			outFile << setw( 6 ) << setfill( '0' ) << hex << offset << "	";
			outFile << hex << static_cast<int>(nibble[0]);
			outFile << hex << static_cast<int>(nibble[1]);
			outFile << hex << static_cast<int>(nibble[2]);
			outFile << hex << static_cast<int>(nibble[3]);
			outFile << "	" << interpretation << endl;

			offset+=2;
		}
        fichier.close();
        cout << "Done." << endl;
    }
    else
	{
		cerr << "Unable to open the file!" << endl;
		cout << endl << "Press any key to continue." << endl;
		cin.ignore( numeric_limits<streamsize>::max(), '\n' );
	}
    return 0;
}
