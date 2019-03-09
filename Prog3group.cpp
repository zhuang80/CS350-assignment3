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

struct CacheEntry {
	int pageNum;
	int timeTag;
};

double* FIFO(int* a);
double* Clock(int* a);
double* Rand(int* a);
double * optimal(int * pages);
double * leastRecentlyUsed (int * workload);

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
	for(int i=0;i<pageAccess;i++){
		int randomIndex=rand()%pageAccess;
		int temp=eightyTotwenty[i];
		eightyTotwenty[i]=eightyTotwenty[randomIndex];
		eightyTotwenty[randomIndex]=temp;
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

	result=optimal(noLocal);
	cout<<"Optimal:";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";
	}
	cout<<endl;

	result=leastRecentlyUsed(noLocal);
	cout<<"LRU:	";
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

	result=optimal(eightyTotwenty);
	cout<<"Optimal:";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";
	}
	cout<<endl;

	result=leastRecentlyUsed(eightyTotwenty);
	cout<<"LRU:	";
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

	result=optimal(looping);
	cout<<"Optimal:";
	for(int i=0;i<20;i++){
		cout<<result[i]<<"	";
	}
	cout<<endl;

	result=leastRecentlyUsed(looping);
	cout<<"LRU:	";
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
		cacheList.clear();
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
		cacheList.clear();
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
		cacheList.clear();
	}
	return resultTable;
}

double * optimal( int * workload) {
	double * returnTable = (double *) malloc(sizeof(double) * 20);
	vector<int> cacheList;

	for(int i=1;i<=20;i++){
		int hitCount=0;
		for(int j=0;j<pageAccess;j++){
			int hitFlag=0;
			for(int k=0;k<cacheList.size();k++){
				if(workload[j]==cacheList[k]){
					hitCount++;
					hitFlag=1;
					break;
				}
			}
			if(hitFlag==0){
				if(cacheList.size()<i*5){
					cacheList.push_back(workload[j]);
				}
				else{
					int largestIndex=0;
					int pageout;
					for(int h=0;h<cacheList.size();h++){
						for(int g=j+1;g<pageAccess;g++){
							if(cacheList[h]==workload[g]){
								if(g>largestIndex){
									largestIndex=g;
									pageout=h;
								}
								break;
							}
						}
					}
					cacheList[pageout]=workload[j];
				}
			}
		}
		returnTable[i-1]=hitCount*1.0/pageAccess*100;
		cacheList.clear();
	}
	return returnTable;
}

double * leastRecentlyUsed (int * workload) {
	double * returnTable = (double *) malloc(sizeof(double) * 20);
	vector<CacheEntry> cacheList;

	for(int i=1; i<=20; i++){
		int hitCount=0;
		for(int j=0;j<pageAccess;j++){
			int hitFlag=0;
			
			for(int k=0;k<cacheList.size();k++){
				if(workload[j]==cacheList[k].pageNum){
					cacheList[k].timeTag=j;
					hitCount++;
					hitFlag=1;
					break;
				}
			}
			if(hitFlag==0){
				if(cacheList.size()<i*5){
					CacheEntry c;
					c.pageNum=workload[j];
					c.timeTag=j;
					cacheList.push_back(c);
				}
				else{
					int index=0;
					for(int h=1;h<cacheList.size();h++){

						if(cacheList[h].timeTag<cacheList[index].timeTag){
							index=h;
						}
					}
					
					cacheList[index].pageNum=workload[j];
					cacheList[index].timeTag=j;
				}
			}
		}
		returnTable[i-1]=hitCount*1.0/pageAccess*100;
		cacheList.clear();
	}
	return returnTable;
}
