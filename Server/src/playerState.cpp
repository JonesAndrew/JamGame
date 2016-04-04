#include "player.hpp"
#include "playerState.hpp"
#include "game.hpp"
#include "bullet.hpp"
#include <iostream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

PlayerState* PlayerState::handleInput(Player& player, Input input) {
    return nullptr;
}

PlayerState* PlayerState::update(Player& player) {
    return nullptr;
}

void WalkState::enter(Player& player) {
	stopped = true;
	player.frameTime = 0;
	player.frame = 0;
}

PlayerState* WalkState::update(Player& player) {
	VECTOR2 vel = player.getVelocity();
	player.setPos(player.getPos()+vel);
	float mag = vel.mag();
	player.frameTime++;
	if (player.frameTime > 6) {
		player.frameTime = 0;
		player.frame++;
	}
	if (mag != 0) {
		if (stopped == true) {
			player.frameTime = 0;
			player.frame = 0;
		}
		if (player.frame > 7) {
			player.frame = 4;
		}
		stopped = false;
		mag-=player.getDeccel();
		if (mag < 0) {
			mag = 0;
		}
		vel.trunc(mag);
		player.setVelocity(vel);
	} else {
		if (stopped == false) {
			player.frameTime = 0;
			player.frame = 4;
		}
		if (player.frame > 3) {
			player.frame = 0;
		}
		stopped = true;
	}
	player.shotTime--;
	return nullptr;
}

PlayerState* WalkState::handleInput(Player& player, Input input) {
	//std::cout<<input.angle<<"\n";
	VECTOR2 i = VECTOR2(float(input.r)/255 - float(input.l)/255,float(input.d)/255 - float(input.u)/255);
	VECTOR2 vel = player.getVelocity();
	vel += i*player.getAccel();
	vel.trunc(player.getMaxVel());
	player.setVelocity(vel);
	player.setAngle(input.angle/M_PI*180);

	if (player.shotTime <= 0) {
		if (input.s && player.bulletCount > 0) {
			player.sfx.push_back(1);
			player.game->setShake(5);
			VECTOR2 hand(12,0);
			hand%=input.angle;
			player.game->makeActor(std::make_shared<Bullet>(player.getPos()+hand,input.angle,player.color));
			player.shotTime = 60;
            player.bulletCount--;
		}
	}

	if (input.sh) {
		return new ShieldState();
	}

	return nullptr;
}

void ShieldState::enter(Player& player) {
	player.sfx.push_back(3);
	player.setVelocity(VECTOR2(0,0));
	player.frame=8;
	player.frameTime = 0;
	time = 40;
}

PlayerState* ShieldState::update(Player& player) {
	player.frameTime++;
	if (player.frameTime >= 6) {
		player.frameTime = 0;
		player.frame++;
	}
	if (player.frame > 12) {
		player.frame = 0;
		player.frameTime = -100;
	}
	player.shotTime--;
	time--;
	if (time == 0) {
		return new WalkState();
	}
	return nullptr;
}

void DeadState::enter(Player& player) {
	player.setVelocity(VECTOR2(0,0));
	player.frame=13;
	player.frameTime = 0;
}

PlayerState* DeadState::update(Player& player) {
	player.frameTime++;
	if (player.frameTime >= 6) {
		player.frameTime = 0;
		player.frame++;
	}
	if (player.frame > 17) {
		player.frame = 17;
	}
	return nullptr;
}
