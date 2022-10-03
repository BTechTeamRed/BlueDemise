#include "Thread.h"
#include "JobQueue.h"
#include "Log.h"
#include <thread>
#include <iostream>

using namespace engine_concurrent;

// THREADS
#pragma region Threads

Thread::Thread() : 
	m_threadJob(nullptr),
	m_thread (new std::thread(&Thread::run, this))
{

}


ThreadJob* Thread::getJob()
{
	return m_threadJob;
}

void Thread::run()
{
	std::chrono::milliseconds sleepTime(500);
	JobQueue* queue = JobQueue::getInstance();
	do 
	{
		if (m_threadJob == nullptr) // If thread doesn't have a job
		{
			//TODO: Change from static sleep time to waiting for notify from JobQueue   
			std::this_thread::sleep_for(sleepTime);
			m_threadJob = queue->getJob();
		} 
		else // If thread has a job to do
		{
			m_threadJob->run(); // Run the thread job
			m_threadJob = queue->getJob();
			//m_threadJob = nullptr; // Then go back to no job
		}
	} while (true); // This 'true' can be changed if we ever need to add a condition for threads to run
}

#pragma endregion