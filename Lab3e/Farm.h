#ifndef _FARM_TASK_H
#define _FARM_TASK_H

#include "Task.h"
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

// Import things we need from the standard library
using std::chrono::duration_cast;
using std::chrono::milliseconds;
typedef std::chrono::steady_clock the_clock;
using namespace std;

#define numThreads 4

class Farm
{
public:
	Farm(){};
	~Farm(){};
	void addTask(Task *task);
	void run();
private:
	void worker();
	mutex taskMutex, timeMutex;
	queue<Task *> taskQueue;
	thread* myThread[numThreads];
	long long time;
};

#endif