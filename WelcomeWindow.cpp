#include "WelcomeWindow.h"

void WelcomeWindow::DrawWelcome(sf::RenderWindow &window) {
    sf::Text welcometext;
    welcometext.setFont(font);
    welcometext.setString("WELCOME TO MINESWEEPER!");
    welcometext.setCharacterSize(24);
    sf::FloatRect welcomeRect = welcometext.getLocalBounds();
    welcometext.setOrigin(welcomeRect.left + welcomeRect.width/2.0f,
                          welcomeRect.top  + welcomeRect.height/2.0f);
    welcometext.setPosition(sf::Vector2f(windowWidth/2.0f,(windowHeight/2.0f)-150));
    welcometext.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Text entername;
    entername.setFont(font);
    entername.setString("Enter your name:");
    entername.setCharacterSize(20);
    sf::FloatRect enterText = entername.getLocalBounds();
    entername.setOrigin(enterText.left + enterText.width/2.0f,
                        enterText.top  + enterText.height/2.0f);
    entername.setPosition(sf::Vector2f(windowWidth/2.0f,(windowHeight/2.0f)-75));
    entername.setStyle(sf::Text::Bold);
    window.draw(welcometext);
    window.draw(entername);

}

void WelcomeWindow::DrawUserInput(sf::Text &usertext, sf::RenderWindow &window, string &username) {
    if(username.length() > 0) {
        username[0] = toupper(username[0]);
        for (int i = 1; i < username.length(); i++) {
            username[i] = tolower(username[i]);
        }
    }
    usertext.setFont(font);
    usertext.setString(username + "|");
    usertext.setCharacterSize(18);
    sf::FloatRect textRect = usertext.getLocalBounds();
    usertext.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
    usertext.setPosition(sf::Vector2f(windowWidth/2.0f,(windowHeight/2.0f)-45));
    usertext.setFillColor(sf::Color(255, 255, 0));
    usertext.setStyle(sf::Text::Bold);
    window.clear(sf::Color(0, 0, 255));
    window.draw(usertext);
    DrawWelcome(window);
    window.display();
}
