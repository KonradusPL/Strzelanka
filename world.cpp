#include "world.h"

void World::addPlayer(Player * p)
{
    gracz = p;
}

void World::kolizje()
{
    bool flag = false;
    Player * tabP[2];
    tabP[0] = gracz;
    tabP[1] = enemy;
    for(int i=0; i<blocks.size(); i++)
    {
        ///Kolizje gracza z bloczkami
        if(blocks[i].czyBlok && gracz->y + gracz->h > blocks[i].y && gracz->y < blocks[i].y + blocks[i].h)
        {
            if(blocks[i].czyBlok && gracz->kX == -1 && gracz->x < blocks[i].x + blocks[i].w
                    && gracz->x > blocks[i].x + blocks[i].w - 5)
                gracz->kX = 0;
            else if(gracz->kX == 1 && gracz->x + gracz->w > blocks[i].x
                    && gracz->x + gracz->w < blocks[i].x + 5)
                gracz->kX = 0;
        }
        if(blocks[i].czyBlok && gracz->y > blocks[i].y  + blocks[i].h/2 && gracz->y < blocks[i].y + blocks[i].h
                && gracz->x < blocks[i].x + blocks[i].w && gracz->x + gracz->w > blocks[i].x)
        {
            if(gracz->czySkacze == true)
            {
                gracz->czySkacze = false;
                gracz->czySpada = true;
                gracz->velocityY = 0;
            }
        }
        else if(blocks[i].czyBlok && gracz->y + gracz->h >= blocks[i].y && gracz->y <= blocks[i].y
                && gracz->x < blocks[i].x + blocks[i].w-5 && gracz->x + gracz->w > blocks[i].x+5)
        {
            if(gracz->czySkacze == false)
            {
                if(gracz->kX != 0)
                    gracz->czyIdzie = true;
                gracz->czySpada = false;
                gracz->velocityY = 0;
                gracz->y = blocks[i].y-gracz->h;
                flag = true;
            }
        }
        ///Kolizje pociskow z bloczkami
        for(int k=0; k<2; k++)
        for(int j=0; j<tabP[k]->pociski.size(); j++)
        {
            if(blocks[i].czyBlok && tabP[k]->pociski[j].x + tabP[k]->pociski[j].w > blocks[i].x && tabP[k]->pociski[j].x < blocks[i].x + blocks[i].w
                && tabP[k]->pociski[j].y + tabP[k]->pociski[j].h > blocks[i].y && tabP[k]->pociski[j].y < blocks[i].y + blocks[i].h)
            {
                tabP[k]->pociski[j].dispose();
                tabP[k]->pociski.erase(tabP[k]->pociski.begin() + j);
            }
        }
        if(!flag)
            gracz->czySpada = true;
    }
    ///Gdy wyjdzie pocisk poza pole
    for(int k=0; k<2; k++)
    for(int j=0; j<tabP[k]->pociski.size(); j++)
    {
        if(tabP[k]->pociski[j].x + tabP[k]->pociski[j].w > SCREEN_WIDTH || tabP[k]->pociski[j].x < 0
            || tabP[k]->pociski[j].y + tabP[k]->pociski[j].h > SCREEN_HEIGHT || tabP[k]->pociski[j].y < 0)
        {
            tabP[k]->pociski[j].dispose();
            tabP[k]->pociski.erase(tabP[k]->pociski.begin() + j);
        }
    }
    ///Gdy pocisk pyknie w przeciwnika
    for(int k=0; k<2; k++)
    {
        int e = 0;
        if(k == 0)
            e = 1;
        for(int j=0; j<tabP[k]->pociski.size(); j++)
        {
            if(tabP[k]->pociski[j].x + tabP[k]->pociski[j].w > tabP[e]->x && tabP[k]->pociski[j].x < tabP[e]->x + tabP[e]->w
                && tabP[k]->pociski[j].y + tabP[k]->pociski[j].h > tabP[e]->y && tabP[k]->pociski[j].y < tabP[e]->y + tabP[e]->h)
            {
                tabP[k]->pociski[j].dispose();
                tabP[k]->pociski.erase(tabP[k]->pociski.begin() + j);
                if(k == 0)
                {
                    tabP[e]->hp -= dmg;
                    if(tabP[e]->hp <= 0)
                    {
                        czyKoniecRundy = true;
                        tabP[k]->wynik++;
                    }
                    else
                    {
                        tabP[e]->hpTekst->loadTexture(toString(tabP[e]->hp));
                    }
                }
            }
        }
    }
    ///Gdy frajer wyjdzie z areny
    for(int k=0; k<2; k++)
    {
        int e = 0;
        if(k == 0)
            e = 1;
        if(tabP[k]->y > SCREEN_HEIGHT)
        {
            czyKoniecRundy = true;
            tabP[e]->wynik++;
        }
    }

}
bool World::run(SDL_Event e,Net * con,float delta)
{
    t1 = SDL_GetTicks();
    con->transferData(czyKoniecRundy);
    if(czyKoniecRundy == true)
        kolejnaRunda();
    zmianaAnimacji();
    bool koniec = handle(e);
    kolizje();
    move(t2-t1);
    renderWorld();
    t2 = SDL_GetTicks();
    if(t2-t1<stopTime)
    {
        gracz->shootTime += stopTime;
        czasOdAnimacji += stopTime;
        SDL_Delay(stopTime-(t2-t1));
    }
    else
    {
        gracz->shootTime += stopTime;
        czasOdAnimacji += stopTime;
    }
    return koniec;
}
void World::renderWorld()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,0xFF);
    tlo->render(renderer);
    gracz->render(renderer);
    enemy->render(renderer);
    for(int i=0; i<blocks.size(); i++)
        blocks[i].render(renderer);
    wynikTexture->render();
    gracz->hpTekst->render();
    enemy->hpTekst->render();
    SDL_RenderPresent(renderer);
}
bool World::handle(SDL_Event e)
{
bool flag = false;
    bool koniec = false;
    ///Jesli klawisze sa nacisniete
    gracz->kX = 0;
    if(aP)
    {
        if(gracz->flip != SDL_FLIP_HORIZONTAL)
            gracz->weapon->setView(0);
        gracz->kX = -1;
        flag = true;

    }
    if(dP)
    {
        if(gracz->flip != SDL_FLIP_NONE)
            gracz->weapon->setView(1);
        gracz->kX = 1;
        flag = true;
    }
    if(sP && gracz->czySkacze == false && gracz->czySpada == false)
    {
        gracz->czySkacze = true;
        gracz->velocityY = -10;
    }
    int key;
    ///Wszystkie eventy
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
            koniec = true;
        else if(e.type == SDL_KEYDOWN)
        {
            key = e.key.keysym.sym;
            if(key == SDLK_a)
                aP = true;
            else if(key == SDLK_d)
                dP = true;
            else if(key == SDLK_SPACE)
                sP = true;
        }
        else if(e.type == SDL_KEYUP)
        {
            key = e.key.keysym.sym;
            if(key == SDLK_a)
                aP = false;
            else if(key == SDLK_d)
                dP = false;
            else if(key == SDLK_SPACE)
                sP = false;
        }
        ///Strerowanie bronia
        else if(e.type == SDL_MOUSEMOTION)
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            y = y - gracz->y - gracz->h/2;
            float yF = y;
            float xF;
            int pY = 1,pX = 1;
            /// Sterowanie rotacja broni
            if(yF < 0)
            {
                pY = -1;
                yF = fabs(yF);
            }
            if(x > gracz->x + gracz->w)
            {
                gracz->flip = SDL_FLIP_NONE;
                x = x - gracz->x - gracz->w - 10;
                xF = x;
                gracz->weapon->kat = yF * pY * 90;
            }
            else if(x < gracz->x)
            {
                gracz->flip = SDL_FLIP_HORIZONTAL;
                x = x - gracz->x + 10;
                xF = x;
                xF = fabs(xF);
            }
            if(xF < yF)
            {
                xF = xF / (yF + xF);
                yF = 1 - xF;
            }
            else
            {
                yF = yF / (yF + xF);
                xF = 1 - yF;
            }
            if(gracz->flip == SDL_FLIP_NONE)
            {
                gracz->weapon->kat = yF * pY * 90;
            }
            else
            {
                gracz->weapon->kat = yF * (pY-2*pY) * 90;
            }
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN && gracz->shootTime > gracz->weapon->shootBreak)
        {
            Mix_PlayChannel(-1,shootChunk,0);
            gracz->nowyPocisk = true;
            gracz->shootTime = 0;
            int kat = gracz->weapon->kat;
            float xP,yP;
            float sin;
            float cos;
            if(kat < 0)
                kat -= 2 * kat;
            else if(kat > 0)
                kat = 360 - kat;
            sin = sinus(kat);
            cos = cosinus(kat);
            if(gracz->flip == SDL_FLIP_NONE)
            {
                xP = (gracz->xPPoczPocisku * cos);
                yP = (gracz->xPPoczPocisku * sin);
                xP += gracz->x + gracz->w/2 - 2;
            }
            else
            {
                xP = (gracz->xLPoczPocisku * cos);
                yP = (gracz->xLPoczPocisku * sin);
                xP += gracz->x + gracz->w/2;
            }
            yP -= 2 * yP;
            yP += gracz->y + gracz->h/2 - 12;
            Obiekt pocisk(xP,yP,"Tekstury/pocisk.png",renderer);
            pocisk.kX = (xP - (gracz->x + gracz->w/2 - 2)) / 5;
            pocisk.kY = (yP - (gracz->y + gracz->h/2 - 12)) / 5;
            gracz->pociski.push_back(pocisk);
        }
    }
    if(gracz->flip == SDL_FLIP_NONE)
        gracz->weapon->setView(1);
    else
        gracz->weapon->setView(0);
    ///Tak dla pewnosci
    if(gracz->czyIdzie == true && gracz->kX == 0)
    {
        gracz->czyIdzie = false;
        gracz->texture = gracz->animTextures[0];
    }
    ///Zaczynanie animacji
    if(flag == true)
    {
        if(!gracz->czySkacze && !gracz->czySpada)
        {
            if(gracz->czyIdzie == false)
                gracz->ktoraAnimacja = 1;
            gracz->czyIdzie = true;
        }
        else
        {
            gracz->czyIdzie = false;
        }
        flag = false;
    }
    return koniec;
}
void World::kolejnaRunda()
{
    czyKoniecRundy = false;
    gracz->x = gracz->xStart;
    gracz->y = gracz->yStart;
    enemy->x = enemy->xStart;
    enemy->y = enemy->yStart;
    string a = toString(gracz->wynik);
    string b = toString(enemy->wynik);
    string c;
    if(gracz->x < 400)
        c = a + " : " + b;
    else
        c = b + " : " + a;
    wynikTexture->loadTexture(c);
    gracz->hpTekst->loadTexture("100");
    enemy->hpTekst->loadTexture("100");
    gracz->hp = 100;
    enemy->hp = 100;
    gracz->velocityY = 0;
    enemy->velocityY = 0;
    gracz->pociski.clear();
    enemy->pociski.clear();
}
void World::zmianaAnimacji()
{
    if(gracz->czyIdzie == true && czasOdAnimacji >= animationTime)
    {
        czasOdAnimacji = 0;
        if(gracz->ktoraAnimacja != 3)
            gracz->texture = gracz->animTextures[gracz->ktoraAnimacja];
        else
            gracz->texture = gracz->animTextures[0];
        if(gracz->ktoraAnimacja == 0)
            gracz->ktoraAnimacja = 2;
        else if(gracz->ktoraAnimacja == 1)
            gracz->ktoraAnimacja = 0;
        else if(gracz->ktoraAnimacja == 2)
            gracz->ktoraAnimacja = 3;
        else
            gracz->ktoraAnimacja = 1;
    }
}
void World::initMap(string sciezka,string tloS)
{
    mapaPlik.open(sciezka.c_str());
    int a = 25;
    for(int i=0; i<SCREEN_HEIGHT/25; i++,a--)
    {
        mapaPlik >> mapS[a];
    }
    tlo = new Obiekt(0,0,tloS,renderer);
    for(int i=0; i<26; i++)
    {
        for(int j=0; j<40; j++)
        {
            if(mapS[i][j] != '0')
            {
                char a = mapS[i][j];
                string c = "xD";
                bool czyBlok = true;
                if(a == '1')
                    c = "Tekstury/ziemia1.png";
                else if(a == '2')
                    c = "Tekstury/ziemia0.png";
                else if(a == '3')
                    c = "Tekstury/ziemia2.png";
                else if(a == '4')
                    c = "Tekstury/ziemia3.png";
                else if(a == '5')
                    c = "Tekstury/piasek0.png";
                else if(a == '6')
                    c = "Tekstury/piasek1.png";
                else if(a == '7')
                    c = "Tekstury/piasek2.png";
                else if(a == '8')
                    c = "Tekstury/piasek3.png";
                else if(a == '#')
                    c = "Tekstury/kamien0.png";
                else if(a == '@')
                    c = "Tekstury/kamien1.png";
                else if(a == '<')
                    c = "Tekstury/kamien2.png";
                else if(a == '>')
                    c = "Tekstury/kamien3.png";
                else if(a == '!')
                {
                    c = "Tekstury/mury0.png";
                    czyBlok = false;
                }
                else if(a == 't')
                {
                    c = "Tekstury/trawa.png";
                    czyBlok = false;
                }
                else if(a == '(')
                    c = "Tekstury/mury1.png";
                else if(a == ')')
                    c = "Tekstury/mury2.png";
                else if(a == 'z')
                    c = "Tekstury/ziemioTrawaM.png";
                else if(a == 'd')
                    c = "Tekstury/deska.png";
                else if(a == 'c')
                    c = "Tekstury/cegla.png";
                else if(a == 'p')
                    c = "Tekstury/ziemiaM.png";
                else if(a == 'k')
                    c = "Tekstury/kamienM.png";
                else if(a == 'l')
                    c = "Tekstury/kamien2M.png";
                else if(a == '[')
                    c = "Tekstury/skrzynia1.png";
                else if(a == ']')
                    c = "Tekstury/skrzynia2.png";
                else if(a == 'q')
                    c = "Tekstury/podloga.png";
                else if(a == '|')
                {
                    c = "Tekstury/drzwi1.png";
                    czyBlok = false;
                }
                else if(a == '/')
                {
                    c = "Tekstury/drzwi2.png";
                    czyBlok = false;
                }
                else if(a == '?')
                {
                    c = "Tekstury/drzwi3.png";
                    czyBlok = false;
                }
                else if(a == '{')
                {
                    c = "Tekstury/lozko1.png";
                    czyBlok = false;
                }
                else if(a == '}')
                {
                    c = "Tekstury/lozko2.png";
                    czyBlok = false;
                }
                else if(a == '_')
                {
                    c = "Tekstury/trawaM.png";
                    czyBlok = false;
                }
                else if(a == 'o')
                    c = "Tekstury/pien.png";
                else if(a == 'i')
                    c = "Tekstury/liscie.png";
                if(c != "xD")
                {
                    Block b(j*30,SCREEN_HEIGHT - i*30-30,c,renderer,czyBlok);
                    blocks.push_back(b);
                }
            }
        }
    }
}
void World::move(float delta)
{
    gracz->move(delta);
    enemy->move(delta,true);
}
World::World(SDL_Renderer * r,Player * p, Player * e,TTF_Font * f)
{
    gracz = p;
    enemy = e;
    renderer = r;
    aP = false;
    dP = false;
    sP = false;
    t1 = 0;
    t2 = 0;
    czasOdAnimacji = 0;
    wynikS = "0 : 0";
    font = f;
    wynikTexture = new Tekst(0,50,renderer,font,120,120,120);
    wynikTexture->loadTexture(wynikS);
    wynikTexture->x = SCREEN_WIDTH / 2 - wynikTexture->w/2;
    czyKoniecRundy = false;
    shootChunk = Mix_LoadWAV("Muzyka/shoot.wav");
}
void World::dispose()
{
    if(blocks.size() != 0)
    {
        tlo->dispose();
        delete tlo;
    }
    for(int i=0; i<blocks.size(); i++)
        SDL_DestroyTexture(blocks[i].texture);
    Mix_FreeChunk(shootChunk);
}
