#include "player.hpp"
#include "playerState.hpp"
#include "game.hpp"
#include "bullet.hpp"
#include <iostream>

PlayerState* PlayerState::handleInput(Player& player, Input input) {
    return nullptr;
}

PlayerState* PlayerState::update(Player& player) {
    return nullptr;
}




void WalkState::enter(Player& player) {
	stopped = true;
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

	if (player.shotTime <= 0) {
		if (input.s) {
			player.game->makeActor(std::make_shared<Bullet>(player.getPos(),input.angle));
			player.shotTime = 15;
		}
	}

	return nullptr;
}