#include "actor.hpp"
#include <memory>

#pragma once

struct Input;

class PlayerState;

class Player : public Actor {
    PlayerState* state_;
    float accel,deccel,maxVel;
public:
    int shotTime;

    Player();
    ~Player();

    float getAccel();
    float getDeccel();
    float getMaxVel();

    virtual void newState(PlayerState* state);

    virtual void handleInput(Input input);

    virtual void update();

    virtual sf::Uint8 getFrame();

    virtual void collideWith(std::shared_ptr<Actor>);
    virtual void collidedBy(std::shared_ptr<Actor>);
    
    virtual void collideWith(std::shared_ptr<Bullet>);
};
