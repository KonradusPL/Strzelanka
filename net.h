#ifndef net.h
#define net.h
#include "obiekt.h"
#include "player.h"
#include "marcin.h"

class Net
{
public:
    bool jestWrog;
    int n;
    Rodzaj rodzaj;
    Player * gracz;
    Player * enemy;
    SDL_Renderer * renderer;
    TCPsocket socket;
    TCPsocket socketEnemy;
    IPaddress ip;
    Mix_Chunk * shootChunk;
    Net(Player * p, string hostName,Rodzaj rodz,SDL_Renderer * r);
    void addEnemy(Player * e);
    void transferData(bool &czyKoniecRundy);
    void dispose();
    void sendMapNumber(string a);
    void recMapNumber(string *aa,string *bb);
};
#endif // net

