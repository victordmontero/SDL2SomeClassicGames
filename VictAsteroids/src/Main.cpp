#include<cstdio>
#include<iostream>
#include<ctime>
#include<cmath>
#include<string>
#include<vector>
#ifdef LINUX
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#elif WIN32
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#endif

const int WIDTH = 1200;
const int HEIGHT = 700;
const int FPS = 1000/60;
float DEGTORAD = 0.017453f;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* spaceShipTex = NULL;
SDL_Texture* backgroundTex = NULL;
SDL_Texture* ScoreTex = NULL;
SDL_Rect rect;
SDL_Event event;

TTF_Font* scoreFont = NULL;

unsigned long Score = 0;

class Entity;

//<-Prototypes->//
void InitSDL();
void QuitSDL();
void InitTTF();
void QuitTTF();
SDL_Texture* TTFToTex(TTF_Font *font,const char *text,SDL_Color fg);
SDL_Texture* LoadImage(char* filename);
void ExitGame();
bool IsCollide(const Entity* uno,const Entity* otro);

std::vector<Entity*> entities;

//Classes//
class Animation
{
    public:
        SDL_Texture* sprite;
        std::vector<SDL_Rect> frames;
        SDL_Rect pos;
        SDL_Rect curFrameRect;
        float speed;
        float frame;
        Animation(){}

        Animation(SDL_Texture* t,int x,int y,int w,int h,int count,float speed,SDL_Point pos)
        {
            this->frame = 0.0;
            this->speed = speed;
            for(int i=0;i<count;i++)
            {
                SDL_Rect rect = {x+i*w,y,w,h};
                frames.push_back(rect);
            }
            this->pos = {pos.x,pos.y,w,h};

            sprite = t;
            t = NULL;
        }

        void Update()
        {
            frame += speed;
            int n = frames.size();
            if(frame >= n) frame -= n;
            curFrameRect = frames[int(frame)];
            if(n>0)
            {
                SDL_RenderCopy(renderer,sprite,&curFrameRect,&pos);
            }
        }

        ~Animation()
        {
            SDL_DestroyTexture(this->sprite);
        }
};

class Entity
{
    public:
        float x,y,dx,dy,R,angle;
        bool life;
        std::string name;
        Animation anim;

        Entity(){life = true;}

        void Settings(const Animation &a,int x,int y,float angle=0,int radius=1)
        {
            this->x=x;
            this->y=y;
            anim = a;
            anim.pos.x = x;
            anim.pos.y = y;
            this->angle=angle;
            R=radius;
        }

        virtual void Update(){};
        
        void Draw(SDL_Renderer* renderer)
        {
            SDL_Rect rect;
            SDL_Rect clip;
            rect = {x,y,anim.pos.w,anim.pos.h};
            clip = anim.curFrameRect;
            SDL_RenderCopyEx(renderer,anim.sprite,&clip,&rect,angle+90,NULL,SDL_FLIP_NONE);
        }
};

class Asteroid : public Entity
{
    public:
        Asteroid(int maxSpeed = 8,int minSpeed = 4)
            : maxSpeed(maxSpeed), minSpeed(minSpeed)
        {
            //srand(time(NULL));
            dx=rand()%this->maxSpeed-this->minSpeed;
            dy=rand()%this->maxSpeed-this->minSpeed;
            name="asteroid";
        }

        void Update()
        {
            anim.Update();
            x+=dx;
            y+=dy;
            if(x>WIDTH) x=-50;
            if(x<-50) x = WIDTH;
            if(y>HEIGHT) y=-50;
            if(y<-50) y=HEIGHT;
        }

        int maxSpeed;
        int minSpeed;
};

class Bullet : public Entity
{
    public:
        Bullet() 
        {
            this->name="bullet";
        }

        void Update()
        {
            this->anim.Update();
            dx=cos(angle*DEGTORAD)*6;
            dy=sin(angle*DEGTORAD)*6;
            x+=dx;
            y+=dy;
            if(x>WIDTH || x<-20 || y>HEIGHT || y<-20) life=false;
        }
};

class TempAnim : public Entity
{
public:

    TempAnim()
    {
        name="TempAnim";
    }

    void Update()
    {
        this->anim.Update();
        if(this->anim.frame >= (this->anim.frames.size()-1))
            this->life = false;
    }
};

class Player : public Entity
{
public:
    Player(int mSpeed = 15)
    {
        name = "ship";
        dx = 0;
        dy = 0;
        angle = 0;
        thrust = false;
        maxSpeed = mSpeed;
    }

    void Fire(std::vector<Entity*>& entities,const Animation& animation)
    {
        Bullet* b = new Bullet();
        b->Settings(animation,x,y,angle,10);
        entities.push_back(b);
    }

    void Update()
    {
        this->anim.Update();

        if(thrust)
        {
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
        }
        else
        {
            dx*=0.99;
            dy*=0.99;
        }
        
        speed = sqrt(dx*dx+dy*dy);
        if(speed>maxSpeed)
        {
            dx*=maxSpeed/speed;
            dy*=maxSpeed/speed;
        }

        x+=dx;
        y+=dy;

        if(x>WIDTH) x = -20;
        if(x<-20) x = WIDTH;
        if(y>HEIGHT) y = -20;
        if(y<-20) y = HEIGHT;
    }

    float speed;
    int maxSpeed;
    bool thrust;
};

int main(int argc, char* argv[])
{
    InitSDL();
    InitTTF();

    srand(time(NULL));

    spaceShipTex = LoadImage("assets/spaceship.png");
    backgroundTex = LoadImage("assets/background.jpg");

    SDL_Point pos = {100,100};
    Animation rockAnim(LoadImage("assets/rock.png"),0,0,64,64,16,0.2,pos);
    Animation bulletAnim(LoadImage("assets/fire_blue.png"),0,0,32,64,16,0.8,pos);
    Animation exploteAnim(LoadImage("assets/explosions/type_A.png"),0,0,50,50,20,0.4,pos);
    Animation shipAnim(LoadImage("assets/spaceship.png"),40,0,40,40,1,0.4,pos);

    Player* player = new Player();
    player->Settings(shipAnim,400,300,0,25);
    entities.push_back(player);

    float frame = 0;
    float animSpeed = 0.4;
    int frameCount = 20;

    float x=300, y=300;
    float dx = 0,dy = 0, angle = 0;
    bool thrust;

    /* for(int i=0;i<10;i++)
    {
        Asteroid* a = new Asteroid();
        a->Settings(rockAnim,rand()%WIDTH,rand()%HEIGHT,rand()%360,25);
        entities.push_back(a);
    } */

    scoreFont = TTF_OpenFont("assets/SFPixelate.ttf",28);

    while(true)
    {
        Uint32 start = SDL_GetTicks();
        Uint32 curr = 0;

        if(entities.size()<25)
        {
            for(int i=0;i<(25-entities.size());i++)
            {
                Asteroid* a = new Asteroid();
                a->Settings(rockAnim,rand()%1210+1100,rand()%710+690,rand()%360,25);
                entities.push_back(a);
            }
        }

        //<-Event handling ->//
        const Uint8* state = SDL_GetKeyboardState(NULL); 
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                exit(0);

            if(event.key.type == SDL_KEYDOWN 
                && event.key.repeat == 0
                && event.key.keysym.scancode == SDL_SCANCODE_J)
            {
                if(player->life)
                    player->Fire(entities,bulletAnim);

                // Bullet* b = new Bullet();
                // b->Settings(bulletAnim,x,y,angle,10);
                // entities.push_back(b);
            }
        }

        if(state[SDL_SCANCODE_X])
        {
            player->life = false;
        }

        if(state[SDL_SCANCODE_F])
        {
            player->angle+=3;
            angle+=3;
        }
        if(state[SDL_SCANCODE_S])
        {
            player->angle-=3;
            angle-=3;
        }
        if(state[SDL_SCANCODE_E])
        {
            player->thrust = true;
            thrust = true;
        }
        else 
        {
            player->thrust = false;
            thrust = false;
        }

        /* if(state[SDL_SCANCODE_J])
        {
            player->Fire(entities,bulletAnim);
            
            // Bullet* b = new Bullet();
            // b->Settings(bulletAnim,x,y,angle,10);
            // entities.push_back(b);
        } */
        
        //Collision//
        for(int i=0;i<entities.size();i++)
        {
            for(int j=0;j<entities.size();j++)
            {
                if(entities[i]->name=="asteroid" && entities[j]->name=="bullet")
                {
                    //fprintf(stdout,"Collision %s %s\n",entities[i]->name.c_str(),entities[j]->name.c_str());
                    if(IsCollide(entities[i],entities[j]))
                    {
                        TempAnim* explode = new TempAnim();
                        explode->Settings(exploteAnim,entities[i]->x,entities[i]->y,0,1);
                        entities.push_back(explode);
                        Score++;
                        entities[i]->life = entities[j]->life = false;
                    }
                }

                if(entities[i]->name=="asteroid" && entities[j]->name=="ship")
                {
                    //fprintf(stdout,"Collision %s %s\n",entities[i]->name.c_str(),entities[j]->name.c_str());
                    if(IsCollide(entities[i],entities[j]))
                    {
                        TempAnim* explode = new TempAnim();
                        explode->Settings(exploteAnim,entities[j]->x,entities[j]->y,0,1);
                        entities.push_back(explode);
                        entities[j]->life = false;
                    }
                }
            }
        }

        //<-Spaceship Movement ->//
        /* if(thrust)
        {
            dx+=cos(angle*DEGTORAD)*0.2;
            dy+=sin(angle*DEGTORAD)*0.2;
        }
        else
        {
            dx*=0.99;
            dy*=0.99;
        }
        
        int maxSpeed = 15;
        float speed = sqrt(dx*dx+dy*dy);
        if(speed>maxSpeed)
        {
            dx*=maxSpeed/speed;
            dy*=maxSpeed/speed;
        }

        x+=dx;
        y+=dy;

        if(x>WIDTH) x = -20;
        if(x<-20) x = WIDTH;
        if(y>HEIGHT) y = -20;
        if(y<-20) y = HEIGHT; */

        for(int it = 0;it<entities.size();it++)
        {
            entities[it]->Update();

            if(entities[it]->life==false)
            {
                entities.erase(entities.begin()+it);
            }
        }

        ///Draw///
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);

        //Draw Background//
        SDL_RenderCopy(renderer,backgroundTex,NULL,NULL);

        //Draw Asteroids
        for(int i=0;i<entities.size();i++)
        {
            entities[i]->Draw(renderer);
        }

        //Draw SpaceShip
        /* SDL_Rect clip = {40,0,40,40};
        rect = {x,y,40,40};
        SDL_RenderCopyEx(renderer,spaceShipTex,&clip,&rect,angle+90,NULL,SDL_FLIP_NONE); */
         
        SDL_Color scoreColor = {255,255,255};
        char scoreText[10];
        sprintf(scoreText,"Score %d",Score);
        ScoreTex = TTFToTex(scoreFont,scoreText,scoreColor);
        SDL_QueryTexture(ScoreTex,NULL,NULL,&rect.w,&rect.h);
        rect.x = 1000;
        rect.y = 20;
        SDL_RenderCopy(renderer,ScoreTex,NULL,&rect);

        //Displays renderer//
        SDL_RenderPresent(renderer);

        curr = SDL_GetTicks() - start;
        if(curr<FPS)
        {
            SDL_Delay(FPS-curr);
        }

    }

    atexit(ExitGame);
}

void InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
#ifdef DEBUG
        fprintf(stderr,"%s",SDL_GetError());
#endif
        system("PAUSE");
        exit(EXIT_FAILURE);
    }

    if(SDL_CreateWindowAndRenderer(WIDTH,HEIGHT,0,&window,&renderer))
    {
#ifdef DEBUG
        fprintf(stderr,"%s",SDL_GetError());
#endif
        system("PAUSE");
        exit(EXIT_FAILURE);
    }
}

void QuitSDL()
{

    for(int i=0;i<entities.size();i++)
    {
        delete entities[i];
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    QuitTTF();
    SDL_Quit();
}

SDL_Texture* LoadImage(char* filename)
{
    SDL_Surface* tempSurface = IMG_Load(filename);
    SDL_Texture* resultTexture = NULL;

    if(!tempSurface)
    {
#ifdef DEBUG
        fprintf(stderr,"%s :%d :%s",__FILE__,__LINE__,SDL_GetError());
        system("PAUSE");
#endif
        exit(1);
    }

    resultTexture = SDL_CreateTextureFromSurface(renderer,tempSurface);

    if(!resultTexture)
    {
#ifdef DEBUG
        fprintf(stderr,"%s :%d :%s",__FILE__,__LINE__,SDL_GetError());
        system("PAUSE");
#endif
        exit(1);
    }
    SDL_FreeSurface(tempSurface); 
    return resultTexture;
}

void ExitGame()
{
    SDL_DestroyTexture(spaceShipTex);
    SDL_DestroyTexture(backgroundTex);
    TTF_CloseFont(scoreFont);
    QuitSDL();
    //exit(0);
}

bool IsCollide(const Entity* uno,const Entity* otro)
{
    return (otro->x - uno->x)*(otro->x - uno->x)+
           (otro->y - uno->y)*(otro->y - uno->y) <
           (uno->R + otro->R)*(uno->R + otro->R);
}

void InitTTF()
{
    if(TTF_Init())
    {
#ifdef DEBUG
        fprintf(stderr,"%s",TTF_GetError());
#endif       
    }
}

void QuitTTF()
{
    TTF_Quit();
}

SDL_Texture* TTFToTex(TTF_Font *font,const char *text,SDL_Color fg)
{
    SDL_Surface* tempSurf = NULL;
    SDL_Texture* tempTex = NULL;

    tempSurf = TTF_RenderText_Solid(font,text,fg);

    if(!tempSurf)
    {
#ifdef DEBUG
        fprintf(stderr,"%s",TTF_GetError());
#endif  
    }

    tempTex = SDL_CreateTextureFromSurface(renderer,tempSurf);

    if(!tempTex)
    {
#ifdef DEBUG
        fprintf(stderr,"%s :%d :%s",__FILE__,__LINE__,SDL_GetError());
#endif
    }
    SDL_FreeSurface(tempSurf);
    return tempTex;
}