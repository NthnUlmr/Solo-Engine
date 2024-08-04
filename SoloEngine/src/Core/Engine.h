

#pragma once
#include <assert.h>
#include <string>
#include <thread>
#include "ThreadSafe.h"
#include "EventRouter.h"
namespace SL
{

	struct EngineCLArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const { assert(index < Count);  return Args[index]; }
	};

	struct EngineSettings {
		std::string name = "Empty Solo App";
		std::string workingDir;
		EngineCLArgs	clArgs;
	};  


    class Engine
    {
    public:
        Engine();
        Engine(const EngineSettings & settings);
        ~Engine();

        void run_async();

        static void OnInit();
        static void OnUpdate();
        

    private:

        void _run();

        ThreadSafe<bool> isRunning;

        std::thread main_thread;

        EngineSettings settings;

        EventRouter router;

        OnUpdateEvent updateEvent;
        OnInitEvent initEvent;

};

} // SL