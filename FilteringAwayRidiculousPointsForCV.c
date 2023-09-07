#include <getopt.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define maxCV_M1 10686
#define maxCV_M2 5010
#define maxCV_M3 9507
#define maxCV_M4 8964
#define maxCV_M5 7940
#define maxCV_M6 4131

#define regmax 6
#define T 0.286


int EdgeTimeList[regmax+1][35000];
float SpeedList[regmax+1][35000];
int windowTimeList[regmax+1][35000];
float windowSpeedList[regmax+1][35000];

int reg,mm;

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
	char number[15], name[50], bulk[200], junkchar;
	int m;
	float window;
	int iterations;
    int swapped, startnum, endnum, timeMax, window_start, window_end, start_line, end_line;
	
	reg = 1;
	
	EdgeTimeList[1][0] = maxCV_M1-1;
	EdgeTimeList[2][0] = maxCV_M2-1;
	EdgeTimeList[3][0] = maxCV_M3-1;
	EdgeTimeList[4][0] = maxCV_M4-1;
	EdgeTimeList[5][0] = maxCV_M5-1;
	EdgeTimeList[6][0] = maxCV_M6-1;
	
	//initializing the result file
	sprintf(number, "%d", reg);
	strcpy(name, "CV");
	strcat(name, number);
	strcat(name, "_final_result.csv");

	printf("Ouputting vel and time to %s...\n", name);
	fOUT = fopen(name, "w");
		fprintf(fOUT, "time,vel\n");
	fclose(fOUT);
	printf("Finished outputting to %s \n", name);
	//finished initializing the result file
			
			
	
	do{
		//reading csv file, putting info into arrays
		mm = EdgeTimeList[reg][0];
		
		sprintf(number, "%d", reg);
		strcpy(name, "CV");
		strcat(name, number);
		strcat(name, "_result.csv");
		
		printf("Opening %s that has %d lines...\n", name, mm);
		fIN = fopen(name, "r");
		
		fgets(bulk, 180, fIN);
			for(m=1;m<=mm;m++){
				
				fscanf(fIN,"%d",&EdgeTimeList[reg][m]);
				fscanf(fIN,"%c",&junkchar);
				fscanf(fIN,"%f",&SpeedList[reg][m]);
			}
		fclose(fIN);
		printf("Finished opening %s...\n", name);
		
		
		window = (T/9)*1000;
		timeMax = EdgeTimeList[reg][mm];
		
		
		for (float i = 1; i <= (floor(timeMax/window)+1); i++) {
			window_start = round(window*(i-1));
			window_end = round(window*i);
			printf("window_start is %d, window_end is %d\n", window_start, window_end);
			
			start_line = 0;
			for (m = 1; m <= mm; m++){
				if (EdgeTimeList[reg][m] > window_start){
					break;
				}
				start_line = m;
			}
			start_line += 1;
			
			end_line = 1;
			for (m = 1; m <= mm; m++){
				if (EdgeTimeList[reg][m] > window_end){
					break;
				}
				end_line = m;
			}
			
			//printf("start_line is %d, end_line is %d\n", start_line, end_line);
			
			for(m = 1; m <= (end_line-start_line+1); m++){
				windowTimeList[reg][m] = EdgeTimeList[reg][start_line+m-1];
				windowSpeedList[reg][m] = SpeedList[reg][start_line+m-1];
			}
			
			
			//Sorting by vel (ascending)
			iterations = 0;
			do {
				swapped = 0;

				for (int i = 1; i < (end_line-start_line+1); i++) {
					if (windowSpeedList[reg][i] > windowSpeedList[reg][i + 1]) {
						floatSwap(&windowSpeedList[reg][i], &windowSpeedList[reg][i + 1]);
						swap(&windowTimeList[reg][i], &windowTimeList[reg][i + 1]);
						swapped = 1;
					}
				}

				iterations++;

				if (iterations >= 10000000000000) {
					break;
				}

			} while (swapped);
			printf("Finished sorting by vel (ascending) after %d iterations...\n", iterations);
			//Finished sorting by vel
			
			
			
			//clipping out first and last 25%
			startnum = round(0.1*(end_line-start_line+1));
			endnum = round(0.74*(end_line-start_line+1));
			//printf("startnum is %d, endnum is %d\n", startnum, endnum);
			//Finished clipping
			
			
			
			//Sorting by time
			iterations = 0;
			do {
				swapped = 0;

				for (int i = (startnum+1); i <= endnum; i++) {
					if (windowTimeList[reg][i] > windowTimeList[reg][i + 1]) {
						floatSwap(&windowSpeedList[reg][i], &windowSpeedList[reg][i + 1]);
						swap(&windowTimeList[reg][i], &windowTimeList[reg][i + 1]);
						swapped = 1;
					}
				}

				iterations++;

				if (iterations >= 10000000000000) {
					break;
				}

			} while (swapped);
			printf("Finished sorting by time (ascending) after %d iterations...\n", iterations);
			//Finished sorting by time
			
			
			
			//outputting
			sprintf(number, "%d", reg);
			strcpy(name, "CV");
			strcat(name, number);
			strcat(name, "_final_result.csv");

			printf("Ouputting vel and time to %s...\n", name);
			fOUT = fopen(name, "a");
				for (int i = (startnum+1); i <= endnum; i++) {
					fprintf(fOUT, "%d,%f\n", windowTimeList[reg][i], windowSpeedList[reg][i]);
					//printf("i is %d, time is %d, Speed is %f\n", i, windowTimeList[reg][i], windowSpeedList[reg][i]);
				}
			fclose(fOUT);
			printf("Finished outputting to %s \n", name);
			//Finished outputting
			

		}
		
		reg += 1;
		
	} while (reg <= regmax);
	
	return 0;
}
