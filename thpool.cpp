#ifndef THPOOLCPP
#define THPOOLCPP

#include<pthread.h>
#include<iostream>
#include<unistd.h>

#include"thpool.hpp"

void Job::init(decltype(routine) r,void* a,int p){
	routine=r;args=a;priority=p;
}
bool operator <(Job a,Job b){
	return a.priority<b.priority;
}
auto Job::getRoutine()->decltype(routine){
	return routine;
}
auto Job::getArgs()->decltype(args){
	return args;
}

void* threadWork(void* args){
	thpool* pool=reinterpret_cast<thpool*>(args);
	while(true){
		pthread_mutex_lock(&((*pool).mutQS));
		while((*pool).queueSize==0){
			pthread_cond_wait(&((*pool).condWorker),&((*pool).mutQS));
		}
		auto job=(*pool).getHead();
		(*pool).popHead();
			pthread_mutex_lock(&((*pool).mutUpd));
			(*pool).activeWorker++;
			pthread_mutex_unlock(&((*pool).mutUpd));
		pthread_mutex_unlock(&((*pool).mutQS));

		auto routine=job.getRoutine();
		auto args=job.getArgs();
		routine(args);

		pthread_mutex_lock(&((*pool).mutUpd));
		(*pool).activeWorker--;
		pthread_mutex_unlock(&((*pool).mutUpd));
		pthread_cond_signal(&((*pool).condPool));
	}
}

void thpool::init(int size){
	if(maxSize){
		std::cout<<"size have been set\n";
		exit(0);
	}
	pthread_mutex_init(&mutUpd,nullptr);
	pthread_mutex_init(&mutQS,nullptr);
	pthread_cond_init(&condWorker,nullptr);
	pthread_cond_init(&condPool,nullptr);
	th.resize(maxSize=size);
	for(int i=0;i<maxSize;i++)
		pthread_create(&th[i],nullptr,threadWork,reinterpret_cast<void*>(this));
}

void thpool::destroy(){
	pthread_mutex_destroy(&mutUpd);
	pthread_mutex_destroy(&mutQS);
	pthread_cond_destroy(&condWorker);
	pthread_cond_destroy(&condPool);
	for(int i=0;i<maxSize;i++);
		// pthread_kill(th[i],SIGALRM);
		// kill the threads in the pool
		// but I don't know how to kill them
		// so they will stay in the loop until the process ends
}

int thpool::getMaxSize(){
	return maxSize;
}

void thpool::addJob(Job j){
	pthread_mutex_lock(&mutQS);
	q.push(j);queueSize++;
	pthread_mutex_unlock(&mutQS);
	pthread_cond_signal(&condWorker);
}

void thpool::waitJob(){
	pthread_mutex_lock(&mutUpd);
	while(activeWorker>0||queueIsEmpty==false){
		pthread_cond_wait(&condPool,&mutUpd);
	}
	pthread_mutex_unlock(&mutUpd);
}

Job thpool::getHead(){
	return q.top();
}

void thpool::popHead(){
//	mutQS is working (queueSize)
	q.pop();queueSize--;
	if(queueSize==0){
		pthread_mutex_lock(&mutUpd);
		queueIsEmpty=true;
		pthread_mutex_unlock(&mutUpd);
		pthread_cond_signal(&condPool);
	}
}

#endif