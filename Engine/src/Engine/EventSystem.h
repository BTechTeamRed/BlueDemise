#pragma once

#include <functional>
#include <map>
#include <mutex>

namespace Engine
{
	/// <summary>
	/// A wrapper that encapsulates a function that triggers other functions
	/// </summary>
	class Event
	{
	public:
		/// <summary>
		/// Event C'tor
		/// </summary>
		/// <param name="trigger">Function that triggers the event</param>
		Event(std::function<bool()> trigger);
		//Should not be copied
		Event(Event& other) = delete;
		void operator=(Event& other) = delete;
		
		/// <summary>
		/// Polls if the event has been triggered
		/// </summary>
		/// <returns>True - if the event was triggered</returns>
		bool pollTrigger();
	protected:
	private:
		/// <summary>
		/// Function that triggers the event
		/// </summary>
		std::function<bool()> m_trigger;
	};

	/// <summary>
	/// System that handles and registers events
	/// </summary>
	class EventSystem
	{
	public:
		//D'tor
		~EventSystem();
		//Deleted copy constructor and assignment for singleton
		EventSystem(EventSystem& other) = delete;
		void operator=(EventSystem& other) = delete;

		/// <summary>
		/// Updates all events
		/// </summary>
		void update();
		/// <summary>
		/// Register an event with the event system
		/// </summary>
		/// <param name="name">Name of the event within the system</param>
		/// <param name="theEvent">Event to be registered</param>
		/// <returns>True if registration succeeds</returns>
		bool registerEvent(std::string& name, Event* theEvent);
		/// <summary>
		/// Removes a event from the registry
		/// </summary>
		/// <param name="theEvent">Event to unregister</param>
		void unregisterEvent(Event* theEvent);
		/// <summary>
		/// Get a registered event by name
		/// </summary>
		/// <param name="name">Name of the registered event</param>
		/// <returns>The event if it exists, nullptr otherwise</returns>
		Event* getEvent(std::string& name);
		/// <summary>
		/// Register a function to an event
		/// </summary>
		/// <param name="theEvent">Event to subscriber to</param>
		/// <param name="subscriber">The subscribing function</param>
		/// <returns>True if registration succeeds</returns>
		bool registerListener(Event* theEvent, std::function<void()> subscriber);

		/// <summary>
		/// Grabs singleton instance
		/// </summary>
		/// <returns>The instance</returns>
		static EventSystem* getInstance();
	protected:
	private:
		//Private c'tor to prevent accidental instantation
		EventSystem();

		/// <summary>
		/// Maps an event to a string
		/// </summary>
		std::map<std::string, Event*> m_events;
		/// <summary>
		/// Maps event to list of listeners
		/// </summary>
		std::map<Event*, std::list<std::function<void()>>*> m_subscriptions;
		/// <summary>
		/// Mutex to protect singleton instance
		/// </summary>
		std::mutex m_instanceMutex;

		/// <summary>
		/// Singleton instance
		/// </summary>
		static EventSystem* m_instance;
	};
}