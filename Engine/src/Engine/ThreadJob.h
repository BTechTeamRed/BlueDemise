#pragma once
#include <string>

namespace engine_concurrent
{
	typedef void EntryPoint(void* param);

	enum class Priority
	{
		LOW, NORMAL, HIGH, CRITICAL
	};

	// Things for the threads to do
	class ThreadJob
	{
		// threadJobs need to take in a generic set of arguments
		// and return something generic to the caller
	public:
		ThreadJob(EntryPoint* entryPoint,
			void* param = nullptr,
			std::string& name = std::string("ANON JOB"),
			Priority priority = Priority::NORMAL);
		//Thread jobs should not be copied
		ThreadJob(ThreadJob& other) = delete;
		void operator=(ThreadJob& other) = delete;
		
		void run();
		const Priority& getPriority();
	protected:
		//

	private:
		EntryPoint* m_pEntryPoint;
		void* m_param;
        Priority m_priority;
		std::string m_jobName;
		//Counter* m_pCounter;

		int count; // 
	};
}