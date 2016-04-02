#include "actor.hpp"
#include "game.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

float cloestPointOnEdge(const VECTOR2 p, const VECTOR2 a, const VECTOR2 b) {
    VECTOR2 e = b-a;
    VECTOR2 f = p-a;
    float e2 = e.dot(e);
    float t = (f.dot(e))/e2;
    if (t<0) {
        t = 0;
    } else if (t>1) {
        t = 1;
    }
    VECTOR2 closest = a+e*t;
    VECTOR2 d = closest-p;
    return d.mag();
}

bool pointInPolygon(const VECTOR2 p, const Polygon pol) {
    int nvert = pol.points.size();
    bool c = false;
    std::vector<VECTOR2> points;
    for (int i=0;i<nvert;i++) {
        points.push_back(pol.points[i]%(pol.angle*M_PI/180));
    }
    for (int i=0, j=nvert-1; i < nvert; j=i++) {
        if  ( ( (points[i].y >= p.y) != (points[j].y >= p.y) ) && 
            (p.x <= (points[j].x - points[i].x) * (p.y-points[i].y) /
            (points[j].y - points[i].y) + points[i].x)) {
            c=!c;
        } 
    }
    return c;
}

bool Polygon::intersectWith(Shape* s) {
    return s->intersected(this);
}

bool Polygon::intersected(Shape* s) {
    return s->intersectWith(this);
}

bool Polygon::intersectWith(Disk* d) {
    if (pointInPolygon(d->pos-pos,*this)) {
        return true;
    }

    for (size_t i=0;i<points.size();i++) {
        size_t next=i+1;
        if (next == points.size()) {
            next = 0;
        }
        if (cloestPointOnEdge(d->pos-pos,points[i]%(angle*M_PI/180),points[next]%(angle*M_PI/180)) < d->radius) {
            return true;
        }
    }
    return false;
}

bool Circle::intersectWith(Shape* s) {
    return s->intersected(this);
}

bool Circle::intersected(Shape* s) {
    return s->intersectWith(this);
}

bool Circle::intersectWith(Disk* d) {
    return d->intersectWith(this);
}


bool Disk::intersectWith(Shape* s) {
    return s->intersected(this);
}

bool Disk::intersected(Shape* s) {
    return s->intersectWith(this);
}

bool Disk::intersectWith(Disk* d) {
    VECTOR2 dif = d->pos - pos;
    if (radius+d->radius>=dif.mag()) {
        return true;
    }
    return false;
}

bool Disk::intersectWith(Circle* c) {
    VECTOR2 dif = c->pos - pos;
    if (c->radius+radius>=dif.mag() && c->radius-radius<=dif.mag()) {
        return true;
    }
    return false;
}

bool Disk::intersectWith(Polygon *p) {return p->intersectWith(this);}

Actor::Actor() {
    first = true;
    anchor = 0;
    radius = 10;
    dead = false;
    deadCount = -1;
    frame = 0;
    time = 0;
    pos = VECTOR2(0,0);
}

void Actor::setPos(VECTOR2 p) {
    pos=p;
}

VECTOR2 Actor::getPos() {
    return pos;
}

float Actor::getAngle() {
    return angle;
}

float Actor::getRadius() {
    return radius;
}

bool Actor::isDead() {
    return dead;
}

void Actor::setDead(bool d) {
    dead = d;
} 

int Actor::getDeadCount() {
    return deadCount;
}

void Actor::update() {
    if (!dead) {
        time++;
        setPos(pos+velocity);
    } else {
        deadCount--;
    }
}

void Actor::setVelocity(VECTOR2 a) {
    velocity = a;
}

void Actor::addVelocity(VECTOR2 a) {
    setVelocity(velocity+a);
}

VECTOR2 Actor::getVelocity() {
    return velocity;
}

void Actor::setGame(Game* g) {
    game = g;
}

sf::Uint8 Actor::getFrame() {
    return frame;
}

void Actor::send(sf::Packet& p) {
    p << num << getPos().x << getPos().y << angle << getFrame() << first;
    if (first) {
        p << anchor << radius;
        first = false;
    }
}

void Actor::setNum(sf::Uint16 &n) {
    num = n++;
}

Shape* Actor::getShape() {
    shape->pos = getPos();
    shape->angle = getAngle();
    return shape;
}
