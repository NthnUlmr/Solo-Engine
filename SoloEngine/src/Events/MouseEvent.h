#pragma once
#include "Event.h"
#include "MouseCodes.h"


namespace Solo {

	class MouseEnteredEvent : public Event
	{
	public:
		MouseEnteredEvent() = default;

		bool entered = false;

		EVENT_CLASS_TYPE(EventType::MOUSE_ENTERED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);

	};

	class MouseRepositionedEvent : public Event
	{
	public:
		MouseRepositionedEvent() {};

		double xPos = 0.0;
		double yPos = 0.0;


		EVENT_CLASS_TYPE(EventType::MOUSE_REPOSITIONED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


	};



	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent() = default;

		double xOffset = 0.0;
		double yOffset = 0.0;


		EVENT_CLASS_TYPE(EventType::MOUSE_SCROLLED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


	};



	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent() = default;

		double xPos = 0.0;
		double yPos = 0.0;


		
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


	};



	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent() = default;

		MouseCode button = 0;



		EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_PRESSED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


	};



	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent() = default;

		MouseCode button = 0;



		EVENT_CLASS_TYPE(EventType::MOUSE_BUTTON_RELEASED);
		EVENT_CLASS_CATEGORY(EventCategory::APPLICATION);


	};


}