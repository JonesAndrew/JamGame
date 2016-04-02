#ifndef __Ludum_Dare__Edit__
#define __Ludum_Dare__Edit__

#include "scene.hpp"
#include <TGUI/TGUI.hpp>

struct Object {
    virtual void draw(sf::RenderTarget *window) = 0;
    virtual bool click(sf::Vector2f p) = 0;
    virtual void addGui(tgui::VerticalLayout::Ptr) = 0;
    virtual void removeGui(tgui::VerticalLayout::Ptr) = 0;
    virtual void save(std::ofstream&) = 0;
    sf::Vector2f pos,vel;
};

struct Planet : public Object {
    int size;
    bool start;
    Planet() {
        size = 30;
        start = false;
    }
    Planet(std::istringstream&);
    virtual void draw(sf::RenderTarget *window) {
        sf::CircleShape shape(size);
        shape.setPosition(pos);
        shape.setOrigin(size,size);
        window->draw(shape);
    }
    virtual bool click(sf::Vector2f p) {
        if ( ( ((pos.x-p.x)*(pos.x-p.x)) + ((pos.y-p.y)*(pos.y-p.y)) ) < (size*size) ) {
            return true;
        }
        return false;
    }
    void setSize(std::string);
    void setVelX(std::string);
    void setVelY(std::string);
    void setX(std::string);
    void setY(std::string);
    void setStart(bool);
    virtual void addGui(tgui::VerticalLayout::Ptr);
    virtual void removeGui(tgui::VerticalLayout::Ptr);
    virtual void save(std::ofstream&);
};

class LevelEdit : public Scene {
    std::vector<std::shared_ptr<Object>> objects;
    std::weak_ptr<Object> selected;
    sf::View view;
    tgui::Gui gui;
    std::string placeType;
    tgui::VerticalLayout::Ptr vert;
public:
    virtual void draw(sf::RenderTarget *window,float alpha);
    virtual void setupScene(sf::RenderWindow &window);
    virtual bool tick(sf::RenderWindow *window);
    virtual void handleEvent(sf::Event event, sf::RenderWindow *window);
    void setPlaceType(std::string);
    void save();
    void load();
    void play();
    void destroy();
};

#endif
