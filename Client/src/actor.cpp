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

Player::Player(sf::Uint8 c) {
    color = c;

    if (c==0) {
        sprite = TextureLoader::getInstance()->getSprite("pink.png");
    } else if (c==1) {
        sprite = TextureLoader::getInstance()->getSprite("orange.png");
    } else if (c==2) {
        sprite = TextureLoader::getInstance()->getSprite("green.png");
    } else if (c==3) {
        sprite = TextureLoader::getInstance()->getSprite("blue.png");
    }

    gun = TextureLoader::getInstance()->getSprite("guns.png");

    sprite.setTextureRect(sf::IntRect(0, 0, 42, 42));
    gun.setTextureRect(sf::IntRect(0,0,32,32));

    sprite.setOrigin(21,42-14);
    gun.setOrigin(16,16);
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
    sf::Uint8 f=frame;
    if (f>=100) {
        sprite.setScale(-1,1);
        f-=100;
    } else {
        sprite.setScale(1,1);
    }
    
    sprite.setTextureRect(sf::IntRect((f%4)*42, (f/4)*42, 42, 42));
    sprite.setPosition(pos);

    VECTOR2 hands = VECTOR2(0,5)%((angle-90)*M_PI/180);

    gun.setPosition(pos+sf::Vector2f(hands.x,hands.y));
    gun.setTextureRect(sf::IntRect((f/20)*32,0,32,32));
    gun.setRotation(angle-90);

    if (angle < 0) {
        window.draw(gun);
    }

    window.draw(sprite);

    if (angle >= 0) {
        window.draw(gun);
    }
}
