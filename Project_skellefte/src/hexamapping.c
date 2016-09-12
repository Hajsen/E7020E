#include "hexamapping.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *hex2bin[] = {"0000", "0001", "0010", "0011",
 "0100", "0101", "0110", "0111",
 "1000", "1001", "1010", "1011",
 "1100", "1101", "1110", "1111"};

char *convertFromHexToBin(char* hexaString)
{

	char* binaryResult = malloc(8 * strlen(hexaString)*sizeof(char*));

	//char** hex2bin;
	//hex2bin = initHexToBinMapping();

	strcpy(binaryResult, "");
	//scanf("%s", hexaDecimal);
	int i = 0;
	while (hexaString[i]){
		switch (hexaString[i])
		{
		case '0':
			strcat(binaryResult, hex2bin[0]);
			//printf("0000");

			break;
		case '1':
			strcat(binaryResult, hex2bin[1]);
			//printf("0001");
			break;
		case '2':
			strcat(binaryResult, hex2bin[2]);
			//printf("0010");
			break;

		case '3':
			strcat(binaryResult, hex2bin[3]);
			//printf("0011");
			break;
		case '4':
			strcat(binaryResult, hex2bin[4]);
			//printf("0100");
			break;
		case '5':
			strcat(binaryResult, hex2bin[5]);
			//printf("0101");
			break;
		case '6':
			strcat(binaryResult, hex2bin[6]);
			//printf("0110");
			break;
		case '7':
			strcat(binaryResult, hex2bin[7]);
			//printf("0111");
			break;
		case '8':
			strcat(binaryResult, hex2bin[8]);
			//printf("1000");
			break;
		case '9':
			strcat(binaryResult, hex2bin[9]);
			//printf("1001");
			break;
		case 'A':
			strcat(binaryResult, hex2bin[10]);
			//printf("1010");
			break;
		case 'B':
			strcat(binaryResult, hex2bin[11]);
			//printf("1011");
			break;
		case 'C':
			strcat(binaryResult, hex2bin[12]);
			//printf("1100");
			break;
		case 'D':
			strcat(binaryResult, hex2bin[13]);
			//printf("1101");
			break;
		case 'E':
			strcat(binaryResult, hex2bin[14]);
			//printf("1110");
			break;
		case 'F':
			strcat(binaryResult, hex2bin[15]);
			//printf("1111");
			break;
		case 'a':
			strcat(binaryResult, hex2bin[10]);
			//printf("1010");
			break;
		case 'b':
			strcat(binaryResult, hex2bin[11]);
			//printf("1011");
			break;
		case 'c':
			strcat(binaryResult, hex2bin[12]);
			//printf("1100");
			break;
		case 'd':
			strcat(binaryResult, hex2bin[13]);
			//printf("1101");
			break;
		case 'e':
			strcat(binaryResult, hex2bin[14]);
			//printf("1110");
			break;
		case 'f':
			strcat(binaryResult, hex2bin[15]);
			//printf("1111");
			break;
		default:
			printf("\nInvalid hexadecimal digit %c ", hexaString[i]);
			return 0;

		}
		i++;
	}
	return binaryResult;

}
