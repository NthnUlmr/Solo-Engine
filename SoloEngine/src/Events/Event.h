#pragma once


namespace Solo {

	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED, WINDOW_ITEM_DROPPED, 
		WINNDOW_CONTENT_SCALED, WINDOW_ICONIFIED, WINDOW_REFRESHED,
		KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_REPOSITIONED, MOUSE_SCROLLED, MOUSE_ENTERED,
		// TODO: Joystick Events
		// TODO: Audio Events (device events?)
		APP_TICK, APP_UPDATE, APP_RENDER

	};



	enum class EventCategory
	{
		None = 0,
		APPLICATION = 0

	};




#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return (int)category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & (int)category;
		}

		bool handled = false;

	};



	class EventDispatcher
	{
	public: 
		EventDispatcher(Event& eventIn)
			: event_(eventIn)
		{

		}

		template<typename T, typename F>
		bool dispatch(const F& fun)
		{
			if (event_.GetEventType() == T::GetStaticType())
			{
				event_.handled |= fun(static_cast<T&>(event_));
				return true;
			}
			return false;
		}

	private:
		Event& event_;

	};

}