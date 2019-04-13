#include "obiekt.h"

Obiekt::Obiekt()
{
    kat = 0;
    flip = SDL_FLIP_NONE;
}
Obiekt::Obiekt(int xx,int yy,string sciezka,SDL_Renderer * renderer)
{
    kat = 0;
    flip = SDL_FLIP_NONE;
    kX = 0;
    kY = 0;
    x = xx;
    y = yy;
    loadTexture(sciezka,renderer);
}

void Obiekt::loadTexture(string sciezka,SDL_Renderer * renderer)
{
    SDL_Surface * pow = NULL;
    pow = IMG_Load(sciezka.c_str());
    if(pow != NULL)
    {
        SDL_SetColorKey(pow,SDL_TRUE,SDL_MapRGB(pow->format,255,255,255));
        w = pow->w;
        h = pow->h;
    }
    texture = SDL_CreateTextureFromSurface(renderer,pow);
    SDL_FreeSurface(pow);
}
void Obiekt::render(SDL_Renderer * renderer)
{
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopyEx(renderer,texture,NULL,&rect,kat,NULL,flip);
}
void Obiekt::dispose()
{
    SDL_DestroyTexture(texture);
    texture = NULL;
}
Weapon::Weapon(string sciezka,SDL_Renderer * renderer,int shootB)
{
    shootBreak = shootB;
    pX = -40;
    pY = 27;
    lX = -40;
    lY = pY;
    wX = pX;
    wY = pY;
    loadTexture(sciezka,renderer);
}
void Weapon::setView(int n)
{
    if(n==0)
    {
        flip = SDL_FLIP_HORIZONTAL;
        wX = lX;
        wY = lY;
    }
    else
    {
        flip = SDL_FLIP_NONE;
        wX = pX;
        wY = pY;
    }
}
