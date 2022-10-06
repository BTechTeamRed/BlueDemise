#include "ThreadPool.h"
#include <iostream>
#include <thread>
#include "Log.h"

using namespace engine_concurrent;

ThreadPool* ThreadPool::tpInstance = nullptr;
std::mutex ThreadPool::mutex; // Instantiate the mutex

// THE JOB OF THREAD POOL IS JUST TO MAKE ALL THE THREADS and maybe more things later 

//make 1 thread for each core, -1 to leave space for the main thread
ThreadPool::ThreadPool() : m_threads(std::thread::hardware_concurrency()-1)
{
    for (int i = 0; i < m_threads.size(); i++)
    {
        m_threads[i] = new Thread(i);
    }
}

ThreadPool* ThreadPool::getInstance()
{
	std::lock_guard<std::mutex> lock(mutex); // Lock getInstance so only one thread can call it at once
	if (tpInstance == nullptr) // This will only run the first time, if tpInstance is still null
	{
		tpInstance = new ThreadPool;
	}
	return tpInstance;
};
