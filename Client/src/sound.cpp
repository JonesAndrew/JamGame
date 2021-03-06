#include "sound.hpp"

SoundPlayer *SoundPlayer::instance = NULL;

SoundPlayer *SoundPlayer::getInstance() {
    if (!instance) {
        instance = new SoundPlayer();
    }
    return instance;
}

void SoundPlayer::playSound(std::string file,int vol) {
    for (size_t i=0; i<sfx.size(); i++) {
        if (sfx[i].getStatus() == sf::Sound::Stopped) {
            sfx.erase(sfx.begin()+i);
            i--;
        }
    }

    Sound *s = sounds[file];

    if (s == NULL) {
        s = new Sound();
        s->s.loadFromFile("res/"+file);
        std::cout<<"res/"+file<<"\n";
        sounds[file] = s;
    }

    bool copy=false;
    for (size_t i=0;i<sfx.size();i++) {
        if (sfx[i].getBuffer() == &s->s) {
            //sfx[i].play();
            //copy = true;
            break;
        }
    }

    if (!copy) {
        sfx.emplace_back();
        sfx.back().setBuffer(s->s);
        sfx.back().setVolume(vol);
        sfx.back().play();
    }
}

void SoundPlayer::loadSound(std::string file) {
    Sound *s = sounds[file];

    if (s == NULL) {
        s = new Sound();
        s->s.loadFromFile("res/"+file);
        std::cout<<"res/"+file<<"\n";
        sounds[file] = s;
    }
}
