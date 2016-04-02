#include "actor.hpp"

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
    return false;
}

void Actor::draw(sf::RenderTarget &window) {
}

Bullet::Bullet() {
    sprite = TextureLoader::getInstance()->getSprite("guns.png");
    sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
    sprite.setOrigin(16,16);
}

void Bullet::draw(sf::RenderTarget &window) {
    sprite.setPosition(pos);
    sprite.setRotation(angle-90);
    window.draw(sprite);
}

Player::Player() {
    sprite = TextureLoader::getInstance()->getSprite("slimes.png");
    face = TextureLoader::getInstance()->getSprite("face.png");

    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

    sprite.setOrigin(16,16);
    face.setOrigin(16,16);
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
    sprite.setPosition(pos);
    window.draw(sprite);
}
