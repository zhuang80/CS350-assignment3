#include<iostream>
#include<malloc.h>
#include<sys/time.h>
#include<deque>
#include<vector>

using namespace std;

const int pageAccess=10000;
const int pages=100;

struct CacheClk{
	int pageNum;
	int clkBit;
};

double* FIFO(int* a);
double* Clock(int* a);
double* Rand(int* a);

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
	
	//
	double* result=(double*) malloc(20 * sizeof(double));
	result=FIFO(noLocal);
	cout<<"------------------noLocal------------------"<<endl;
	cout<<"block size:";
        for(int i=1;i<=20;i++) cout<<i*5<<"	";
	cout<<endl;
	cout<<"FIFO:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	result=Clock(noLocal);
	cout<<"Clock:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	result=Rand(noLocal);
	cout<<"Rand:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	cout<<"------------------80-to-20------------------"<<endl;
	cout<<"block size:";
        for(int i=1;i<=20;i++) cout<<i*5<<"	";
	cout<<endl;
	result=FIFO(eightyTotwenty);
	cout<<"FIFO:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	result=Clock(eightyTotwenty);
	cout<<"Clock:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	result=Rand(eightyTotwenty);
	cout<<"Rand:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;

	cout<<"------------------looping------------------"<<endl;
	cout<<"block size:";
        for(int i=1;i<=20;i++) cout<<i*5<<"	";
	cout<<endl;
	result=FIFO(looping);
	cout<<"FIFO:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;
	
	result=Clock(looping);
	cout<<"Clock:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;
	
	result=Rand(looping);
	cout<<"Rand:	";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";	
	}
	cout<<endl;	
	return 0;
}

double* FIFO(int* pageRef){
	double* resultTable=(double*) malloc(20 * sizeof(double));
	deque<int> cacheList;
		
	for(int i=1; i<=20;i++){
		int hitCount=0;
		for(int j=0;j<10000;j++){
			int hitFlag=0;
			for(deque<int>::iterator p=cacheList.begin(); p!=cacheList.end(); *p++){
				if(pageRef[j]==*p){
					hitCount++;
					hitFlag=1;
					break;				
				}			
			}
			if(hitFlag==0){// page fault 
				if(cacheList.size()<i*5){// have free space
					cacheList.push_back(pageRef[j]);	
				}
				else{
					cacheList.pop_front();
					cacheList.push_back(pageRef[j]);				
				}
			}		
		}
		resultTable[i-1]=hitCount*1.0/10000*100;	
	}
	return resultTable;
}

double* Clock(int* pageRef){
	double* resultTable=(double*) malloc(20 * sizeof(double));
	vector<CacheClk> cacheList;
	
	for(int i=1; i<=20; i++){
		int hitCount=0;
		int pointer=0;
		for(int j=0;j<10000;j++){
			int hitFlag=0;
			for(int k=0;k<cacheList.size();k++){
				if(pageRef[j]==cacheList[k].pageNum){
					cacheList[k].clkBit=1;
					hitCount++;
					hitFlag=1;
					break;				
				}			
			}
			if(hitFlag==0){
				if(cacheList.size()<i*5){
					CacheClk c;
					c.pageNum=pageRef[j];
					c.clkBit=1;
					cacheList.push_back(c);
				}
				else{
					while(cacheList[pointer].clkBit!=0){
						cacheList[pointer].clkBit=0;
						pointer=(pointer+1)%(i*5);					
					}
					cacheList[pointer].pageNum=pageRef[j];
					cacheList[pointer].clkBit=1;				
				}			
			}	
		}
		resultTable[i-1]=hitCount*1.0/10000*100;	
	}
	return resultTable;
}

double* Rand(int* pageRef){
	double* resultTable=(double*) malloc(20 * sizeof(double));
	vector<int> cacheList;
	for(int i=1; i<=20; i++){
		int hitCount=0;
		for(int j=0;j<10000;j++){
			int hitFlag=0;
			for(int k=0;k<cacheList.size();k++){
				if(pageRef[j]==cacheList[k]){
					hitCount++;
					hitFlag=1;
					break;				
				}
			}
			if(hitFlag==0){
				if(cacheList.size()<i*5){
					cacheList.push_back(pageRef[j]);
				}
				else{
					int pageout=rand()%(i*5);
					cacheList[pageout]=pageRef[j];				
				}
			}
		}
		resultTable[i-1]=hitCount*1.0/10000*100;
	}
	return resultTable;
} 
