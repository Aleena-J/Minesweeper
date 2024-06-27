#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

class WelcomeWindow{
private:
    int windowWidth;
    int windowHeight;
    sf::Font font;
    string usersname;
public:
    WelcomeWindow(){
    }
    WelcomeWindow(int width, int height, sf::Font font){
        this->windowWidth = width;
        this->windowHeight = height;
        this->font = font;
    }
    ~WelcomeWindow(){
    }
    bool Display(){
        sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);
        window.setFramerateLimit(60);
        window.clear(sf::Color(0,0,255));
        DrawWelcome(window);
        window.display();
        string username;
        sf::Text usertext;
        DrawUserInput(usertext, window, username);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed){
                    window.close();
                    return false;
                }
                if (event.type == sf::Event::KeyPressed){
                    if(event.key.code == sf::Keyboard::BackSpace) {
                        if (username.size() >= 1) {
                            username.pop_back();
                            DrawUserInput(usertext, window, username);
                        }
                    }
                    if(event.key.code == sf::Keyboard::Enter){
                        if(username.length() > 0) {
                            window.close();
                            this->usersname = username;
                            return true;
                        }
                    }
                }
                if (event.type == sf::Event::TextEntered) {
                    if (isprint(event.text.unicode) and isalpha(event.text.unicode)) {
                        if (username.length() < 10) {
                            username += static_cast<char>(event.text.unicode);
                            DrawUserInput(usertext, window, username);
                        }
                    }
                }
            }
        }
    }

    void DrawWelcome(sf::RenderWindow& window);

    void DrawUserInput(sf::Text& usertext, sf::RenderWindow& window, string& username);

    string ReturnUsername(){
        return this->usersname;
    }

};
