//
//  Director.h
//  Ludum Dare
//
//  Created by Andrew Jones on 8/22/14.
//  Copyright (c) 2014 Andrew Jones. All rights reserved.
//

#ifndef __Ludum_Dare__Director__
#define __Ludum_Dare__Director__

#include <iostream>
#include "scene.hpp"
#include <SFML/Network.hpp>

class FPS
{
public:
    /// @brief Constructor with initialization.
    ///
    FPS() : mFrame(0), mFps(0) {}

    /// @brief Update the frame count.
    ///
    void update();

    /// @brief Get the current FPS count.
    /// @return FPS count.
    float getFPS() { return mFps; }

private:
    unsigned int mFrame;
    float mFps;
    sf::Clock mClock;
};

class Director {
    sf::Clock timer;

    sf::Time time_now;
    sf::Time time_old;
    sf::Time time_delta;
    sf::Time time_acc;

    FPS fps;

    Scene *currentScene;
    Scene *nextScene;

    bool next;
    float scale;

    static Director *instance;

    Director(){};
    Director(Director const&){};

public:
    sf::RenderWindow window;
    bool running;
    void initialize();
    float getScale();

    static Director *getInstance();
    sf::RenderWindow *getWindow();

    void startWithScene(Scene *scene);
    void enqueueScene(Scene *scene);

};

#endif /* defined(__Ludum_Dare__Director__) */
