#include "player.hpp"
#include "playerState.hpp"
#include "game.hpp"
#include "bullet.hpp"
#include <iostream>
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

PlayerState* PlayerState::handleInput(Player& player, Input input) {
    return nullptr;
}

PlayerState* PlayerState::update(Player& player) {
	player.shotTime--;
	player.dodgeTime--;
    return nullptr;
}

void WalkState::enter(Player& player) {
	stopped = true;
	player.frameTime = 0;
	player.frame = 0;
}

PlayerState* WalkState::update(Player& player) {
	PlayerState::update(player);
	VECTOR2 vel = player.getVelocity();
	player.setPos(player.getPos()+vel);
	float mag = vel.mag();
	if (mag != 0) {
		if (stopped == true) {
			player.frameTime = 0;
			player.frame = 4;
		}
		player.frameTime++;
		if (player.frameTime > 6) {
			player.frameTime = 0;
			player.frame++;
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
			player.frame = 0;
		}
		player.frameTime++;
		if (player.frameTime > 8) {
			player.frameTime = 0;
			player.frame++;
		}
		if (player.frame > 3) {
			player.frame = 0;
		}
		stopped = true;
	}
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
			VECTOR2 hand(15,0);
			hand%=input.angle;
			float speed = 3.25;
			if (player.getRandom() != 0) {
				speed = 5.25;
				player.sfx.push_back(2);
				player.game->setShake(10);
			} else {
				player.sfx.push_back(1);
				player.game->setShake(5);
			}
			player.game->makeActor(std::make_shared<Bullet>(player.getPos()+hand,speed,input.angle,player.color));
			player.shotTime = 40;
            player.bulletCount--;
            player.rotTarget++;
            player.b[0] = false;
            player.shift();
            if (player.rotTarget == 6) {
            	player.rotTarget = 0;
            }
		}
	}

	if (input.sh) {
		if (i.x == 0 && i.y == 0) {
			return new ShieldState();
		} else if (player.dodgeTime <= 0) {
			player.dodgeTime = 40;
			player.setVelocity(VECTOR2(player.getMaxVel(),0)%std::atan2(i.y,i.x));
			return new RollState();
		}
	}

	return nullptr;
}

void RollState::enter(Player& player) {
	player.frame=8;
	player.frameTime = 0;
}

PlayerState* RollState::update(Player& player) {
	PlayerState::update(player);
	VECTOR2 vel = player.getVelocity();
	player.setPos(player.getPos()+vel);
	player.frameTime++;
	if (player.frameTime >= 6) {
		player.frameTime = 0;
		player.frame++;
	}
	if (player.frame > 11) {
		return new WalkState();
	}
	return nullptr;
}

void ShieldState::enter(Player& player) {
	player.sfx.push_back(3);
	player.setVelocity(VECTOR2(0,0));
	player.frame=12;
	player.frameTime = 0;
	time = 40;
}

PlayerState* ShieldState::update(Player& player) {
	PlayerState::update(player);
	player.frameTime++;
	if (player.frameTime >= 6) {
		player.frameTime = 0;
		player.frame++;
	}
	if (player.frame > 15) {
		player.frame = 0;
		player.frameTime = -100;
	}
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
	PlayerState::update(player);
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
