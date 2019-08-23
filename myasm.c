/*Name: Gabriel Suttner
 * Class: cs224
 * Lab1 Assembly
 * Description: This program will take a file of Assembly instructions and convert them to machine code for the machine to run. 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LENGTH 128

//Functions needed
_Bool convertLine(char* line,char* output);
void getLine(FILE* file,char* line);
void printByte(char* instr);
void printBinary(char* line, FILE* outFS);
int getReg(char* reg1, char* reg2);
int find(char* line, char c, int num);
void setReg(char* line, int num, char* regOutput);
void getImmediate(char* line, char* imChar);
void calcImm(char* output, int i);
char convToHex(char c);
_Bool seeIfD(char* line, int s);

_Bool big = 0;
_Bool returned;

int main(int argc, char** argv){
	//Check to insure a file is passed as a paramater. 
	if(argc < 2){
		printf("I'm sorry but not enough files were supplied. Please pass an input file and output file for this program");
		exit(1);
	}
	

	char output[15];
	int outLen = strlen(argv[1]);


	for(int i = 0; i < outLen; i++){
		output[i] = argv[1][i];
	}

	//check to insure the provided file is a .s file. if it is then create a .o file for the the program. 
	if(output[outLen - 1] != 's'){
		printf("Error: input file is incorrect format\n");
		return 4;
	}	else{
		output[outLen - 1] = 'o';
	}
	
	//open the .s input file and the .o output file. 
	FILE* inFS = fopen(argv[1], "r");
	FILE* outFS = fopen(output, "w");
	
	//if either didn't open, warn the user. 
	if(!inFS){
		printf("the file %s couldn't open.\n", argv[1]);
		return 2;
	}

	if(!outFS){
		printf("the file %s couldn't open.", output);
		return 3;
	}


	char sPrintLine[11];
	char sLine[LENGTH];
	char error;
	int i = 1;


	while(i < 15){
		getLine(inFS, sLine);
		if(strlen(sLine) < 3){
			getLine(inFS, sLine);
			if(strlen(sLine) < 3){
				break;
			}
		}
		returned = convertLine(sLine, sPrintLine);
		if(returned == 0){
			printBinary(sPrintLine, outFS);
		}else{

			//print out any invalid instrucitons
			printf("Invalid instruction found on line %d.\n", i);
		}
		i++;
		//convert all bits in the string to 0x00
		for(int k = 0; k < 11; k++){
			sPrintLine[k] = 0x00;
		}
	}
		
	fclose(outFS);
	fclose(inFS);
	return 0;
}


void printBinary(char* line, FILE* outFS){
	int t;
	if(strlen(line)!= 0){
		for(int i = 0; i < 2; i++){
				//for(int k = 0; k < 8; k++){
					char c = line[i];
					printf("%x", c);
					//	t = !!((c << k) & 0x80);
					fprintf(outFS, "%x", c);
				//}
				//fprintf(outFS, " ");
		 }
		if(big == 1){
			for(int i = 2; i < 10; i++){
				//for(int k = 0; k < 8; k++){
						char c = line[i];
						printf("%x", c);
					 // t = !!((c << k) & 0x80);
					fprintf(outFS, "%x", c);
					//}
			}
			//fprintf(outFS, " ");
		}
	}else{
		fprintf(outFS, "%x", 0x00);
	}
	//fprintf(outFS, "\n");
 big = 0;
}
char convToHex(char c){
	switch(c){
		case 48:
			c = 0x00;
			break;
		case 49:
			c = 0x01;
			break;
		case 50:
			c = 0x02;
			break;
		case 51:
			c = 0x03;
			break;
		case 52:
			c = 0x04;
			break;
		case 53:
			c = 0x05;
			break;
		case 54:
			c = 0x06;
			break;
		case 55:
			c = 0x07;
			break;
		case 56:
			c = 0x08;
			break;
		case 57:
			c = 0x09;
			break;
		case 97:
			c = 0x0a;
			break;
		case 98:
			c = 0x0b;
			break;
		case 99:
			c = 0x0c;
			break;
		case 100:
			c = 0x0d;
			break;
		case 101:
			c = 0x0e;
			break;
		case 102:
			c = 0x0f;
			break;
		default:
			c = 0x00;
			break;
	}
	return c;		
}

int getReg(char* reg1, char* reg2){
	unsigned char regBit1;
	unsigned char regBit2;
	unsigned char combinedBit;
	regBit1 = (strcmp(reg1, "%rax")== 0)? 0x00 :
		(strcmp(reg1, "%rcx")==0)? 0x1 :
		(strcmp(reg1, "%rdx")==0)? 0x2 :
		(strcmp(reg1, "%rbx")==0)? 0x3 :
		(strcmp(reg1, "%rsp")==0)? 0x4 :
		(strcmp(reg1, "%rbp")==0)? 0x5 :
		(strcmp(reg1, "%rsi")==0)? 0x6 :
		(strcmp(reg1, "%rdi")==0)? 0x7 :
		(strcmp(reg1, "%r8")==0)? 0x8 :
		(strcmp(reg1, "%r9")==0)? 0x9 :
		(strcmp(reg1, "%r10")==0)? 0xa :
		(strcmp(reg1, "%r11")==0)? 0xb :
		(strcmp(reg1, "%r12")==0)? 0xc :
		(strcmp(reg1, "%r13")==0)? 0xd :
		(strcmp(reg1, "%r14")==0)? 0xe :
		(strcmp(reg1, "%r15")==0)? 0xf :
		0xfa;
  regBit2 = (strcmp(reg2, "%rax")==0)? 0x00 :
		(strcmp(reg2, "%rcx")==0)? 0x01 :
		(strcmp(reg2, "%rdx")==0)? 0x02 :
		(strcmp(reg2, "%rbx")==0)? 0x03 :
		(strcmp(reg2, "%rsp")==0)? 0x04 :
		(strcmp(reg2, "%rbp")==0)? 0x05 :
		(strcmp(reg2, "%rsi")==0)? 0x06 :
		(strcmp(reg2, "%rdi")==0)? 0x07 :
		(strcmp(reg2, "%r8")==0)? 0x08 :
		(strcmp(reg2, "%r9")==0)? 0x09 :
		(strcmp(reg2, "%r10")==0)? 0x0a :
		(strcmp(reg2, "%r11")==0)? 0x0b :
		(strcmp(reg2, "%r12")==0)? 0x0c :
		(strcmp(reg2, "%r13")==0)? 0x0d :
		(strcmp(reg2, "%r14")==0)? 0x0e :
		(strcmp(reg2, "%r15")==0)? 0x0f :
		0xfa;	

	regBit1 = regBit1 << 4;	
	combinedBit = (regBit1 ^ regBit2);
	return combinedBit;
}
void getImmediate(char* line, char* output){
	int loc = find(line, '$', 0);
	int comma = find(line, ',', 0);
	int i;
	int dif = comma - loc - 1;
	if(loc != -1){
		for(i = 0; i < dif; i++){
			loc++;
			output[i] = line[loc];
		}	
		
	}
}

void calcImm(char* output, int i){
	char hex[8];
	hex[8] = 0x00;
	int j = 0;
	char c1;
  char c2;
	
	while(i != 0){
		int temp = 0; 
		temp = i % 16;
		
		if(temp < 10){
			 c1 = temp + 48;
			}
		else{
			 c1 = temp + 87;
		 }

		i = i / 16;

		temp = i % 16;
		
		if(temp < 10){
			 c2 = temp + 48;
		 }
		else{
			 c2 = temp + 87;
		 }
		i = i / 16;
		c1 = convToHex(c1);
		c2 = convToHex(c2);
		c2 = c2 << 4;

		hex[j] = (c1 | c2);
		c1 = 0; 
		c2 = 0;
		j++;
	}
	i = 2; 
	//This is where I'm getting an error. 
	for(int k = 0; k < 8; k++){
		c1 = hex[k];
		output[i] = c1;
		i++;	
	}
}

int find(char* line, char c, int num){
	 for(int i = num; i < strlen(line) - 1; i++){
		if(line[i] == c){
			return i;
		}
	}
	return -1;
}
void setReg(char* line, int num, char* regOutput){
	int loc = find(line, '%', 0);
	int i;
	switch(num){
		case 1:
			for(i = 0; i < 4; i++){
				if(line[loc] != 0x00){
					regOutput[i] = line[loc];
				}
				loc++;
			}
			 break;
		case 2:
			loc = find(line, '%', loc + 1);
			if(loc != -1){
				for(i = 0; i < 4; i++){
					if(line[loc] != 0x00){
						regOutput[i] = line[loc];
					}
					loc++;
				}
				regOutput[i] = 0x00;
			}else{
				regOutput[0] = 58;
				regOutput[1] = 0x00;
			}
		
		 	 break;
	}
} 

_Bool convertLine(char* line, char* output){
	char c = tolower(line[0]);
	char reg1[5];	
	setReg(line, 1, reg1);
	char reg2[5];	
 	setReg(line, 2, reg2);
	char imChar[9];
	getImmediate(line, imChar);
	long int iImmediate;
	char sample[9];
	_Bool dest;
	int returned = find(line, ',', 0);

switch(c){
	  //halt
		case 104:
			output[0] = 0x00;
			output[1] = '\0';	
			return 0;
			break;
	  //no Op 
		case 110:
			output[0] = 0x10;
			output[1] = 0;
			return 0;
			break;
	  //cmove 
		case 99:
			if(returned == -1){
				return 1;
			}
			c = tolower(line[4]);
			switch(c){
				//cmovle, cmovl
				case 108:
					c = tolower(line[5]);
					if(c == 101){
						output[0] = 0x21;
						output[1] = getReg(reg1, reg2);
			 		  output[2] = '\0';	
			    }else{
						output[0] = 0x22;
						output[1] = getReg(reg1, reg2);
					  output[2] = '\0';	
			    }
					break;
					//cmove
				case 101:
					output[0] = 0x23;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';	
			    break;
					//cmovne
				case 110:
					output[0] = 0x24;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';	
			    break;
					//cmovge, cmovg
				case 71:
					c = tolower(line[5]);
					if(c == 101){
						output[0] = 0x25;
						output[1] = getReg(reg1, reg2);
			    	output[2] = '\0';	
				   }else{
						output[0] = 0x26;
						output[1] = getReg(reg1, reg2);
				    output[2] = '\0';	
				   }
					
					break;

			}	
			break;
		//irmovq
		 case 105:
			if(returned == -1){
				return 1;
			}
			big = 1;
			output[0] = 0x30;
			output[1] = getReg(reg2, reg1);
			if(imChar[1] == 'x'){
				iImmediate = strtol(imChar, NULL, 0);
			}else{
				iImmediate = atoi(imChar);
			}

			calcImm(output, iImmediate);	
		 	break;
		//rmmovq or rrmovq 
		 case 114:
			
			c = line[1];
			switch(c){
				case 114:
					output[0] = 0x20;
					output[1] = getReg(reg1, reg2);
					
					break;
				case 109:
					if(returned == -1){
						return 1;
					}
					big = 1;

					output[0] = 0x40;
					output[1] = getReg(reg1, reg2);
				  dest = seeIfD(line, 0);
					
					int loc	= find(line, ',', 0);
					int comma	= find(line, '(', 0);
					int i;
					int dif = comma - loc - 1;
				
					if(dif > 2){
						loc++;
						comma--;
						for(i = 0; i < dif -1; i++){
							loc++;
							imChar[i] = line[loc];
						}	
						i++;
						imChar[i] = '\0';
						
					iImmediate = atoi(imChar);
					}

					if(dest == 1){
						iImmediate = atoi(imChar);
						calcImm(output, iImmediate);
					}
					break;
			}
			break;
		//mrmovq
		 case 109:
			if(returned == -1){
				return 1;
			}
      big = 1;
			output[0] = 0x50;
			output[1] = getReg(reg1, reg2);
			
			dest = seeIfD(line, 1);
					
			int loc = find(line, 'q', 0);
			int comma = find(line, '(', 0);
			int i;
			int dif = comma - loc - 1;
		
			if(dif > 2){
				loc++;
				comma--;
				for(i = 0; i < dif - 1; i++){
					loc++;
					imChar[i] = line[loc];
				}	
				i++;
				imChar[i] = '\0';
				if(imChar[1] == 'x'){
					iImmediate = strtol(imChar, NULL, 0);
				}else{
					iImmediate = atoi(imChar);
				
			 }
			}
			if(dest == 1){
				calcImm(output, iImmediate);
			}

			break;
		//opQ
		 case 97:
		 case 115:
		 case 120:
		 if(returned == -1){
				return 1;
			}
	    c = tolower(line[1]);
			switch(c){
				//addq
				case 100:
					output[0] = 0x60;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';
					break;
					//subq
				case 117:
					output[0] = 0x61;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';
					break;
					//andq
				case 110:
					output[0] = 0x62;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';
					break;
					//xorq
				case 111:
					output[0] = 0x63;
					output[1] = getReg(reg1, reg2);
					output[2] = '\0';
					break;
			}
			break;
		//handles all others
		default:
		return 1;	
			break;
	}
		if(output[2] == 0xff){
			printf("hit Here");
			return 1;
		}
		return 0;	
 }
_Bool seeIfD(char* line, int s){
	int j;
	if(s == 1){
		int i = find(line, 'q', 0);
		int k = find(line, '(', 0);
		j = k - i;
	}
	else{
		int i = find(line, ',', 0);
		int k = find(line, '(', 0);
		j = k - i;
	}
	if(j < 3){
			return 0;
	}else{
			return 1;
	}

}
void getLine(FILE* file, char* line){

	if(file == NULL){
		printf("Error: file pointer is Null.");
		exit(2);
	}

	char sLineBuffer[LENGTH];

	char ch = getc(file);
	int count = 0; 

	while ((&ch != "\n") && (ch != EOF) && (ch != 13) && (ch != 10)){
		sLineBuffer[count] = ch;
		count++;
		
		ch = getc(file);
	}
	sLineBuffer[count] = 0x00;
	strncpy(line, sLineBuffer, (count + 1));
}

void printByte(char* instr){
	for(int i = 0; i < strlen(instr); i++){
		printf("%x ", instr[i]);	
	}
}

