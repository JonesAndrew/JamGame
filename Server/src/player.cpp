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
    // Disk *d = new Disk();
    // d->radius = radius;
    // shape = d;

    nPol = new Polygon();
    nPol->points.emplace_back(radius,radius);
    nPol->points.emplace_back(-radius,radius);
    nPol->points.emplace_back(-radius,-radius);
    nPol->points.emplace_back(radius,-radius);
    shape = nPol;

    sPol = new Polygon();
    sPol->points.emplace_back(radius*2,radius*2);
    sPol->points.emplace_back(-radius*2,radius*2);
    sPol->points.emplace_back(-radius*2,-radius*2);
    sPol->points.emplace_back(radius*2,-radius*2);

    accel = 0.3;
    maxVel = 2.25;
    deccel = 0.15;
    classNum = 2;

    rotTarget = 0;

    res();
}

void Player::res() {
    for (int i=0;i<6;i++) {
        b[i] = true;
    }
    bulletCount = 6;
    shotTime = 0;
    velocity = VECTOR2(0,0);
    dead = false;
    splat = false;
    dodgeTime = 0;
    charge = 0;
    newState(new WalkState());
}

int Player::getRandom() {
    if (bag.size() == 0) {
        for (int i=0;i<7;i++) {
            bag.push_back(0);
        }
        bag.push_back(1);
    }

    int r=rand()%bag.size();
    int value = bag[r];
    if (value != 0) {
        bag.erase(bag.begin()+r);
        //bag.clear();
    } else {
        bag.erase(bag.begin()+r);
    }
    return value;
}

void Player::shift() {
    bool copy=b[5];
    for (int i=1;i<6;i++) {
        b[6-i]=b[5-i];
    }
    b[0]=copy;
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
    if (velocity.x > 0) {
        facingRight = true;
    } else if (velocity.x < 0) {
        facingRight = false;
    }
    bu = 0;
    for (size_t t=0;t<6;t++) {
        if (b[t]) {
            bu += (1<<t);
        }
    }
}

sf::Uint8 Player::getFrame() {
    sf::Uint8 f=frame;
    if (shotTime > 44) {
        f+=20;
    }
    if (facingRight) {
        f+=100;
    }
    return f;
}

void Player::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Player>(shared_from_this()));
}
void Player::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Player>(shared_from_this()));
}

void Player::collideWith(std::shared_ptr<Bullet> bul) {
    if (!splat) {
        if (bul->time > 8 && bul->live) {
            if (frame < 8) {
                newState(new DeadState());
                game->setRestartTime(60);
                game->addPoint(1-(color/2));
                sfx.push_back(2);
                game->setShake(10);
                bul->setDead(true);
                splat = true;
            } else if (frame >= 12) {
                bul->live = false;
                if (bul->getPos().x > pos.x) {
                    bul->setVelocity(VECTOR2(0.5,-3));
                    bul->setAngle(180);
                } else {
                    bul->setVelocity(VECTOR2(-0.5,-3));
                    bul->setAngle(0);
                }
            }
            game->setShake(5);
        } else if (bul->step == 2 && bulletCount < 6) {
            bul->setDead(true);
            bulletCount++;
            if(!b[0]) {
                b[0] = true;
            } else {
                for (int i=1;i<6;i++) {
                    if(!b[6-i]) {
                        b[6-i] = true;
                        break;
                    }
                }
            }
        }
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
