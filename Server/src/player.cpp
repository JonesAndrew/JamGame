#include "player.hpp"
#include "game.hpp"
#include "playerState.hpp"
#include <iostream>
#include "math.h"
#include "bullet.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Player::Player() : Actor() {
    pos = VECTOR2(50,100);
    anchor = 0;
    frame = 0;
    radius = 16;
    state_ = nullptr;
    newState(new WalkState());
    Disk *d = new Disk();
    d->radius = radius;
    shape = d;
    accel = 0.6;
    maxVel = 4.5;
    deccel = 0.3;
    shotTime = 0;
    classNum = 2;
}

void Player::res() {
    shotTime = 0;
    velocity = VECTOR2(0,0);
    dead = false;
}

float Player::getAccel() {
    return accel;
}

float Player::getDeccel() {
    return deccel;
}

float Player::getMaxVel() {
    return maxVel;
}

Player::~Player() {
    if (game) {
        game = nullptr;
    }
}

void Player::newState(PlayerState* state)
{
    if (state != nullptr)
    {
        if (state_ != nullptr) {
            delete state_;
        }
        state_ = state;
        state_->enter(*this);
    }
}

void Player::handleInput(Input input)
{
    newState(state_->handleInput(*this, input));
}

void Player::update()
{
    if (!dead) {
        newState(state_->update(*this));
    }
}

sf::Uint8 Player::getFrame() {
    return frame;
}

void Player::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Player>(shared_from_this()));
}
void Player::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Player>(shared_from_this()));
}

void Player::collideWith(std::shared_ptr<Bullet> b) {
    if (b->time > 12) {
        dead = true;
        b->setDead(true);
    }
} 
