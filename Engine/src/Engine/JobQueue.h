#pragma once

#include <list>
#include <mutex>
#include "ThreadJob.h"

namespace engine_concurrent
{
	class JobQueue
	{
	public:
		~JobQueue();
		JobQueue(JobQueue& other) = delete;
		void operator=(const JobQueue&) = delete;

		void postJob(ThreadJob* job);
		ThreadJob* getJob();

		static JobQueue* getInstance();
	protected:
	private:
		JobQueue();

		static JobQueue* instance;
		static std::mutex instanceMutex;
		static std::mutex queueMutex;
		std::list<ThreadJob*>* m_jobList;
	};
}