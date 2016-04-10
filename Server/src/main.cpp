#include <SFML/Network.hpp>
#include "game.hpp"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include "vector2.hpp"

enum gameType : sf::Uint8 {
    SOLO_GAME,
    MULTIPLAYER_GAME,
};

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

sf::Uint32 GAME_NUMBER = 0;

std::vector<std::shared_ptr<PlayerNetwork> > players;
std::vector<std::weak_ptr<PlayerNetwork> > que;
std::vector<std::shared_ptr<Game> > games;

sf::TcpListener listener;
sf::UdpSocket gameSocket;

#ifdef __unix__

void my_handler(int s){

    printf("Caught signal %d\n",s);
    players.clear();
    que.clear();
    games.clear();
    listener.close();
    exit(0);

}

#endif

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
    float getFPS() const { return mFps; }

private:
    unsigned int mFrame;
    float mFps;
    sf::Clock mClock;
};

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

double dt = 1.0/60.0;

void startGame(std::vector<std::weak_ptr<PlayerNetwork> > p) {

    std::shared_ptr<Game> game = std::make_shared<Game>();
    for (size_t i=0; i<p.size(); i++) {

        game->players.push_back(p[i].lock());
        p[i].lock()->game = game;

    }
    game->socket = &gameSocket;
    game->start();
    games.push_back(game);

}

// void startSoloGame(std::vector<std::weak_ptr<PlayerNetwork> > p) {

//     std::shared_ptr<SoloGame> game = std::make_shared<SoloGame>();
//     for (size_t i=0; i<p.size(); i++) {

//         game->players.push_back(p[i].lock());
//         p[i].lock()->game = game;

//     }
//     game->socket = &gameSocket;
//     game->start();
//     games.push_back(game);

// }

void udpLoop() {

    sf::Packet packet;
    sf::IpAddress address;
    unsigned short port;

    while (gameSocket.receive(packet,address,port) == sf::Socket::Done) {

        sf::Uint8 type;
        packet>>type;

        // std::cout<<"UDP LOOP START\n";
        // std::cout<<"TYPE:" << (int)type << "\n";

        if (type == 0) {

            sf::Uint32 id;
            packet>>id;

            for (size_t i=0; i<players.size(); i++) {

                if (!players[i]->matched && id == players[i]->id) {
                    std::cout<<"UDP MATCHED\n";
                    players[i]->address = address;
                    players[i]->port = port;
                    players[i]->matched = true;
                    sf::Packet p;
                    players[i]->socket->send(p);
                    std::cout<<"UDP MATCHED END\n";
                    break;

                }

            }

        } else if (type == 1)  {

            for (size_t i=0; i<players.size(); i++) {

                if (players[i]->address == address && players[i]->port == port && players[i]->game != NULL) {

                    players[i]->game->handleInput(packet,players[i]);

                }

            }

        }

        // std::cout<<"UDP LOOP END\n";

    }

}

void erasePlayer(int i) {

    std::cout<<"DISCONNECTED\n";

    for (size_t g=0;g<games.size();g++) {

        if (games[g] == players[i]->game) {

            games.erase(games.begin()+g);
            std::cout<<"GAMES ERASE\n";
            break;

        }

    }

    players.erase(players.begin()+i);

    for (size_t q=0;q<que.size();q++) {

        if (que[q].expired()) {

            que.erase(que.begin()+q);
            std::cout<<"QUE ERASE\n";
            break;

        }

    }

}

void tcpLoop() {

    for (size_t i=0; i<players.size(); i++) {

        sf::Packet packet;
        sf::Socket::Status status = players[i]->socket->receive(packet);

        if (status == sf::Socket::Done) {

            std::cout<<"RECIEVED\n";

            if (players[i]->state == 0) {

                players[i]->state=1;

                sf::Uint8 numP,gameT;
                packet>>numP>>gameT;
                players[i]->setNumPlayers(int(numP));

                std::cout << "PLAYERS: " << int(numP) << "\n";

                if (gameT == gameType::SOLO_GAME) {

                    // std::vector<std::weak_ptr<PlayerNetwork> > p;
                    // p.push_back(players[i]);
                    // startSoloGame(p);

                } else if (gameT == gameType::MULTIPLAYER_GAME) {

                    if (numP == 4) {

                        std::vector<std::weak_ptr<PlayerNetwork> > p;
                        p.push_back(players[i]);
                        startGame(p);

                    } else {

                        que.push_back(players[i]);

                        if (que.size() == 4) {
                            startGame(que);
                            que.clear();
                        }

                    }

                }

            } else {

                if (players[i]->game != nullptr) {
                    players[i]->game->handleInput(packet,players[i]);
                }

            }

        } else if (status != sf::Socket::NotReady) {

            erasePlayer(i);
            i--;

        }

    }

}

int main()
{
    // VECTOR2 v(1,10);
    // std::cout<<v.to_str()<<std::endl;

    // std::cout<<(v%-(M_PI/2)).to_str()<<std::endl;
    sf::Clock timer;

    sf::Time time_now;
    sf::Time time_old;
    sf::Time time_delta;
    sf::Time time_acc;

    FPS fps;

    time_now = timer.getElapsedTime();
    time_delta = time_now - time_old;
    time_old = time_now;

    #ifdef __unix__

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    #endif

    sf::Uint32 idGen=0;

    gameSocket.bind(50000);
    gameSocket.setBlocking(false);

    listener.listen(40000);
    listener.setBlocking(false);

    while (true) {

        time_now = timer.getElapsedTime();
        time_delta = time_now - time_old;
        time_old = time_now;

        if (time_delta.asSeconds() > 0.5)
        time_delta = sf::seconds(0.5);

        time_acc += time_delta;

        int i=0;
        while (time_acc.asSeconds() >= dt) {

            sf::TcpSocket *socket = new sf::TcpSocket;

            if (listener.accept(*socket) == sf::Socket::Done) {

                socket->setBlocking(false);
                std::shared_ptr<PlayerNetwork> p = std::make_shared<PlayerNetwork>();
                p->socket = socket;
                p->id = idGen++;
                sf::Packet resp;
                resp<<p->id;
                p->socket->send(resp);
                players.push_back(p);
                std::cout<<"NEW CONNECTION\n";

            } else {

                delete socket;

            }

            udpLoop();

            tcpLoop();

            for (size_t i=0; i<games.size(); i++) {

                games[i]->update();

            }

            fps.update();
            time_acc = sf::seconds(time_acc.asSeconds()-dt);
            i++;
        }
        if (i>1) {
            std::cout<<"MULTI:"<<i<<"\n";
        }
    }

    return EXIT_SUCCESS;

}
