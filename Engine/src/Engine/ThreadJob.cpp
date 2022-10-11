#include "ThreadJob.h"

using namespace Engine;

ThreadJob::ThreadJob(EntryPoint* entryPoint, void* param, std::string& name, Priority priority)
	: m_pEntryPoint(entryPoint),    // Entry point is a pointer to the functions to be run
	m_priority(priority), 
	m_param(param),                 // Parameters to be passed to the entry point function
	m_jobName(name)
{
}

void ThreadJob::run()
{
	m_pEntryPoint(m_param); // Run the passed-in functions with the passed params
}

const Priority& ThreadJob::getPriority()
{
	return m_priority;
}

const std::string& ThreadJob::getName()
{
	return m_jobName;
}
