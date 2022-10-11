THREAD README

Sections
S1. Threading
S1.1 Adding jobs for threads
S1.2 Considerations for threading
A1. System overview
A2. Future add-ons

--- S1. Threading ---

S1.1 Adding jobs for threads

1. #include ThreadJob.h and #include JobQueue.h if not already included 
2. Make a threadJob object and pass it the functions and params you want it to run, but cast the function to EntryPoint*.  If there are no params, just give it nullptr.  Please also give it a name, as it's helpful for debugging.
3. You can also give it a 'Priority' depending on whether or not it needs to be run ASAP, though this is not necessary.
4. Get the JobQueue and call its postJob() function, passing it a reference to the newly created threadJob object

 - The job is now on the queue!
 - Threads will automatically grab jobs from the queue when they don't have one to do!
 - Available threads are all made at runtime, so don't worry about them
 - If you need to pass in multiple parameters, they can be condensed into a pair or tuple.  Just make sure to cast the overall object to void* as is the expected input type for thread job parameters.
 
S1.2 Considerations for threading

++ The threads assume that they are working in a thread-safe environment,
   it is up to you to provide that.
   
++ Lazy-instantiated singletons are not thread-safe by default, please refer
   to ThreadPool.h and .cpp for thread-safe singletons.
   
++ Threads are self-contained worker threads that will seek for jobs to do on the
   JobQueue. You should use the JobQueue and ThreadJob to interact with them.
   
++ Not everything is critical, critical should be reserved for time sensitive tasks
   
--- End S1. Threading ---
--- A1. Systems Overview ---

++ ThreadPool is a singleton that holds the references to the worker threads

++ JobQueue is a singleton containing the priority queue of jobs for threads to do

++ ThreadJob is a wrapper for the function that a thread should run

++ Thread is a wrapper for a std::thread

++ ThreadPool is lazily instantiated, so it's getInstance() has to be called

++ Threads are self-contained worker threads that will seek for jobs to do on the
   JobQueue. 
   
--- End A1. Systems Overview ---
--- A2. Future Considerations ---

++ Using conditional variables to awake threads when a job is place on the queue
   instead of the current sleeping/checking cycle

++ Exposing queue size (possibly in reference to number of threads) to ensure worker
   threads are not overworked and allow callers to defer work to the main thread.
   
--- End A2. Future Considerations ---