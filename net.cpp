#include "net.h"

Net::Net(Player * p, string hostName,Rodzaj rodz,SDL_Renderer * r)
{
    gracz = p;
    rodzaj = rodz;
    jestWrog = false;
    renderer = r;
    if(rodzaj == klient)
    {
        SDLNet_ResolveHost(&ip,hostName.c_str(),1234);
        socketEnemy = SDLNet_TCP_Open(&ip);
        gracz->hpTekst = new Tekst(1070,700,renderer,gracz->hpFont,0,0,128);
        gracz->xStart = 1100;
    }
    else
    {
        SDLNet_ResolveHost(&ip,NULL,1234);
        socket = SDLNet_TCP_Open(&ip);
        gracz->hpTekst = new Tekst(20,700,renderer,gracz->hpFont,0,0,128);
        gracz->xStart = 100;
    }
    gracz->hpTekst->loadTexture("100");
    shootChunk = Mix_LoadWAV("Muzyka/shoot.wav");
}
void Net::dispose()
{
    SDLNet_TCP_Close(socket);
    SDLNet_TCP_Close(socketEnemy);
    Mix_FreeChunk(shootChunk);
}
void Net::addEnemy(Player * e)
{
    enemy = e;
    if(rodzaj == klient)
    {
        enemy->hpTekst = new Tekst(20,700,renderer,gracz->hpFont,168,8,8);
    }
    else
    {
        enemy->hpTekst = new Tekst(1070,700,renderer,gracz->hpFont,168,8,8);
    }
    enemy->hpTekst->loadTexture("100");
}
void Net::transferData(bool &czyKoniecRundy)
{
    int x=0,y=0,liczbaP,hp = gracz->hp;
    SDL_RendererFlip flip;
    int kat;
    int nowyPocisk;
    {
        x = gracz->x;
        y = gracz->y;
        flip = gracz->flip;
        kat = gracz->weapon->kat;
        nowyPocisk = gracz->nowyPocisk;
        liczbaP = gracz->pociski.size();
        SDLNet_TCP_Send(socketEnemy,&x,4);
        SDLNet_TCP_Send(socketEnemy,&y,4);
        SDLNet_TCP_Send(socketEnemy,&flip,4);
        SDLNet_TCP_Send(socketEnemy,&kat,4);
        SDLNet_TCP_Send(socketEnemy,&nowyPocisk,4);
        if(nowyPocisk == 1)
        {
            gracz->nowyPocisk = false;
            SDLNet_TCP_Send(socketEnemy,&gracz->pociski[gracz->pociski.size()-1].x,4);
            SDLNet_TCP_Send(socketEnemy,&gracz->pociski[gracz->pociski.size()-1].y,4);
            SDLNet_TCP_Send(socketEnemy,&gracz->pociski[gracz->pociski.size()-1].kX,4);
            SDLNet_TCP_Send(socketEnemy,&gracz->pociski[gracz->pociski.size()-1].kY,4);
        }
        SDLNet_TCP_Send(socketEnemy,&gracz->ktoraAnimacja,4);
        SDLNet_TCP_Send(socketEnemy,&enemy->hp,4);
        if(SDLNet_TCP_Recv(socketEnemy,&x,4) > 0);
            enemy->x = x;
        if(SDLNet_TCP_Recv(socketEnemy,&y,4) > 0);
            enemy->y = y;
        if(SDLNet_TCP_Recv(socketEnemy,&flip,4) > 0);
        {
            enemy->flip = flip;
            enemy->weapon->flip = flip;
        }
        if(SDLNet_TCP_Recv(socketEnemy,&kat,4) > 0);
            enemy->weapon->kat = kat;
        if(SDLNet_TCP_Recv(socketEnemy,&nowyPocisk,4) > 0)
            if(nowyPocisk == 1)
            {
                int xP,yP,kX,kY;
                SDLNet_TCP_Recv(socketEnemy,&xP,4);
                SDLNet_TCP_Recv(socketEnemy,&yP,4);
                SDLNet_TCP_Recv(socketEnemy,&kX,4);
                SDLNet_TCP_Recv(socketEnemy,&kY,4);
                Obiekt p(xP,yP,"Tekstury/pocisk.png",renderer);
                p.kX = kX;
                p.kY = kY;
                enemy->pociski.push_back(p);
                Mix_PlayChannel(-1,shootChunk,0);
            }
        SDLNet_TCP_Recv(socketEnemy,&enemy->ktoraAnimacja,4);
        SDLNet_TCP_Recv(socketEnemy,&gracz->hp,4);
        if(hp != gracz->hp)
        {
            gracz->hpTekst->loadTexture(toString(gracz->hp));
            if(gracz->hp <= 0)
            {
                czyKoniecRundy = true;
                enemy->wynik++;
            }
        }
    }
    if(enemy->ktoraAnimacja == 3)
        enemy->texture = enemy->animTextures[0];
    else
        enemy->texture = enemy->animTextures[enemy->ktoraAnimacja];
}
void Net::sendMapNumber(string a)
{
    int n;
    if(a == "Mapy/mapa1.txt")
        n = 1;
    else if(a == "Mapy/mapa2.txt")
        n = 2;
    else if(a == "Mapy/mapa3.txt")
        n = 3;
    else if(a == "Mapy/mapa4.txt")
        n = 4;
    SDLNet_TCP_Send(socketEnemy,&n,4);
}
void Net::recMapNumber(string *aa,string *bb)
{
    while(true)
    {
        if(SDLNet_TCP_Recv(socketEnemy,&n,4) > 0)
            break;
    }
    if(n == 1)
    {
        *aa = "Mapy/mapa1.txt";
        *bb = "Tekstury/tlo1.png";
    }
    else if(n == 2)
    {
        *aa = "Mapy/mapa2.txt";
        *bb = "Tekstury/tlo2.png";
    }
    else if(n == 3)
    {
        *aa = "Mapy/mapa3.txt";
        *bb = "Tekstury/tlo3.png";
    }
    else if(n == 4)
    {
        *aa = "Mapy/mapa4.txt";
        *bb = "Tekstury/tlo4.png";
    }
}
