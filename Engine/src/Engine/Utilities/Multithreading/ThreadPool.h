#pragma once
#include "Thread.h"
#include <vector>
#include <mutex>

namespace Engine
{
	//whatever code is in here is in the above namespace
	// Singleton
	// Object pool for threads
	class ThreadPool
	{
	public:
		// D'tor
		~ThreadPool();
		// Prevent copying or assigning this class multiple times to keep it a singleton
		ThreadPool(ThreadPool& other) = delete; 
		void operator=(const ThreadPool&) = delete;

		// Returning a pointer reference prevents duplicating the pool to make it singleton
		static ThreadPool* getInstance(); 

	private:
		// Private c'tor to prevent accidental construction
		ThreadPool(); // Redefine constructor to ensure threadpool can't be instantiated outside the class

		// Mutex ensuring that only one instance of the class is created
		static std::mutex m_mutex; // 'Static' mutex so the class can use this mutex without it being bound to an instance
		// Singleton instance
		static ThreadPool* m_tpInstance;
		// The threads
		std::vector<Thread*> m_threads; // This is the pool of threads
	};
}
