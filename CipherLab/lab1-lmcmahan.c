// CipherLab
// Liam McMahan

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Encrypt Method
void encrypt(char *text, int e){
	
	// Record size
        int size = strlen(text);
	
	// Increment the characters by Encryption factor
        for(int i=0; i<size; i++){
		text[i] = (text[i]+e)%127;
        }
	
	// Print the output on the next line
	printf("%s\n", text);
}

// Decrypt Method
void decrypt(char *text, int e){

	// Record size
        int size = strlen(text);
	
	// Decrement the characters by Encryption factor
	for(int i=0; i<size; i++){
   		text[i] = (text[i]-e)%127;
        }

        // Print the output on the next line
        printf("%s\n", text);
}

// Encode Method
void encode(char *text){

	// Record size
	int size = strlen(text);

        // For loop to iterate through each of the input string's characters
        for(int i=0;i<size;i++){

        	// Convert the character to their corresponding ASCII int value
        	int val = (int) text[i];

                // Create an int array called "s" with length 8, to represent a binary number
                int s[8];

                // Create an int j for use in both for loops
                int j;

                // Binary Mathematics
                for(j=0;val>0;j++){
                	s[j]=val%2;
                        val=val/2;
                }

                // Gradual Output of each binary digit, decrementing from j-1
                for(int k=j-1;k>=0;k--){

                	// If this is the first digit...
                        if(k==j-1)
                        	// Zero for MSB
                                printf("%d",0);
                        // Print the digit      
                        printf("%d",s[k]);
                }
	}

        printf("\n");
}

// Decode Method
void decode(char *text){

	// If the number of digits doesn't properly represent a binary coded message...
        if(strlen(text)%8 != 0)
        	printf("[ERROR] Please enter binary value correctly!\n");

        // If we're good, do this...
        else{

        	// Record size
                int size = strlen(text);

                // Wariables (long long for array of binary values)
                int numChars = size/8;
                long long binNums[numChars];
                int decNums[numChars];
                int chars[numChars];

                // Iterate through each character
                for (int i=0; i<numChars; i++){

                	// Start value at 0
                        binNums[i] = 0;

                        // Convert each string segment (length 8) into a long long type
                       for(int j=0; j<8; j++){

			       // Shift the existing bits to the right
                               binNums[i] *= 10;
                                                
			       // Add LSB to the rightmost side
                               binNums[i] += (text[(8*i)+j] - '0');
                       }

                       // Binary to Decimal Conversion
                       int leastSigBit;
                       int base = 1;
                       decNums[i]=0;
                       while (binNums[i] > 0){
                       
			       // Pull LSB
                               leastSigBit = binNums[i] % 10;
                               
			       // Adjust the decimal value by adding LSB * 2^n
                               decNums[i] = decNums[i] + leastSigBit * base;
                               
			       // Left shift the bits by 1
                               binNums[i] = binNums[i] / 10;
                                                
			       // 2^n, where n increases by 1 everytime this line is called
                               base = base * 2;                 
		       }

                                        
		       // Decimal to Character Conversion and printing
      		       chars[i] = (char) decNums[i];
		       printf("%c",chars[i]);
		}
		
		printf("\n");
	}
}


int main(){

	// Command loop will break once user enters exit()
	while(1){
	
		// User prompt and input get
		char input[100];
		printf("lmcmahan $ ");
		fgets(input, 100, stdin);

		// Tokenization process
		char *cmd, *text;
        	cmd = strtok(input, "()");
		text = strtok(NULL, "()");

		// Encryption Factor
		int e=5;

		// Convert String to char array for future use
                int size = strlen(text);
                char in[size];
                strcpy(in,text);

		// Cases for different commands

		// Encrypt
		if (strcmp(cmd, "encrypt") == 0){
			encrypt(in,e);
		} 

		// Decrypt
		else if (strcmp(cmd, "decrypt") == 0){
			decrypt(in,e);
		}

		// Encode
		else if (strcmp(cmd, "encode") == 0){
			encode(in);	
	        }

		// Decode
		else if (strcmp(cmd, "decode") == 0){
			decode(in);
		}

		// Exit
		else if (strcmp(cmd, "exit") == 0){
			printf("See ya!\n");
			exit(0);
	        }

		// Error handling
		else{
			printf("[ERROR] Please enter command correctly!\n");
		}
	}
	
	// Close the Program
	return 0;
}
