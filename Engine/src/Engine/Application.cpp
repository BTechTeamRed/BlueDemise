#include "Application.h"
#include "ThreadPool.h"
#include "JobQueue.h"
#include "ThreadJob.h"
#include "Log.h"
#include <tuple>
#include <sstream>

using namespace engine_concurrent;

namespace Engine
{
	static std::mutex letterProtect;

	void job()
	{
		GE_CORE_ERROR("HELP!");
	}

	void count(void* param)
	{
		std::pair<int, char*>* data = (std::pair<int, char*>*) param;
		std::unique_lock lock(letterProtect);
		char letter = *data->second;
		int stop = data->first;
		lock.unlock();

		for (int i = 0; i < stop; i++)
		{
			GE_CORE_INFO("{0}: {1}", letter, i);
		}
		
		lock.lock();
		(* data->second)++;
		lock.unlock();
		delete data;
	}

	void spawn(void* param)
	{
		std::pair<int, char*>* data = (std::pair<int, char*>*) param;
		std::pair<int, char*>* otherParam;
		int stop = data->first;
		ThreadJob* job;
		std::string name;

		for (int i = 0; i < stop; i++)
		{
			std::stringstream str;
			str << "Count Job " << i;
			otherParam = new std::pair<int, char*>(i, data->second);
			name = str.str();
			job = new ThreadJob((EntryPoint*)count, (void*)otherParam, name);
			JobQueue::getInstance()->postJob(job);
		}
	}

	void Application::Run()
	{
		ThreadPool* thePool = ThreadPool::getInstance();
		char letter = 'A';
		for (int i = 1; i <= 10; i++)
		{
			std::stringstream str;
			str << "Spawn Job " << i;
			std::string name = str.str();
			std::pair<int, char*>* data = new std::pair(i, &letter);
			ThreadJob* job = new ThreadJob((EntryPoint*)spawn, (void*)data, name, Priority::NORMAL);
			JobQueue::getInstance()->postJob(job);
		}
		while (true);
	}
}