#include "JobQueue.h"

using namespace engine_concurrent;

JobQueue* JobQueue::instance = nullptr;
std::mutex JobQueue::mutex;

JobQueue* JobQueue::getInstance()
{
	if (instance == nullptr)
	{
		instance = new JobQueue();
	}
	return instance;
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
	auto it = m_jobList.begin();
	for (;(*it)->getPriority() >= job->getPriority() &&
		it != m_jobList.end(); ++it)
	{
	}
	m_jobList.insert(it, job);
}

ThreadJob* JobQueue::getJob()
{
	std::unique_lock<std::mutex> lock(mutex);
	ThreadJob* job = nullptr;

	if (!m_jobList.empty())
	{
		job = m_jobList.front();
		m_jobList.pop_front();
	}

	return job;
}