#pragma once
#include "Thread.h"
#include <vector>

namespace engine_concurrent
{
	//whatever code is in here is in the above namespace
	class ThreadPool
	{
	public:
		static ThreadPool& getInstance(); // Returning a pointer reference prevents duplicating the pool to make it singleton
		void getThread();

	protected:

	private:
		static ThreadPool tpInstance;
		ThreadPool(); // Ensure threadpool can't be instantiated outside the class
		std::vector<Thread> m_threads; // This is the pool of threads
	};
}
