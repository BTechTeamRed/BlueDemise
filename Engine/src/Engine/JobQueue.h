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
		// D'tor
		~JobQueue();
		// Do not copy object (singleton)
		JobQueue(JobQueue& other) = delete;
		void operator=(const JobQueue&) = delete;

		// Post a job onto the job queue
		// job - job to post to queue
		void postJob(ThreadJob* job);
		// Grab a job from the queue
		// Return - the job at the front of the queue
		ThreadJob* getJob();

		// Method to get the singleton instance
		// Return - the singleton instance
		static JobQueue* getInstance();
	protected:
	private:
		// Private c'tor to prevent accidental construction
		JobQueue();

		// The instance
		static JobQueue* instance;
		// Mutex insuring only one instance is made
		static std::mutex instanceMutex;
		// Mutex protecting access to the job queue
		static std::mutex queueMutex;
		// List backing the priority queue 
		std::list<ThreadJob*>* m_jobList;
	};
}