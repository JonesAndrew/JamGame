#include "wall.hpp"
#include "bullet.hpp"
#include "player.hpp"
#include <iostream>
#include <cmath>

Wall::Wall(VECTOR2 a, VECTOR2 b) {
	pos = a;
	size = b;
	angle = 0;
	Polygon *p = new Polygon();
    p->points.emplace_back(size.x/2,size.y/2);
    p->points.emplace_back(-size.x/2,size.y/2);
    p->points.emplace_back(-size.x/2,-size.y/2);
    p->points.emplace_back(size.x/2,-size.y/2);
    shape = p;
}

void Wall::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Wall>(shared_from_this()));
}
void Wall::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Wall>(shared_from_this()));
}

void Wall::collideWith(std::shared_ptr<Bullet> b) {
	VECTOR2 dif = b->getPos()-pos;
	VECTOR2 newpos;

	if ((size.x/2)-abs(dif.x) > (size.y/2)-abs(dif.y)) {
		newpos = VECTOR2(b->getPos().x,
			pos.y+copysign((size.y/2)+b->getRadius(),dif.y));
		b->setVelocity(VECTOR2(b->getVelocity().x,-b->getVelocity().y));
	} else {
		newpos = VECTOR2(pos.x+copysign((size.x/2)+b->getRadius(),dif.x),
			b->getPos().y);
		b->setVelocity(VECTOR2(-b->getVelocity().x,b->getVelocity().y));
	}

	b->setPos(newpos);
}

void Wall::collideWith(std::shared_ptr<Player> p) {
	VECTOR2 dif = p->getPos()-pos;
	VECTOR2 newpos;

	if ((size.x/2)-abs(dif.x) > (size.y/2)-abs(dif.y)) {
		newpos = VECTOR2(p->getPos().x,
			pos.y+copysign((size.y/2)+p->getRadius(),dif.y));
		//p->setVelocity(VECTOR2(0,0));
	} else {
		newpos = VECTOR2(pos.x+copysign((size.x/2)+p->getRadius(),dif.x),
			p->getPos().y);
		//p->setVelocity(VECTOR2(0,0));
	}

	p->setPos(newpos);
}