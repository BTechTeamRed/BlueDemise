#include "Thread.h"
#include "JobQueue.h"
#include "Log.h"

using namespace Engine;

// THREADS
#pragma region Threads

int Thread::m_sleepTime = 500;

Thread::Thread(int id) : 
	m_threadJob(nullptr),
	m_thread (new std::thread(&Thread::run, this)),
	m_id(id),
	m_run(true)
{
}

Thread::~Thread()
{
	m_run = false;
	m_thread->join();
	if (m_thread)
	{
		delete m_thread;
	}
	if (m_threadJob)
	{
		delete m_threadJob;
	}
}

// Returns the job being performed by the thread
ThreadJob* Thread::getJob()
{
	return m_threadJob;
}

// Method to run m_threadJob
void Thread::run()
{
	std::chrono::milliseconds sleepTime(m_sleepTime);
	JobQueue* queue = JobQueue::getInstance();
	do 
	{
		if (m_threadJob == nullptr) // If thread doesn't have a job
		{
			std::this_thread::sleep_for(sleepTime);
			m_threadJob = queue->getJob();
		}
		else // If thread has a job to do
		{
//			GE_CORE_TRACE("Thread {d} working on {s}", m_id, m_threadJob->getName());
			m_threadJob->run(); // Run the thread job
			//GE_CORE_TRACE("Thread {0} finished {1}", m_id, m_threadJob->getName());
			delete m_threadJob;
//			GE_CORE_TRACE("Thread {0} grabbing another job", m_id);
			m_threadJob = queue->getJob();
//			GE_CORE_TRACE("Thread {0} grabbed {1}", m_id, m_threadJob == nullptr ? "nothing" : m_threadJob->getName());
			//m_threadJob = nullptr; // Then go back to no job
		}
	} while (m_run); // This 'true' can be changed if we ever need to add a condition for threads to run
}

#pragma endregion