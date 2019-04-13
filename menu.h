#ifndef menu.h
#define menu.h
#include "obiekt.h"
#include "tekst.h"
#include "marcin.h"
class Menu
{
public:
    int t1,t2;
    bool koniecMenu,czySerwer;
    string hostName,a,b;
    Rodzaj rodzaj;
    RodzajSceny rodzajSceny;
    SDL_Renderer * renderer;
    TTF_Font * font;
    Obiekt * createServerGUI;
    Obiekt * joinServerGUI;
    Obiekt * titleGUI;
    Obiekt * czekanieNaWrogaGUI;
    Obiekt * mapImg[4];
    Tekst * wybierzMapeTekst;
    Tekst * wpiszSerwerTekst;
    Tekst * ipSerweraTekst;
    Menu(SDL_Renderer * r,TTF_Font * f);
    bool run(SDL_Event e);
    void render();
    bool handle(SDL_Event e);
    void dispose();
};

#endif // menu
