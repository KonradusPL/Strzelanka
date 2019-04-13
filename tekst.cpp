#include "tekst.h"

Tekst::Tekst(int xx,int yy,SDL_Renderer * r,TTF_Font * f,int R,int G,int B)
{
    color = {R,G,B};
    x = xx;
    y = yy;
    w = 0;
    h = 0;
    texture = NULL;
    renderer = r;
    font = f;
}
void Tekst::free()
{
    if(texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        w = 0;
        h = 0;
    }
}
void Tekst::render()
{
    SDL_Rect renderQuad = {x, y, w, h};

	SDL_RenderCopy(renderer,texture,NULL,&renderQuad);
}
void Tekst::loadTexture(string sciezka)
{
    free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( font, sciezka.c_str(),color);

	if(textSurface != NULL)
	{
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if( texture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			w = textSurface->w;
			h = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
}
