#pragma once
#include <string>

namespace Engine
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

		// Thread jobs should not be copied, able to be cloned or assigned.  These statements prevent this:
		ThreadJob(ThreadJob& other) = delete;
		void operator=(ThreadJob& other) = delete;
		
		// Called by the thread to run the passed in function with the passed in parameters
		void run();
		// Accessors to return job properties
		const Priority& getPriority();
		const std::string& getName();
	private:
		// EntryPoint run by thread, contains the function provided by the caller
		EntryPoint* m_pEntryPoint;
		void* m_param;          // Parameters of entrypoint
        Priority m_priority;    // Priority of job
		std::string m_jobName;  // Name of job, for debugging
	};
}