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
        // Return - the job performed by the thread
        ThreadJob* getJob(); // Does this have use
        // Method run by the underlying thread
        void run();
    protected:
    private:
        // The thread that this class wraps
        std::thread* m_thread;
        // Current job being executed by thread
        ThreadJob* m_threadJob; // pointer to the thread's current job
        // Unique id of thread
        // TO DO: ensure the id is unique to thread
        int m_id;
    };
}