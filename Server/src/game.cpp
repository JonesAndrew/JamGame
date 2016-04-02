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
    restartTime = 0;

    makeActor(std::make_shared<Wall>(VECTOR2(128,10),VECTOR2(256+192,20)));
    makeActor(std::make_shared<Wall>(VECTOR2(128,234),VECTOR2(256+192,20)));
    makeActor(std::make_shared<Wall>(VECTOR2(22 ,128),VECTOR2(20,256)));
    makeActor(std::make_shared<Wall>(VECTOR2(234+192,128),VECTOR2(20,256)));

    for (int i=0; i<2; i++) {
        gamePlayers.push_back(std::make_shared<Player>());
        gamePlayers.back()->setPos(VECTOR2(100+300*i,100+300*i));
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

            } else if (restartTime < 0) {
                restartTime = 60;
            }
            count++;
        }
    }

    restartTime--;
    if (restartTime == 0) {
        if (actors.size() > 6) {
            actors.erase(actors.begin()+6,actors.end());
        }
        for (int i=0; i<2; i++) {
            gamePlayers[i]->res();
            gamePlayers[i]->setPos(VECTOR2(100+300*i,100+300*i));
        }
    }

    for (int i=actors.size()-1;i>=0;i--) {
        actors[i]->update();
    }

    for (size_t i=0;i<actors.size();i++) {
        for (size_t t=i+1;t<actors.size();t++) {
            VECTOR2 dif = actors[i]->getPos()-actors[t]->getPos();
            if (!actors[i]->isDead() && !actors[t]->isDead() && actors[i]->getShape()->intersectWith(actors[t]->getShape())) {
                actors[i]->collideWith(actors[t]);
            }
        }
    }

    for (int i=actors.size()-1;i>=0;i--) {
        if (actors[i]->isDead() && actors[i]->getDeadCount() == 0) {
            actors.erase(actors.begin()+i);
        }
    }

    if (tick%NETWORK_UPDATE_RATE==0) {
        sf::Packet packet;
        sf::Uint16 t=0;

        packet<<tick;
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
    sf::Uint16 i=0;
    for (auto &p : players) {
        sf::Packet pac;
        pac<<i;
        i++;
        pac.append(packet.getData(),packet.getDataSize());
        socket->send(pac,p->address,p->port);
    }
}

void Game::makeActor(std::shared_ptr<Actor> p) {
    p->setGame(this);
    p->setNum(actorCount);
    actors.push_back(p);
}
