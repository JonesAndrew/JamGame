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

Bullet::Bullet(VECTOR2 p, float a, sf::Uint8 c) : Actor() {
	pos = p;
	color = c;
	velocity = VECTOR2(3.25,0)%a;
	radius = 5;
	anchor = 0;
	Disk *d = new Disk();
    d->radius = radius;
    shape = d;
    classNum = 1;
    live = true;
    step = 0;
}

void Bullet::update() {
    Actor::update();
    if (live) {
    	frameTime++;
    	if (frameTime>6) {
    		frameTime=0;
    		frame++;
    		if (frame > 1) {
    			frame = 0;
    		}
    	}
        angle = std::atan2(velocity.y, velocity.x)/M_PI*180;
    } else {
        frame = 2;
        if (step == 0) {
            velocity.y += 0.2;
            if (velocity.x > 0) {
                angle += 5;
            } else {
                angle -= 5;
            }
            if (velocity.y > 3.6) {
                step++;
                velocity.y = -1.6;
            }
        } else if (step == 1) {
            velocity.y += 0.2;
            if (velocity.x > 0) {
                angle += 0.5;
            } else {
                angle -= 0.5;
            }
            if (velocity.y >= 1.6) {
                step++;
                if (velocity.x > 0) {
                    angle = 0;
                } else {
                    angle = 180;
                }
                velocity = VECTOR2(0,0);
            }
        }
    }
}

void Bullet::send(sf::Packet& p) {
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

void Bullet::collideWith(std::shared_ptr<Bullet> b) {
    if (live && b->live) {
        VECTOR2 collision = pos-b->getPos();
        double distance = collision.mag();
        collision /= distance;
        double aci = getVelocity().dot(collision);
        double bci = b->getVelocity().dot(collision);

        double acf = bci;
        double bcf = aci;

        setVelocity(getVelocity()+collision*(acf-aci));
        b->setVelocity(b->getVelocity()+collision*(bcf-bci));
        sfx.push_back(0);
    }
}
