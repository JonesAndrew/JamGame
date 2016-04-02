#include "bullet.hpp"
#include <memory>
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void Bullet::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Bullet>(shared_from_this()));
}
void Bullet::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Bullet>(shared_from_this()));
}

Bullet::Bullet(VECTOR2 p, float a) : Actor() {
	pos = p;
	velocity = VECTOR2(4,0)%a;
	radius = 4;
	anchor = 0;
	Disk *d = new Disk();
    d->radius = radius;
    shape = d;
    classNum = 1;
}

void Bullet::update() {
	Actor::update();
	angle = std::atan2(velocity.y, velocity.x)/M_PI*180;
}