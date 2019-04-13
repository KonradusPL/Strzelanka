#ifndef player.h
#define player.h
#include "tekst.h"
#include "obiekt.h"

const int velocityX = 3;
class Player : public Obiekt
{
public:
    SDL_Texture * animTextures[3];
    vector <Obiekt> pociski;
    Weapon * weapon;
    TTF_Font * hpFont;
    Tekst  * hpTekst;
    int kX,ktoraAnimacja,xLPoczPocisku,xPPoczPocisku,ySPoczPocisku,shootTime,wynik,hp,xStart,yStart;
    float velocityY,xPoczPocisku,yPoczPocisku;
    bool czySpada,czySkacze,czyIdzie,nowyPocisk,czyTrafiony;
    void render(SDL_Renderer * renderer);
    void handleEvent(SDL_Event &e);
    void move(float delta,bool czyWrog = false);
    void dispose();
    SDL_Texture * loadTexture(string sciezka,SDL_Renderer * renderer);
    Player(int xx,int yy,string sciezka,string a1,string a2,string sWeapon,SDL_Renderer * renderer);
};
#endif // player
