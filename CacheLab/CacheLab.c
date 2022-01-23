// CacheLab.c
// 
// TO DO
// - figure out how to change the executable name to cachelab instead of a.out
// - perform LRU algorithm in LRU option from user input to calculate hits and misses
// - output the hits and misses as you go. You'll display the value, and the H/M accordingly on a line.
// - keep count of your total misses and hits
// - show miss rate, which is misses/(hits+misses)
// - show number of cycles for run time
// NOTE: type gg=G for code formatting
// NOTE: Must compile with gcc program.c -lm to use Math.pow! Reference this in readme and look up why on google!
// NOTE: gcc -o cachelab CacheLab.c -lm to compile under the name cachelab

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//supplementary structs
typedef struct CacheLine{
	int v; //valid bit
	char *t; //tag
	int c; //count/age
} cacheLine;

//B2D method for conversion
int B2D(char *bin,int s){
	char b[s];
	strcpy(b,bin);
	int mult = 0;
	int sum = 0;
	for(int q = s - 1; q>=0; q--){
	       	sum += ((b[q] - '0')*(1 << mult++));
	}
	return sum;
}

//H2B method for conversion
char *H2B(char* hexVal, int m){

	// wariables and initialization
	int digit = 0;
	int digitDiff = m-(strlen(hexVal)*4);
	char *result = malloc(sizeof(char)*m);
	if(result!=NULL)
		result[0] = '\0';

	// extra 0's
	for(int i=0; i<digitDiff; i++){
		strcat(result,"0");
	}

	// hex conversion
	while (digit < strlen(hexVal)) {

		switch (hexVal[digit]) {
			case '0':
				strcat(result,"0000");
				break;
			case '1':
				strcat(result,"0001");
				break;
			case '2':
				strcat(result,"0010");
				break;
			case '3':
				strcat(result,"0011");
				break;
			case '4':
				strcat(result,"0100");
				break;
			case '5':
				strcat(result,"0101");
				break;
			case '6':
				strcat(result,"0110");
				break;
			case '7':
				strcat(result,"0111");
				break;
			case '8':
				strcat(result,"1000");
				break;
			case '9':
				strcat(result,"1001");
				break;
			case 'A':
				strcat(result,"1010");
				break;
			case 'B':
				strcat(result,"1011");
				break;
			case 'C':
				strcat(result,"1100");
				break;
			case 'D':
				strcat(result,"1101");
				break;
			case 'E':
				strcat(result,"1110");
				break;
			case 'F':
				strcat(result,"1111");
				break;
			default:
				printf("[Error] Please check file, you have a bad digit...\n");
		}
		digit++;
	}
	return result;
	free(result);
}

//main method
int main(int argc, char **argv){

	// getopt block
	int opt,m,s,e,b;
	char *i = NULL;
	char *r = NULL;
	if(argc != 13){
		printf("[Error] Please enter the following syntax: ./cachelab -m 4 -s 2 -e 0 -b 1 -i address01 -r <your algorithm> \n");
	}
	else{
		while((opt = getopt(argc,argv,"m:s:e:b:i:r:")) != -1){
			switch(opt){
				case 'm':
					m = atoi(optarg);
					break;
				case 's':
					s = atoi(optarg);
					break;
				case 'e':
					e = atoi(optarg);
					break;
				case 'b':
					b = atoi(optarg);
					break;
					// inputFile
				case 'i':
					i = optarg;
					break;
					//algortihm method
				case 'r':
					r = optarg;
					break;
				default:
					printf("[Error] Please enter the following syntax: ./cachelab -m 4 -s 2 -e 0 -b 1 -i address01 -r <your algorithm> \n");
					break;
			}
		}

		//Wariables
		int sets = (int) pow(2.0, (double) s);
		int lines = (int) pow(2.0 , (double) e);
		int numHits = 0;
		int numMisses = 0;
		int tagLength = m-(b+s);
		int indexLength = s;
		//LRU array that mirrors outside of the set rows
		int LRU[sets];
		for(int a=0; a<sets; a++){
			LRU[a]=0;
		}

		//Cache of type CacheLine
		cacheLine cache[sets][lines];	

		//Initialize counts in the lines, such that l=0 has age 1, 1=1 has age 2, etc.
		for(int k=0; k<sets; k++){
			for(int l=0; l<lines; l++){
				cache[k][l].v=0;
				cache[k][l].t = "";
				cache[k][l].c = lines-l;
			}
		}	

		//Fscanf block
		FILE *pFile;
		char *fileName = strcat(i,".txt");
		pFile = fopen(fileName,"r");
		int val;

		//reads each line of the address file as a hex number
		while(fscanf(pFile, "%X", &val) > 0){

			// convert hexadecimal int into char*
			char stringVal[m/4];
			sprintf(stringVal,"%X",val);
			char *hexVal = malloc(sizeof(char)*strlen(stringVal));
			if(hexVal!=NULL){
				strcpy(hexVal,stringVal);
				hexVal[m/4] = '\0';
			}

			// take hexadecimal and convert to binary
			char *binVal = malloc(sizeof(char)*m); 
			if(binVal!=NULL){
				binVal = H2B(hexVal,m);
				binVal[m] = '\0';
			}

			// derive the tag and index from the binVal string
			char tag[tagLength];
			char index[indexLength];
			strncpy(tag,binVal,tagLength);
			strncpy(index,binVal+tagLength,indexLength);
			tag[m-(s+b)] = '\0';
			index[s] = '\0';

			//needs to be converted to base 10 before proceeding further...
			int ind = B2D(index,s);

			//printing
			printf("%X", val);

			//cache checking, where e = number of "ways"

			// increment through a row to find a hit, since we know the index
			int hitSwitch = 0;
			for(int j=0; j<lines; j++){
				//hit
				if((hitSwitch==0) && (strcmp(cache[ind][j].t,tag) == 0)){
					printf(" H\n");
					numHits++;
					hitSwitch = 1;
				}
				//miss keep looking
			}

			//miss completely
			if(hitSwitch == 0){

				printf(" M\n");
				//find oldest in row and record it as the value in LRU[ind]
				int oldestAge=0;
				for(int col=0; col<lines; col++){
					if(cache[ind][col].c > oldestAge){
						oldestAge = cache[ind][col].c;
						LRU[ind] = col;
					}
				}

				//update cache variables
				cache[ind][LRU[ind]].v = 1;
				cache[ind][LRU[ind]].t = tag;
				cache[ind][LRU[ind]].c = 0; //new youngest
				numMisses++;
			}

			//update ages outside of hits and misses
			for(int w=0; w<lines; w++){
				cache[ind][w].c++;
			}

			// free variables at the very end
			free(hexVal);
			free(binVal);
			hexVal=NULL;
			binVal=NULL;
		}
		printf("[Result] Hits: %d Misses: %d Miss rate: %d percent Total running time: %d cycles\n",numHits,numMisses, (int) (100*((double) numMisses/ (double) (numHits+numMisses))),(numHits+(numMisses*100)));
		fclose(pFile);
	}
	return 0;
}
