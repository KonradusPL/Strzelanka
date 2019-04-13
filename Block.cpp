#include "block.h"

Block::Block(int xx,int yy,string sciezka,SDL_Renderer * renderer, bool blok)
{
    x = xx;
    y = yy;
    czyBlok = blok;
    loadTexture(sciezka,renderer);
}
