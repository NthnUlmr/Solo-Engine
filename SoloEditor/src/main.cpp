
#include <Solo.h>
#include <iostream>
int main(int argc, char **argv){


    // Setup Editor Program
    SL::EngineSettings editorSettings;
    editorSettings.name = "Editor";
    SL::Engine Editor = SL::Engine(editorSettings);

    Editor.run_async();

    // Setup Client 1 Program
    SL::EngineSettings engineSettings;
    engineSettings.name = "Engine";
    SL::Engine GameClient1 = SL::Engine(engineSettings);

    GameClient1.run_async();


    // Setup Client 2 Program
    SL::EngineSettings engineSettings2;
    engineSettings2.name = "Engine";
    SL::Engine GameClient2 = SL::Engine(engineSettings2);

    GameClient2.run_async();


    while(true) SL::Logger::Log("Waiting", SL::LogLevel::INSANE); // Hold terminal open
};