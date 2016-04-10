#include "actor.hpp"

#pragma once

class Bullet : public Actor {
public:
	virtual void collideWith(std::shared_ptr<Actor>);
    virtual void collidedBy(std::shared_ptr<Actor>);

    Bullet(VECTOR2,float,float,sf::Uint8);
    virtual void update();

    virtual void send(sf::Packet&);

    virtual void collideWith(std::shared_ptr<Bullet> b);

    sf::Uint8 color;
    bool live;
    int step;
    std::shared_ptr<Bullet> last;
    int countdown;
    int bounce;
    float speed;
};
