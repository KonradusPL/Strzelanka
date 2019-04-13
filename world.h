#ifndef world.h
#define world.h
#include <vector>
#include "obiekt.h"
#include "player.h"
#include "block.h"
#include "marcin.h"
#include "net.h"
#include "tekst.h"

const int sizeX = 100;
const int sizeY = 30;
const int blockSize = 25;
const int mapWidth = 1000;
const int mapHeight = 650;
const int dmg = 5;

class World
{
public:
    SDL_Renderer * renderer;
    TTF_Font * font;
    Mix_Chunk * shootChunk;
    ifstream mapaPlik;
    string mapS[26],wynikS;
    int ox,oy,czasOdAnimacji;
    Uint32 t1,t2;
    bool aP,dP,sP,czyKoniecRundy;
    vector <Block> blocks;
    Player * gracz;
    Player * enemy;
    Obiekt * tlo;
    Tekst * wynikTexture;
    void addPlayer(Player * p);
    void createTerrain(SDL_Renderer * renderer);
    void renderWorld();
    void dispose();
    void kolizje();
    void move(float delta);
    void initMap(string sciezka,string tloS);
    void zmianaAnimacji();
    bool handle(SDL_Event e);
    bool run(SDL_Event e,Net * con,float delta);
    void kolejnaRunda();
    World(SDL_Renderer * r,Player * p, Player * e,TTF_Font * f);
};

#endif // world
