//
//  Game.cpp
//  Ludum Dare
//
//  Created by Andrew Jones on 8/22/14.
//  Copyright (c) 2014 Andrew Jones. All rights reserved.
//

#include "director.hpp"
#include "game.hpp"
#include "textureLoader.hpp"
#include <cmath>
#ifdef _WIN32
#else
// #include "ResourcePath.hpp"
#endif

#define PLAYERS 1

sf::IpAddress server = "127.0.0.1";
unsigned short port = 50000;

void Game::setupScene(sf::RenderWindow &window) {
    gameSocket = new sf::UdpSocket();
    if (gameSocket->bind(0) != sf::Socket::Done)
    {
        // error...
    }
    if (PLAYERS == 2) {
        server = "127.0.0.1";
    } else {
        server = "104.236.122.65";
    }
    socket.connect(server,40000);
    std::cout<<"CONNECTED\n";
    sf::Packet p;
    socket.receive(p);
    socket.setBlocking(false);
    sf::Uint8 type;
    sf::Uint32 id;
    type = 0;
    p>>id;
    p.clear();
    p<<type<<id;
    sf::Packet resp;
    while (socket.receive(resp) != sf::Socket::Done) {
        gameSocket->send(p,server,port);
        std::cout<<"send\n";
        sf::sleep(sf::seconds(0.1));
    }
    p.clear();
    p<<sf::Uint8(PLAYERS)<<sf::Uint8(1);
    socket.send(p);
    gameSocket->setBlocking(false);
    start();
    view = window.getView();
}

void Game::start() {
    tickTarget = 0;
    tickCurrent = 0;
    wait = true;
    actors.clear();
    //view.reset(sf::FloatRect(1280, 720, 640, 360));
}

void Game::draw(sf::RenderTarget *window,float alpha) {
    if (wait) {
        alpha = 1;
    }
    alpha = 1;
    for (auto it = actors.cbegin(); it != actors.cend() /* not hoisted */; /* no increment */)
    {
        if (tickCurrent > 10 && it->second->prep(tickCurrent,alpha))
        {
            //actors.erase(it++);
            ++it;
        }
        else
        {
            if (it->first == 0) {
                float chase=0.06;
                sf::Vector2f v = view.getCenter();
                sf::Vector2f p = it->second->sprite.getPosition();
                //view.setCenter(v.x+(p.x-v.x)*chase,v.y+(p.y-v.y)*chase);
                window->setView(view);
            }
            //view.setCenter(it->second->shape.getPosition());
            it->second->draw(*window);
            ++it;
        }
    }
}

void Game::handleEvent(sf::Event event, sf::RenderWindow *window) {
    if (event.key.code == sf::Keyboard::Escape)
    {
        // LevelEdit *scene = new LevelEdit();
        // Director::getInstance()->enqueueScene(scene);
        // Director::getInstance()->running = false;
    }
}



bool Game::tick(sf::RenderWindow *window) {
    sf::Packet packet;
    sf::IpAddress sender;
    unsigned short p;
    while (gameSocket->receive(packet,sender,p) == sf::Socket::Done) {
        packet>>player;
        sf::Uint32 last = tickTarget;
        packet>>tickTarget;
        if (tickTarget > last) {

            if (tickTarget-last > 2)
            std::cout << "Dif:" << int(tickTarget-last) << "\n";

            int i=0;
            while (true) {
                i++;
                float x,y,angle;
                sf::Uint16 num;
                sf::Uint8 frame;
                bool first;
                if (packet>>num>>x>>y>>angle>>frame>>first) {
                    if (first) {
                        sf::Uint8 a;
                        float r;
                        packet>>a>>r;
                        actors[num] = std::make_shared<Actor>(r);
                        actors[num]->setAnchor(a);
                    } else if (actors.find(num) == actors.end()) {
                        actors[num] = std::make_shared<Actor>(10);
                    }
                    actors[num]->state[tickTarget].pos = sf::Vector2f(x,y);
                    actors[num]->state[tickTarget].angle = angle;
                    actors[num]->state[tickTarget].frame = frame;
                    actors[num]->inter(last,tickTarget);
                } else {
                    break;
                }
            }
        }
    }

    sf::Uint32 diff = tickTarget-tickCurrent;
    //std::cout<<"DIFF:"<<diff<<"\n";

    if (diff >= 2) {
        wait = false;
    }
    if (diff == 0) {
        wait = true;
        std::cout<<"WAIT\n";
    }
    if (!wait) {
        if (diff >= 5) {
            tickCurrent++;
            std::cout<<"QUICK\n";
        }
        tickCurrent++;
    }

    input(window);

    return true;
}

void Game::input(sf::RenderWindow *window) {
    sf::Packet packet;
    sf::Uint8 type;
    type = 1;
    packet<<type<<tickTarget;
    for(int i=0; i<PLAYERS; i++) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            packet<<sf::Uint8(255);
        } else if (sf::Joystick::getAxisPosition(i, sf::Joystick::X) > 20) {
            float x = sf::Joystick::getAxisPosition(i, sf::Joystick::X);
            x = x > 70 ? 70 : x;
            sf::Uint8 value = 255.0/50.0*(x-20);
            packet<<value;
        } else {
            packet<<sf::Uint8(0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            packet<<sf::Uint8(255);
        } else if (sf::Joystick::getAxisPosition(i, sf::Joystick::X) < -20) {
            float x = -sf::Joystick::getAxisPosition(i, sf::Joystick::X);
            x = x > 70 ? 70 : x;
            sf::Uint8 value = 255.0/50.0*(x-20);
            packet<<value;
        } else {
            packet<<sf::Uint8(0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            packet<<sf::Uint8(255);
        } else if (sf::Joystick::getAxisPosition(i, sf::Joystick::Y) > 20) {
            float y = sf::Joystick::getAxisPosition(i, sf::Joystick::Y);
            y = y > 70 ? 70 : y;
            sf::Uint8 value = 255.0/50.0*(y-20);
            packet<<value;
        } else {
            packet<<sf::Uint8(0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            packet<<sf::Uint8(255);
        } else if (sf::Joystick::getAxisPosition(i, sf::Joystick::Y) < -20) {
            float y = -sf::Joystick::getAxisPosition(i, sf::Joystick::Y);
            y = y > 70 ? 70 : y;
            sf::Uint8 value = 255.0/50.0*(y-20);
            packet<<value;
        } else {
            packet<<sf::Uint8(0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(i, 3) || sf::Joystick::isButtonPressed(i, 0)) {
            packet<<true;
        } else {
            packet<<false;
        }

        if (actors.size() > 0) {
            sf::Vector2i tm = sf::Mouse::getPosition(*window);
            sf::Vector2f p = sf::Vector2f(tm.x,tm.y);
            sf::Vector2f pos = actors[4+player]->sprite.getPosition();
            p-=pos;

            packet<<float(std::atan2(p.y, p.x));
        } else {
            packet<<float(0);
        }
        

    }
    gameSocket->send(packet,server,port);
}

Game::~Game() {
    std::cout << "stop\n";
    delete gameSocket;
    socket.disconnect();
}
