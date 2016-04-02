#include <memory>
#include <SFML/Network.hpp>

#pragma once

struct PlayerNetwork;

#include "actor.hpp"
#include "player.hpp"

class Game {
protected:
    int gameNum;
    int w,h;
    int restartTime;
    sf::Uint32 tick;
    sf::Uint16 actorCount;
    std::vector<sf::Vector2i> dir;
public:
    std::vector<std::shared_ptr<PlayerNetwork> > players;
    std::vector<std::shared_ptr<Player> > gamePlayers;
    std::vector<std::shared_ptr<Actor>> actors;
    sf::UdpSocket *socket;

    virtual void start();

    void handleInput(sf::Packet packet,std::shared_ptr<PlayerNetwork> p);

    void send(sf::Packet packet);

    void update();

    void makeActor(std::shared_ptr<Actor> p);
};

struct Input {
    sf::Uint8 r,l,d,u;
    bool s;
    bool pressed;
    float angle;
    Input() {
        s=pressed=false;
        r=l=d=u=0;
    }
};

struct PlayerNetwork {
    PlayerNetwork() {
        state=0;
        matched = false;
    }
    ~PlayerNetwork() {
        if (socket)
        {
            delete socket;
        }
    }
    void setNumPlayers(int n) {
        numberOfPlayers = n;
        for (int i=0;i<n;i++) {
            input.emplace_back();
        }
    }
    sf::TcpSocket *socket;
    std::shared_ptr<Game> game;
    int state,numberOfPlayers;
    sf::Uint32 id,lastTick;
    std::vector<Input> input;
    sf::IpAddress address;
    unsigned short port;
    bool matched;
};
