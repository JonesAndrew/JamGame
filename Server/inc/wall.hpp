#include "actor.hpp"

#pragma once

class Wall : public Actor {
public:
	virtual void collideWith(std::shared_ptr<Actor>);
    virtual void collidedBy(std::shared_ptr<Actor>);

    virtual void collideWith(std::shared_ptr<Bullet>);
    virtual void collideWith(std::shared_ptr<Player>);

    Wall(VECTOR2 a, VECTOR2 b);
private:
	VECTOR2 size;
};