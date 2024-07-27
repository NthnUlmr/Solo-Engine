
#include "Engine.h"
#include <iostream>
namespace SL
{
Engine::Engine()
{
    std::cout << "Engine::Ctor()" << std::endl;
}

Engine::~Engine()
{
    std::cout << "Engine::Dtor()" << std::endl;
}

void Engine::run()
{
    std::cout << "Engine::run()" << std::endl;
} 
}