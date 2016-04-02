#include "game.hpp"
#include "bullet.hpp"
#include "wall.hpp"
#include <iostream>

#define NETWORK_UPDATE_RATE 1

void Game::start() {
    actors.clear();
    gamePlayers.clear();
    w=1280*2;
    h=720*2;
    tick=0;
    actorCount=0;

    makeActor(std::make_shared<Wall>(VECTOR2(250,0),VECTOR2(500,10)));
    makeActor(std::make_shared<Wall>(VECTOR2(250,500),VECTOR2(500,10)));
    makeActor(std::make_shared<Wall>(VECTOR2(0,250),VECTOR2(10,500)));
    makeActor(std::make_shared<Wall>(VECTOR2(500,250),VECTOR2(10,500)));

    for (int i=0; i<1; i++) {
        gamePlayers.push_back(std::make_shared<Player>());
        makeActor(gamePlayers.back());
    }

    //makeActor(std::make_shared<Bullet>());
}

void Game::update() {
    tick++;

    int count=0;
    for (auto &p : players) {
        for (int t=0;t<p->numberOfPlayers;t++) {
            if (!gamePlayers[count]->isDead()) {

                gamePlayers[count]->handleInput(p->input[t]);

            }
            count++;
        }
    }

    for (int i=actors.size()-1;i>=0;i--) {
        actors[i]->update();
        if (actors[i]->isDead() && actors[i]->getDeadCount() == 0) {
            actors.erase(actors.begin()+i);
        }
    }

    for (size_t i=0;i<actors.size();i++) {
        for (size_t t=i+1;t<actors.size();t++) {
            VECTOR2 dif = actors[i]->getPos()-actors[t]->getPos();
            if (!actors[i]->isDead() && !actors[t]->isDead() && actors[i]->getShape()->intersectWith(actors[t]->getShape())) {
                actors[i]->collideWith(actors[t]);
            }
        }
    }

    if (tick%NETWORK_UPDATE_RATE==0) {
        sf::Packet packet;
        sf::Uint16 t=0;

        packet<<t<<tick;
        for (auto &a : actors) {
            a->send(packet);
        }

        send(packet);
    }

}

void Game::handleInput(sf::Packet packet,std::shared_ptr<PlayerNetwork> p) {
    packet>>p->lastTick;
    for (int t=0;t<p->numberOfPlayers;t++) {
        packet>>p->input[t].r>>p->input[t].l>>p->input[t].d>>p->input[t].u
              >>p->input[t].s>>p->input[t].angle;
    }
}

void Game::send(sf::Packet packet) {
    for (auto &p : players) {
        socket->send(packet,p->address,p->port);
    }
}

void Game::makeActor(std::shared_ptr<Actor> p) {
    p->setGame(this);
    p->setNum(actorCount);
    actors.push_back(p);
}
