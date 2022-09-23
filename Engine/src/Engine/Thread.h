#pragma once
#include <thread>

// this is the standard for each thread

namespace engine_concurrent
{
	// Things for the threads to do
	class ThreadJob
	{
		// threadJobs need to take in a generic set of arguments
		// and return something generic to the caller
	public:
		void run();

	protected:
		//

	private:
		int count; // 

	};

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
