
#include "Engine.h"
#include <iostream>
#include "Logger.h"
namespace SL
{
    Engine::Engine()
    {
        Logger::SetLevel(LogLevel::TRACE);
        Logger::Log("Engine::DefaultCtor()", LogLevel::TRACE);

    }

    Engine::Engine(const EngineSettings & inSettings)
    {
        Logger::SetLevel(LogLevel::TRACE);
        Logger::Log("Engine::Ctor()", LogLevel::TRACE);

        settings = inSettings;
    }

    Engine::~Engine()
    {
        Logger::Log("Engine::Dtor()", LogLevel::TRACE);

        main_thread.join();
    }

    void Engine::run_async()
    {
        Logger::Log("Engine::run()", LogLevel::TRACE);
        
        isRunning = true;
        main_thread = std::thread(&Engine::_run, this);
    } 


    void Engine::_run()
    {
        Logger::Log("Engine::_run()", LogLevel::TRACE);
        while(static_cast<bool>(isRunning))
        {
            Logger::Log("Running " + settings.name , LogLevel::INSANE);
        }
    }

} // namespace