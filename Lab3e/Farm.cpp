#include "Farm.h"

void Farm::addTask(Task* task)
{
	taskQueue.push(task);
}

void Farm::run()
{
	time = 0;
	//set each thread working tasks
	for (int i = 0; i < numThreads; i++)
	{
		myThread[i] = new thread(mem_fun(&Farm::worker), this);
	}

	for (int i = 0; i < numThreads; i++)
	{
		myThread[i]->join();
	}	
	cout << "Computing the slice took " << time / 64 << " ms." << endl;
}

void Farm::worker()
{
	//while there is work to be done
	while (!taskQueue.empty()) 
	{ 		
		//prevents duplication of tasks
		taskMutex.lock();

		if (taskQueue.empty())
		{
			//prevents mutex from being permanently locked
			taskMutex.unlock();
			//prevents trying to access the empty queue
			break;
		}		
		Task* t = taskQueue.front();
		taskQueue.pop();	
		taskMutex.unlock();		

		the_clock::time_point start_slice = the_clock::now();
		//do task
		t->run();
		// Stop timing
		the_clock::time_point end_slice = the_clock::now();

		// Compute the difference between the two times in milliseconds
		auto time_taken = duration_cast<milliseconds>(end_slice - start_slice).count();
		
		unique_lock<mutex> lock(timeMutex);
		time += time_taken;
	}
}
