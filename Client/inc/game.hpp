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

class Game : public Scene {
    std::map<sf::Uint16,std::shared_ptr<Actor> > actors;
    std::vector<std::shared_ptr<Player>> players;
    std::map<sf::Uint16,std::vector<sf::Uint8> > sfx;
    std::map<sf::Uint8,std::string> sfxToName;
    std::map<sf::Uint8,int> sfxToVol;

    sf::TcpSocket socket;
    sf::UdpSocket *gameSocket;

    bool wait;

    float shake;

    float lastAngle[2];

    sf::Uint32 tickCurrent;
    sf::Uint32 tickTarget;

    sf::Uint16 player;

    sf::View view;
    sf::View view2;
    sf::Sprite tileSheet;
public:
    void start();
    void input(sf::RenderWindow *window);
    virtual void draw(sf::RenderTarget *window,float alpha);
    virtual void setupScene(sf::RenderWindow &window);
    virtual bool tick(sf::RenderWindow *window);
    virtual void handleEvent(sf::Event event, sf::RenderWindow *window);
    ~Game();
};

#endif /* defined(__Ludum_Dare__Game__) */
