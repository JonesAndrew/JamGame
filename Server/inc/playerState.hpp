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
public:
	virtual void enter(Player& player);
	virtual ~WalkState() {}
	virtual PlayerState* handleInput(Player& player, Input input);
	virtual PlayerState* update(Player& player);
};