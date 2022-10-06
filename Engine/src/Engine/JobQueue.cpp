#include "Log.h"
#include "JobQueue.h"

using namespace engine_concurrent;

JobQueue* JobQueue::instance = nullptr;
std::mutex JobQueue::instanceMutex;
std::mutex JobQueue::queueMutex;

JobQueue* JobQueue::getInstance()
{
	GE_CORE_TRACE("Grabbing JobQueue instance");
	std::unique_lock lock(instanceMutex);
	if (instance == nullptr)
	{
		instance = new JobQueue();
	}
	return instance;
}

JobQueue::JobQueue() : m_jobList() {}
JobQueue::~JobQueue() { delete m_jobList; }

void JobQueue::postJob(ThreadJob* job)
{
	GE_CORE_TRACE("Posting a job...");
	std::unique_lock lock(queueMutex);
	//std::condition_variable
	GE_CORE_TRACE("Checking if board is empty");
	if (m_jobList->empty())
	{
		m_jobList->push_back(job);
		return;
	}
	GE_CORE_TRACE("Checking for board placement");
	// Iterate over the job list
	auto it = m_jobList->begin();
	for (; it != m_jobList->end(); ++it)// &&
		//(*it)->getPriority() >= job->getPriority(); ++it)
	{
	}
	m_jobList->insert(it, job);
	//lock.unlock();
	GE_CORE_TRACE("Job posted!");
}

ThreadJob* JobQueue::getJob()
{
	GE_CORE_TRACE("Grabbing a job");
	//std::unique_lock<std::mutex> lock(queueMutex);
	ThreadJob* job = nullptr;

	if (!m_jobList->empty())
	{
		GE_CORE_TRACE("Job found!");
		job = m_jobList->front();
		m_jobList->pop_front();
	}
	//lock.unlock();

	return job;
}