
#pragma once

struct EventBase
{
    EventBase()
    {

    }

    ~EventBase()
    {

    }

    int type = 0;

};


struct OnInitEvent : public EventBase
{
    OnInitEvent()
    {
        type = 1;
    }

    ~OnInitEvent()
    {

    }
};


struct OnUpdateEvent : public EventBase
{
    OnUpdateEvent()
    {
        type = 2;
    }

    ~OnUpdateEvent()
    {

    }
};