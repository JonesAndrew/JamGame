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
    sf::CircleShape shape;
    sf::RectangleShape rec;
    sf::Sprite sprite;
    float radius;
    int frame;

    Actor(float r);

    void setAnchor(sf::Uint8 a);

    bool prep(sf::Uint32 t,float alpha);

    void draw(sf::RenderTarget &window);

    void inter(sf::Uint32 t1,sf::Uint32 t2);

    sf::Uint8 anchor;
    std::map<sf::Uint32,ActorState> state;
    ActorState activeState;
};

#endif
