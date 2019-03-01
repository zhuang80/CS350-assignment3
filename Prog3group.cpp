#include<iostream>
#include<malloc.h>
#include<sys/time.h>

using namespace std;

const int pageAccess=10000;
const int pages=100;

int main(int argc, char ** argv){
	int * noLocal=(int*) malloc(pageAccess * sizeof(int));
	int * eightyTotwenty=(int*) malloc(pageAccess * sizeof(int));
	int * looping=(int*) malloc(pageAccess * sizeof(int));

	unsigned long seed;
	struct timeval t;
	gettimeofday(&t, NULL);
	srand(t.tv_sec);
	//for nolocality
	for(int i=0;i<10000;i++){
		noLocal[i]= rand()%100;	
	}	
	//for 80-20
	for(int i=0;i<8000;i++){
		eightyTotwenty[i]=rand()%20;	
	}
	for(int i=8000;i<10000;i++){
		eightyTotwenty[i]=rand()%(99-20+1)+20;
	}
	for(int i=0;i<10000;i++){
		int index1=rand()%10000;
		int index2=rand()%10000;
		int temp;
		temp=eightyTotwenty[index1];
		eightyTotwenty[index1]=eightyTotwenty[index2];
		eightyTotwenty[temp];
	}
	//for looping
	for(int i=0;i<200;i++){
		for(int j=0;j<50;j++){
			looping[i*50+j]=j;		
		}			
	}
		
	return 0;
}
