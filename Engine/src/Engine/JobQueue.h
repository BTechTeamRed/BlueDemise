#pragma once

#include <list>
#include "ThreadJob.h"

namespace engine_concurrent
{
	class JobQueue
	{
	public:
		JobQueue(JobQueue& other) = delete;
		void operator=(const JobQueue&) = delete;

		void postJob(ThreadJob* job);
		ThreadJob* getJob();

		static JobQueue* getInstance();
	protected:
	private:
		JobQueue();

		static JobQueue* INSTANCE;
		std::list<ThreadJob*> m_jobList;
	};
}