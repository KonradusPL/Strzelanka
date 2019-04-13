#include "menu.h"

Menu::Menu(SDL_Renderer * r,TTF_Font * f)
{
    t1 = 0;
    t2 = 0;
    renderer = r;
    font = f;
    koniecMenu = false;
    rodzaj = nic;
    rodzajSceny = menuScreen;
    titleGUI = new Obiekt(190,0,"Tekstury/title.png",renderer);
    createServerGUI = new Obiekt(500,320,"Tekstury/createServerGUI.png",renderer);
    joinServerGUI = new Obiekt(500,420,"Tekstury/joinServerGUI.png",renderer);
    czekanieNaWrogaGUI = new Obiekt(200,260,"Tekstury/czekanieNaWroga.png",renderer);
    mapImg[0] = new Obiekt(180,250,"Tekstury/mapImg0.png",renderer);
    mapImg[1] = new Obiekt(480,250,"Tekstury/mapImg1.png",renderer);
    mapImg[2] = new Obiekt(780,250,"Tekstury/mapImg2.png",renderer);
    mapImg[3] = new Obiekt(180,450,"Tekstury/mapImg3.png",renderer);
    wybierzMapeTekst = new Tekst(150,50,r,f,30,167,225);
    wybierzMapeTekst->loadTexture("select the map :");
    wpiszSerwerTekst = new Tekst(100,50,r,f,30,167,225);
    wpiszSerwerTekst->loadTexture("write a server-ip :");
    ipSerweraTekst = new Tekst(50,350,r,f,255,255,255);
    ipSerweraTekst->loadTexture("");
}
bool Menu::run(SDL_Event e)
{
    t1 = SDL_GetTicks();
    bool koniec = handle(e);
    render();
    t2 = SDL_GetTicks();
    if(t2-t1<stopTime)
        SDL_Delay(stopTime-(t2-t1));
    else
        SDL_Delay(stopTime);
    return koniec;
}
bool Menu::handle(SDL_Event e)
{
    bool koniec = false;
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            koniec = true;
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            if(rodzajSceny == menuScreen)
            {
                if(x >= createServerGUI->x && x <= createServerGUI->x + createServerGUI->w
                && y >= createServerGUI->y && y <= createServerGUI->y + createServerGUI->h)
                {
                    //koniecMenu = true;
                    rodzaj = serwer;
                    rodzajSceny = wyborMapy;
                }
                else if(x >= joinServerGUI->x && x <= joinServerGUI->x + joinServerGUI->w
                && y >= joinServerGUI->y && y <= joinServerGUI->y + joinServerGUI->h)
                {
                    //koniecMenu = true;
                    rodzaj = klient;
                    rodzajSceny = wpisywanieSerwera;
                    czySerwer = false;
                }
            }
            else if(rodzajSceny == wyborMapy)
            {
                for(int i=0; i<4; i++)
                {
                    if(x >= mapImg[i]->x && x <= mapImg[i]->x + mapImg[i]->w
                    && y >= mapImg[i]->y && y <= mapImg[i]->y + mapImg[i]->h)
                    {
                        koniecMenu = true;
                        if(i == 0)
                        {
                            a = "Mapy/mapa1.txt";
                            b = "Tekstury/tlo1.png";
                        }
                        else if(i == 1)
                        {
                            a = "Mapy/mapa2.txt";
                            b = "Tekstury/tlo2.png";
                        }
                        else if(i == 2)
                        {
                            a = "Mapy/mapa3.txt";
                            b = "Tekstury/tlo3.png";
                        }
                        else if(i == 3)
                        {
                            a = "Mapy/mapa4.txt";
                            b = "Tekstury/tlo4.png";
                        }
                    }
                }
            }
        }
        if(e.type == SDL_KEYDOWN)
        {
            int key = e.key.keysym.sym;
            if(rodzajSceny == wpisywanieSerwera)
            {
                if(key == SDLK_RETURN)
                {
                    koniecMenu = true;
                }
                else if(key == SDLK_BACKSPACE && hostName.size() > 0)
                {
                    hostName.erase(hostName.size()-1);
                }
                else if(key != SDLK_BACKSPACE)
                {
                    char znak = key;
                    hostName += znak;
                }
                ipSerweraTekst->loadTexture(hostName);
            }
        }
    }
    return koniec;
}
void Menu::render()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,192,192,192,0xFF);
    if(rodzajSceny == menuScreen)
    {
        createServerGUI->render(renderer);
        joinServerGUI->render(renderer);
        titleGUI->render(renderer);
    }
    else if(rodzajSceny == czekanieNaWroga)
    {
        czekanieNaWrogaGUI->render(renderer);
    }
    else if(rodzajSceny == wyborMapy)
    {
        wybierzMapeTekst->render();
        mapImg[0]->render(renderer);
        mapImg[1]->render(renderer);
        mapImg[2]->render(renderer);
        mapImg[3]->render(renderer);
    }
    else if(rodzajSceny == wpisywanieSerwera)
    {
        wpiszSerwerTekst->render();
        ipSerweraTekst->render();
    }

    SDL_RenderPresent(renderer);
}
void Menu::dispose()
{
    SDL_DestroyTexture(createServerGUI->texture);
    SDL_DestroyTexture(joinServerGUI->texture);
    SDL_DestroyTexture(titleGUI->texture);
    SDL_DestroyTexture(czekanieNaWrogaGUI->texture);
    SDL_DestroyTexture(mapImg[0]->texture);
    SDL_DestroyTexture(mapImg[1]->texture);
    SDL_DestroyTexture(mapImg[2]->texture);
    SDL_DestroyTexture(mapImg[3]->texture);
    SDL_DestroyTexture(wybierzMapeTekst->texture);
    SDL_DestroyTexture(wpiszSerwerTekst->texture);
    SDL_DestroyTexture(ipSerweraTekst->texture);
    delete czekanieNaWrogaGUI;
    delete createServerGUI;
    delete joinServerGUI;
    delete titleGUI;
    delete mapImg[0];
    delete mapImg[1];
    delete mapImg[2];
    delete mapImg[3];
    delete wybierzMapeTekst;
    delete wpiszSerwerTekst;
    delete ipSerweraTekst;
}
