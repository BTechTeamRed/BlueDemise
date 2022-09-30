#pragma once
#include <thread>
#include "ThreadJob.h"

// this is the standard for each thread

namespace engine_concurrent
{
	// Wrapper class for the actual threads
	class Thread
	{
	public:
		//TODO: Threads need a null job to start with
		explicit Thread(); // explicit constructor for threads
		void assignJob(ThreadJob*);
		ThreadJob* getJob();
		void run();
		void init();

	protected:
		//

	private:
		std::thread* m_thread;
		ThreadJob* m_threadJob; // pointer to the thread's current job

	};
}
