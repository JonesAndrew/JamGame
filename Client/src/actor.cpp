#include "actor.hpp"
#include "vector2.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

Actor::Actor() {
}

void Actor::setClassNum(sf::Uint8 c) {
    classNum = c;
}

bool Actor::prep(sf::Uint32 t,float alpha) {
    if (state[t].pos.x == -1000)
    return true;

    float x = state[t].pos.x * alpha + state[t-1].pos.x * (1.0 - alpha);
    float y = state[t].pos.y * alpha + state[t-1].pos.y * (1.0 - alpha);

    pos = sf::Vector2f(x,y);
    angle = state[t].angle;
    frame = state[t].frame;
    return false;
}

void Actor::draw(sf::RenderTarget &window) {
}

Bullet::Bullet() {
    sprite = TextureLoader::getInstance()->getSprite("guns.png");
    sprite.setOrigin(16,16);
    fire = TextureLoader::getInstance()->getSprite("guns.png");
    fire.setTextureRect(sf::IntRect(224, 0, 32, 32));
    fire.setOrigin(16,16);
}

void Bullet::draw(sf::RenderTarget &window) {
    sprite.setPosition(pos);
    sprite.setRotation(angle-90);
    sprite.setTextureRect(sf::IntRect(64+32*color, 0, 32, 32));
    fire.setPosition(pos);
    fire.setRotation(angle-90);
    fire.setTextureRect(sf::IntRect(224+32*frame, 0, 32, 32));
    window.draw(sprite);
    if (frame < 2) {
        window.draw(fire);
    }
}

Player::Player() {
    sprite = TextureLoader::getInstance()->getSprite("slimes.png");
    face = TextureLoader::getInstance()->getSprite("face.png");
    gun = TextureLoader::getInstance()->getSprite("guns.png");
    shield = TextureLoader::getInstance()->getSprite("shields.png");
    dead = TextureLoader::getInstance()->getSprite("death.png");

    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    gun.setTextureRect(sf::IntRect(0,0,32,32));

    sprite.setOrigin(16,18);
    face.setOrigin(16,18);
    gun.setOrigin(16,16);
    shield.setOrigin(32,42);
    dead.setOrigin(48,42);
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

void Player::draw(sf::RenderTarget &window) {
    sprite.setTextureRect(sf::IntRect((frame%20)*32, 32*color, 32, 32));
    sprite.setPosition(pos);

    shield.setTextureRect(sf::IntRect((frame%20-8)*64,64*color,64,64));
    shield.setPosition(pos);

    dead.setTextureRect(sf::IntRect(((frame%20-13)%3)*96,64*color*2+64*((frame%20-13)/3),64,64));
    dead.setPosition(pos);

    face.setPosition(pos);

    VECTOR2 hands = VECTOR2(0,5)%((angle-90)*M_PI/180);

    gun.setPosition(pos+sf::Vector2f(hands.x,hands.y));
    gun.setTextureRect(sf::IntRect((frame/20)*32,0,32,32));
    gun.setRotation(angle-90);

    if (angle < 0) {
        window.draw(gun);
    }
    if (frame%20 < 8) {
        window.draw(sprite);
        window.draw(face);
    } else if (frame%20 < 13) {
        window.draw(shield);
    } else {
        window.draw(dead);
    }
    if (angle >= 0) {
        window.draw(gun);
    }
}
