#include "actor.hpp"
#include <memory>

#pragma once

struct Input;

class PlayerState;

class Player : public Actor {
    PlayerState* state_;
    float accel,deccel,maxVel;
    bool splat;
    bool facingRight;
public:
    std::vector<int> bag;

    Polygon *nPol;
    Polygon *sPol;

    int shotTime,shield,bulletCount,dodgeTime,charge;
    sf::Uint8 rotTarget;
    sf::Uint8 color;
    sf::Uint8 bu;
    bool b[6];

    Player(sf::Uint8);
    ~Player();

    float getAccel();
    float getDeccel();
    float getMaxVel();

    int getRandom();

    void shift();

    virtual void newState(PlayerState* state);

    virtual void handleInput(Input input);

    virtual void update();

    virtual sf::Uint8 getFrame();

    virtual void collideWith(std::shared_ptr<Actor>);
    virtual void collidedBy(std::shared_ptr<Actor>);

    virtual void collideWith(std::shared_ptr<Bullet>);

    virtual void send(sf::Packet&);

    void res();
};
