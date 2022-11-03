#include "Engine/EventSystem.h"

using namespace Engine;

#pragma region Event
Event::Event(std::function<bool()> trigger)
	:m_trigger(trigger)
{}

bool Event::pollTrigger()
{
	return m_trigger();
}
#pragma endregion

#pragma region EventSystem
EventSystem::EventSystem() {}

EventSystem::~EventSystem()
{
	for (auto pair : m_subscriptions)
	{
		//Delete event
		if (pair.first)
		{
			delete pair.first;
		}
		//Delete subscriber list
		if (pair.second)
		{
			delete pair.second;
		}
	}
}

void EventSystem::update()
{
	Event* theEvent;
	//Iterate over all events
	for (auto subList : m_subscriptions)
	{
		//If activated
		if(subList.second != nullptr && 
			subList.first->pollTrigger())
		{
			//Notify subscribers
			for (auto sub : (*subList.second))
			{
				sub();
			}
		}
	}
}

bool EventSystem::registerEvent(std::string& name, Event* theEvent)
{
	Event* test = nullptr;
	bool registered = true;
	try
	{
		//If name is in use then test should be assigned
		test = m_events.at(name);
		//Check if there is still a subscription list
		if (m_subscriptions[test] == nullptr)
		{
			//Signal that name is available
			test = nullptr;
		}
		else
		{
			registered = false;
		}
	}
	catch (std::out_of_range e) 
	{
		//Register event if name is not registered
		m_events[name] = theEvent;
	}
	if (test == nullptr)
	{
		//Create subscription list for the event
		m_subscriptions[theEvent] = new std::list<std::function<void()>>();
	}

	return registered;
}

// Lazy deletion method
void EventSystem::unregisterEvent(Event* theEvent)
{
	try
	{
		//Find subscription list
		std::list<std::function<void()>>* subs = m_subscriptions.at(theEvent);
		if (subs)
		{
			delete subs;
		}
	}
	catch (std::out_of_range e) {}
}

Event* EventSystem::getEvent(std::string& name)
{
	Event* theEvent = nullptr;

	try
	{
		theEvent = m_events.at(name);
	}
	catch (std::out_of_range e) {}

	return theEvent;
}

bool EventSystem::registerListener(Event* theEvent, std::function<void()> subscriber)
{
	std::list<std::function<void()>>* test = nullptr;
	bool success = true;

	try
	{
		test = m_subscriptions.at(theEvent);
		if (test)
		{
			test->push_back(subscriber);
		}
	}
	catch(std::out_of_range e)
	{
		success = false;
	}

	return success;
}
#pragma endregion
