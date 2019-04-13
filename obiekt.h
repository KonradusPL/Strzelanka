#ifndef obiekt.h
#define obiekt.h
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <SDL_net.h>

using namespace std;

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 780;
const float gravity = 0.3f;
const int stopTime = 18;
const int animationTime = 150;

enum Rodzaj{serwer,klient,nic};
enum RodzajSceny{menuScreen,czekanieNaWroga,wyborMapy,wpisywanieSerwera};

class Obiekt
{
public:
    SDL_RendererFlip flip;
    int x,y,w,h,kat,kX,kY;
    SDL_Texture * texture;
    void loadTexture(string sciezka,SDL_Renderer * renderer);
    void render(SDL_Renderer * renderer);
    void dispose();
    Obiekt(int xx,int yy,string sciezka,SDL_Renderer * renderer);
    Obiekt();
};
class Weapon : public Obiekt
{
public:
    int lX,lY,pX,pY,wX,wY,shootBreak;
    Weapon(string sciezka,SDL_Renderer * renderer,int shootB);
    void setView(int n);
};
SDL_Texture * loadTexture(string sciezka,SDL_Renderer * renderer);

#endif
