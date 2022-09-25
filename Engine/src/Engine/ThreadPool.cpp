#include "ThreadPool.h"
#include <iostream>
#include <thread>

using namespace engine_concurrent;

ThreadPool* ThreadPool::tpInstance = nullptr;
std::mutex ThreadPool::mutex; // Instantiate the mutex

//make 1 thread for each core, -1 to leave space for the main thread
ThreadPool::ThreadPool() : m_threads(std::thread::hardware_concurrency()-1)
{
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

void ThreadPool::getThread()
{
	std::cout << "Get Thread";
	for (int i = 0; i < m_threads.size(); i++) // For each of the threads
	{
		std::cerr << "assigning job" << std::endl;
		m_threads[i].assignJob(new ThreadJob); // Assign a new ThreadJob to that thread
	}
};