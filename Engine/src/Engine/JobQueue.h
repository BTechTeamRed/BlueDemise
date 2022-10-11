#pragma once

#include <list>
#include <mutex>
#include "ThreadJob.h"

namespace Engine
{
	// Singleton
	// Queue for jobs to go onto to be picked up by threads
	// 
	// To request a job to be done for a thread a ThreadJob
	// object must be created with the requesite function pointer
	// to the function where the work will be done. 
	// 
	class JobQueue
	{
	public:
		// D'tor
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
	private:
		// Private c'tor to prevent accidental construction
		JobQueue();

		static JobQueue* m_instance;          // The instance
		static std::mutex m_instanceMutex;    // Mutex insuring only one instance is made
		static std::mutex m_queueMutex;       // Mutex protecting access to the job queue
		std::list<ThreadJob*>* m_jobList;   // List backing the priority queue
	};
}