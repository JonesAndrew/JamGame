#ifndef __GameClient__Sound__
#define __GameClient__Sound__

#include <iostream>
#include <map>
#include <SFML/Audio.hpp>

struct Sound {
    sf::SoundBuffer s;
    int refCount;
};

class SoundPlayer {
private:

    std::map<std::string, Sound*> sounds;
    std::vector<sf::Sound> sfx;

    static SoundPlayer *instance;

    SoundPlayer(){};
    SoundPlayer(SoundPlayer const&){};

public:

    static SoundPlayer *getInstance();
    void playSound(std::string file,int vol=100);
    void loadSound(std::string file);
};

#endif /* defined(__GameClient__Sound__) */
