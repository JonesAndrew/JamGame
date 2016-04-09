//
//  Director.cpp
//  Ludum Dare
//
//  Created by Andrew Jones on 8/22/14.
//  Copyright (c) 2014 Andrew Jones. All rights reserved.
//

#include "director.hpp"
#include <iostream>

void FPS::update()
{
    if(mClock.getElapsedTime().asSeconds() >= 1.f)
    {
        mFps = mFrame/mClock.getElapsedTime().asSeconds();
        mFrame = 0;
        mClock.restart();
        std::cout << "FPS:" << getFPS() << "\n";
    }

    ++mFrame;
}

Director *Director::instance = NULL;

Director *Director::getInstance() {
    if (!instance) instance = new Director();
    return instance;
}

float Director::getScale() {
    return scale;
}

void Director::initialize() {
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        std::cout << "Mode #" << i << ": "
                  << mode.width << "x" << mode.height << " - "
                  << mode.bitsPerPixel << " bpp" << std::endl;
    }

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::VideoMode big = sf::VideoMode(1920, 1080);
    sf::VideoMode small = sf::VideoMode(960, 540);
    std::cout<<"960: "<< small.isValid()<<"\n";
    // Create a window with the same pixel depth as the desktop

    if (big.width > desktop.width) {
        window.create(small, "Bullets", sf::Style::Default);
        scale = 1;
    } else {
        window.create(big, "Bullets", sf::Style::Fullscreen);
        scale = 2;
    }
    //window.setMouseCursorVisible(false);

    window.setVerticalSyncEnabled(true);
    currentScene = NULL;

    timer.restart();
}

void Director::startWithScene(Scene *scene) {
    nextScene = scene;

    do {
        if (currentScene != NULL) {
            delete currentScene;
        }
        currentScene = nextScene;
            time_now = timer.getElapsedTime();
            time_delta = time_now - time_old;
            time_old = time_now;

        currentScene->setupScene(window);

        next = false;

        running = true;
        bool redraw = true;

        while (running) {
            time_now = timer.getElapsedTime();
            time_delta = time_now - time_old;
            time_old = time_now;

            if (time_delta.asSeconds() > 0.5)
                time_delta = sf::seconds(0.5);

            time_acc += time_delta;
            int i=0;
            while (time_acc.asSeconds() >= 1.0/60.0) {
                running = currentScene->tick(getWindow());
                time_acc = sf::seconds(time_acc.asSeconds()-1.0/60.0);
                i++;
                //fps.update();
            }
            if (i>1) {
                std::cout << "MULTI:" << i << "\n";
            }

            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed) {
                    window.close();
                    running = false;
                } else {
                    currentScene->handleEvent(event, getWindow());
                }
            }

            float alpha = time_acc.asSeconds() / (1.0 / 60.0);
            //std::cout<<alpha<<"\n";
            window.clear();
            currentScene->draw(getWindow(),alpha);
            window.display();
            window.setView(window.getDefaultView());
            fps.update();
        }
    } while (next);

}

sf::RenderWindow *Director::getWindow() {
    return &window;
}

void Director::enqueueScene(Scene *scene) {
    next = true;
    nextScene = scene;
}
