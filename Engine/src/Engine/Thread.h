#pragma once
#include <thread>
#include "ThreadJob.h"

// this is the standard for each thread

namespace engine_concurrent
{
    // Wrapper class for the actual threads
    class Thread
    {
    public:
        // C'tor
        // id - identifying number for the thread
        explicit Thread(int id); // explicit constructor for threads

        // Accessor for job
        // Returns: the job being performed by the thread
        // Currently not used by anything, just for debugging
        ThreadJob* getJob();

        // Method run by the underlying thread, runs whatever is in m_threadJob
        void run();
    protected:
    private:
        // The thread that this class wraps
        std::thread* m_thread;

        // Pointer to current job being executed by thread
        ThreadJob* m_threadJob;
        // Unique id of thread
        int m_id;
    };
}