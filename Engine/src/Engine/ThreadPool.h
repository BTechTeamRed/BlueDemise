#pragma once
#include "Thread.h"
#include <vector>
#include <mutex>

namespace engine_concurrent
{
	//whatever code is in here is in the above namespace
	class ThreadPool
	{
	public:
		// Prevent copying or assigning this class multiple times to keep it a singleton
		ThreadPool(ThreadPool& other) = delete; 
		void operator=(const ThreadPool&) = delete;

		// Returning a pointer reference prevents duplicating the pool to make it singleton
		static ThreadPool* getInstance(); 

	protected:

	private:
		static std::mutex mutex; // 'Static' mutex so the class can use this mutex without it being bound to an instance
		static ThreadPool* tpInstance;
		ThreadPool(); // Redefine constructor to ensure threadpool can't be instantiated outside the class
		std::vector<Thread> m_threads; // This is the pool of threads
	};
}
