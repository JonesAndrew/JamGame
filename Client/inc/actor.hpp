#ifndef __Ludum_Dare__Actor__
#define __Ludum_Dare__Actor__

#include "textureLoader.hpp"

struct ActorState {
    sf::Vector2f pos;
    float angle;
    sf::Uint8 frame;
    ActorState() {
        pos.x = -1000;
        pos.y = -1000;
        frame = 0;
    }
};

struct Actor {
    int frame;
    float angle;
    sf::Vector2f pos;

    Actor();

    void setClassNum(sf::Uint8);

    bool prep(sf::Uint32 t,float alpha);

    virtual void draw(sf::RenderTarget &window);

    void inter(sf::Uint32 t1,sf::Uint32 t2);

    sf::Uint8 classNum;
    std::map<sf::Uint32,ActorState> state;
    ActorState activeState;

    virtual float getBot() {return pos.y;}
};

struct Player : public Actor {
    Player(sf::Uint8 c);
    sf::Sprite sprite,gun;
    sf::Uint8 color;
    virtual void draw(sf::RenderTarget &window);
};

struct Bullet : public Actor {
    Bullet();
    sf::Sprite sprite;
    sf::Sprite fire;
    sf::Uint8 color;
    virtual void draw(sf::RenderTarget &window);
    virtual float getBot() {
        if (frame < 2) {
            return pos.y+100;
        } 
        return pos.y-10;
    }
};

#endif
