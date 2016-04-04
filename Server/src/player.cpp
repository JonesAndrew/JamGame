#include "player.hpp"
#include "game.hpp"
#include "playerState.hpp"
#include <iostream>
#include "math.h"
#include "bullet.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Player::Player(sf::Uint8 c) : Actor() {
    pos = VECTOR2(50,100);
    anchor = 0;
    frame = 0;
    radius = 15;
    color = c*2;
    state_ = nullptr;
    newState(new WalkState());

    // Disk *d = new Disk();
    // d->radius = radius;
    // shape = d;

    Polygon *p = new Polygon();
    p->points.emplace_back(radius,radius);
    p->points.emplace_back(-radius,radius);
    p->points.emplace_back(-radius,-radius);
    p->points.emplace_back(radius,-radius);
    shape = p;

    accel = 0.3;
    maxVel = 2.25;
    deccel = 0.15;
    shotTime = 0;
    classNum = 2;

    bulletCount = 6;
}

void Player::res() {
    bulletCount = 6;
    shotTime = 0;
    velocity = VECTOR2(0,0);
    dead = false;
    newState(new WalkState());
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
    if (shotTime > 44) {
        return frame+20;
    }
    return frame;
}

void Player::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Player>(shared_from_this()));
}
void Player::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Player>(shared_from_this()));
}

void Player::collideWith(std::shared_ptr<Bullet> b) {
    if (b->time > 8 && b->live) {
        if (frame < 8) {
            newState(new DeadState());
            game->setRestartTime(60);
            sfx.push_back(2);
            game->setShake(20);
            b->setDead(true);
        } else {
            b->live = false;
            if (b->getPos().x > pos.x) {
                b->setVelocity(VECTOR2(0.5,-3));
                b->setAngle(180);
            } else {
                b->setVelocity(VECTOR2(-0.5,-3));
                b->setAngle(0);
            }
        }
        game->setShake(10);
    } else if (b->step == 2 && bulletCount < 6) {
        b->setDead(true);
        bulletCount++;
    }
}

void Player::send(sf::Packet& p) {
    p << num << getPos().x << getPos().y << angle << getFrame();
    p << sf::Uint8(sfx.size());
    for (int i=0;i<sfx.size();i++) {
        p << sfx[i];
    }
    sfx.clear();
    p << first;
    if (first) {
        p << classNum << color;
        first = false;
    }
}
