#pragma once
#include <string>

namespace engine_concurrent
{
	//Type definition for thread function pointer
	typedef void EntryPoint(void* param);

	//Enum defining priority of job for thread
	enum class Priority
	{
		LOW, NORMAL, HIGH, CRITICAL
	};

	// Wrapper for jobs performed by threads
	// Goes into a queue to be polled by threads
	class ThreadJob
	{
	public:
		// C'tor
		// entryPoint - Pointer to the function called by thread
		// name		  - name of the job (used for debugging)
		// priority   - priority of the job
		ThreadJob(EntryPoint* entryPoint,
			void* param = nullptr,
			std::string& name = std::string("ANON JOB"),
			Priority priority = Priority::NORMAL);
		// Thread jobs should not be copied
		ThreadJob(ThreadJob& other) = delete;
		void operator=(ThreadJob& other) = delete;
		
		// Called by the thread
		void run();
		// Accessors
		const Priority& getPriority();
		const std::string& getName();
	protected:
		//

	private:
		// EntryPoint run by thread
		EntryPoint* m_pEntryPoint;
		// Parameters of entrypoint
		void* m_param;
		// Priority of job
        Priority m_priority;
		// Name of job
		std::string m_jobName;
		//Counter* m_pCounter;
	};
}