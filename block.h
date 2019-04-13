#ifndef block.h
#define block.h
#include "obiekt.h"

class Block : public Obiekt
{
public:
    int hp,czyBlok;
    Block(int xx,int yy,string sciezka,SDL_Renderer * renderer, bool blok = true);
};

#endif // block
