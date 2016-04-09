#include <memory>

#pragma once

class Player;

class PlayerState
{
public:
    virtual void enter(Player& player) {}
    virtual ~PlayerState() {}
    virtual PlayerState* handleInput(Player& player, Input input);
    virtual PlayerState* update(Player& player);
protected:
    int frameTime;
};

class WalkState : public PlayerState
{
	bool stopped;
public:
	virtual void enter(Player& player);
	virtual ~WalkState() {}
	virtual PlayerState* handleInput(Player& player, Input input);
	virtual PlayerState* update(Player& player);
};

class RollState : public PlayerState {
public:
	virtual void enter(Player& player);
	virtual PlayerState* update(Player& player);
};

class ShieldState : public PlayerState {
	int time;
public:
	virtual void enter(Player& player);
	virtual PlayerState* update(Player& player);
};

class DeadState : public PlayerState {
public:
	virtual void enter(Player& player);
	virtual PlayerState* update(Player& player);
};