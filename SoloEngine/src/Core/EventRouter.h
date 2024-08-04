

#pragma once

#include "EventBase.h"
#include <map>
#include <vector>
namespace SL
{
    class EventRouter
    {
    public:
        EventRouter();

        ~EventRouter();

        void registerEvent(int addrAlias, EventBase& event, float delay = 0.0f);

        void subscribe(int addrAlias, EventBase& event, void* callbackFun);
    
    private:
        typedef  void* callback;
        std::map<int, std::vector<callback>> eventMap;

    };
} // Namespace