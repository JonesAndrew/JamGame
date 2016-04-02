#include <SFML/Network.hpp>
#include <memory>

#pragma once

#include "vector2.hpp"

class Game;
class Bullet;
class Player;
class Wall;

struct Disk;
struct Circle;
struct Polygon;

struct Shape 
{
    VECTOR2 pos;
    float angle;
    virtual bool intersectWith(Shape*) = 0;
    virtual bool intersected(Shape*) = 0;

    virtual bool intersectWith(Disk*)    {return false;}
    virtual bool intersectWith(Circle*)  {return false;}
    virtual bool intersectWith(Polygon*) {return false;}
};

struct Circle : public Shape
{
    int radius;
    virtual bool intersectWith(Shape* s);
    virtual bool intersected(Shape* s);
    virtual bool intersectWith(Disk* d);
};

struct Polygon : public Shape
{
    std::vector<VECTOR2> points;
    virtual bool intersectWith(Shape* s);
    virtual bool intersected(Shape* s);
    virtual bool intersectWith(Disk* d);
};

struct Disk : public Shape {
    int radius;
    virtual bool intersectWith(Shape* s);
    virtual bool intersected(Shape* s);
    virtual bool intersectWith(Disk* d);
    virtual bool intersectWith(Circle* c);
    virtual bool intersectWith(Polygon* p);
};

float cloestPointOnEdge(const VECTOR2 p, const VECTOR2 a, const VECTOR2 b);

class Actor : public std::enable_shared_from_this<Actor> {
protected:

    VECTOR2 velocity;
    VECTOR2 pos;

    float angle,radius;

    sf::Uint16 num;
    Shape *shape;

    bool first,dead;
    int deadCount;
    sf::Uint8 frame,anchor,classNum;

public:

    int time;

    Game *game;

    Actor();

    void setPos(VECTOR2);
    VECTOR2 getPos();

    float getAngle();

    float getRadius();

    bool isDead();
    void setDead(bool);

    int getDeadCount();

    virtual sf::Uint8 getFrame();

    virtual void setVelocity(VECTOR2);
    void addVelocity(VECTOR2);
    VECTOR2 getVelocity();

    virtual void setNum(sf::Uint16&);

    void setGame(Game*);

    virtual void update();

    virtual void send(sf::Packet&);

    virtual void collideWith(std::shared_ptr<Actor>) = 0;
    virtual void collidedBy(std::shared_ptr<Actor>) = 0;

    virtual void collideWith(std::shared_ptr<Bullet>) {}
    virtual void collideWith(std::shared_ptr<Player>) {}
    virtual void collideWith(std::shared_ptr<Wall>) {}

    virtual Shape* getShape();
};
