#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

class Minesweeper{
public:

    Minesweeper(){}
    Minesweeper(int width, int height, int numMines, string user, sf::Font font);

    struct Tile{
        Tile(int x,  int y){
            this->positionx = x;
            this->positiony = y;
        }
        bool HasMine = false;
        bool isFlagged = false;
        int surroundingMineCount = 0;
        Tile* neighbors[8];
        int positionx;
        int positiony;
        vector<sf::Sprite> undertilesprites;
        bool Hidden = true;
    };

    struct Score{
        Score(string name, string time){
            this->time = time;
            this->name = name;
        }
        string name;
        string time;
    };



    void Display(){
        sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);
        window.setFramerateLimit(60);
        random_device rd;
        mt19937 gen(rd());
        create_Tiles();
        generateMines(numMines, this->tiles, gen);
        setNeighbors(this->tiles);
        countMines(this->tiles);
        auto tileTextures = getTileTextures();
        auto UITextures = getUITextures();
        auto digitTextures = getDigitTextures();
        sf::Clock Clock;
        readLB();

        while (window.isOpen()) {
            currentelapsed = Clock.getElapsedTime();
            setTileTexture(window, tiles, tileTextures, UITextures, digitTextures, play, currentelapsed.asSeconds() + savedTime);
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if(event.type == sf::Event::MouseButtonPressed){
                    if(event.mouseButton.button == sf::Mouse::Left){
                        sf::Mouse mouse;
                        auto coordinate = mouse.getPosition(window);
                            if ((coordinate.y < windowHeight - 100) and (!gameWin) and (!gameLose) and (play) and (!leaderboard)) {
                                for (unsigned int i = 0; i < tiles.size(); i++) {
                                    auto tilebounds = tiles[i].undertilesprites[0].getGlobalBounds();
                                    if (tilebounds.contains(coordinate.x, coordinate.y)) {
                                        if ((!tiles[i].isFlagged) and (tiles[i].Hidden)) {
                                            tiles[i].Hidden = false;
                                        }
                                        if ((tiles[i].HasMine) and (!tiles[i].isFlagged)) {
                                            tiles[i].Hidden = true;
                                            this->gameLose = true;
                                            if (this->debug) {
                                                this->debug = false;
                                                debugMines(window, tiles, tileTextures);
                                            }
                                            for (unsigned int i = 0; i < tiles.size(); i++) {
                                                if (tiles[i].HasMine) {
                                                    tiles[i].Hidden = false;
                                                    if (tiles[i].isFlagged) {
                                                        sf::Sprite flag;
                                                        flag.setPosition(tiles[i].positionx, tiles[i].positiony);
                                                        flag.setTexture(tileTextures.at(11));
                                                        tiles[i].undertilesprites[1] = (flag);
                                                        sf::Sprite mine;
                                                        mine.setPosition(tiles[i].positionx, tiles[i].positiony);
                                                        mine.setTexture(tileTextures.at(9));
                                                        tiles[i].undertilesprites.push_back(mine);
                                                    }
                                                }
                                            }
                                            savedTime += currentelapsed.asSeconds();
                                            timer = false;
                                        }
                                        else if ((!tiles[i].HasMine) and (!tiles[i].isFlagged)) {
                                            if(tiles[i].surroundingMineCount == 0) {
                                                clearTilesRecursive(tiles[i], this->tiles);
                                            }
                                            if(checkifwin(this->tiles)){
                                                gameWin = true;
                                                this->debug = false;
                                                debugMines(window, tiles, tileTextures);
                                                mineCounter = 0;
                                                for(unsigned int i = 0; i < tiles.size(); i++){
                                                    if((tiles[i].HasMine) and (!tiles[i].isFlagged)){
                                                        tiles[i].isFlagged = true;
                                                    }
                                                }
                                                savedTime += currentelapsed.asSeconds();
                                                timer = false;
                                                createEntry();
                                                displayLB(window, tiles, tileTextures, UITextures, digitTextures, play,
                                                          currentelapsed.asSeconds());
                                            }
                                        }
                                    }
                                }
                        }else if (!leaderboard){
                            for(unsigned int i = 0; i < UIButtons.size(); i++){
                                auto buttonbounds = UIButtons[i].getGlobalBounds();
                                if(buttonbounds.contains(coordinate.x, coordinate.y)){
                                    if((i == 0) and (!gameLose) and (!gameWin)){
                                        play = !play;
                                        if(!play){
                                            savedTime += currentelapsed.asSeconds();
                                            timer = false;
                                        }else{
                                            timer = true;
                                            Clock.restart();
                                        }
                                    }if ((i == 1) and (!gameLose) and (!gameWin) and (play)){
                                        this->debug = !this->debug;
                                        debugMines(window, tiles, tileTextures);
                                    }if (i == 2){
                                        if(!play) {
                                            if((!gameWin) and (!gameLose)) {
                                                readLB();
                                                displayLB(window, tiles, tileTextures, UITextures, digitTextures, play,
                                                          currentelapsed.asSeconds());
                                                Clock.restart();
                                            }else{
                                                displayLB(window, tiles, tileTextures, UITextures, digitTextures, play,
                                                          currentelapsed.asSeconds());
                                            }
                                        }else{
                                            if((!gameWin) and (!gameLose)) {
                                                readLB();
                                                savedTime += currentelapsed.asSeconds();
                                                timer = false;
                                                play = false;
                                                displayLB(window, tiles, tileTextures, UITextures, digitTextures, play,
                                                          currentelapsed.asSeconds());
                                                Clock.restart();
                                                play = true;
                                                timer = true;
                                            }else{
                                                displayLB(window, tiles, tileTextures, UITextures, digitTextures, play,
                                                          currentelapsed.asSeconds());
                                            }
                                        }
                                    }if (i == 3){
                                        this->play = true;
                                        this->debug = false;
                                        this->gameLose = false;
                                        this->gameWin = false;
                                        savedTime = 0;
                                        Clock.restart();
                                        tiles.clear();
                                        UIButtons.clear();
                                        create_Tiles();
                                        generateMines(numMines, this->tiles, gen);
                                        setNeighbors(this->tiles);
                                        countMines(this->tiles);
                                        currentScores = "";
                                        timer = true;
                                        readLB();
                                    }
                                }
                            }
                        }
                    }else if(event.mouseButton.button == sf::Mouse::Right) {
                        if ((!gameLose) and (!gameWin) and (play) and (!leaderboard)) {
                            sf::Mouse mouse;
                            auto coordinate = mouse.getPosition(window);
                            for (unsigned int i = 0; i < tiles.size(); i++) {
                                auto tilebounds = tiles[i].undertilesprites[0].getGlobalBounds();
                                if ((tilebounds.contains(coordinate.x, coordinate.y)) and (tiles[i].Hidden)) {
                                    if (tiles[i].isFlagged) {
                                        tiles[i].isFlagged = false;
                                        mineCounter++;
                                    } else {
                                        tiles[i].isFlagged = true;
                                        mineCounter--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void create_Tiles();
    void generateMines(int& minecount, vector<Tile>& tiles, mt19937& gen);
    void setNeighbors(vector<Tile>& tiles);
    void countMines(vector<Tile>& tiles);
    void setTileTexture(sf::RenderWindow& window, vector<Tile>& tiles, map<int, sf::Texture>& tiletextures, map<int,sf::Texture>& UITextures, map<char, sf::Texture> digitTextures, bool play, int timeElapsedseconds);
    map<int, sf::Texture> getTileTextures();
    map<int, sf::Texture> getUITextures();
    void debugMines(sf::RenderWindow& window, vector<Tile>& tiles, map<int, sf::Texture>& tiletextures);
    void displayCounter(sf::RenderWindow &window, int& mineCounter, map<char, sf::Texture>& digitTextures);
    map<char, sf::Texture> getDigitTextures();
    bool checkifwin(vector<Tile>& tiles);
    void clearTilesRecursive(Tile& tile, vector<Tile>& tiles);
    void displayTime(sf::RenderWindow& window, int timeElapsedseconds, map<char, sf::Texture>& digitTextures);
    void displayLB(sf::RenderWindow& window, vector<Tile>& tiles, map<int, sf::Texture>& tiletextures, map<int,sf::Texture>& UITextures, map<char, sf::Texture> digitTextures, bool play, int timeElapsedseconds);
    void readLB();
    void createEntry();

private:
    string user;
    int windowWidth;
    int tilesinrow;
    int tilesincol;
    int windowHeight;
    int numMines;
    int numTiles;
    vector<Tile> tiles;
    bool play = true;
    bool debug = false;
    bool gameWin = false;
    bool gameLose = false;
    vector<sf::Sprite> UIButtons;
    int mineCounter;
    string currentScores;
    sf::Font font;
    vector<Score> scores;
    bool timer = true;
    sf::Time currentelapsed;
    int savedTime = 0;
    bool leaderboard = false;
};
