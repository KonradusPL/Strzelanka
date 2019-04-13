#include "obiekt.h"
#include "world.h"
#include "menu.h"
#include "player.h"
#include "net.h"

bool init();
void initMaps();
void close();
void start();

SDL_Window * okno;
SDL_Renderer * renderer;
SDL_Event e;
Player * gracz;
Player * enemy;
World * swiat;
Menu * menuGry;
Net * connection;
TTF_Font * font = NULL;
ifstream mapy[5];
bool koniec = false,jestPolaczenie = false;
int n = 0,t1,t2;

int main(int args,char * argv[])
{
    koniec = !init();
    if(!koniec)
        start();
    while(!koniec && !menuGry->koniecMenu)
    {
        koniec = menuGry->run(e);
    }
    if(menuGry->rodzaj == serwer && !koniec)
    {
        menuGry->czySerwer = true;
        connection = new Net(gracz,"",serwer,renderer);
        menuGry->rodzajSceny = czekanieNaWroga;
        swiat->initMap(menuGry->a,menuGry->b);
        while(true)
        {
            if(menuGry->run(e))
               break;
            connection->socketEnemy = SDLNet_TCP_Accept(connection->socket);
            if(connection->socketEnemy)
            {
                connection->addEnemy(enemy);
                jestPolaczenie = true;
                int a = 1;
                SDLNet_TCP_Send(connection->socketEnemy,&a,4);
                connection->sendMapNumber(menuGry->a);
                break;
            }
        }
    }
    else if(menuGry->rodzaj == klient && !koniec)
    {
        string aa,bb;
        menuGry->czySerwer = false;
        connection = new Net(gracz,menuGry->hostName,klient,renderer);
        connection->addEnemy(enemy);
        int a = 6;
        while(true)
        {
            if(SDLNet_TCP_Recv(connection->socketEnemy,&a,4) > 0)
            {
                if(a == 1)
                    jestPolaczenie = true;
                gracz->x = 1100;
                enemy->x = 100;
                connection->recMapNumber(&aa,&bb);
                swiat->initMap(aa,bb);
                break;
            }
        }
    }
    if(jestPolaczenie && !koniec)
    {
        while(koniec == false)
        {
            koniec = swiat->run(e,connection,0);
        }
    }
    close();
    return 0;
}
void start()
{
    gracz = new Player(100,200,"Tekstury/player0.png","Tekstury/player1.png","Tekstury/player2.png","Tekstury/reka.png",renderer);
    enemy = new Player(1100,200,"Tekstury/enemy0.png","Tekstury/enemy1.png","Tekstury/enemy2.png","Tekstury/rekaE.png",renderer);
    menuGry = new Menu(renderer,font);
    swiat = new World(renderer,gracz,enemy,font);
}
bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return false;
    SDLNet_Init();
    TTF_Init();
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    okno = SDL_CreateWindow("Strzelanka 2",300,150,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(okno == NULL)
        return false;
    renderer = SDL_CreateRenderer(okno,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
        return false;
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
        cout<<IMG_GetError();
    font = TTF_OpenFont("Font/future.ttf", 80);

    return true;
}
void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(okno);
    swiat->dispose();
    gracz->dispose();
    enemy->dispose();
    menuGry->dispose();
    if(connection != NULL)
    {
        connection->dispose();
        delete connection;
    }
    delete enemy;
    delete gracz;
    delete swiat;
    delete menuGry;
    delete font;
    okno = NULL;
    renderer = NULL;
    SDLNet_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
