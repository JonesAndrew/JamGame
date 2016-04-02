#include "actor.hpp"

Actor::Actor(float r) {
    radius = r;
    shape.setRadius(r);
    shape.setFillColor(sf::Color(rand()%100+155,rand()%100+155,rand()%100+155));
    sprite = TextureLoader::getInstance()->getSprite("DGJane.png");
    sprite.setTextureRect(sf::IntRect(0,0,48,48));
    sprite.setOrigin(24,45);
    rec.setFillColor(sf::Color(rand()%100+155,rand()%100+155,rand()%100+155));
    rec.setOrigin(r,1);
    rec.setSize(sf::Vector2f(r*2,2));
}

void Actor::setAnchor(sf::Uint8 a) {
    anchor = a;
    if (anchor == 0) {
        shape.setOrigin(radius,radius);
    } else {
        shape.setOrigin(radius,radius*2);
    }
}

bool Actor::prep(sf::Uint32 t,float alpha) {
    if (state[t].pos.x == -1000)
    return true;

    float x = state[t].pos.x * alpha + state[t-1].pos.x * (1.0 - alpha);
    float y = state[t].pos.y * alpha + state[t-1].pos.y * (1.0 - alpha);

    shape.setPosition(x,y);
    shape.setRotation(state[t].angle+90);
    sprite.setPosition(x,y);
    sprite.setRotation(state[t].angle+90);
    rec.setPosition(x,y);
    rec.setRotation(state[t].angle);
    frame = state[t].frame;
    if (frame > 0) {
        if (frame > 128) {
            frame-=128;
            sprite.setScale(-1,1);
        } else {
            sprite.setScale(1,1);
        }
        int f=frame-1;
        sprite.setTextureRect(sf::IntRect((f%10)*48,(f/10)*48,48,48));
    }
    return false;
}

void Actor::draw(sf::RenderTarget &window) {
    if (frame == 100) {
        window.draw(rec);
    } if (frame != 0) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}

void Actor::inter(sf::Uint32 t1,sf::Uint32 t2) {
    if (state[t1].pos.x != -1000 && state[t2].pos.x != -1000) {
        sf::Uint32 d = t2-t1;
        for (sf::Uint32 i=1;i<d;i++) {
            double alpha = double(i)/double(d);
            state[t1+i].pos.x = state[t2].pos.x * alpha + state[t1].pos.x * (1.0 - alpha);
            state[t1+i].pos.y = state[t2].pos.y * alpha + state[t1].pos.y * (1.0 - alpha);
            state[t1+i].angle = state[t2].angle;
            state[t1+i].frame = state[t2].frame;
        }
    }
}
