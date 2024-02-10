#pragma once
#define _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING
#include "App.h"

extern Solo::App* Solo::CreateApp(Solo::AppCLArgs);


int main(int argc, char** argv)
{
    std::cout << "Starting SoloEngine App" << std::endl;
    Solo::App* app = nullptr;
    app = Solo::CreateApp({ argc, argv });
    app->run();

    std::cout << "Stopping SoloEngine App" << std::endl;
    app->stop();
    if (app) delete app;
}