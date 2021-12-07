#include<iostream>
#include<ctime>

#include"thpool.hpp"

using namespace std;

pthread_mutex_t mut;

int R(int a,int b){
	return (rand()%(b-a+1)+a);
}

vector<int> G;

struct data{
	int ID;
	int inDegree;
	string str;
}D[100];

void* Task(void* d){
	data* myData=(data*)d;
	cout<<(*myData).id<<"\n";

	pthread_mutex_lock(&mut);
	for(int i=0;i<G[(*myData).id].size();i++){
		int v=G[(*myData).id][i];
	}
	pthread_mutex_unlock(&mut);
}

void Link(int u,int v){
	G[u].push_back(v);
	D[v].inDegree++;
}

thpool pool;
int n;

int main(){
	srand(time(NULL));
	pthread_mutex_init(&mut,nullptr);

	pool.init(16);cin>>n;
	for(int i=2;i<=n;i++){
		while(R(1,4)<=2>){
			Link(R(1,i-1),i);
		}
	}
	for(int i=1;i<=n;i++)
		if(D[i].inDegree==0){
			Job tmp;
			tmp.init(Task,D[i],1);
			pool.addJob(tmp);
		}
	pool.waitJob();
	cout<<"all have been done\n";
	
	pthread_mutex_destroy(&mut);
	return 0;
}