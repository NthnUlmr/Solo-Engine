#pragma once
#include "Event.h"
namespace Solo {


	

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;


		EVENT_CLASS_TYPE(EventType::WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION)

	};



	class WindowItemDroppedEvent : public Event
	{
	public:
		WindowItemDroppedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_ITEM_DROPPED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};


	class WindowContentScaledEvent : public Event
	{
	public:
		WindowContentScaledEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINNDOW_CONTENT_SCALED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_FOCUS);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindoIconifiedEvent : public Event
	{
	public:
		WindoIconifiedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_ICONIFIED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindowMaximizedEvent : public Event
	{
	public:
		WindowMaximizedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindowRepositionedEvent : public Event
	{
	public:
		WindowRepositionedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_MOVED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindowRefreshedEvent : public Event
	{
	public:
		WindowRefreshedEvent() = default;

		EVENT_CLASS_TYPE(EventType::WINDOW_REFRESHED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent() = default;

		int width = 0;
		int height = 0;

		EVENT_CLASS_TYPE(EventType::WINDOW_RESIZE);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};





}