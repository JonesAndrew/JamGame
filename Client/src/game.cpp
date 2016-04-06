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
#include "sound.hpp"
#include <cmath>
#ifdef _WIN32
#else
// #include "ResourcePath.hpp"
#endif

#define PLAYERS 2

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

sf::IpAddress server = "127.0.0.1";
unsigned short port = 50000;

bool comparator(const std::shared_ptr<Actor> &a, const std::shared_ptr<Actor> &b) {
    return a->getBot() < b->getBot();
}

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
    view.zoom(1/Director::getInstance()->getScale());
    view.setCenter(11.5*32,6.5*32);
    view2 = view;
    tileSheet = TextureLoader::getInstance()->getSprite("tiles.png");
    tileSheet.setTextureRect(sf::IntRect(0, 64, 32, 32));

    SoundPlayer::getInstance()->loadSound("hit.wav");
    SoundPlayer::getInstance()->loadSound("shoot.wav");
    SoundPlayer::getInstance()->loadSound("death.wav");
    SoundPlayer::getInstance()->loadSound("shield.wav");

    sfxToName[0] = "hit.wav";
    sfxToVol[0]  = 25;
    sfxToName[1] = "shoot.wav";
    sfxToVol[1]  = 60;
    sfxToName[2] = "death.wav";
    sfxToVol[2]  = 100;
    sfxToName[3] = "shield.wav";
    sfxToVol[3]  = 60;

    shake = 0;

    for (int i=0;i<2;i++) {
        lastAngle[i] = 0;
    }
}

void Game::start() {
    tickTarget = 0;
    tickCurrent = 0;
    wait = true;
    actors.clear();
    //view.reset(sf::FloatRect(1280, 720, 640, 360));
}

void Game::draw(sf::RenderTarget *window,float alpha) {
    if (shake <= 0) {
        window->setView(view);
    } else {
        window->setView(view2);
    }
    int sizeX=23;
    int sizeY=13;
    for (int x=0;x<sizeX;x++) {
        for (int y=0;y<sizeY;y++) {
            if (x==0 && y==0) {
                tileSheet.setTextureRect(sf::IntRect(0, 0, 32, 32));
            } else if (x==sizeX-1 && y==0) {
                tileSheet.setTextureRect(sf::IntRect(32, 0, 32, 32));
            } else if (x==0 && y==sizeY-1) {
                tileSheet.setTextureRect(sf::IntRect(64, 0, 32, 32));
            } else if (x==sizeX-1 && y==sizeY-1) {
                tileSheet.setTextureRect(sf::IntRect(96, 0, 32, 32));
            } else if (x==0) {
                tileSheet.setTextureRect(sf::IntRect(96, 32, 32, 32));
            } else if (x==sizeX-1) {
                tileSheet.setTextureRect(sf::IntRect(64, 32, 32, 32));
            } else if (y==0) {
                tileSheet.setTextureRect(sf::IntRect(0, 32, 32, 32));
            } else if (y==sizeY-1) {
                tileSheet.setTextureRect(sf::IntRect(32, 32, 32, 32));
            } else {
                tileSheet.setTextureRect(sf::IntRect(0, 64, 32, 32));
            }
            tileSheet.setPosition(x*32,y*32);
            window->draw(tileSheet);
        }
    }

    if (wait) {
        alpha = 1;
    }
    alpha = 1;
    std::vector<std::shared_ptr<Actor>> acts;
    for (auto it = actors.cbegin(); it != actors.cend() /* not hoisted */; /* no increment */)
    {
        if (tickCurrent > 10 && it->second->prep(tickCurrent,alpha))
        {
            //actors.erase(it++);
            ++it;
        }
        else
        {
            acts.push_back(it->second);
            ++it;
        }
    }

    std::sort(acts.begin(), acts.end(), comparator);

    for (size_t i=0;i<acts.size();i++) {
        acts[i]->draw(*window);
    }

    window->setView(window->getDefaultView());
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
        sf::Uint8 tS;
        packet>>tS;
        if (tS > 0) {
            shake = float(tS);
        }
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
                sf::Uint8 count;
                if (packet>>num>>x>>y>>angle>>frame>>count) {
                    for (int i=0;i<count;i++) {
                        sf::Uint8 sound;
                        packet>>sound;
                        sfx[tickTarget].push_back(sound);
                    }
                    packet>>first;
                    if (first) {
                        sf::Uint8 cNum;
                        packet>>cNum;
                        if (cNum == 1) {
                            std::shared_ptr<Bullet> b = std::make_shared<Bullet>();
                            packet>>b->color;
                            actors[num] = b;
                        } else if (cNum == 2) {
                            std::shared_ptr<Player> p = std::make_shared<Player>();
                            players.push_back(p);
                            packet>>p->color;
                            actors[num] = p;
                        } else {
                            actors[num] = std::make_shared<Actor>();
                        }
                        actors[num]->setClassNum(cNum);
                    } else if (actors.find(num) == actors.end()) {
                        actors[num] = std::make_shared<Actor>();
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

    if (diff >= 4) {
        wait = false;
    }
    if (diff == 0) {
        wait = true;
        std::cout<<"WAIT\n";
    }
    if (!wait) {
        if (diff >= 8) {
            tickCurrent++;
            std::cout<<"QUICK\n";
        }
        tickCurrent++;
        for (size_t i=0;i<sfx[tickCurrent].size();i++) {
            SoundPlayer::getInstance()->playSound(sfxToName[sfx[tickCurrent][i]],sfxToVol[sfx[tickCurrent][i]]);
        }

        view2.setCenter(view.getCenter());

        if (shake > 0) {
            int angle = rand()%360;
            float x=cos(angle*M_PI/180)*shake;
            float y=sin(angle*M_PI/180)*shake;
            view2.move(x,y);
            shake-=0.4;
        } else {
            shake = 0;
        }
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)  || sf::Joystick::isButtonPressed(i, 4)) {
            packet<<true;
        } else {
            packet<<false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Joystick::isButtonPressed(i, 5)) {
            packet<<true;
        } else {
            packet<<false;
        }

        if (PLAYERS == 1) {
            if (actors.size() > 0) {
                sf::Vector2i tm = sf::Mouse::getPosition(*window);
                sf::Vector2f p = window->mapPixelToCoords(tm,view);
                sf::Vector2f pos = players[player]->pos;
                p-=pos;

                packet<<float(std::atan2(p.y, p.x));
            } else {
                packet<<float(0);
            }
        } else {
            if (abs(sf::Joystick::getAxisPosition(i, sf::Joystick::Z)) > 15 ||
                abs(sf::Joystick::getAxisPosition(i, sf::Joystick::R)) > 15) {
                lastAngle[i] = float(std::atan2(sf::Joystick::getAxisPosition(i, sf::Joystick::Z), sf::Joystick::getAxisPosition(i, sf::Joystick::R)));
            }
            packet<<lastAngle[i];
        }
    }
    gameSocket->send(packet,server,port);
}

Game::~Game() {
    std::cout << "stop\n";
    delete gameSocket;
    socket.disconnect();
}
