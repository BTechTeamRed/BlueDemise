#include "Engine/Utilities/Log.h"
#include "JobQueue.h"

using namespace Engine;

JobQueue* JobQueue::m_instance = nullptr;
std::mutex JobQueue::m_instanceMutex;
std::mutex JobQueue::m_queueMutex;

JobQueue* JobQueue::getInstance()
{
	std::unique_lock lock(m_instanceMutex);
	if (m_instance == nullptr)
	{
		m_instance = new JobQueue();
	}
	return m_instance;
}

JobQueue::JobQueue() : m_jobList(new std::list<ThreadJob*>()) {}
JobQueue::~JobQueue() { delete m_jobList; }

void JobQueue::postJob(ThreadJob* job)
{
	std::unique_lock lock(m_queueMutex);
	//std::condition_variable
	if (m_jobList->empty())
	{
		m_jobList->push_back(job);
		return;
	}
	// Iterate over the job list
	auto it = m_jobList->begin();
	for (; (it != m_jobList->end() // If we're not at the end of JobList,
        // and the priority of the job at the iterator is less or equal to the priority of the job we're adding,
        && ((*it)->getPriority() <= job->getPriority())); ++it) // Increment the iterator
	{}
    // Otherwise stop the iterator at that point and post the job
	m_jobList->insert(it, job);
}
// Grabs and returns the job at the front of m_jobList, which will be the highest priority job
ThreadJob* JobQueue::getJob()
{
	std::unique_lock<std::mutex> lock(m_queueMutex);
	ThreadJob* job = nullptr;

	if (!m_jobList->empty())
	{
		job = m_jobList->front();
		m_jobList->pop_front();
	}

	return job;
}