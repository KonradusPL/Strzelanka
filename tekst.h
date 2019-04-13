#ifndef tekst.h
#define tekst.h
#include "obiekt.h"
class Tekst
{
public:
    SDL_Texture * texture;
    SDL_Renderer * renderer;
    TTF_Font * font;
    SDL_Color color;
    int w,h,x,y;
    Tekst(int xx,int yy,SDL_Renderer * r,TTF_Font * f,int R,int G,int B);
    void free();
    void loadTexture(string sciezka);
    void render();
};
#endif // tekst




