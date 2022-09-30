#include "Thread.h"
#include "Log.h"
#include <thread>
#include <iostream>

using namespace engine_concurrent;

// THREADS
#pragma region Threads

Thread::Thread() : 
	m_threadJob(nullptr)
{
	m_thread = new std::thread(&Thread::run, this);
	m_thread->join();
}

void Thread::init()
{
	m_thread = new std::thread;
}

void Thread::assignJob(ThreadJob* newJob)
{
	m_threadJob = newJob;
}

ThreadJob* Thread::getJob()
{
	return m_threadJob;
}

void Thread::run()
{
	std::chrono::milliseconds sleepTime(500);
	do 
	{
		if (m_threadJob == nullptr) // If thread doesn't have a job
		{
			std::this_thread::sleep_for(sleepTime);
		} 
		else // If thread has a job to do
		{
			m_threadJob->run(); // Run the thread job
			m_threadJob = nullptr; // Then go back to no job
		}
	} while (true);
}

#pragma endregion

// THREAD JOBS

void ThreadJob::run()
{
	std::chrono::milliseconds sleepTime(500);
	count = 0;
	while (count < 100)
	{
		count++;
		GE_CORE_TRACE(count);
		std::this_thread::sleep_for(sleepTime);
	}
}