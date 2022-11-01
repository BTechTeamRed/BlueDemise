#pragma once

#include <functional>

namespace Engine
{
	class Event
	{
	public:
		Event(std::function<void> trigger);
		bool pollTrigger();
		void onTrigger();
	protected:
	private:
		std::function<void> m_trigger;
		
	};

	class EventSystem
	{
	public:
		EventSystem(EventSystem& other) = delete;
		void operator=(EventSystem& other) = delete;

		void update();
	protected:
	private:
		std::list<Event*> m_events;
	};
}