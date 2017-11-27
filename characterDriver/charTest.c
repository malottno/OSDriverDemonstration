#include <stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>

#define BUFFER_LENGTH 80
static char rx[BUFFER_LENGTH];


void *threadWriter(void *vargp)
{
	FILE* fp;
	int f;
	int i = 0;
	int j = 0;
	int tx = 0;
	int rxc = 0;
	int retVal = 0;
	int processed = 0;
	char strig[BUFFER_LENGTH] = "abcdefghijklmnopqrstuvwxyz";
	sleep(1);
	//Character driver tests, read/write stream to driver
	
	f = open("/dev/charDevice", O_RDWR);
	if (f < 0){
		printf("FAILED: Open Device\n");
		return 1;
	}
	for(i = 0; i < 10000; i++){
		retVal = write(f, strig, (i%25) + 1);
		processed += retVal;
		if(retVal < 0){
			printf("FAILED: Write to device\n");
			return 2;
		}

		for(j = 0; j < 10; j++){
			retVal = read(f, rx, BUFFER_LENGTH);
			//printf("%s\n\n",rx, BUFFER_LENGTH);
			if(retVal < 0){
				printf("FAILED: Read from device\n");
				return 3;
			}
			rxc = rxc + 1;
		}
		tx = tx + 1;
	}

	close(f);
	fp=fopen("./testData.csv","a");
	fprintf(fp, "%d,%d,%d,",processed,tx,rxc);
	fclose(fp);
	
	printf("Character Device successfully processed %d characters\n\tover %d writes and reads\n",processed, tx);

	return NULL;
}


int main(){
	FILE* fo;
	int msec;
	time_t timer = time(0);
	char dt[100];
	pthread_t tid_0, tid_1, tid_2, tid_3;

	printf("------------------------------\n");
	printf("Beginning char driver tests\n");
	strftime(dt,100,"%Y-%m-%d %H:%M:%S", localtime(&timer));
	printf("Test started at: %s\n\n", dt);

	fo = fopen("./testData.csv","a");
	fprintf(fo, "charDriverTest,%s,",dt);
	fclose(fo);

	clock_t start = clock(), diff;
	pthread_create(&tid_0, NULL, threadWriter, NULL);
	pthread_create(&tid_1, NULL, threadWriter, NULL);
	pthread_create(&tid_2, NULL, threadWriter, NULL);
	pthread_create(&tid_3, NULL, threadWriter, NULL);
	pthread_join(tid_0, NULL);
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	pthread_join(tid_3, NULL);

	diff = clock() - start;
	msec = diff * 1000 / CLOCKS_PER_SEC;

	//printf("Character Device successfully processed %d characters\n",processed);
	//printf("\tover %d writes and reads\n", tx);
	printf("\tin %d milliseconds\n\n", msec);

	fo = fopen("./testData.csv","a");
	fprintf(fo, "%d\n",msec);
	fclose(fo);

	return 0;
}
