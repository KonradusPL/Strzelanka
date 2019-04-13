#include "player.h"

Player::Player(int xx,int yy,string sciezka,string a1,string a2,string sWeapon,SDL_Renderer * renderer)
{
    shootTime = 0;
    kX = 0;
    velocityY = 0;
    x = xx;
    y = yy;
    yStart = y;
    nowyPocisk = false;
    czyTrafiony = false;
    weapon = new Weapon(sWeapon,renderer,100);
    xPoczPocisku = 0;
    yPoczPocisku = 0;
    xLPoczPocisku = -50;
    xPPoczPocisku = 50;
    ktoraAnimacja = 0;
    wynik = 0;
    hp = 100;
    czySpada = true;
    czySkacze = false;
    czyIdzie = false;
    animTextures[0] = loadTexture(sciezka,renderer);
    animTextures[1] = loadTexture(a1,renderer);
    animTextures[2] = loadTexture(a2,renderer);
    texture = animTextures[0];
    hpFont = TTF_OpenFont("Font/future.ttf", 60);
}
SDL_Texture * Player::loadTexture(string sciezka,SDL_Renderer * renderer)
{
    SDL_Surface * pow = NULL;
    SDL_Texture * tex = NULL;
    pow = IMG_Load(sciezka.c_str());
    if(pow != NULL)
    {
        SDL_SetColorKey(pow,SDL_TRUE,SDL_MapRGB(pow->format,255,255,255));
        w = pow->w;
        h = pow->h;
    }
    tex = SDL_CreateTextureFromSurface(renderer,pow);
    SDL_FreeSurface(pow);
    return tex;
}
void Player::render(SDL_Renderer * renderer)
{
    SDL_Rect rect = {x,y,w,h};
    SDL_RenderCopyEx(renderer,texture,NULL,&rect,kat,NULL,flip);
    weapon->x = weapon->wX + x;
    weapon->y = weapon->wY + y;
    weapon->render(renderer);
    for(int i=0; i<pociski.size(); i++)
        pociski[i].render(renderer);
}
void Player::move(float delta,bool czyWrog)
{
    if(czyWrog == false)
    {
        if(czySkacze == true && velocityY > 0)
        {
            czySkacze = false;
            czySpada = true;
        }
        if(czySkacze || czySpada)
            velocityY += gravity;
        x += kX * velocityX;
        y += velocityY;
    }
    for(int i=0; i<pociski.size(); i++)
    {
        pociski[i].x += pociski[i].kX;
        pociski[i].y += pociski[i].kY;
    }
}
void Player::dispose()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(animTextures[0]);
    SDL_DestroyTexture(animTextures[1]);
    SDL_DestroyTexture(animTextures[2]);
    animTextures[0] = NULL;
    animTextures[1] = NULL;
    animTextures[2] = NULL;
    texture = NULL;
    weapon->dispose();
    for(int i=0; i<pociski.size(); i++)
        pociski[i].dispose();
    delete weapon;
    delete hpFont;
}

