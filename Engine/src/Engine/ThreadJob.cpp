#include "ThreadJob.h"

using namespace engine_concurrent

ThreadJob::ThreadJob(EntryPoint* entryPoint, Priority priority, void* param)
	: m_pEntryPoint(entryPoint), // Entry point is a pointer to the functions to be run
	m_priority(priority), 
	m_param(param) // Param are the parameters to be passed to the entry point function  
{
}

void ThreadJob::run()
{
	m_pEntryPoint(param); // Run the passed-in functions with the passed params 
}

const Priority& ThreadJob::getPriority()
{
	return m_priority;
}

