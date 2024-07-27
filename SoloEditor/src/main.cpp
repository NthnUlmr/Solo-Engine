
#include <Solo.h>

int main(int argc, char **argv){

    
    SL::Engine Editor = SL::Engine();

    Editor.run();




    SL::Engine Game = SL::Engine();

    Game.run();


    while(true); // Hold terminal open
};