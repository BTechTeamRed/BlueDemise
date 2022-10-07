#pragma once

#include <list>
#include <mutex>
#include "ThreadJob.h"

namespace engine_concurrent
{
	// Singleton
	// Queue for jobs to go onto to be picked up by threads
	class JobQueue
	{
	public:
		// C'tor
		~JobQueue();

		// The job queue is a singleton, so it should not be copied or cloned
		JobQueue(JobQueue& other) = delete;
		void operator=(const JobQueue&) = delete;

		// Post a job onto the job queue
		void postJob(ThreadJob* job);

		// Grab a job from the queue, will be called by threads when they need a job
		// Returns: the job at the front of the queue
		ThreadJob* getJob();

		// Method to get the singleton instance
		// Returns: the singleton instance
		static JobQueue* getInstance();
	protected:
	private:
		// Private c'tor to prevent accidental construction
		JobQueue();

		static JobQueue* instance;          // The instance
		static std::mutex instanceMutex;    // Mutex insuring only one instance is made
		static std::mutex queueMutex;       // Mutex protecting access to the job queue
		std::list<ThreadJob*>* m_jobList;   // List backing the priority queue
	};
}