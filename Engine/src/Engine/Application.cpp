#include "Application.h"
#include "ThreadPool.h"
#include "JobQueue.h"
#include "ThreadJob.h"
#include "Log.h"
#include <tuple>

using namespace engine_concurrent;

namespace Engine
{
	//static std::mutex letterProtect;

	void job()
	{
		GE_CORE_ERROR("HELP!");
	}

	void count(void* param)
	{
		std::pair<int, char*>* data = (std::pair<int, char*>*) param;
		char letter = *data->second;
		int stop = data->first;

		GE_CORE_TRACE("Working...");
		for (int i = 0; i < stop; i++)
		{
			GE_CORE_INFO("{0}: {1}", letter, i);
		}
		//std::unique_lock lock(letterProtect);
		(* data->second)++;
		//lock.unlock();
		delete data;
	}

	void Application::Run()
	{
		GE_CORE_TRACE("Starting application!");
		GE_CORE_TRACE("Creating thread pool");
		ThreadPool* thePool = ThreadPool::getInstance();
		char letter = 'a';
		GE_CORE_TRACE("Creating jobs...");
		for (int i = 0; i < 100; i++)
		{
			GE_CORE_TRACE("\tCreating job {0}", i);
			std::string name = "Test Job " + i;
			std::pair<int, char*>* data = new std::pair(i, &letter);
			ThreadJob* job = new ThreadJob((EntryPoint*)count, (void*)data, name, Priority::NORMAL);
			GE_CORE_TRACE("\tJob {0} created", i);
			GE_CORE_TRACE("\t Adding job {0} to queue", i);
			JobQueue::getInstance()->postJob(job);
			GE_CORE_TRACE("Job {0} added to queue", i);
		}
		GE_CORE_TRACE("Job's done!");
		while (true);
	}
}