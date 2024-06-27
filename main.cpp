#include <iostream>
#include <SFML/Graphics.hpp>
#include "WelcomeWindow.h"
#include "Minesweeper.h"
using namespace std;

int main() {
    int windowWidth;
    int windowHeight;
    int numMines;
    fstream configFile;
    configFile.open("files/board_config.cfg", ios_base::in);
    if(configFile.is_open()) {
        string info;
        getline(configFile, info);
        windowWidth = (stoi(info) * 32);
        getline(configFile, info);
        windowHeight = (stoi(info) * 32) + 100;
        getline(configFile, info);
        numMines = stoi(info);
    }
    sf::Font font;
    font.loadFromFile("files/font.ttf");

    WelcomeWindow window(windowWidth, windowHeight, font);
    bool start_Game = window.Display();
    if(start_Game){
        string user = window.ReturnUsername();
        Minesweeper game(windowWidth, windowHeight, numMines, user, font);
        game.Display();
    }

    return 0;
}
