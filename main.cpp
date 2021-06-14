#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include<game_world_classes.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main()
{
    int choice;
    do{
        Menu menu;
        choice=menu.loop();
        if(choice == 1){
            Scene scene;
            scene.loop();
        }
    }while(choice!=0);

}
