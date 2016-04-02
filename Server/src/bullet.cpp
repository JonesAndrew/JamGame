#include "bullet.hpp"
#include <memory>

void Bullet::collideWith(std::shared_ptr<Actor> a) {
    a->collidedBy(std::static_pointer_cast<Bullet>(shared_from_this()));
}
void Bullet::collidedBy(std::shared_ptr<Actor> a) {
    a->collideWith(std::static_pointer_cast<Bullet>(shared_from_this()));
}

Bullet::Bullet(VECTOR2 p, float a) {
	pos = p;
	velocity = VECTOR2(4,0)%a;
	radius = 5;
	anchor = 0;
	Disk *d = new Disk();
    d->radius = radius;
    shape = d;
    classNum = 1;
}