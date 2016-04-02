//
//  Scene.h
//  Ludum Dare
//
//  Created by Andrew Jones on 8/22/14.
//  Copyright (c) 2014 Andrew Jones. All rights reserved.
//

#ifndef __Ludum_Dare__Scene__
#define __Ludum_Dare__Scene__

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Scene {
public:
    Scene();
    virtual ~Scene();

    virtual void setupScene(sf::RenderWindow &window);
    virtual bool tick(sf::RenderWindow *window);
    virtual void draw(sf::RenderTarget *window,float alpha);
    virtual void handleEvent(sf::Event event, sf::RenderWindow *window) = 0;
};

#endif /* defined(__Ludum_Dare__Scene__) */
