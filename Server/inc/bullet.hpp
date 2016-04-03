#include "actor.hpp"

#pragma once

class Bullet : public Actor {
public:
	virtual void collideWith(std::shared_ptr<Actor>);
    virtual void collidedBy(std::shared_ptr<Actor>);

    Bullet(VECTOR2,float,sf::Uint8);
    virtual void update();

    virtual void send(sf::Packet&);

    sf::Uint8 color;
    bool live;
};