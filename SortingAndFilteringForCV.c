#include <getopt.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define maxCV_M1 15999
#define maxCV_M2 10566
#define maxCV_M3 18079
#define maxCV_M4 29206
#define maxCV_M5 27911
#define maxCV_M6 20668

#define regmax 6

int TrackIDList[regmax+1][40000];
int EdgeTimeList[regmax+1][40000];
float EdgeXList[regmax+1][40000];
float EdgeYList[regmax+1][40000];
float SpeedList[regmax+1][40000];
float DXList[regmax+1][40000];
float DYList[regmax+1][40000];
float LinearityList[regmax+1][40000];

int reg,mm,filteredmm;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void floatSwap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}


int main(void){
	FILE *fOUT, *fIN;
	char number[15], name[50], bulk[200];
	int m;
	char *delimiter = ",";
    char *token;
    int iterations = 0;
    int swapped;
	
	TrackIDList[1][0] = maxCV_M1-4;
	TrackIDList[2][0] = maxCV_M2-4;
	TrackIDList[3][0] = maxCV_M3-4;
	TrackIDList[4][0] = maxCV_M4-4;
	TrackIDList[5][0] = maxCV_M5-4;
	TrackIDList[6][0] = maxCV_M6-4;
	
	reg = 1;
	
	do{
		//reading csv file, putting info into arrays
		mm = TrackIDList[reg][0];
		
		
		sprintf(number, "%d", reg);
		strcpy(name, "CV");
		strcat(name, number);
		strcat(name, ".csv");
		
		printf("Opening %s that has %d lines...\n", name, mm);
		fIN = fopen(name, "r");
		
		fgets(bulk, 180, fIN);
		fgets(bulk, 180, fIN);
		fgets(bulk, 180, fIN);
		fgets(bulk, 180, fIN);
			for(m=1;m<=mm;m++){
				fgets(bulk, 180, fIN);
				token = strtok(bulk, delimiter);
				token = strtok(NULL, delimiter);
				TrackIDList[reg][m] = atoi(token);
				token = strtok(NULL, delimiter);
				token = strtok(NULL, delimiter);
				token = strtok(NULL, delimiter);
				token = strtok(NULL, delimiter);
				token = strtok(NULL, delimiter);
				SpeedList[reg][m] = atof(token);
				token = strtok(NULL, delimiter);
				token = strtok(NULL, delimiter);
				EdgeTimeList[reg][m] = atoi(token);
				token = strtok(NULL, delimiter);
				EdgeXList[reg][m] = atof(token);
				token = strtok(NULL, delimiter);
				EdgeYList[reg][m] = atof(token);
			}
		fclose(fIN);
		printf("Finished opening %s...\n", name);
		
		
		//for (int i = 0; i < mm; i++) {
			//printf("(%d, %d) ", TrackIDList[reg][i], EdgeTimeList[reg][i]);
		//}
		
		//finished reading file
		
		
		
		
		
		
		
		//sorting arrays (TrackID first priority, ascending; EdgeTime second priority, ascending)
		do {
			swapped = 0;

			for (int i = 1; i < mm; i++) {
				if (TrackIDList[reg][i] > TrackIDList[reg][i + 1] || (TrackIDList[reg][i] == TrackIDList[reg][i + 1] && EdgeTimeList[reg][i] > EdgeTimeList[reg][i + 1])) {
					swap(&TrackIDList[reg][i], &TrackIDList[reg][i + 1]);
					swap(&EdgeTimeList[reg][i], &EdgeTimeList[reg][i + 1]);
					floatSwap(&SpeedList[reg][i], &SpeedList[reg][i + 1]);
					floatSwap(&EdgeXList[reg][i], &EdgeXList[reg][i + 1]);
					floatSwap(&EdgeYList[reg][i], &EdgeYList[reg][i + 1]);
					swapped = 1;
				}
			}

			iterations++;

			if (iterations >= 10000000000000) {
				break;
			}

		} while (swapped);
		
		
		printf("Finished sorting by TrackID (first priority, ascending) and EdgeTime (second priority, ascending) after %d iterations...\n", iterations);
		//for (int i = 0; i < mm; i++) {
			//printf("(%d, %d, %f, %f, %f) \n", TrackIDList[reg][i], EdgeTimeList[reg][i], EdgeXList[reg][i], EdgeYList[reg][i], SpeedList[reg][i]);
		//}
		
		
		//finished sorting arrays
		
		
		
		
		
		
		
		
		
		//calculating DX, DY, Linearity
		for (int i = 1; i <= mm; i++) {
			if (TrackIDList[reg][i] == TrackIDList[reg][i+1]) {
				DXList[reg][i] = EdgeXList[reg][i] - EdgeXList[reg][i+1];
				DYList[reg][i] = fabs(EdgeYList[reg][i] - EdgeYList[reg][i+1]);
				LinearityList[reg][i] = DXList[reg][i]/(DYList[reg][i]+0.000001);
				
			} else {
				DXList[reg][i] = -99999999.;
				DYList[reg][i] = -99999999.;
				LinearityList[reg][i] = -99999999.;
			}
		}
		printf("Finished calculating DX, DY and Linearity...\n");
		
		//for (int i = 0; i <= mm; i++) {
			//printf("(%f, %f, %f) \n", DXList[reg][i], DYList[reg][i], LinearityList[reg][i]);
		//}
		
		
		//finished calculating
		
		
		
		
		
		
		
		
		
		//sort by linearity
		do {
			swapped = 0;

			for (int i = 1; i < mm; i++) {
				if (LinearityList[reg][i] < LinearityList[reg][i + 1]) {
					floatSwap(&LinearityList[reg][i], &LinearityList[reg][i + 1]);
					floatSwap(&SpeedList[reg][i], &SpeedList[reg][i + 1]);
					swap(&EdgeTimeList[reg][i], &EdgeTimeList[reg][i + 1]);
					swapped = 1;
				}
			}

			iterations++;

			if (iterations >= 10000000000000) {
				break;
			}

		} while (swapped);
		printf("Finished sorting by Linearity (descending) after %d iterations...\n", iterations);
		//for (int i = 0; i <= mm; i++) {
			//printf("(%f, %f, %d) \n", LinearityList[reg][i], SpeedList[reg][i], EdgeTimeList[reg][i]);
		//}
		
		
		//finish sorting by linearity
		
		
		
		
		
		
		
		
		//remove linearity < 5
		for (int i = 1; i <= mm; i++) {
			if (LinearityList[reg][i] < 5.0) {
				filteredmm = i;
				break;
			}
		}
		printf("Finished filtering Linearity below 5...\n");

		//for (int i = 0; i < filteredmm; i++) {
			//printf("(%f, %f, %d) \n", LinearityList[reg][i], SpeedList[reg][i], EdgeTimeList[reg][i]);
		//}
		
		
		//finished filtering
		
		
		
		
		
		
		
		
		
		//sort by time
		do {
			swapped = 0;

			for (int i = 1; i < (filteredmm-1); i++) {
				if (EdgeTimeList[reg][i] > EdgeTimeList[reg][i + 1]) {
					swap(&EdgeTimeList[reg][i], &EdgeTimeList[reg][i + 1]);
					floatSwap(&SpeedList[reg][i], &SpeedList[reg][i + 1]);
					swapped = 1;
				}
			}

			iterations++;

			if (iterations >= 10000000000000) {
				break;
			}

		} while (swapped);
		printf("Finished sorting by time (ascending) after %d iterations...\n", iterations);
		
		//for (int i = 1; i < filteredmm; i++) {
			//printf("(%f, %d) \n", SpeedList[reg][i], EdgeTimeList[reg][i]);
		//}
		
		
		//finished sorting by time
		
		
		
		
		//output
		sprintf(number, "%d", reg);
		strcpy(name, "CV");
		strcat(name, number);
		strcat(name, "_result.csv");

		printf("Ouputting vel and time to %s...\n", name);
		fOUT = fopen(name, "w");
			fprintf(fOUT, "time,vel\n");
			for (int i = 1; i < filteredmm; i++) {
				fprintf(fOUT, "%d,%f\n", EdgeTimeList[reg][i], SpeedList[reg][i]);
			}
		fclose(fOUT);
		printf("Finished outputting to %s \n\n", name);
		//finished outputting

		
		reg += 1;
	} while (reg <= regmax);

	return 0;
}
