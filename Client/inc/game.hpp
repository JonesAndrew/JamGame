//
//  Game.h
//  Ludum Dare
//
//  Created by Andrew Jones on 8/22/14.
//  Copyright (c) 2014 Andrew Jones. All rights reserved.
//

#ifndef __Ludum_Dare__Game__
#define __Ludum_Dare__Game__

#include "scene.hpp"
#include "textureLoader.hpp"
#include <SFML/Network.hpp>
#include <memory>
#include "actor.hpp"

class Label {
    sf::Text text;

    bool center;
public:
    void setFont(sf::Font &font) {
        text.setFont(font);
    }

    void setCentered(bool c) {
        center = c;
    }

    void setString(std::string set) {
        if (text.getString() != set) {
            text.setString(set);
            pos();
        }
    }

    void setSize(int size) {
        text.setCharacterSize(size);
    }

    void pos() {
        if (center) {
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(int(textRect.left + textRect.width/2.0f),0);
        }
    }

    void setPosition(int x, int y) {
        text.setPosition(x,y);
    }

    void setColor(sf::Color c) {
        text.setColor(c);
    }

    void draw(sf::RenderTarget &window) {
        window.draw(text);
    }
};

class Game : public Scene {
    std::map<sf::Uint32,std::shared_ptr<Actor> > actors;
    std::vector<std::shared_ptr<Player>> players;
    std::map<sf::Uint32,std::vector<sf::Uint8> > sfx;
    std::vector<std::map<sf::Uint32,sf::Uint8> > rotTarget;
    std::vector<std::map<sf::Uint32,sf::Uint8> > bu;
    std::map<sf::Uint8,std::string> sfxToName;
    std::map<sf::Uint8,int> sfxToVol;

    sf::TcpSocket socket;
    sf::UdpSocket *gameSocket;

    bool wait;

    float shake;

    float lastAngle[2];

    sf::Uint32 tickCurrent;
    sf::Uint32 tickTarget;

    sf::Music music;

    sf::Font font;

    Label s1,s2,mid;

    sf::Uint16 player;

    sf::Uint8 score[2];

    sf::View view;
    sf::View view2;

    sf::Vector2f windowSize;

    sf::Sprite tileSheet;

    sf::Sprite gun[2];
    sf::Sprite bullets;

    sf::Uint8 rotCurrent[2];
public:
    void start();
    void input(sf::RenderWindow *window);
    virtual void draw(sf::RenderTarget *window,float alpha);
    virtual void setupScene(sf::RenderWindow &window);
    virtual bool tick(sf::RenderWindow *window);
    virtual void handleEvent(sf::Event event, sf::RenderWindow *window);
    virtual void logicUpdate();
    ~Game();
};

#endif /* defined(__Ludum_Dare__Game__) */
