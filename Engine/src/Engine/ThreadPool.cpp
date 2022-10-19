#include "ThreadPool.h"
#include "Log.h"

using namespace Engine;

ThreadPool* ThreadPool::m_tpInstance = nullptr;
std::mutex ThreadPool::m_mutex; // Instantiate the mutex

// THE JOB OF THREAD POOL IS JUST TO MAKE ALL THE THREADS and maybe more things later 

// Constructor for thread pool, is run once at the start and shouldn't be run again
//make 1 thread for each core, -1 to leave space for the main thread
ThreadPool::ThreadPool() : m_threads(std::thread::hardware_concurrency()-1)
{
    for (int i = 0; i < m_threads.size(); i++)
    {
        m_threads[i] = new Thread(i);
    }
}

// D'tor
ThreadPool::~ThreadPool()
{
	for (auto t : m_threads)
	{
		delete t;
	}
}

// Get the instance of the thread pool, returns a pointer to the singleton instance
ThreadPool* ThreadPool::getInstance()
{
	std::lock_guard<std::mutex> lock(m_mutex); // Lock getInstance so only one thread can call it at once
	if (m_tpInstance == nullptr) // This will only run the first time, if tpInstance is still null
	{
		m_tpInstance = new ThreadPool;
	}
	return m_tpInstance;
};
