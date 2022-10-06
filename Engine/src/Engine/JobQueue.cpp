#include "Log.h"
#include "JobQueue.h"

using namespace engine_concurrent;

JobQueue* JobQueue::instance = nullptr;
std::mutex JobQueue::instanceMutex;
std::mutex JobQueue::queueMutex;

JobQueue* JobQueue::getInstance()
{
	std::unique_lock lock(instanceMutex);
	if (instance == nullptr)
	{
		instance = new JobQueue();
	}
	return instance;
}

JobQueue::JobQueue() : m_jobList(new std::list<ThreadJob*>()) {}
JobQueue::~JobQueue() { delete m_jobList; }

void JobQueue::postJob(ThreadJob* job)
{
	std::unique_lock lock(queueMutex);
	//std::condition_variable
	if (m_jobList->empty())
	{
		m_jobList->push_back(job);
		return;
	}
	// Iterate over the job list
	auto it = m_jobList->begin();
	for (; it != m_jobList->end(); ++it)// &&
		//(*it)->getPriority() >= job->getPriority(); ++it)
	{
	}
	m_jobList->insert(it, job);
	//lock.unlock();
}

ThreadJob* JobQueue::getJob()
{
	std::unique_lock<std::mutex> lock(queueMutex);
	ThreadJob* job = nullptr;

	if (!m_jobList->empty())
	{
		job = m_jobList->front();
		m_jobList->pop_front();
	}
	//lock.unlock();

	return job;
}