#include "JobQueue.h"

using namespace engine_concurrent;

static JobQueue* JobQueue::INSTANCE = nullptr;

static JobQueue* JobQueue::getInstance()
{
	if (INSTANCE == nullptr)
	{
		INSTANCE = new JobQueue();
	}
	return INSTANCE;
}

JobQueue::JobQueue() : m_jobList() {}

void JobQueue::postJob(ThreadJob* job)
{
	if (m_jobList.empty())
	{
		m_jobList.push_back(job);
		return;
	}
	// Iterate over the job list
	auto it = m_jobList = m_jobList.begin();
	for (;it->getPriority() >= job->getPriority() && 
		it != m_jobList.end(); ++it)
	{
	}
	m_jobList.insert(it, job);
}

ThreadJob* JobQueue::getJob()
{
	ThreadJob* job = nullptr;

	if (!m_jobList.empty())
	{
		job = m_jobList.front();
		m_jobList.pop_front();
	}

	return job;
}