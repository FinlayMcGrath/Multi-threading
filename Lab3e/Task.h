#ifndef _TASK_H
#define _TASK_H

class Task
{
public:
	virtual ~Task()
	{

	}
	virtual void run() = 0;
};

#endif