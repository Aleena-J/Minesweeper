#include "Minesweeper.h"

Minesweeper::Minesweeper(int width, int height, int numMines, string user, sf::Font font) {
    windowWidth = width;
    windowHeight = height;
    this->numMines = numMines;
    this->numTiles = ((height - 100)/32) * (width/32);
    this->numTiles = ((height - 100)/32) * (width/32);
    tilesinrow = width/32;
    tilesincol = (height-100)/32;
    this->user = user;
    this->font = font;
    random_device rd;
    mt19937 gen(rd());
}

void Minesweeper::create_Tiles() {
    for(int i = 0; i < windowHeight - 100; i += 32){
        for(int j = 0; j < windowWidth; j += 32){
            Tile newTile = Tile(j, i);
            tiles.push_back(newTile);
        }
    }
}

void Minesweeper::generateMines(int& minecount, vector<Tile> &tiles, mt19937& gen) {
    vector<int> randomNums;
    uniform_int_distribution<> distr(0, numTiles-1);
    for(unsigned int i = 0; i < numMines;){
        int tilenumber = distr(gen);
        if(find(randomNums.begin(), randomNums.end(), tilenumber) == randomNums.end()) {
            randomNums.push_back(tilenumber);
            i++;
        }
    }
    for(unsigned int i = 0; i < randomNums.size(); i++){
        tiles[randomNums[i]].HasMine = true;
    }
    mineCounter = numMines;
}

void Minesweeper::setTileTexture(sf::RenderWindow &window, vector<Tile>& tiles, map<int, sf::Texture>& tiletextures, map<int,sf::Texture>& UITextures, map<char, sf::Texture> digitTextures, bool play, int timeElapsedseconds) {
    if(play) {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if ((!tiles[i].Hidden) and ((!tiles[i].isFlagged)) and (tiles[i].undertilesprites.size() == 1)) {
                sf::Sprite revealed;
                revealed.setPosition(tiles[i].positionx, tiles[i].positiony);
                revealed.setTexture(tiletextures.at(0));
                tiles[i].undertilesprites.push_back(revealed);
                if (tiles[i].HasMine) {
                    sf::Sprite mine;
                    mine.setPosition(tiles[i].positionx, tiles[i].positiony);
                    mine.setTexture(tiletextures.at(9));
                    tiles[i].undertilesprites.push_back(mine);
                } else if (tiles[i].surroundingMineCount != 0) {
                    sf::Sprite mineCount;
                    mineCount.setPosition(tiles[i].positionx, tiles[i].positiony);
                    mineCount.setTexture(tiletextures.at(tiles[i].surroundingMineCount));
                    tiles[i].undertilesprites.push_back(mineCount);
                }
            } else if (tiles[i].Hidden) {
                if (tiles[i].undertilesprites.empty()) {
                    sf::Sprite hidden;
                    hidden.setPosition(tiles[i].positionx, tiles[i].positiony);
                    hidden.setTexture(tiletextures.at(10));
                    tiles[i].undertilesprites.push_back(hidden);
                } else if ((tiles[i].isFlagged) and (tiles[i].undertilesprites.size() == 1)) {
                    sf::Sprite flag;
                    flag.setPosition(tiles[i].positionx, tiles[i].positiony);
                    flag.setTexture(tiletextures.at(11));
                    tiles[i].undertilesprites.push_back(flag);
                } else if ((tiles[i].undertilesprites.size() == 2) and (!tiles[i].isFlagged) and (!this->debug)) {
                    tiles[i].undertilesprites.pop_back();
                } else if ((tiles[i].undertilesprites.size() == 2) and (!tiles[i].isFlagged) and (!tiles[i].HasMine) and
                           (this->debug)) {
                    tiles[i].undertilesprites.pop_back();
                }
            }
        }
    }
    window.clear(sf::Color(255,255,255));
    if(this->play){
        if(this->UIButtons.empty()) {
            sf::Sprite playButton;
            playButton.setTexture(UITextures.at(7));
            playButton.setPosition((tilesinrow * 32) - 240, 32 * (tilesincol + 0.5f));
            this->UIButtons.push_back(playButton);
        }else{
            sf::Sprite playButton;
            playButton.setTexture(UITextures.at(7));
            playButton.setPosition((tilesinrow * 32) - 240, 32 * (tilesincol + 0.5f));
            UIButtons[0] = playButton;
        }
    }else{
        sf::Sprite pauseButton;
        pauseButton.setTexture(UITextures.at(6));
        pauseButton.setPosition((tilesinrow * 32) - 240, 32 * (tilesincol+0.5f));
        UIButtons[0] = pauseButton;
    }

    if(this->UIButtons.size() != 4){
        sf::Sprite debugButton;
        debugButton.setTexture(UITextures.at(0));
        debugButton.setPosition((tilesinrow * 32) - 304, 32 * (tilesincol +0.5f));
        this->UIButtons.push_back(debugButton);

        sf::Sprite leaderboardButton;
        leaderboardButton.setTexture(UITextures.at(5));
        leaderboardButton.setPosition((tilesinrow * 32) - 176, 32 * (tilesincol+0.5f));
        this->UIButtons.push_back(leaderboardButton);

        sf::Sprite happyFaceButton;
        happyFaceButton.setTexture(UITextures.at(2));
        happyFaceButton.setPosition(((tilesinrow / 2.0) * 32) - 32, 32 * (tilesincol + 0.5f));
        this->UIButtons.push_back(happyFaceButton);
    }
    if(this->gameLose){
        sf::Sprite faceLose;
        faceLose.setTexture(UITextures.at(3));
        faceLose.setPosition(((tilesinrow/2.0) * 32) - 32, 32 * (tilesincol +0.5f));
        UIButtons[3] = faceLose;
    } else if(this->gameWin){
        sf::Sprite faceWin;
        faceWin.setTexture(UITextures.at(4));
        faceWin.setPosition(((tilesinrow/2.0) * 32) - 32, 32 * (tilesincol +0.5f));
        UIButtons[3] = faceWin;
    }

    if(play){
        for (unsigned int i = 0; i < tiles.size(); i++) {
            for (unsigned int j = 0; j < tiles[i].undertilesprites.size(); j++) {
                window.draw(tiles[i].undertilesprites[j]);
            }
        }
    }else {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            sf::Sprite revealed;
            revealed.setPosition(tiles[i].positionx, tiles[i].positiony);
            revealed.setTexture(tiletextures.at(0));
            window.draw(revealed);
        }
    }
    for(unsigned int i = 0; i < this->UIButtons.size(); i++){
        window.draw(UIButtons[i]);
    }
    displayCounter(window, this->mineCounter, digitTextures);
    displayTime(window, timeElapsedseconds, digitTextures);

    window.display();
}

void Minesweeper::setNeighbors(vector<Tile> &tiles) {
    for(unsigned int i = 0; i < tiles.size(); i++){
        if(tiles[i].positionx - 32 >= 0){
            tiles[i].neighbors[3] = &tiles[i-1];
            if(tiles[i].positiony +32 < (windowHeight-100)){
                tiles[i].neighbors[5] = &tiles[i+(tilesinrow-1)];
            }else{
                tiles[i].neighbors[5] = nullptr;
            }
            if(tiles[i].positiony - 32 >= 0){
                tiles[i].neighbors[0] = &tiles[i-(tilesinrow+1)];
            }else{
                tiles[i].neighbors[0] = nullptr;
            }
        }else{
            tiles[i].neighbors[0] = nullptr;
            tiles[i].neighbors[3] = nullptr;
            tiles[i].neighbors[5] = nullptr;
        }


        if(tiles[i].positionx + 32 < windowWidth){
            tiles[i].neighbors[4] = &tiles[i+1];
            if(tiles[i].positiony +32 < (windowHeight-100)){
                tiles[i].neighbors[7] = &tiles[i+(tilesinrow+1)];
            }else{
                tiles[i].neighbors[7] = nullptr;
            }
            if(tiles[i].positiony - 32 >= 0){
                tiles[i].neighbors[2] = &tiles[i-(tilesinrow-1)];
            }else{
                tiles[i].neighbors[2] = nullptr;
            }
        }else{
            tiles[i].neighbors[2] = nullptr;
            tiles[i].neighbors[4] = nullptr;
            tiles[i].neighbors[7] = nullptr;
        }


        if(tiles[i].positiony - 32 >= 0){
            tiles[i].neighbors[1] = &tiles[i-tilesinrow];
        }else{
            tiles[i].neighbors[1] = nullptr;
        }


        if(tiles[i].positiony +32 < (windowHeight-100)){
            tiles[i].neighbors[6] = &tiles[i+tilesinrow];
        }else{
            tiles[i].neighbors[6] = nullptr;
        }
    }
}

void Minesweeper::countMines(vector<Tile> &tiles) {
    for(unsigned int i = 0; i < tiles.size(); i++){
        for(unsigned int j = 0; j < 8; j++){
            if((tiles[i].neighbors[j] != nullptr) and (tiles[i].neighbors[j]->HasMine)){
                tiles[i].surroundingMineCount++;
            }
        }
    }
}

map<int, sf::Texture> Minesweeper::getTileTextures(){
    map<int, sf::Texture> tiletextures;

    sf::Texture revealedtiletexture;
    tiletextures.emplace(0, revealedtiletexture);
    tiletextures.at(0).loadFromFile("files/images/tile_revealed.png");

    sf::Texture num1texture;
    tiletextures.emplace(1, num1texture);
    tiletextures.at(1).loadFromFile("files/images/number_1.png");

    sf::Texture num2texture;
    tiletextures.emplace(2, num2texture);
    tiletextures.at(2).loadFromFile("files/images/number_2.png");

    sf::Texture num3texture;
    tiletextures.emplace(3, num3texture);
    tiletextures.at(3).loadFromFile("files/images/number_3.png");

    sf::Texture num4texture;
    tiletextures.emplace(4, num4texture);
    tiletextures.at(4).loadFromFile("files/images/number_4.png");

    sf::Texture num5texture;
    tiletextures.emplace(5, num5texture);
    tiletextures.at(5).loadFromFile("files/images/number_5.png");

    sf::Texture num6texture;
    tiletextures.emplace(6, num6texture);
    tiletextures.at(6).loadFromFile("files/images/number_6.png");

    sf::Texture num7texture;
    tiletextures.emplace(7, num7texture);
    tiletextures.at(7).loadFromFile("files/images/number_7.png");

    sf::Texture num8texture;
    tiletextures.emplace(8, num8texture);
    tiletextures.at(8).loadFromFile("files/images/number_8.png");

    sf::Texture minetexture;
    tiletextures.emplace(9, minetexture);
    tiletextures.at(9).loadFromFile("files/images/mine.png");

    sf::Texture hiddentexture;
    tiletextures.emplace(10, hiddentexture);
    tiletextures.at(10).loadFromFile("files/images/tile_hidden.png");

    sf::Texture flagtexture;
    tiletextures.emplace(11, flagtexture);
    tiletextures.at(11).loadFromFile("files/images/flag.png");

    return tiletextures;
}

map<int, sf::Texture> Minesweeper::getUITextures(){
    map<int, sf::Texture> UItextures;

    sf::Texture debugButtontexture;
    UItextures.emplace(0, debugButtontexture);
    UItextures.at(0).loadFromFile("files/images/debug.png");

    sf::Texture face_happytexture;
    UItextures.emplace(2, face_happytexture);
    UItextures.at(2).loadFromFile("files/images/face_happy.png");

    sf::Texture face_losetexture;
    UItextures.emplace(3, face_losetexture);
    UItextures.at(3).loadFromFile("files/images/face_lose.png");

    sf::Texture face_wintexture;
    UItextures.emplace(4, face_wintexture);
    UItextures.at(4).loadFromFile("files/images/face_win.png");

    sf::Texture leaderboardtexture;
    UItextures.emplace(5, leaderboardtexture);
    UItextures.at(5).loadFromFile("files/images/leaderboard.png");

    sf::Texture pausetexture;
    UItextures.emplace(6, pausetexture);
    UItextures.at(6).loadFromFile("files/images/pause.png");

    sf::Texture playtexture;
    UItextures.emplace(7, playtexture);
    UItextures.at(7).loadFromFile("files/images/play.png");

    return UItextures;
}

void Minesweeper::debugMines(sf::RenderWindow &window, vector<Tile> &tiles, map<int, sf::Texture> &tiletextures) {
    if(this->debug) {
        for (unsigned int i= 0; i < tiles.size(); i++) {
            if (tiles[i].HasMine and tiles[i].Hidden) {
                sf::Sprite mine;
                mine.setPosition(tiles[i].positionx, tiles[i].positiony);
                mine.setTexture(tiletextures.at(9));
                tiles[i].undertilesprites.push_back(mine);
            }
        }
    }else{
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if(tiles[i].HasMine and tiles[i].Hidden){
                tiles[i].undertilesprites.pop_back();
            }
        }
    }
}

void Minesweeper::displayCounter(sf::RenderWindow &window,int &mineCounter, map<char, sf::Texture>& digitTextures) {
    string minecount = to_string(mineCounter);
    if(minecount[0] == '-'){
        sf::Sprite negative;
        negative.setTexture(digitTextures.at('-'));
        negative.setPosition(12, 32 *(tilesincol+0.5f)+16);
        window.draw(negative);
        int j = 33;
        for(unsigned int i = 1; i < minecount.size(); i++){
            sf::Sprite sprite;
            sprite.setPosition(j, 32 * (tilesincol+0.5f)+16);
            sprite.setTexture(digitTextures.at(minecount[i]));
            window.draw(sprite);
            j += 21;
        }
    }else{
        int j = 33;
        for(unsigned int i = 0; i < minecount.size(); i++) {
            sf::Sprite sprite;
            sprite.setPosition(j, 32 * (tilesincol + 0.5f) + 16);
            sprite.setTexture(digitTextures.at(minecount[i]));
            window.draw(sprite);
            j += 21;
        }
    }
}

map<char, sf::Texture> Minesweeper::getDigitTextures() {
    map<char, sf::Texture> digitTextures;
    sf::Texture negativesign;
    digitTextures.emplace('-', negativesign);
    digitTextures.at('-').loadFromFile("files/images/digits.png", sf::IntRect(210, 0, 21, 32));

    sf::Texture num1texture;
    digitTextures.emplace('1', num1texture);
    digitTextures.at('1').loadFromFile("files/images/digits.png", sf::IntRect(21, 0, 21, 32));

    sf::Texture num2texture;
    digitTextures.emplace('2', num2texture);
    digitTextures.at('2').loadFromFile("files/images/digits.png", sf::IntRect(42, 0, 21, 32));

    sf::Texture num3texture;
    digitTextures.emplace('3', num3texture);
    digitTextures.at('3').loadFromFile("files/images/digits.png", sf::IntRect(63, 0, 21, 32));

    sf::Texture num4texture;
    digitTextures.emplace('4', num4texture);
    digitTextures.at('4').loadFromFile("files/images/digits.png", sf::IntRect(84, 0, 21, 32));

    sf::Texture num5texture;
    digitTextures.emplace('5', num5texture);
    digitTextures.at('5').loadFromFile("files/images/digits.png", sf::IntRect(105, 0, 21, 32));

    sf::Texture num6texture;
    digitTextures.emplace('6', num6texture);
    digitTextures.at('6').loadFromFile("files/images/digits.png", sf::IntRect(126, 0, 21, 32));

    sf::Texture num7texture;
    digitTextures.emplace('7', num7texture);
    digitTextures.at('7').loadFromFile("files/images/digits.png", sf::IntRect(147, 0, 21, 32));

    sf::Texture num8texture;
    digitTextures.emplace('8', num8texture);
    digitTextures.at('8').loadFromFile("files/images/digits.png", sf::IntRect(168, 0, 21, 32));

    sf::Texture num9texture;
    digitTextures.emplace('9', num9texture);
    digitTextures.at('9').loadFromFile("files/images/digits.png", sf::IntRect(189, 0, 21, 32));

    sf::Texture num0texture;
    digitTextures.emplace('0', num0texture);
    digitTextures.at('0').loadFromFile("files/images/digits.png", sf::IntRect(0, 0, 21, 32));

    return digitTextures;
}

bool Minesweeper::checkifwin(vector<Tile> &tiles) {
    unsigned int total_nonmine = tiles.size() - numMines;
    unsigned int total_revealed = 0;
    for(unsigned int i = 0; i < tiles.size(); i++){
        if((!tiles[i].HasMine) and (!tiles[i].Hidden)){
            total_revealed++;
        }
    }
    if(total_revealed == total_nonmine){
        return true;
    }else {
        return false;
    }
}

void Minesweeper::clearTilesRecursive(Tile& tile,vector<Tile> &tiles) {
    for(unsigned int i = 0; i < 8; i++){
        if((tile.neighbors[i] != nullptr) and (!tile.neighbors[i]->HasMine) and (tile.neighbors[i]->Hidden) and (tile.neighbors[i]->surroundingMineCount == 0) and (!tile.neighbors[i]->isFlagged)){
            tile.neighbors[i]->Hidden = false;
            clearTilesRecursive(*tile.neighbors[i], this->tiles);
        }else if ((tile.neighbors[i] != nullptr) and (!tile.neighbors[i]->HasMine) and (tile.neighbors[i]->Hidden) and (!tile.neighbors[i]->isFlagged)){
            tile.neighbors[i]->Hidden = false;
        }
    }
}

void Minesweeper::displayTime(sf::RenderWindow &window, int timeElapsedseconds, map<char, sf::Texture> &digitTextures) {
    int totalSeconds;
    if(timer) {
        totalSeconds = timeElapsedseconds;
    }else{
        totalSeconds = savedTime;
    }
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    string min = to_string(minutes);
    string sec = to_string(seconds);
    if(min.size() == 2){
        float position = (tilesinrow * 32) - 97;
        for(unsigned int i = 0; i < 2; i++) {
            sf::Sprite sprite;
            sprite.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
            sprite.setTexture(digitTextures.at(min[i]));
            window.draw(sprite);
            position += 21;
        }
    }else{
        float position = (tilesinrow * 32) - 97;
        sf::Sprite zero;
        zero.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
        zero.setTexture(digitTextures.at('0'));
        window.draw(zero);
        position += 21;
        sf::Sprite sprite;
        sprite.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
        sprite.setTexture(digitTextures.at(min[0]));
        window.draw(sprite);
    }
    if(sec.size() == 2){
        float position = (tilesinrow * 32) - 54;
        for(unsigned int i = 0; i < 2; i++) {
            sf::Sprite sprite;
            sprite.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
            sprite.setTexture(digitTextures.at(sec[i]));
            window.draw(sprite);
            position += 21;
        }

    }else{
        float position = (tilesinrow * 32) - 54;
        sf::Sprite zero;
        zero.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
        zero.setTexture(digitTextures.at('0'));
        window.draw(zero);
        position += 21;
        sf::Sprite sprite;
        sprite.setPosition(position, 32 * (tilesincol + 0.5f) + 16);
        sprite.setTexture(digitTextures.at(sec[0]));
        window.draw(sprite);
    }
}

void Minesweeper::displayLB(sf::RenderWindow& window, vector<Tile>& tiles, map<int, sf::Texture>& tiletextures, map<int,sf::Texture>& UITextures, map<char, sf::Texture> digitTextures, bool play, int timeElapsedseconds) {
    leaderboard = true;
    sf::RenderWindow leader(sf::VideoMode(windowWidth/2, windowHeight/2), "Leaderboard", sf::Style::Close);
    leader.setFramerateLimit(60);
    leader.clear(sf::Color(0,0,255));
    sf::Text leaderboardtext;
    leaderboardtext.setFont(font);
    leaderboardtext.setString("LEADERBOARD");
    leaderboardtext.setCharacterSize(20);
    sf::FloatRect leadRect = leaderboardtext.getLocalBounds();
    leaderboardtext.setOrigin(leadRect.left + leadRect.width/2.0f,
                              leadRect.top  + leadRect.height/2.0f);
    leaderboardtext.setPosition(sf::Vector2f(windowWidth/4,(windowHeight/4)-120));
    leaderboardtext.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leader.draw(leaderboardtext);
        sf::Text userscore;
        userscore.setFont(font);
        userscore.setString(this->currentScores);
        userscore.setCharacterSize(18);
        sf::FloatRect scoreRect = userscore.getLocalBounds();
        userscore.setOrigin(scoreRect.left + scoreRect.width/2.0f,
                                  scoreRect.top  + scoreRect.height/2.0f);
        userscore.setPosition(sf::Vector2f((windowWidth/4),((windowHeight/4)+20)));
        userscore.setStyle(sf::Text::Bold);
        leader.draw(userscore);
    leader.display();

    while(leader.isOpen()){
        setTileTexture(window, tiles, tiletextures, UITextures, digitTextures, play, currentelapsed.asSeconds() + savedTime);
        sf::Event event;
        while (leader.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                leaderboard = false;
                leader.close();
            }
        }
    }
}

void Minesweeper::readLB() {
    fstream leaderboardFile;
    leaderboardFile.open("files/leaderboard.txt", ios_base::in);
    string line;
    string line2;
    int i = 1;
    if(leaderboardFile.is_open()) {
            while (getline(leaderboardFile, line2)) {
                string name;
                string time;
                istringstream leaderboardInfo(line2);
                getline(leaderboardInfo, time, ',');
                getline(leaderboardInfo, name);
                string score = time+","+name;
                Score prevuserscore(name, time);
                scores.push_back(prevuserscore);
                string place = to_string(i);
                line += place + "." + "\t" + time + "\t" + name + "\n" + "\n";
                i += 1;
            }
    }
    this->currentScores = line;
}

void Minesweeper::createEntry() {
    int position = 0;
    string amin;
    string asec;
    int minutes = savedTime / 60;
    int seconds = savedTime % 60;
    string rmin = to_string(minutes);
    string rsec = to_string(seconds);
    if (rmin.size() == 1) {
        amin = "0" + rmin;
    } else {
        amin = rmin;
    }

    if (rsec.size() == 1) {
        asec = "0" + rsec;
    } else {
        asec = rsec;
    }
    Score userscore(user, amin + ":" + asec);

    for (unsigned int i = 0; i < scores.size(); i++) {
        if (userscore.time <= scores[i].time) {
            scores.insert(scores.begin() + i, userscore);
            position = i;
            i = scores.size();
        }
    }

    string line;
    fstream leaderboardFile;
    leaderboardFile.open("files/leaderboard.txt", ios_base::out);
    if (leaderboardFile.is_open()) {
        for (unsigned int i = 0; i < 5; i++) {
            string place = to_string(i+1);
            leaderboardFile << scores[i].time + ","+ scores[i].name << endl;
            if(i == position){
                line += place + "." + "\t" + scores[i].time + "\t" + scores[i].name + "*" + "\n" + "\n";
            }else {
                line += place + "." + "\t" + scores[i].time + "\t" + scores[i].name + "\n" + "\n";
            }
            }
        }
    this->currentScores = line;
    }





