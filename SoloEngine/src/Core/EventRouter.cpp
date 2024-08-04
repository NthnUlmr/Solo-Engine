

#include "EventRouter.h"

#include "Logger.h"
#include "typeinfo"

namespace SL
{
    
    EventRouter::EventRouter()
    {
        Logger::Log("EventRouter::Ctor()", LogLevel::TRACE);

    }

    EventRouter::~EventRouter()
    {
        Logger::Log("EventRouter::Dtor()", LogLevel::TRACE);

    }

    void EventRouter::registerEvent(int addrAlias, EventBase& event, float delay)
    {
        Logger::Log("EventRouter::registerEvent()" + std::string(typeid(EventBase).name()), LogLevel::TRACE);
        for(callback fun: eventMap[event.type])
        {
            ((void(*)())fun)();
        }
    }

    void EventRouter::subscribe(int addrAlias, EventBase& event, callback callbackFun)
    {
        if(eventMap.count(event.type) > 0)
        {
            eventMap[event.type].push_back(callbackFun); 
        }
        else
        {
            eventMap[event.type] = std::vector<callback>({callbackFun});
        }
    }
} // Namespace