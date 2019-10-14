/*OneSweetLM: Think file header with comments. Call-outs, called from, purpose, variable definitions and explainations. Makes things a lot easier.*/

#define MaxObjects 800
#define HM 5
#include <windows.h> 
#include <mmsystem.h>  
#include <string.h>  
HBITMAP Scale; 
#define NumberOfSounds 15
int oldsoundtoplay = 4;
int soundtoplay = 4;
int password_window();
extern int MaxLvl;
int LoadCustom(unsigned char *lvl);
#define clear_async();  GetAsyncKeyState('S'); GetAsyncKeyState('s'); GetAsyncKeyState('U'); GetAsyncKeyState('u'); GetAsyncKeyState(32);     GetAsyncKeyState('p');     GetAsyncKeyState('h');     GetAsyncKeyState('H');     GetAsyncKeyState('P');     GetAsyncKeyState(VK_DOWN);     GetAsyncKeyState(VK_UP);     GetAsyncKeyState(13);   GetAsyncKeyState(10);     GetAsyncKeyState(VK_LEFT);     GetAsyncKeyState(VK_RIGHT);     GetAsyncKeyState('a');   
extern unsigned char PassWord[800];
void DrawAlpha(HDC hdestdc, HDC hsrcdc, HDC alphamaskdc, int destx, int desty, int srcx, int srcy, int maskx, int masky, int width, int height, int drawflag);
#define SOUND 9
#define debugkeys 9
#define CCR 201
#define exp1 8
#define GAME_MENU 1
#define GAME_PLAY 2
//#define OmegaCollisionSystem 9 /*memo to self: delete this collision code, it don't work.*/
//The CCR is the collision check range, the smaller the value, the fewer the objects checked for collision status.
#define DoubleBuffer 9
#define RUNTIME_PSEUDO_RELOC_SUPPORT
//I'll just globalize these variables so I don't have to change every load level call.
int bkgx = 0;
int bkgy = 0;


int GeneratePasswordString(unsigned char *lvl, int lvlnum,unsigned char *PassWord);
#ifndef SOUND
#define ChangeBGM(); GetAsyncKeyState(32);
#endif

#ifdef SOUND
#include <dsound.h>  /*define SOUND to enable sound code*/

#define ChangeBGM();  \
if(soundtoplay == 5)\
{\
               PlaySound(MAKEINTRESOURCE(212),apphinst,SND_RESOURCE);}\
if(oldsoundtoplay != soundtoplay)\
{ProtoBuffersStr[oldsoundtoplay]->lpVtbl->Stop(ProtoBuffersStr[oldsoundtoplay]);\
ProtoBuffersStr[soundtoplay]->lpVtbl->Play(ProtoBuffersStr[soundtoplay], 0,0, DSBPLAY_LOOPING);}

#endif
/*
This program had troubles running on some computer setups for some reason, including my old 700 mhz machine, this seems to fix the problem
I got gdb to tell me it was something wrong with _pei386_runtime_relocator, the error Windows told me was 0197:00408928
obtained from: http:/sources.redhat.com/ml/binutils/2002-07/msg00035/crtest.c
*/ 
#if !defined(RUNTIME_PSEUDO_RELOC_SUPPORT)
#include <windows.h>
typedef struct
{
    DWORD addend;
    DWORD target;
}
runtime_pseudo_reloc;

extern runtime_pseudo_reloc __RUNTIME_PSEUDO_RELOC_LIST__;
extern runtime_pseudo_reloc __RUNTIME_PSEUDO_RELOC_LIST_END__;
extern void* _data_start__;
extern void* _image_base__;

void
_pei386_runtime_relocator (runtime_pseudo_reloc* start, runtime_pseudo_reloc* end)
{
    DWORD reloc_target;
    runtime_pseudo_reloc* r;
    for (r = start; r < end; r++)
    {
        //printf ("ptr=%x, addend=%d\n", (long)r->target, r->addend);
        reloc_target = (DWORD) &_image_base__ + r->target;
        //printf ("reloc target=%x\n", reloc_target);
        *((DWORD*) reloc_target) += r->addend;
    }
}
#endif

HINSTANCE apphinst;
HBITMAP PaulaBMP, IceBlastBMP, Canvas, FX, Tile, Osprites;

int ptinrect(int x, int y, RECT obj)
{
    if( (x >= obj.left && x <= obj.right) && (y >= obj.top && y <= obj.bottom) )
    return 1;
    else 
    return 0;
        
}

int checkintersect(RECT obj1, RECT obj2)
{
    if( (ptinrect(obj1.top, obj1.left, obj2) == 1) || (ptinrect(obj1.top, obj1.right, obj2) == 1) || (ptinrect(obj1.bottom, obj1.left, obj2) == 1) || (ptinrect(obj1.bottom, obj1.right, obj2) == 1) )
    return 1;
    if( (ptinrect(obj2.top, obj2.left, obj1) == 1) || (ptinrect(obj2.top, obj2.right, obj1) == 1) || (ptinrect(obj2.bottom, obj2.left, obj1) == 1) || (ptinrect(obj2.bottom, obj2.right, obj1) == 1) )
    return 1;
        
    return 0;
}


#ifdef SOUND
LPDIRECTSOUND lpds;
DSBUFFERDESC bufferdesc, bufferdesc2, ProtoBuffers[15];
LPDIRECTSOUNDBUFFER lplpDirectSoundBuffer, lpSecondaryBuffer[3], ProtoBuffersStr[NumberOfSounds+2];
LPDIRECTSOUNDBUFFER lpPlayBuffers[NumberOfSounds+2];
BYTE Bufferinuse[24];
BYTE Bufferused[24];
DWORD rdl, rdl2;
DWORD off = 0;
WAVEFORMATEX WaveFormat, BGMFormat;
WAVEFORMATEX LOWBGMFormat;
VOID *RawData, *RawData2;
BYTE *MemPtr;
HRSRC waveres[NumberOfSounds+3];  int wavesize[NumberOfSounds+3];
HGLOBAL wavemem[NumberOfSounds+3];
BYTE destroycount[24];
#endif

int jumpcounter = 0;

typedef struct tagitemject{
    short int posx:12;
    short int posy:12;
    int tilex, tiley;
    int tilemaskx:12, tilemasky:12;
    int width:9;
    int height:9;
    BYTE healthpickup:1;
    BYTE solid:1; 
    BYTE keio:1;  
    BYTE ghost:1; 
    BYTE ghostt:1;
    BYTE exit:1;
    BYTE kill:1;
    BYTE jumper:1;
    BYTE climber:1;
    BYTE ani:2;   /*Animation adding I am!*/
    BYTE donly:2;
    BYTE numframes:5;
    BYTE aniframe:1;
    BYTE framecounter; /*To slow them down:-P*/
    BYTE framerate;    /*Wellll... delay*/
    BYTE Particles; /* Some sort of particle effect? You bet :)*/
    BYTE PType:4;     /*Hard coded type of particle, 1 = random, jumpy type*/
    BYTE PNumber:4;   /*Number of Particles 1 - 9*/
    short int ParticleData[45]; /*Array for Particle Data, 5 bytes per particle*/
    BYTE Teleportal:1;
    int tx: 12, ty:12;
    BYTE OneUp:1;
} ObjectStruct;


typedef struct tagenemyobject{
    int posx:12,posy:12;
    int ospx:12, ospy:12;
    int ospxm:12, ospym:12;
    int forceposy;
    BYTE fb;
    BYTE counter;
    int width, height;
    int xspeed, yspeed;
    BYTE collisionshrink;
    BYTE direction, prev;
    BYTE keio:1;
    BYTE type, sprite;
    BYTE shootimgx;
    BYTE shootimgy;
    BYTE shootimgwidth;
    BYTE shootimgheight;
    BYTE lunge;
    BYTE health;
    BYTE speedbase;
    int shootx:12, shooty:12, shootset, shootxspeed:10, shootyspeed:10, shootcount;
    BYTE deathset, hitset;
    int hitx, hity;
    BYTE boss;
    BYTE cd:2;
} EnemyStruct;
/*RNG*/

HBITMAP BossPumpkin;

int randomz(int max, int seeder)           /*A random number generator...very basic*/
{
    SYSTEMTIME gen;
    static seed = 1;
    float temp;
    int number;
        
    if(max == -1)
    {seed = seeder; return 0;}
        
    GetLocalTime(&gen);
    /*
   temp = (gen.wMilliseconds * 1.34765350008) + (gen.wSecond * 1.00000865) + (seed * 0.985) + (seed * 2.341188106 * max);
   temp = temp * 2;
      -Most Random so far*/
        
    temp = (gen.wMilliseconds * 1.34765350008) + (gen.wSecond * 1.00000865) + (seed * 0.985) + (seed * 2.341188106 * max * (seed*0.15));
    temp = temp * 2.1;
        
    if(max == 0)
    {return (int)temp;}
        
    seed++;
    if(seed > 1900)
    {seed = 1;}
    number = (long)temp % max;
    return number;
}

#ifdef OmegaCollisionSystem
HRGN XLevelGlobal;
HRGN XGlobalCopy;
HRGN XNewRegion;
HRGN EmptyRegion;

void CreateLGRegion(ObjectStruct *objs)
{
    int i;
    BYTE skipmake;
    DeleteObject(XLevelGlobal);         
    DeleteObject(XNewRegion);
    DeleteObject(XGlobalCopy);
    DeleteObject(EmptyRegion);
    EmptyRegion = CreateRectRgn(0,0,0,0);
    XLevelGlobal = CreateRectRgn(0,0,0,0);
    for(i = 0; i < MaxObjects; i++)
    {
        CombineRgn(XGlobalCopy, XLevelGlobal, EmptyRegion, RGN_OR);
        skipmake = 0;
        if(objs[i].keio == 1 && objs[i].solid == 1)
        {
            XNewRegion = CreateRectRgn(objs[i].posx*10, objs[i].posy*10, (objs[i].posx+objs[i].width)*10, (objs[i].posy+objs[i].height)*10);
        }
        else
        {skipmake=1;}
        if(skipmake != 1)
        {
            CombineRgn(XLevelGlobal, XGlobalCopy, XNewRegion, RGN_OR);
            DeleteObject(XNewRegion);
        }
    }
    DeleteObject(EmptyRegion);
}

unsigned char OmegaCollisionDetect(int objleft, int objtop, int objright, int objbottom, RECT *ovr,int objxspeed, int objyspeed)
{
    RECT obj;
    int shiftx, shifty, i;
    BOOL rir;
        
    obj.top = objtop * 10;
    obj.left = objleft *10;
    obj.right = objright * 10;
    obj.bottom = objbottom * 10;
    shiftx = objxspeed;
    shifty = objyspeed;
        
    for(i = 0; i < 10; i++)
    {
        rir = RectInRegion(XLevelGlobal, &obj);
        if(rir == TRUE)
        {
            MessageBox(NULL, "M", "M", MB_OK|MB_TASKMODAL);
            return 1;
        }
    }     
    return 0;
}

#endif


/*Begin level data section*/
/*Moved to zomlevel.c*/
extern unsigned char level[30][3600];

unsigned char SolidityMap[40][200];

int abbs(int in)
{
    return( in * (in < 0 ? -1 : 1) );
}

RECT RectStopper[MaxObjects];
int RectStopperType[MaxObjects];
char objused[MaxObjects];
// FIX ME LATER DAMMIT
RECT StopperRect[MaxObjects];
int StopperRectType[MaxObjects];

int rects = 0;
int NumofLines; /*Load level fills this in*/
/*Take the above solidity map and make new collision rectangles*/
#ifdef JJJJJJJ
void CreateStoppers(void) /*All it's input is global as is its output*/
{
    int col = 0, line = 0;
    int done = 0;
    int i, set = 999;
    int CurrentRect = 0, Going = 0;
    int linedone[40];
    rects = 0;
    while(done == 0)
    {
        if(SolidityMap[line][col] == 0 || SolidityMap[line][col] > 6)
        {
            if(Going != 0)
            {
                Going = 0;
                CurrentRect++;                      
                rects++;
            }
        }
        else if( SolidityMap[line][col] == 1 || SolidityMap[line][col]==3 || SolidityMap[line][col]==4)
        {
            if(Going == 0)
            {
                RectStopper[CurrentRect].top = line*24;
                RectStopper[CurrentRect].left = col*24;
                    
                Going = SolidityMap[line][col];
            }
            if(Going == SolidityMap[line][col])
            {
                RectStopper[CurrentRect].bottom = (line+1)*24;
                RectStopper[CurrentRect].right = (col+1)*24;      
            }
            if(Going != SolidityMap[line][col])
            {
                Going = SolidityMap[line][col];
                CurrentRect++;
                rects++;
                RectStopper[CurrentRect].top = line*24;
                RectStopper[CurrentRect].left = col*24;
                RectStopper[CurrentRect].bottom = (line+1)*24;
                RectStopper[CurrentRect].right = (col+1)*24;
                Going = SolidityMap[line][col];                
            }
                
        }
        else if(SolidityMap[line][col] == 10)
        {linedone[line] = 1;}
        else
        {/*Nothing for here right now :)*/}
            
        line++;
        if(line >= NumofLines)
        {
            Going = 0;
            CurrentRect++;
            rects++;
            line = 0;
            col++;               
        }
        set = 0;
        for(i = 0; i < NumofLines; i++)
        {
            if(linedone[i] == 1)
            {
                set++;          
            }   
        }
        if(set == NumofLines)
        {
            done = 1;
        }
    }
        
}  //Backburnering this thing.. maybe another aproach will work better...
#endif

void LoadLevel(int *startx, int *starty, ObjectStruct *objs, EnemyStruct *ens, unsigned char *level, int height)
{
    int point = 1, posx = 0, posy = 0, i;
    int o = 0, e = 0, line = 0, col = 0;
    int startdefheight = 240;
    int q, m, z, j;
    int teleports[20];
    char num[6];
    oldsoundtoplay = soundtoplay;
    soundtoplay = 4;
    if(level[0]==49)
    {soundtoplay = 6;}
    if(level[0]==50)
    {soundtoplay = 7;}
    if(level[0]==51)
    {soundtoplay = 8;}
    if(level[0]==52)
    {soundtoplay = 9;}
    
    bkgx = (level[0]&1)*320;
    bkgy = (((level[0]-48)&14)>>1)*240;
    posy = height-startdefheight;
    for(q = 0; q < 40; q++)
    {
        for(m = 0; m < 200; m++)
        {
            SolidityMap[q][m] = 0;           
        }      
    }
    
    for(z = 0; z < 20; z++)
    {
        teleports[z] = -1;  
    }
    
    for(o = 0; o < MaxObjects; o++)
    {
        objs[o].keio = 0;
        objs[o].tilex = 0;
        objs[o].tiley = 0;
        objs[o].tilemaskx = 0;
        objs[o].tilemasky = 0;
        objs[o].width = 24;
        objs[o].height = 24;
        objs[o].solid = 1;
        objs[o].ghost = 0;
        objs[o].ghostt = 0;
        objs[o].exit = 0;
        objs[o].kill = 0;
        objs[o].jumper = 0;
        objs[o].donly = 0;
        objs[o].climber = 0;
        objs[o].ani=0;
        objs[o].numframes=1;
        objs[o].aniframe=0;
        objs[o].healthpickup = 0;
        objs[o].Particles = 0;
        objs[o].OneUp = 0;
        objs[o].PType = 0;
        objs[o].Teleportal = 0;
        objs[o].PNumber = 0;
        objs[o].framerate = 1;
        objs[o].framecounter = 0;
        e = o;
        ens[e].keio = 0;
        ens[e].type = 0;
        ens[e].shootimgx = 94;
        ens[e].shootimgy = 80;
        ens[e].shootimgwidth = 15;
        ens[e].shootimgheight = 15;
        ens[e].width = 16;
        ens[e].height = 24;
        ens[e].sprite = 0;
        ens[e].xspeed = -4;
        ens[e].yspeed = 0;
        ens[e].direction = 0;
        ens[e].counter = 8;
        ens[e].collisionshrink = 1;
        ens[e].fb = 0;
        ens[e].forceposy = 0;
        ens[e].prev = 0;
        ens[e].ospx = 0;
        ens[e].ospy = 0;
        ens[e].ospxm = 0;
        ens[e].lunge = 0;
        ens[e].ospym = 0;
        ens[e].health = 2+randomz(2,0);
        ens[e].speedbase = 2+randomz(3,0);
        ens[e].shootx = 0;
        ens[e].shooty = 0;
        ens[e].shootset = 0;
        ens[e].deathset = 0;
        ens[e].hitset = 0;
        ens[e].boss = 0;
        ens[e].cd = 0;
    }
    e = 0;
    o = 0;
    z = 0;
    while(1)
    {
        if(level[point] == ')')
        {
            ens[e].boss = 1; /*Bosses are akllowed to fall!*/
            ens[e].ospx = 0;
            ens[e].ospy = 0;
            ens[e].ospxm = 0;
            ens[e].ospym = 160;
            ens[e].width = 160;
            ens[e].height = 160;
            ens[e].keio = 1;
            ens[e].shootimgx = 176;
            ens[e].shootimgy = 0;
            ens[e].shootimgwidth = 48;
            ens[e].shootimgheight = 48;
            ens[e].speedbase = 3+randomz(3,0);
            ens[e].xspeed = 5;
            ens[e].type = 3;
            ens[e].collisionshrink = 18;
            ens[e].health += 52;
//            ens[e].shootimgx = 64;
//            ens[e].shootimgy = 64;            
            e++;
            soundtoplay = 5;
        }
        if(level[point] == 'Q')
        {NumofLines = line+1; 
            for(z = 0; z < 20; z++)
            {
                if(teleports[z] != -1)
                {
                    num[0] = level[point+1];
                    num[1] = level[point+2];            
                    num[2] = level[point+3];
                    num[3] = level[point+4];            
                    num[4] = level[point+5];
                    num[5] = 0x00;            
                    objs[teleports[z]].tx = atoi(num);
                    num[0] = level[point+6];
                    num[1] = level[point+7];            
                    num[2] = level[point+8];
                    num[3] = level[point+9];            
                    num[4] = level[point+10];
                    num[5] = 0x00;            
                    objs[teleports[z]].ty = atoi(num);                                        
                    point += 10;
                }  
                
            }
            MessageBox(NULL, &level[point+1], "Level", MB_OK|MB_TASKMODAL);
            break;}
        if(level[point] == 10)
        {
            posx = -24;
            posy += 24;
            line++;
        }
        objs[o].posx = posx;
        objs[o].posy = posy;
        ens[e].posx = posx;
        ens[e].posy = posy;

        if(level[point] == '9')
        {
            objs[o].keio = 1;
            objs[o].tilex = 288;
            objs[o].tiley = 216;
            objs[o].tilemaskx = 312;
            objs[o].tilemasky = 216;
            objs[o].solid = 0;
            objs[o].OneUp = 1;
            o++;
        }
        if(level[point] == '8')
        {
            objs[o].keio = 1;
            objs[o].tilex = 264;
            objs[o].tiley = 240;
            objs[o].tilemaskx = 264+24;
            objs[o].tilemasky = 240;
            objs[o].donly = 1;
            objs[o].healthpickup = 0;
            objs[o].Teleportal = 1;
            objs[o].tx = 0;
            objs[o].ty = 0;
            teleports[z] = o;
            z++;
            o++;
                        
        }
        if(level[point] == 'h')
        {
            objs[o].keio = 1;
            objs[o].tilex = 0;
            objs[o].tiley = 240;
            objs[o].tilemaskx = 24;
            objs[o].tilemasky = 240;
            objs[o].solid = 0;
            objs[o].healthpickup = 1;
            o++;
                
        }
        if(level[point] == 'r')
        {
            objs[o].keio = 1;
            objs[o].tilex = 120;
            objs[o].tiley = 192;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 192;
            objs[o].climber = 1;
            
            o++;
        }
        if(level[point] == 'o')
        {
            objs[o].keio = 1;
            objs[o].tilex = 120;
            objs[o].tiley = 216;
            objs[o].tilemaskx = 72;
            objs[o].tilemasky = 168;
            objs[o].ani=1;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            o++;
        }          
        if(level[point] == 'O')
        {
            objs[o].keio = 1;
            objs[o].tilex = 192;
            objs[o].tiley = 192;
            objs[o].tilemaskx = 192;
            objs[o].tilemasky = 168;
            objs[o].ani=1;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            o++;
        }          
            
        if(level[point] == 'U')
        {
            objs[o].keio = 1;
            objs[o].tilex = 120;
            objs[o].tiley = 240;
            objs[o].tilemaskx = 72;
            objs[o].tilemasky = 192;
            objs[o].ani=1;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 0;
            objs[o].jumper = 1;             
            o++;             
                
        }
        if(level[point] == 'c')
        {
            objs[o].keio = 1;
            objs[o].tilex = 168;
            objs[o].tiley = 192;
            objs[o].tilemaskx = 168;
            objs[o].tilemasky = 168;
            objs[o].ani=0;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 1;
            objs[o].jumper = 0;             
            o++;             
                
        }
        if(level[point] == 'g')
        {
            objs[o].keio = 1;
            objs[o].tilex = 168;
            objs[o].tiley = 240;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 0;
            objs[o].ghostt = 1;
            
            o++;
        }
        if(level[point] == 'C')
        {
            objs[o].keio = 1;
            objs[o].tilex = 240;
            objs[o].tiley = 192;
            objs[o].tilemaskx = 240;
            objs[o].tilemasky = 168;
            objs[o].ani=0;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 1;
            objs[o].donly = 1;             
            o++;             
                
        }      
        if(level[point] == 'B')
        {
            objs[o].keio = 1;
            objs[o].tilex = 168;
            objs[o].tiley = 216;
            objs[o].tilemaskx = 192;
            objs[o].tilemasky = 0;
            objs[o].ani=0;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 1;
            objs[o].jumper = 0;             
            o++;             
                
        }
        if(level[point] == 'e')
        {
            objs[o].keio = 1;
            objs[o].tilex = 216;
            objs[o].tiley = 240;
            objs[o].tilemaskx = 216;
            objs[o].tilemasky = 216;
            objs[o].ani=1;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 0;
            objs[o].kill = 1;             
            o++;             
                
        }
        if(level[point] == 'q')
        {
            objs[o].keio = 1;
            objs[o].tilex = 24;
            objs[o].tiley = 168;
            objs[o].tilemaskx = 24;
            objs[o].tilemasky = 192;
            objs[o].ani=1;
            objs[o].numframes=2;
            objs[o].aniframe=randomz(2,0);
            objs[o].solid = 0;
            objs[o].kill = 1;             
            o++;             
                
        }
            
        if(level[point] == '-')
        {
            objs[o].keio = 1;
            objs[o].tilex = 0;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 0;
            objs[o].tilemasky = 24;
            objs[o].climber = 1;
            
            if(level[0] == '1')
            {
                objs[o].tilex = 24;
                objs[o].tiley =  48;        
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 288;
                objs[o].tiley =  24;        
            }
                
            if(level[0] == '3')
            {
                objs[o].tilex = 240;
                objs[o].tiley = 120;
                objs[o].tilemaskx = 240;
                objs[o].tilemasky = 144;
            }
                
            o++;
        }
        if(level[point] == '/')
        {
            objs[o].keio = 1;
            objs[o].tilex = 24;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 96;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == '_')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 120;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == 'a')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 264;
            objs[o].tilemasky = 120;
            
            o++;
        }
        if(level[point] == 'A')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 264;
            objs[o].tilemasky = 144;
            
            o++;
        }
        if(level[point] == 't')
        {
            objs[o].keio = 1;
            objs[o].tilex = 96;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 288;
            objs[o].tilemasky = 120;
            
            o++;
        }
        if(level[point] == 'T')
        {
            objs[o].keio = 1;
            objs[o].tilex = 96;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 288;
            objs[o].tilemasky = 144;
            
            o++;
        }
        if(level[point] == 'y')
        {
            objs[o].keio = 1;
            objs[o].tilex = 120;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 312;
            objs[o].tilemasky = 120;
            
            o++;
        }
        if(level[point] == 'Y')
        {
            objs[o].keio = 1;
            objs[o].tilex = 120;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 312;
            objs[o].tilemasky = 144;
            
            o++;
        }
            
        if(level[point] == 'i')
        {
            objs[o].keio = 1;
            objs[o].tilex = 144;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 336;
            objs[o].tilemasky = 120;
            
            o++;
        }
        if(level[point] == 'I')
        {
            objs[o].keio = 1;
            objs[o].tilex = 144;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 336;
            objs[o].tilemasky = 144;
            
            o++;
        }
        if(level[point] == 'j')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 192;
            objs[o].tilemasky = 0;
            
            o++;
        }
        if(level[point] == 'J')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 192;
            objs[o].tilemasky = 0;
            
            o++;
        }
            
        if(level[point] == '\\')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == 'L')
        {
            objs[o].keio = 1;
            objs[o].tilex = 24;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 96;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if(level[point] == 'k')
        {
            objs[o].keio = 1;
            objs[o].tilex = 192;
            objs[o].tiley = 144;
            objs[o].tilemaskx = 192-24;
            objs[o].tilemasky = 144;
            objs[o].kill = 1;
            objs[o].solid = 0;
            if(level[0] == '3')
            {
                objs[o].keio = 1;
                objs[o].tilex = 264;
                objs[o].tiley = 48;
                objs[o].tilemaskx = 264+24;
                objs[o].tilemasky = 48;
                objs[o].kill = 1;
                objs[o].solid = 0;    
                objs[o].Particles = 1;
                objs[o].PNumber = 7;
                for(i = 0; i < objs[o].PNumber; i++)
                {
                    objs[o].ParticleData[i*5] = randomz(objs[o].width,0)+randomz(objs[o].width,0)+randomz(objs[o].width/4,0);   /*x and y relative positions of particles*/
                    objs[o].ParticleData[(i*5)+1] = randomz(objs[o].height,0)+randomz(objs[o].height,0)+randomz(objs[o].height/4,0);   
                    if(randomz(1,0)+randomz(1,0) == 1 || randomz(1,0)+randomz(1,0) == 2)
                    {
                        objs[o].ParticleData[(i*5)+2] = 255*randomz(2,0);
                        objs[o].ParticleData[(i*5)+3] = 255;
                        objs[o].ParticleData[(i*5)+4] = 255;                                                      
                    }
                    else
                    {
                        objs[o].ParticleData[(i*5)+2] = 255;
                        objs[o].ParticleData[(i*5)+3] = 255;
                        objs[o].ParticleData[(i*5)+4] = 0;                                                                    
                    }
                }
            }
            o++;
        }
        
        if(level[point] == '=')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 120;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if(level[point] == 'V')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if(level[point] == 'G')
        {
            objs[o].keio = 1;
            objs[o].tilex = 168;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 0;
            objs[o].ghostt = 1;
            
            if(level[0] == '1')
            {
                objs[o].tilex = 144;
                objs[o].tiley = 48;         
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 312;
                objs[o].tiley = 24;         
            }
                
            if(level[0] == '3')
            {
                objs[o].tilex = 264;
                objs[o].tiley = 72;         
                objs[o].tilemaskx = 288;
                objs[o].tilemasky = 72;
            }
                
            o++;
        }
        if(level[point] == 'v')
        {
            objs[o].keio = 1;
            objs[o].tilex = 144;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 24;
            objs[o].solid = 0;
            objs[o].kill = 1;
            if(level[0] == '1')
            {
                objs[o].tilex = 216;
                objs[o].tiley = 120;         
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 336;
                objs[o].tiley =  72;       
                objs[o].tilemaskx = 264;
                objs[o].tilemasky = 192+24;
                
            }     
/*            if(level[0] == '3')
            {
                objs[o].tilex = 192;
                objs[o].tiley =  0;       
                objs[o].tilemaskx = 312;
                objs[o].tilemasky = 240;
                
            }   */  
            o++;
        }
        if(level[point] == 'z')
        {
            objs[o].keio = 1;
            objs[o].tilex = 0;
            objs[o].tiley = 120;
            objs[o].tilemaskx = 0;
            objs[o].tilemasky = 144;
            objs[o].solid = 0;
            objs[o].kill = 1;
            if(level[0] == '1')
            {
                objs[o].tilex = 0;
                objs[o].tiley = 96;         
            }
            if(level[0] == '4')
            {
                objs[o].keio = 1;
                objs[o].tilex = 288;
                objs[o].tiley = 96;
                objs[o].tilemaskx = 264;
                objs[o].tilemasky = 168;
                objs[o].ani=1;
                objs[o].numframes=2;
                objs[o].aniframe=randomz(2,0);
                objs[o].framerate = 6;
                objs[o].framecounter = randomz(5,0) + randomz(5,0);
                objs[o].solid = 0;
                objs[o].kill = 1;             
            }
            o++;
        }
        if(level[point] == 'x')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 48;
            objs[o].tilemaskx = 72;
            objs[o].tilemasky = 48;
            objs[o].solid = 0;
            objs[o].jumper = 1;
            if(level[0] == '1')
            {
                objs[o].tilex = 96;
                objs[o].tiley = 48;         
            }
                
            if(level[0] == '3')
            {
                objs[o].tilex = 216;
                objs[o].tiley = 144;         
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 312;
                objs[o].tiley = 168;                                 
            }    
            o++;
        }
        if(level[point] == 'p')
        {
            objs[o].keio = 1;
            objs[o].tilex = 216;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 240;
            objs[o].tilemasky = 0;
            objs[o].solid = 1;
            objs[o].donly = 1;
            
            if(level[0] == '1')
            {
                objs[o].tilex = 240;
                objs[o].tiley = 24;         
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 288;
                objs[o].tiley = 0;         
                objs[o].tilemaskx = 240;
                objs[o].tilemasky = 168;                
            }
                
            o++;
        }
            
        if(level[point] == '+')
        {
            objs[o].keio = 1;
            objs[o].tilex = 96;
            objs[o].tiley = 0;
            objs[o].tilemaskx = 96+24;
            objs[o].tilemasky = 0;
            
                
            if(level[0] == '1')
            {
                objs[o].tilex = 240;
                objs[o].tiley =  72;        
            }
            o++;
        }
            
        if(level[point] == '^')
        {
            objs[o].keio = 1;
            objs[o].tilex = 0;
            objs[o].tiley = 48;
            objs[o].tilemaskx = 0;
            objs[o].tilemasky = 48+24;
            objs[o].solid = 0;
            objs[o].kill = 1;
            if(level[0] == '1')
            {
                objs[o].tilex = 216;
                objs[o].tiley =  96;       
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 336;
                objs[o].tiley =  48;       
                objs[o].tilemaskx = 264;
                objs[o].tilemasky = 192;
                
            }
            o++;
        }
        if(level[point] == 'R')
        {
            objs[o].keio = 1;
            objs[o].tilex = 168;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 24;
            objs[o].ghostt = 1;
            objs[o].ghost = 1;
            objs[o].solid = 0;
            if(level[0] == '1')
            {
                objs[o].tilex = 168;
                objs[o].tiley = 48;         
            }
                
            if(level[0] == '3')
            {
                objs[o].tilex = 312;
                objs[o].tiley = 72;         
                objs[o].tilemaskx = 312;
                objs[o].tilemasky = 48;
            }
                
            o++;
        }
        if(level[point] == 'E')
        {
            objs[o].keio = 1;
            objs[o].tilex = 96;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 96+24;
            objs[o].tilemasky = 24;
            objs[o].solid = 0;
            objs[o].exit = 1;
            if(level[0] == '1')
            {
                objs[o].tilex = 168;
                objs[o].tiley =  72;        
            }
            if(level[0] == '3')
            {
                objs[o].tilex = 264;
                objs[o].tiley =  24;       
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 336;
                objs[o].tiley = 24;        
            }    
            o++;
        }
        if(level[point] == 'M')
        {
            ens[e].keio = 1;
            ens[e].ospx = 64;
            ens[e].ospy = 0;
            ens[e].type = 1;
            ens[e].ospxm = 64;
            ens[e].ospym = 40;
            ens[e].xspeed = 0;
            ens[e].width = 24;
            ens[e].height = 40;
            ens[e].collisionshrink = 2;
            e++;
        }
        if(level[point] == '~')
        {
            ens[e].keio = 1;
            ens[e].ospx = 109;
            ens[e].ospy = 128;
            ens[e].type = 1;
            ens[e].ospxm = 109;
            ens[e].ospym = 128+24;
            ens[e].xspeed = 0;
            ens[e].width = 16;
            ens[e].height = 24;
            ens[e].shootimgx = 30;
            ens[e].shootimgy = 144;
            ens[e].collisionshrink = 2;
            ens[e].shootimgwidth = 15;
            ens[e].shootimgheight = 15;
            ens[e].health = 1 + randomz(3,0);

            e++;
        }
        if(level[point] == '*')
        {
            ens[e].keio = 1;
            ens[e].ospx = 0;
            ens[e].ospy = 48;
            ens[e].collisionshrink = 2;
            ens[e].ospxm = 0;
            ens[e].ospym = 72;
            ens[e].health += (1 + randomz(2,0));
            e++;
        }
        if(level[point] == '!')
        {
            ens[e].width = 24;
            ens[e].height = 24;
            ens[e].keio = 1;
            ens[e].type = 3;
            ens[e].ospx = 0;
            ens[e].ospy = 96;
            ens[e].ospxm = 0;
            ens[e].ospym = 96+24;
            ens[e].speedbase += 1;
            ens[e].shootimgx = 0;
            ens[e].shootimgy = 144;
            ens[e].shootimgwidth = 15;
            ens[e].shootimgheight = 15;
            ens[e].health += 4+randomz(3,0);
            e++;
        }
        if(level[point] == '#')
        {
            ens[e].width = 16;
            ens[e].height = 16;
            ens[e].keio = 1;
            ens[e].type = 0;
            ens[e].ospx = 109;
            ens[e].ospy = 96;
            ens[e].ospxm = 109;
            ens[e].ospym = 96+16;
            ens[e].speedbase = 3+randomz(5,0);
            ens[e].xspeed = 5;
            ens[e].health = 1+randomz(2,0);
            e++;
        }
        
        if(level[point] == 'S')
        {
            *startx = posx;
            *starty = posy;
        }
        if(level[point] == 'H')
        {
            objs[o].keio = 1;
            objs[o].tilex = 216;
            objs[o].tiley = 24;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 0;
            objs[o].ghostt = 1;
            
            o++;
        }
        if(level[point] == 'W')
        {
            objs[o].keio = 1;
            objs[o].tilex = 192;
            objs[o].tiley = 48;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 0;
            
            if(level[0] == '1')
            {
                objs[o].tilex = 120;
                objs[o].tiley =  48;        
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 312;
                objs[o].tiley =  0;        
            }
            if(level[0] == '3')
            {
                objs[o].tilex = 240;
                objs[o].tiley =  96;        
                objs[o].tilemaskx = 264;
                objs[o].tilemasky = 96;           
            }
                
            o++;
        }
        if(level[point] == 'w')
        {
            objs[o].keio = 1;
            objs[o].tilex = 192+48;
            objs[o].tiley = 48;
            objs[o].tilemaskx = 168+24;
            objs[o].tilemasky = 0;
            
            if(level[0] == '1')
            {
                objs[o].tilex = 192;
                objs[o].tiley = 120;         
            }
            if(level[0] == '4')
            {
                objs[o].tilex = 336;
                objs[o].tiley =  0;        
            }

            o++;
        }
        if(level[point] == '<')
        {
            objs[o].keio = 1;
            objs[o].tilex = 216;
            objs[o].tiley = 48;
            objs[o].tilemaskx = 216;
            objs[o].tilemasky = 72;
            objs[o].solid = 0;
            if(level[0] == '1')
            {
                objs[o].tilex = 168;
                objs[o].tiley = 120;         
            }
            o++;
        }
        if(level[point] == '>')
        {
            objs[o].keio = 1;
            objs[o].tilex = 192;
            objs[o].tiley = 72;
            objs[o].tilemaskx = 192;
            objs[o].tilemasky = 96;
            objs[o].solid = 0;
            if(level[0] == '1')
            {
                objs[o].tilex = 168;
                objs[o].tiley =  96;        
            }
            o++;
        }
            
        if(level[point] == '1')
        {
            objs[o].keio = 1;
            objs[o].tilex = 24;
            objs[o].tiley = 72;
            objs[o].tilemaskx = 96;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == '2')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 72;
            objs[o].tilemaskx = 120;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == '3')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 72;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 72;
            
            o++;
        }
        if(level[point] == '4')
        {
            objs[o].keio = 1;
            objs[o].tilex = 24;
            objs[o].tiley = 24+72;
            objs[o].tilemaskx = 96;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if(level[point] == '5')
        {
            objs[o].keio = 1;
            objs[o].tilex = 48;
            objs[o].tiley = 24+72;
            objs[o].tilemaskx = 120;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if(level[point] == '6')
        {
            objs[o].keio = 1;
            objs[o].tilex = 72;
            objs[o].tiley = 24+72;
            objs[o].tilemaskx = 144;
            objs[o].tilemasky = 96;
            
            o++;
        }
        if( o > MaxObjects - 1)
        {
            o = MaxObjects-1;
        }    
        if( e > MaxObjects - 1)
        {
            e = MaxObjects-1;
        }    
        posx += 24;
        point++;
        col++;
    }
    //    CreateStoppers();  Let's try another way of making the stoppers!
        
        
    for(q = 0; q < MaxObjects; q++)
    {
        objused[q] = 0;            
    }
    rects = 0;
    for(q = 0; q < MaxObjects; q++)
    {
        for(z = 0; z < MaxObjects; z++)
        {
            if(z != q)
            {
                if( objs[q].climber == objs[z].climber && objs[q].solid == objs[z].solid && objs[q].solid == 1&& objs[q].ghostt == objs[z].ghostt && objs[q].donly == 0&&objs[q].donly == objs[z].donly && objs[q].keio == 1 && objs[z].keio == 1 && objs[q].width == objs[z].width && objs[q].posx == objs[z].posx && ( (objs[q].posy+objs[q].height)==objs[z].posy || (objs[z].posy+objs[z].height)==objs[q].posy) )
                {
                    if(objused[q] == 0 && objused[z] == 0)
                    {
                        objused[q] = objused[z] = 1;
                        StopperRect[rects].top = objs[q].posy < objs[z].posy ? objs[q].posy : objs[z].posy;
                        StopperRectType[rects] = objs[q].ghostt * 2 + objs[z].donly + (objs[z].climber*4);
                        StopperRect[rects].left = objs[q].posx;
                        StopperRect[rects].bottom = objs[q].posy < objs[z].posy ? objs[z].posy+objs[z].height : objs[q].posy+objs[q].height;
                        StopperRect[rects].right = objs[q].posx+objs[q].width;
                        rects++;                              
                    }                      
                }
            }
        }            
        if(objused[q] != 1 && objs[q].solid == 1 && objs[q].keio == 1)
        { 
            StopperRect[rects].top = objs[q].posy;
            StopperRect[rects].bottom = objs[q].posy + objs[q].height;
            StopperRect[rects].left = objs[q].posx;
            StopperRect[rects].right = objs[q].posx + objs[q].width;
            StopperRectType[rects] = objs[q].ghostt * 2 + objs[q].donly + (objs[q].climber*4);
            objused[q] = 1;
            rects++;
        }
    }
        
        
    j = 0;
    for(e = 0; e < 6; e++)
    {
        j = 0;
        for(q = 0; q < MaxObjects; q++)
        {
            objused[q] = 0;            
        }            
        /*Consolodate the rects somewhat, bah, somewhat?  How about: ALOT*/
            
        for(q = 0; q < rects; q++)
        {
            for(z = 0; z < rects; z++)
            {
                if(z != q)
                {
                    if( StopperRect[q].left == StopperRect[z].left && StopperRect[q].right == StopperRect[z].right && (StopperRect[q].top == StopperRect[z].bottom || StopperRect[q].bottom == StopperRect[z].top) && StopperRectType[q] == StopperRectType[z] && (StopperRectType[q]&1) != 1 )
                    {
                        if(objused[q] == 0 && objused[z] == 0)
                        {
                            objused[q] = objused[z] = 1;
                            RectStopper[j].top = StopperRect[q].top < StopperRect[z].top ? StopperRect[q].top : StopperRect[z].top;
                            RectStopperType[j] = StopperRectType[q];
                            RectStopper[j].left = StopperRect[q].left;
                            RectStopper[j].bottom = StopperRect[q].top < StopperRect[z].top ? StopperRect[z].bottom : StopperRect[q].bottom;
                            RectStopper[j].right = StopperRect[q].right;
                            j++;                              
                        }                      
                    }
                }
            }            
            if(objused[q] != 1)
            { 
                RectStopper[j].top = StopperRect[q].top;
                RectStopper[j].bottom = StopperRect[q].bottom;
                RectStopper[j].left = StopperRect[q].left;
                RectStopper[j].right = StopperRect[q].right;
                RectStopperType[j] = StopperRectType[q];
                objused[q] = 1;
                j++;
            }
        }      
        memcpy(StopperRect, RectStopper, sizeof(int)*4*MaxObjects);
        memcpy(StopperRectType, RectStopperType, sizeof(int)*MaxObjects);
        rects = j;
    }
   /*HORIZONTAL CONSOLIDTAION*/     



        j = 0;
        for(q = 0; q < MaxObjects; q++)
        {
            objused[q] = 0;            
        }            
        /*Consolodate the rects somewhat, bah, somewhat?  How about: ALOT*/
            
        for(q = 0; q < rects; q++)
        {
            for(z = 0; z < rects; z++)
            {
                if(z != q)
                {
                    if( StopperRect[q].bottom == StopperRect[z].bottom && StopperRect[q].top == StopperRect[z].top && (StopperRect[q].left == StopperRect[z].right || StopperRect[q].right == StopperRect[z].left) && StopperRectType[q] == StopperRectType[z] && (StopperRectType[q]&1) != 1 )
                    {
                        if(objused[q] == 0 && objused[z] == 0)
                        {
                            objused[q] = objused[z] = 1;
                            RectStopper[j].left = StopperRect[q].left < StopperRect[z].left ? StopperRect[q].left : StopperRect[z].left;
                            RectStopperType[j] = StopperRectType[q];
                            RectStopper[j].top = StopperRect[q].top;
                            RectStopper[j].right = StopperRect[q].left < StopperRect[z].left ? StopperRect[z].right : StopperRect[q].right;
                            RectStopper[j].bottom = StopperRect[q].bottom;
                            j++;                              
                        }                      
                    }
                }
            }            
            if(objused[q] != 1)
            { 
                RectStopper[j].top = StopperRect[q].top;
                RectStopper[j].bottom = StopperRect[q].bottom;
                RectStopper[j].left = StopperRect[q].left;
                RectStopper[j].right = StopperRect[q].right;
                RectStopperType[j] = StopperRectType[q];
                objused[q] = 1;
                j++;
            }
        }      
        memcpy(StopperRect, RectStopper, sizeof(int)*4*MaxObjects);
        memcpy(StopperRectType, RectStopperType, sizeof(int)*MaxObjects);
        rects = j;

/*MORE SO*/
        j = 0;
        for(q = 0; q < MaxObjects; q++)
        {
            objused[q] = 0;            
        }            
        /*Consolodate the rects somewhat, bah, somewhat?  How about: ALOT*/
            
        for(q = 0; q < rects; q++)
        {
            for(z = 0; z < rects; z++)
            {
                if(z != q)
                {
                    if( StopperRect[q].bottom == StopperRect[z].bottom && StopperRect[q].top == StopperRect[z].top && (StopperRect[q].left == StopperRect[z].right || StopperRect[q].right == StopperRect[z].left) && StopperRectType[q] == StopperRectType[z] && (StopperRectType[q]&1) != 1 )
                    {
                        if(objused[q] == 0 && objused[z] == 0)
                        {
                            objused[q] = objused[z] = 1;
                            RectStopper[j].left = StopperRect[q].left < StopperRect[z].left ? StopperRect[q].left : StopperRect[z].left;
                            RectStopperType[j] = StopperRectType[q];
                            RectStopper[j].top = StopperRect[q].top;
                            RectStopper[j].right = StopperRect[q].left < StopperRect[z].left ? StopperRect[z].right : StopperRect[q].right;
                            RectStopper[j].bottom = StopperRect[q].bottom;
                            j++;                              
                        }                      
                    }
                }
            }            
            if(objused[q] != 1)
            { 
                RectStopper[j].top = StopperRect[q].top;
                RectStopper[j].bottom = StopperRect[q].bottom;
                RectStopper[j].left = StopperRect[q].left;
                RectStopper[j].right = StopperRect[q].right;
                RectStopperType[j] = StopperRectType[q];
                objused[q] = 1;
                j++;
            }
        }      
        memcpy(StopperRect, RectStopper, sizeof(int)*4*MaxObjects);
        memcpy(StopperRectType, RectStopperType, sizeof(int)*MaxObjects);
        rects = j;
/*One more for good measure*/
        j = 0;
        for(q = 0; q < MaxObjects; q++)
        {
            objused[q] = 0;            
        }            
        /*Consolodate the rects somewhat, bah, somewhat?  How about: ALOT*/
            
        for(q = 0; q < rects; q++)
        {
            for(z = 0; z < rects; z++)
            {
                if(z != q)
                {
                    if( StopperRect[q].bottom == StopperRect[z].bottom && StopperRect[q].top == StopperRect[z].top && (StopperRect[q].left == StopperRect[z].right || StopperRect[q].right == StopperRect[z].left) && StopperRectType[q] == StopperRectType[z] && (StopperRectType[q]&1) != 1 )
                    {
                        if(objused[q] == 0 && objused[z] == 0)
                        {
                            objused[q] = objused[z] = 1;
                            RectStopper[j].left = StopperRect[q].left < StopperRect[z].left ? StopperRect[q].left : StopperRect[z].left;
                            RectStopperType[j] = StopperRectType[q];
                            RectStopper[j].top = StopperRect[q].top;
                            RectStopper[j].right = StopperRect[q].left < StopperRect[z].left ? StopperRect[z].right : StopperRect[q].right;
                            RectStopper[j].bottom = StopperRect[q].bottom;
                            j++;                              
                        }                      
                    }
                }
            }            
            if(objused[q] != 1)
            { 
                RectStopper[j].top = StopperRect[q].top;
                RectStopper[j].bottom = StopperRect[q].bottom;
                RectStopper[j].left = StopperRect[q].left;
                RectStopper[j].right = StopperRect[q].right;
                RectStopperType[j] = StopperRectType[q];
                objused[q] = 1;
                j++;
            }
        }      
        memcpy(StopperRect, RectStopper, sizeof(int)*4*MaxObjects);
        memcpy(StopperRectType, RectStopperType, sizeof(int)*MaxObjects);
        rects = j;
/*Oh, and another*/
        j = 0;
        for(q = 0; q < MaxObjects; q++)
        {
            objused[q] = 0;            
        }            
        /*Consolodate the rects somewhat, bah, somewhat?  How about: ALOT*/
            
        for(q = 0; q < rects; q++)
        {
            for(z = 0; z < rects; z++)
            {
                if(z != q)
                {
                    if( StopperRect[q].bottom == StopperRect[z].bottom && StopperRect[q].top == StopperRect[z].top && (StopperRect[q].left == StopperRect[z].right || StopperRect[q].right == StopperRect[z].left) && StopperRectType[q] == StopperRectType[z] && (StopperRectType[q]&1) != 1 )
                    {
                        if(objused[q] == 0 && objused[z] == 0)
                        {
                            objused[q] = objused[z] = 1;
                            RectStopper[j].left = StopperRect[q].left < StopperRect[z].left ? StopperRect[q].left : StopperRect[z].left;
                            RectStopperType[j] = StopperRectType[q];
                            RectStopper[j].top = StopperRect[q].top;
                            RectStopper[j].right = StopperRect[q].left < StopperRect[z].left ? StopperRect[z].right : StopperRect[q].right;
                            RectStopper[j].bottom = StopperRect[q].bottom;
                            j++;                              
                        }                      
                    }
                }
            }            
            if(objused[q] != 1)
            { 
                RectStopper[j].top = StopperRect[q].top;
                RectStopper[j].bottom = StopperRect[q].bottom;
                RectStopper[j].left = StopperRect[q].left;
                RectStopper[j].right = StopperRect[q].right;
                RectStopperType[j] = StopperRectType[q];
                objused[q] = 1;
                j++;
            }
        }      
        memcpy(StopperRect, RectStopper, sizeof(int)*4*MaxObjects);
        memcpy(StopperRectType, RectStopperType, sizeof(int)*MaxObjects);
        rects = j;
        
}
BYTE DetectCollision(int left1, int top1, int right1, int bottom1, int left2, int top2, int right2, int bottom2, int xspeed1, int yspeed1, int xspeed2, int yspeed2, int projectile, int ignoreleft,int ignoretop, int ignorebottom, int ignoreright)
{
    RECT object1, object2;
    RECT intersection;
    int i;
    int shiftx1=0, shifty1=0, shiftx2=0, shifty2=0;
    int sx1, sy1, sx2, sy2, retx=0, rety=0;
    int prec=0;
        
    object1.top = top1*10;
    object1.left = left1*10;
    object1.right = right1*10;
    object1.bottom = bottom1*10;
        
    object2.top = top2*10;
    object2.left = left2*10;
    object2.right = right2*10;
    object2.bottom = bottom2*10;  /*Make them bigger for easier analysis*/
        
        
    if( IntersectRect(&intersection,&object1,&object2) != 0 && projectile == 1)
    {return 32;}
        
        
    sx1 = xspeed1*10;
    sy1 = yspeed1*10;
    sx2 = xspeed2*10;
    sy2 = yspeed2*10;
    shiftx1 = sx1 / 10;
    shifty1 = sy1 / 10;
    shiftx2 = sx2 / 10;
    shifty2 = sy2 / 10;
        
    //  if( (yspeed1 + yspeed2) == -3 )
    //  {MessageBox(NULL,"-3","-3",MB_OK|MB_TASKMODAL);}
        
    /* object1.top += shifty1*10;
       object1.bottom += shifty1*10;
       object2.top += shifty2*10;
       object2.bottom += shifty2*10;
       
      if((shifty1+shifty2) > 20 && (shifty1+shifty2) < -20)
      {
      
      if(IntersectRect(&intersection, &object1, &object2) != 0)
      {
      if(ignoretop != 1)
      {
      if(shifty1 > 0)
      {rety = 2;} 
      }
      if(ignorebottom != 1)
      {
      if(shifty1 < 0)
      {rety = 1;} 
      }
      }

      
       object1.left += shiftx1*10;
       object1.right += shiftx1*10;
       object2.left += shiftx2*10;
       object2.right += shiftx2*10;
       //object1 inside object2
      if(IntersectRect(&intersection, &object1, &object2) != 0)
      {
      if(ignoreright != 1)
      {
      if(shiftx1 > 0)
      {retx = 8;} 
      }
      if(ignoreleft != 1)
      {
      if(shiftx1 < 0)
      {retx = 4;} 
      }
      }
      
      
      
      if(prec != 1)
      {
      if(rety != 0)
      {return rety;}
      if(retx != 0)
      {return retx;}
      }
      else
      {

      if(rety != 0)
      {return rety;}        
      
      if(retx != 0)
      {return retx;}
      }
       object1.left -= shiftx1*10;
       object1.right -= shiftx1*10;
       object2.left -= shiftx2*10;
       object2.right -= shiftx2*10;

      }
       object1.top -= shifty1*10;
       object1.bottom -= shifty1*10;
       object2.top -= shifty2*10;
       object2.bottom -= shifty2*10;*/
        
        
        
    for(i = 0; i < 10; i++)
    {
        //  OffsetRect(&object1, 0, shifty1);
        //  OffsetRect(&object2, 0, shifty2);
        object1.top += shifty1;
        object1.bottom += shifty1;
        object2.top += shifty2;
        object2.bottom += shifty2;
            
            
        if(IntersectRect(&intersection, &object1, &object2) != 0)
        {
            if(ignoretop != 1)
            {
                if(shifty1 > 0)
                {rety = 2;} /*Top side of Object2*/
            }
            if(ignorebottom != 1)
            {
                if(shifty1 < 0)
                {rety = 1;} /*Bottom s of obj 2*/
            }
        }
            
        object1.left += shiftx1;
        object1.right += shiftx1;
        object2.left += shiftx2;
        object2.right += shiftx2;
        //object1 inside object2
        if(IntersectRect(&intersection, &object1, &object2) != 0)
        {
            if(ignoreright != 1)
            {
                if(shiftx1 > 0)
                {retx = 8;} /*Left side of Object2*/
            }
            if(ignoreleft != 1)
            {
                if(shiftx1 < 0)
                {retx = 4;} /*Right s of obj 2*/
            }
        }
            
            
            
        if(prec != 1)
        {
            if(rety != 0)
            {return rety;}
            if(retx != 0)
            {return retx;}
        }
        else
        {
                
            if(rety != 0)
            {return rety;}        
                
            if(retx != 0)
            {return retx;}
                
        }
    }
        
    return 0;
}  /* Return fields: a0p0lrtb a = Do not move l = collision on left of object2, r col. on right, t col. on top, etc. p = Projectile collision*/

LRESULT CALLBACK
MainWndProc (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
        
    static int    cx, cy;          /* Height and width of our button. */
        
    HDC           hdc, mem, mem2;             /* A device context used for drawing */
    PAINTSTRUCT   ps;              /* Also used during window drawing */
    static BYTE Invincible = 0, Ulives = 0, Spidey = 0, Dkey = 0;
    static ObjectStruct objects[MaxObjects];
    int q, bosshere;
    static EnemyStruct enemies[MaxObjects];
    static RECT          rc;              /* A rectangle used during drawing */
    static int flower = 0, wallage = 0;    
    static unsigned char doubleswitch = 0;
    static unsigned char PassWord2[800];
    static int blastx[3], blasty[3], blastset[3], paulax=6, paulay=6;
    static BYTE paulastatus=1, paulaspeed=0, pauladirection=1, blastdirection[3], blaststatus[3];
    static int fire=0, jump=0, vertpaula=0, i, x;
    static int jumptimer = 0;   /*<-  Controllable jumpage for mushroom*/
    static int scrx = 0, scry = 0, scrsize=1680, psx=0, psy=0;
    static RECT prect = {0,0,16,24}; /*Defines image locations and sizes for Paula*/
    static BYTE cact, cactus, psxset=0, psyset=0;
    static BYTE GhostStatus= 0, ghostpause=0, sprite = 0;
    static BYTE Health = 5, Healthmax = 5, Livesmax = 12, Lives = 12, hitOK = 0, bcol =0, ShotOK=1;
    static BYTE SoundStatus = 0, SoundPause = 0, HelpPause = 0;
    static BYTE Levelnum = 0, t, twait = 10;
    static BYTE GameStatus = GAME_MENU;
    static char buf[500];
    static char thestory[1600] = "The Story thus-far: An evil zombie-spirit has transported Paula to a strange zombie-infested world.  However, the Great Pumpkin has come to her aid by giving her \"Exit Pumpkins\" she can use to progress through this zombie-world and eventually back to our own, but the problem is that she must find her way to the Exit Pumpkins on her own...\nControls:\np\t\tPause/check password*\nUp/ctrl\t\tJump\nDown\t\tAction Button (aka Use object)\nLeft/Right\t\tMove left and right\nS\t\tEnable/Disable Sound\nh\t\tView Help Dialogs\nSpace\t\tPSI FREEZE!\nu\t\tIncrease/decrease game display size.\n*(Passwords can be used to continue a game)\n\nObjective of game: Reach exit pumpkin to progress.";
    static char helpcntrl[500] = "Controls:\np\t\tPause\nUp/ctrl\t\tJump\nDown\t\tAction Button (aka Use object)\nLeft/Right\t\tMove left and right\nS\t\tEnable/Disable Sound\nh\t\tView Help Dialogs\nSpace\t\tPSI FREEZE!";
    static char helpadv[950] = "Climbing: If you jump toward certain blocks and hold up, it may\n allow you to climb them.\nTunnel Sliding: Hold up or ctrl to move down a tunnel slightly faster than you would by walking.\n\nUse (press down on) Life-Runes (Gray with red marking) to become a pseudo-ghost that can pass through certain kinds of walls and is invunerable to damage from enemies. (Can\'t shoot, You are NOT invunerable to spikes and the like as a ghost!)\n\nTo exit the level, get to the Exit pumpkin and use it.  (Doesn\'t work as ghost, you must be in Human form to win)\n\nUse (press down) the Golden Mushroom to jump high into the sky!\n\nBeware spikes, ecto-plasmic ooze, and other bad things out there as they cause instant death on contact.";
    static char windexp[650] = "Display Explanations:\nThe Red bar (Upper-right):\tThis bar describes your Health status\nThe Green bar (Upper-right):\tNumber of lives remaining\nThe black and orange bar (Lower-left):\tVertical speed\nBlue Bar (Lower-left):\tWhen you get hit, this bar will appear, as long as the bar is there, you are immune to additional damage.\n\nThere are 25 non-custom levels.\nStarting Lives: 12\nStarting Health: 5\nYes, this game is beatable.";
    static char credits[550] = "Tileset/Programming/non-character sprites/background:\n\tEvilPuffBall\nSound Effects:\n\tWhat Evar, EvilPuffBall\nSprites and Background music:\n\tEarthBound,Push Over,Mother\nOriginal moon photo:\n\tNASA\nExcessive Testing:\n\tWhat Evar, BB, B. Solem, and Odis\n(For the curious) APIs Utilized:\n\tWin32 API, DirectX 7.0 (DirectSound) API, FLTK\n(UPX Compressed Executable)\nAll the stuff I blatantly stole belongs to their respective\ncopyright owners.";
    static char finalmessage[120] = "Know when to run! Know when to fight!  Good Luck!";
    int ysize; /*ysize, describes total y-spanning of level, no need for static*/
    int monktast;
    static BYTE MenuPosition = 0;    
    /*
         * Perform processing based on what kind of message we got.
         */
    switch (nMsg)
    {
        case WM_CREATE:
        {
            GetClientRect( hwnd, &rc );
            for(i = 0; i < MaxObjects; i++)
            {
                objects[i].keio = 0;
                enemies[i].keio = 0;
            }
            randomz(-1, randomz(2000,0));
            /*LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);
            twait = 10;
            scrx = 0;*/
                
            #ifdef SOUND
            for(i = 0; i < 24; i++)
            {
                Bufferinuse[i] = 0;
                Bufferused[i] = 0;
            }
            #endif
                
            /*Initialize an object for test*/
            if(Canvas == NULL)
            {
                printf("monkeys failure");
            }
            if( (SetTimer( hwnd, 900, 55, NULL )) == 0)
            {
                MessageBox(hwnd, "I can\'t allocate a timer for the game to run on!\nTry restarting your computer and running me again.","Timer allocation error", MB_OK);
            }
            blastset[0] = 0;
            blastset[1] = 0;
            blastset[2] = 0;
            clear_async();
            #ifdef SOUND
            printf("%s", DirectSoundCreate(NULL, &lpds, NULL) == DS_OK ? "DSC == TRUE" : "DSC == FALSE");
            lpds->lpVtbl->SetCooperativeLevel(lpds,hwnd, DSSCL_NORMAL);
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 1;
            WaveFormat.nSamplesPerSec = 44100;
            WaveFormat.nAvgBytesPerSec = 44100;
            WaveFormat.nBlockAlign = 1;
            WaveFormat.wBitsPerSample = 8;
            WaveFormat.cbSize = 0;
            BGMFormat.wFormatTag = WAVE_FORMAT_PCM;
            BGMFormat.nChannels = 1;
            BGMFormat.nSamplesPerSec = 22050;
            BGMFormat.nAvgBytesPerSec = 22050;
            BGMFormat.nBlockAlign = 1;
            BGMFormat.wBitsPerSample = 8;
            BGMFormat.cbSize = 0;
            LOWBGMFormat.wFormatTag = WAVE_FORMAT_PCM;
            LOWBGMFormat.nChannels = 1;
            LOWBGMFormat.nSamplesPerSec = 11025;
            LOWBGMFormat.nAvgBytesPerSec = 11025;
            LOWBGMFormat.nBlockAlign = 1;
            LOWBGMFormat.wBitsPerSample = 8;
            LOWBGMFormat.cbSize = 0;
            bufferdesc.dwSize = sizeof(DSBUFFERDESC);
            bufferdesc.dwFlags = DSBCAPS_CTRLVOLUME|DSBCAPS_PRIMARYBUFFER;
            bufferdesc.dwBufferBytes = 0;
            bufferdesc.dwReserved = 0;
            bufferdesc.lpwfxFormat = NULL;
            printf("%s", lpds->lpVtbl->CreateSoundBuffer(lpds, &bufferdesc, &lplpDirectSoundBuffer, NULL) == DS_OK ? "CDSB == TRUE" : "CDSB == FALSE" );
            lplpDirectSoundBuffer->lpVtbl->SetFormat(lplpDirectSoundBuffer, &WaveFormat);
            for(i = 0; i < NumberOfSounds; i++)
            {
                ProtoBuffers[i].dwSize = sizeof(DSBUFFERDESC);
                ProtoBuffers[i].dwFlags = DSBCAPS_CTRLVOLUME;
                ProtoBuffers[i].dwBufferBytes = wavesize[i+1]+4;
                ProtoBuffers[i].dwReserved = 0;
                ProtoBuffers[i].lpwfxFormat = &WaveFormat;
                if(i+1==5)
                {
                    ProtoBuffers[i].lpwfxFormat = &BGMFormat;
                }
                if(i+1==6)
                {
                    ProtoBuffers[i].lpwfxFormat = &LOWBGMFormat;
                }
                if(i+1==7)
                {
                    ProtoBuffers[i].lpwfxFormat = &BGMFormat;
                }
                if(i+1==8)
                {
                    ProtoBuffers[i].lpwfxFormat = &LOWBGMFormat;
                }
                if(i+1==9)
                {
                    ProtoBuffers[i].lpwfxFormat = &LOWBGMFormat;
                }
                if(i+1==10)
                {
                    ProtoBuffers[i].lpwfxFormat = &LOWBGMFormat;
                }
                
                lpds->lpVtbl->CreateSoundBuffer(lpds, &ProtoBuffers[i], &ProtoBuffersStr[i], NULL);
                ProtoBuffersStr[i]->lpVtbl->Lock(ProtoBuffersStr[i], 0, wavesize[i+1],&RawData, &rdl,&RawData2,&rdl2,0);
                if(wavemem[i+1] != NULL)
                {
                MemPtr = wavemem[i+1];
                memcpy(RawData,&MemPtr[44],rdl);
                if(rdl2 != 0 || RawData2 != NULL)
                {
                    //printf("!");
                    memcpy(RawData2,&MemPtr[44+rdl], rdl2);
                }
                }                
                else
                {
                    /*Do nothing!*/
                }
                ProtoBuffersStr[i]->lpVtbl->SetCurrentPosition(ProtoBuffersStr[i], 0);
                ProtoBuffersStr[i]->lpVtbl->Unlock(ProtoBuffersStr[i], RawData, rdl, RawData2, rdl2);
            }
                
            bufferdesc2.dwSize = sizeof(DSBUFFERDESC);
            bufferdesc2.dwFlags = DSBCAPS_CTRLVOLUME;
            bufferdesc2.dwBufferBytes = wavesize[0]+4;
            bufferdesc2.dwReserved = 0;
            bufferdesc2.lpwfxFormat = &WaveFormat;
                
                
            printf("%s", lpds->lpVtbl->CreateSoundBuffer(lpds, &bufferdesc2, &lpSecondaryBuffer[0], NULL) == DS_OK ? "DSCB2 == TRUE" : "DSCB2 == FALSE");
            printf("%s",lpSecondaryBuffer[0]->lpVtbl->Lock(lpSecondaryBuffer[0], 0, wavesize[0],&RawData, &rdl,&RawData2,&rdl2,0) == DS_OK ? "BADFORMAT":"GOODFORMAT");
                
            MemPtr = wavemem[0];
            if(wavemem[0] != NULL)
            {
            memcpy(RawData,&MemPtr[44],rdl);
            if(rdl2 != 0 || RawData2 != NULL)
            {
                printf("!");
                memcpy(RawData2,&MemPtr[44+rdl], rdl2);
            }
            }
            /*    for(i = 0; i < rdl; i++)
                                 {
                                    *MemPtr = &RawData + i;
                                    printf("%X", MemPtr[0]);
                                 }   */
            lpSecondaryBuffer[0]->lpVtbl->SetCurrentPosition(lpSecondaryBuffer[0], 0);
            printf("%s", lpSecondaryBuffer[0]->lpVtbl->Unlock(lpSecondaryBuffer[0], RawData, rdl, RawData2, rdl2) == DS_OK ? "UNLOCKTRUE" : "UNLOCKFALSE");
            /*  lpSecondaryBuffer[0]->lpVtbl->SetCurrentPosition(lpSecondaryBuffer[0], 0);*/
            /*   printf("%s", lplpDirectSoundBuffer->lpVtbl->Play(lplpDirectSoundBuffer, 0, 0, DSBPLAY_LOOPING) == DSERR_UNINITIALIZED ? "MONKEYS" : "CRAPPERS!");  */
            lpds->lpVtbl->DuplicateSoundBuffer(lpds, lpSecondaryBuffer[0], &lpSecondaryBuffer[1]);
            lpds->lpVtbl->DuplicateSoundBuffer(lpds, lpSecondaryBuffer[0], &lpSecondaryBuffer[2]);
            ProtoBuffersStr[soundtoplay]->lpVtbl->Play(ProtoBuffersStr[soundtoplay], 0,0, DSBPLAY_LOOPING);
            #endif         
            #ifdef OmegaCollisionSystem
            CreateLGRegion(objects);
                
                
            #endif
                
                
            hdc = GetDC(hwnd);
            FX = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            if(FX == NULL)
            {printf("double monk fail");}
            ReleaseDC(hwnd, hdc);
                
            return 0;
            break;
        }
        case WM_TIMER:
        {
            wallage = 0;    
            psxset = 0;
            psyset = 0;
            if(GetActiveWindow() != hwnd)
            {
                clear_async();                 
            }    
            if(GetActiveWindow() == hwnd && GameStatus == GAME_PLAY)
            {
                if(Invincible == 1)
                {Health = Healthmax;}
                if(Ulives == 1)
                {Lives = Livesmax;}
                
                #ifdef debugkeys
                if(Dkey == 1)
                {
                if(GetAsyncKeyState(VK_F10) != 0)
                {
                    Levelnum += 1;
                    Health = Healthmax; Lives = Livesmax; scry = 0;
                    LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                    ChangeBGM();
                    vertpaula = 0; twait = 10;
                }
                if(GetAsyncKeyState(VK_F11) != 0)
                {
                    Levelnum--;
                    Health = Healthmax; Lives = Livesmax; scry = 0;
                    LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                    ChangeBGM();
                    vertpaula = 0; twait = 10;
                }
                if(GetAsyncKeyState(VK_F12) != 0)
                {                             
                    Health = Healthmax;
                }
                }
                #endif
                if(GetAsyncKeyState('u') != 0 || GetAsyncKeyState('U') != 0)
                {
                    doubleswitch = (doubleswitch + 1)&1;
                    if(doubleswitch == 1)
                    {
                       SetWindowPos(hwnd, HWND_TOP,0,0, 640+GetSystemMetrics(SM_CXBORDER),480+GetSystemMetrics(SM_CYCAPTION),SWP_NOMOVE);
                    }
                    if(doubleswitch == 0)
                    {
                       SetWindowPos(hwnd, HWND_TOP,0,0, 320+GetSystemMetrics(SM_CXBORDER),240+GetSystemMetrics(SM_CYCAPTION),SWP_NOMOVE);                                    
                    }
                    clear_async();
                    Sleep(500);
                    clear_async();
                }
                #ifdef SOUND
                for(i = 0; i < 24; i++)
                {
                    if(Bufferinuse[i] >= 1)
                    {
                        lpPlayBuffers[i]->lpVtbl->GetStatus(lpPlayBuffers[i], &off);
                        if( off == DSBSTATUS_BUFFERLOST)
                        {
                            lpPlayBuffers[i]->lpVtbl->Restore(lpPlayBuffers[i]);
                        }  
                            
                        if( off != DSBSTATUS_PLAYING)
                        {
                            //printf("NOT PLAYING");
                            if( Bufferinuse[i] == 1)
                            {
                                Bufferinuse[i] = 58;
                            }
                            else
                            {
                                Bufferinuse[i] -= (Bufferinuse[i]>0?1:0);
                            }
                            if(Bufferinuse[i] == 2)
                            {
                                Bufferinuse[i] = 0;
                            }
                        }
                            
                    }
                    //printf("  Bufferinuse[%d] = %d  ", i, Bufferinuse[i]);
                        
                }   
                    
                    
                    
                if(GetAsyncKeyState('s') != 0 || GetAsyncKeyState('S') != 0)
                {
                    if(SoundStatus == 0 && SoundPause == 0)
                    {
                        ProtoBuffersStr[soundtoplay]->lpVtbl->Stop(ProtoBuffersStr[soundtoplay]);
                        SoundPause = 20;
                        SoundStatus = 1;
                        goto monk;
                    }
                    if(SoundStatus == 1 && SoundPause == 0)
                    {
                        ProtoBuffersStr[soundtoplay]->lpVtbl->Play(ProtoBuffersStr[soundtoplay], 0,0, DSBPLAY_LOOPING);
                        SoundPause = 20;
                        SoundStatus = 0;
                    }
                    monk: ;
                }
                #endif
                    
                if(GetAsyncKeyState('h') != 0  || GetAsyncKeyState('H') != 0)
                {
                    if(HelpPause == 0)
                    {
                        HelpPause = 20;
                        MessageBox(hwnd, thestory, "Help", MB_OK);
                        //MessageBox(hwnd, helpcntrl , "Help Box: Basic Controls", MB_OK);
                        MessageBox(hwnd, helpadv, "Advanced control and item notes:", MB_OK);
                        MessageBox(hwnd, windexp, "Display And Miscellaneous Info", MB_OK);
                        MessageBox(hwnd, credits, "Credits", MB_OK);
                        //MessageBox(hwnd, finalmessage, "And a word from our sponsors!", MB_OK);
                            
                    }
                }
                    
                HelpPause -= (HelpPause>0 ? 1 : 0);
                    
                SoundPause -= ((SoundPause > 0) ? 1 : 0);
                jumpcounter -= ((jumpcounter > 0 )?1:0);
                jumptimer -= ((jumptimer > 0)?1:0);    
                if(GetAsyncKeyState(VK_LEFT) != 0)
                {
                    if(twait == 0)
                    {
                        if(pauladirection == 1)
                        { pauladirection=0; paulaspeed=0; }
                        else
                        {paulaspeed = 6;}
                    }
                }
                if(GetAsyncKeyState(VK_RIGHT) != 0)
                {
                    if(twait == 0)
                    {
                        if(pauladirection == 0)
                        { pauladirection=1; paulaspeed=0; }
                        else
                        {paulaspeed = 6;}
                    }
                }
                    
                if(GetAsyncKeyState('p') != 0 || GetAsyncKeyState('P') != 0)
                {
                    KillTimer(hwnd,900);
                    GeneratePasswordString(level[Levelnum], Levelnum, &PassWord2);
                    sprintf(buf, "Game is paused, press OK to continue.\nPassWord: %s", PassWord2);
                    MessageBox(hwnd, buf, "PAUSE", MB_OK);
                    SetTimer(hwnd, 900, 55, NULL);
                }
                flower = 0;    
                if(GetAsyncKeyState(VK_DOWN) != 0)
                {
                    flower = 1;
                    for(i = 0; i < MaxObjects; i++)
                    {
                        if(objects[i].keio == 1 && objects[i].Teleportal == 1 && GhostStatus == 0)
                        {
                           cact = 0;
                           cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height, 1, 0, 0, 0, 0, 0, 0, paulay<(objects[i].posy+objects[i].height)?1:0, 0);
                           if(cact != 0)
                           {
                              vertpaula = -1;
                              paulaspeed = 0;
                              paulax = objects[i].tx;
                              paulay = objects[i].ty;                              
                              #ifdef SOUND
                              if(SoundStatus == 0)                              
                              PlaySound(MAKEINTRESOURCE(207),apphinst,SND_RESOURCE);
                              #endif
                           }
                        }
                        if(ghostpause == 0)
                        {
                            if(objects[i].keio == 1 && (objects[i].ghost == 1 || objects[i].exit == 1 || objects[i].jumper == 1) && objects[i].posx > (paulax-CCR) && objects[i].posx < (paulax+CCR)   ) 
                            {
                                cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height, 1, 0, 0, 0, 0, 0, 0, paulay<(objects[i].posy+objects[i].height)?1:0, 0);
                                if(cact != 0)
                                {
                                    if(objects[i].jumper == 1)
                                    {
                                        if(twait == 0)
                                        {
                                            vertpaula = 23;
                                            jumptimer = 9;
                                        }
                                    }
                                    bosshere = 0;
                                    for(x = 0; x < MaxObjects; x++)
                                    {    
                                       if(enemies[x].boss == 1 && enemies[x].keio == 1)
                                       bosshere = 1;
                                    }
                                    
                                    if(objects[i].exit == 1 && GhostStatus == 0)
                                    {
                                        /*Do win stuff here*/
                                        if(bosshere != 1)
                                        {
                                        Levelnum++;
                                        if(Levelnum == MaxLvl)
                                        {
                                            KillTimer(hwnd, 900);
                              #ifdef SOUND
                              if(SoundStatus == 0)                              
                              {ProtoBuffersStr[soundtoplay]->lpVtbl->Stop(ProtoBuffersStr[soundtoplay]);PlaySound(MAKEINTRESOURCE(211),apphinst,SND_RESOURCE);ProtoBuffersStr[soundtoplay]->lpVtbl->Play(ProtoBuffersStr[soundtoplay], 0,0, DSBPLAY_LOOPING);}
                              #endif
                                            if( MessageBox(hwnd, "YAY!  You manged to defeat the zombie hordes over treacherous terrain!  What will you do next?\nPlay Again?", "[SSB Voice]Congratulations![/voice]", MB_YESNO) == IDYES)
                                            {
                                                Health = Healthmax; Lives = Livesmax; Levelnum = 0;
                                                LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                                                ChangeBGM();
                                                vertpaula = 0; twait = 10;SetTimer(hwnd, 900, 55, NULL);
                                                bosshere = 1;    
                                            }
                                            else
                                            {                                                
//                                                DestroyWindow(hwnd);
                                                SendMessage(hwnd, WM_DESTROY, 0,0);
                                                bosshere = 1;
                                            }
                                                
                                                
                                        }
                                        else
                                        {
                                        if(Levelnum+1 <= MaxLvl)
                                        {Health = Healthmax;
                                        LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                                        ChangeBGM();
                                        vertpaula = 0; twait = 10;
                                        }
                                        }
                                        goto monkey;
                                        }    
                                        else
                                        {MessageBox(hwnd, "You can't leave!  A powerful force has sealed this exit pumpkin!", "Message", MB_OK|MB_TASKMODAL);}
                                    }
                                        
                                    if(objects[i].ghost == 1)
                                    {
                                        GhostStatus=(GhostStatus+1)&1;
                                        KillTimer(hwnd, 900);
                                        #ifdef SOUND
                                        if(SoundStatus == 0)
                                        {
                                            PlaySound(MAKEINTRESOURCE(205), apphinst, SND_RESOURCE);
                                        }
                                        else
                                        {
                                            Sleep( 1000 );
                                        }
                                        #endif
                                        SetTimer(hwnd, 900, 55, NULL);
                                            
                                    }
                                    ghostpause = 11;
                                    monkey: ;
                                }
                            }
                        }
                    }
                }
                if(GetAsyncKeyState(32) == 0)
                {
                    ShotOK = 1;
                }
                    
                if(GetAsyncKeyState(VK_UP) != 0 || GetAsyncKeyState(VK_CONTROL) != 0)
                {
                    if(twait == 0/* && jumpcounter == 0*/)
                    {jump=1; jumpcounter = 16/*Jump velocity+1*//2;/*Jump velocity+1*/;}
                        
                }
                if(GetAsyncKeyState(32) != 0)
                {
                        
                    if(GhostStatus == 0 && ShotOK == 1)
                    {fire=1; ShotOK = 0;}
                        
                }
                    
                    
                    
                if(ghostpause > 0)
                {ghostpause--;}
                ysize=NumofLines*24-6;
                twait -= twait > 0 ? 1 : 0;
                paulastatus = (paulastatus+1)&1;
                vertpaula -= 2;
                if(paulay >= ysize-prect.bottom)
                {vertpaula = 0; paulay = ysize-prect.bottom;}
                /*if(paulay < 0)
                        {vertpaula = -2; paulay = 0;}*/
                    
                cactus = 0;
                    
                    
                /*if(paulay >= rc.bottom-prect.bottom)
                        {vertpaula = 0; paulay = rc.bottom-prect.bottom;}*/
                    
                if(jump == 1 && (vertpaula == 0 || (vertpaula == 1 && cactus == 1))  )
                {
                    paulastatus = 0;
                    jump == 15;
                } /* Leave in for stuff */
                if(jump == 0 && vertpaula > 5 && jumptimer == 0)
                {
                    paulastatus = 0;
                    vertpaula = 5;
                }
                bcol = 0;
                /*Collision routines!*/
                #ifndef exp1
                for(i = 0; i < MaxObjects; i++)
                {
                    cact = 0;
                    if(objects[i].keio == 1 && objects[i].solid == 1 && (GhostStatus != 1 || objects[i].ghostt != 1 ))
                    {
                        if(objects[i].donly != 1)
                        {
                            cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height-3, paulaspeed * (pauladirection == 0 ? -1 : 1), -1*vertpaula, 0, 0, 0,0,paulay>=objects[i].posy ? 1 : 0,(paulay+prect.bottom)<=(objects[i].posy+objects[i].height),0);
                        }
                        else
                        {
                            cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height-3, paulaspeed * (pauladirection == 0 ? -1 : 1), -1*vertpaula, 0, 0, 0,1,(paulay+prect.bottom) < (objects[i].posy+5) ? 0 : 1,1,1);
                        }
                        //printf("cact==%d, i==%d", cact, i);
                    }
                    if(cact != 0)  
                    {
                        if((cact&1) == 1)
                        {
                            paulay = objects[i].posy+objects[i].height;
                            vertpaula = -1;
                            paulax = paulax + (paulaspeed*((pauladirection == 1)?1:-1));
                            paulay = paulay + (-1*vertpaula);
                            paulaspeed = 2;
                            /*                              bcol = 1;*/
                        }
                        if((cact&2) == 2)
                        {
                            psy = paulay = objects[i].posy-prect.bottom;
                            psyset = 1;
                            vertpaula = 1;
                            paulax = paulax + (paulaspeed*((pauladirection == 1)?1:-1));
                            paulay = paulay + (-1*vertpaula);
                            vertpaula = 0;
                            if(pauladirection == 1)
                            {vertpaula = 0;}
                        }
                        if((cact&4) == 4)
                        {
                            psx = paulax = objects[i].posx+objects[i].width;
                            psxset = 1;
                            paulay = paulay + (-1*vertpaula);
                        }
                        if((cact&8) == 8)
                        {
                            psx = paulax = objects[i].posx-prect.right;
                            psxset = 1;
                            paulay = paulay + (-1*vertpaula);
                            //printf("\n8 == %d", paulax);
                        }
                        cactus = 1;
                    }
                }
                if(psxset == 1)
                {paulax = psx;}  
                if(psyset == 1)
                {paulay = psy;}  
                #endif
                    
                //........................................MYU!
                    
                #ifdef exp1
                for(i = 0; i < rects; i++)
                {
                    cact = 0;
                    if( !(GhostStatus == 1 && (StopperRectType[i]&2) == 2) )
                    {
                        if((StopperRectType[i]&1) != 1)
                        {
                            cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, StopperRect[i].left, StopperRect[i].top+1, StopperRect[i].right, StopperRect[i].bottom-3, paulaspeed * (pauladirection == 0 ? -1 : 1), -1*vertpaula, 0, 0, 0,0,paulay>=StopperRect[i].top ? 1 : 0,(paulay+prect.bottom)<=(StopperRect[i].bottom),0);
                        }
                        else
                        {
                            cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, StopperRect[i].left, StopperRect[i].top+1, StopperRect[i].right, StopperRect[i].bottom-3, paulaspeed * (pauladirection == 0 ? -1 : 1), -1*vertpaula, 0, 0, 0,1,(paulay+prect.bottom) < (StopperRect[i].top+5) ? 0 : 1,1,1);
                        }
                        //printf("cact==%d, i==%d", cact, i);
                    }
                    if(cact != 0)  
                    {
                        if((cact&1) == 1)
                        {
                            paulay = StopperRect[i].bottom;
                            vertpaula = -1;
                            paulax = paulax + (paulaspeed*((pauladirection == 1)?1:-1));
                            paulay = paulay + (-1*vertpaula);
                            paulaspeed = 2;
                            /*                              bcol = 1;*/
                        }
                        if((cact&2) == 2)
                        {
                            psy = paulay = StopperRect[i].top-prect.bottom;
                            psyset = 1;
                            vertpaula = 1;
                            paulax = paulax + (paulaspeed*((pauladirection == 1)?1:-1));
                            paulay = paulay + (-1*vertpaula);
                            vertpaula = 0;
                            if(pauladirection == 1)
                            {vertpaula = 0;}
                        }
                        if((cact&4) == 4)
                        {
                            psx = paulax = StopperRect[i].right;
                            paulaspeed = 0;
                            psxset = 1;
                            if( Spidey == 1 )
                            {
                              if(flower == 1)
                              {vertpaula = 0; wallage = 1;}
                            }
                            if((StopperRectType[i]&4) == 4)
                            {jumpcounter = 12;}
                            paulay = paulay + (-1*vertpaula);
                        }
                        if((cact&8) == 8)
                        {
                            psx = paulax = StopperRect[i].left-prect.right;
                            paulaspeed = 0;
                            psxset = 1;
                            if( Spidey == 1 )
                            {
                               if(flower == 1)
                               {vertpaula = 0; wallage = 1;}
                            }
                            if((StopperRectType[i]&4) == 4)
                            {jumpcounter = 12;}
                            paulay = paulay + (-1*vertpaula);
                            //printf("\n8 == %d", paulax);
                        }
                        cactus = 1;
                    }
                }
                if(psxset == 1)
                {paulax = psx;}  
                if(psyset == 1)
                {paulay = psy;}  
                #endif
                jumpcounter=jumpcounter+(psxset*2)+psyset;
                    
                    
                cact = 0; /*Set for later usage in enemy collision check*/                        
                /*Enemy section start...*/
                for(x =0; x < MaxObjects; x++)
                {
                    psxset = 0; psyset = 0;
                    monktast = 0;
                        
                    if(paulay+prect.bottom <= enemies[x].posy+enemies[x].height+15 && paulay+prect.bottom >= enemies[x].posy+enemies[x].height-15 && paulax > enemies[x].posx - 80 && paulax < enemies[x].posx + 80 && (enemies[x].type == 0|| enemies[x].type == 3) && enemies[x].keio == 1 )
                    {
                        if(enemies[x].lunge == 0 && (enemies[x].type == 0 || enemies[x].type ==3) && enemies[x].keio == 1)
                        {
                            /*   PlaySound(MAKEINTRESOURCE(200),apphinst,SND_ASYNC|SND_RESOURCE|SND_NOSTOP);  */
                            #ifdef SOUND
                            off = 0;
                                
                            if(SoundStatus == 0)
                            {
                                for(t = 0; t < 24; t++)
                                {
                                    if(Bufferinuse[t] == 0 && off == 0)
                                    {
                                        if(Bufferused[t] == 1)
                                        {
                                            lpPlayBuffers[t]->lpVtbl->Release(lpPlayBuffers[t]);
                                            Bufferused[t] = 0;
                                        }
                                        
                                        //printf("\nBuffer: %d", t);
                                        if( (enemies[x].ospx == 0 && enemies[x].ospy == 96 && enemies[x].width == 24 && enemies[x].height == 24 && enemies[x].type == 3))
                                        {
                                            lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[12], &lpPlayBuffers[t]);
                                        }
                                        else if( (enemies[x].ospx == 109 && enemies[x].ospy == 96 && enemies[x].width == 16 && enemies[x].height == 16 && enemies[x].type == 0))
                                        {
                                             lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[14], &lpPlayBuffers[t]);
                                        }
                                        else
                                        lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[0], &lpPlayBuffers[t]);
                                        
                                        lpPlayBuffers[t]->lpVtbl->SetCurrentPosition(lpPlayBuffers[t], 0);
                                        lpPlayBuffers[t]->lpVtbl->Play(lpPlayBuffers[t], 0, 0, 0);
                                        Bufferinuse[t] = 1;
                                        Bufferused[t] = 1;
                                        off = 1;
                                    }
                                }
                            }
                            #endif
                        }
                        enemies[x].xspeed = (enemies[x].speedbase+1) * ((paulax-enemies[x].posx) < 0 ? -1 : 1);
                        enemies[x].lunge = 1;
                    }
                    else
                    {
                        /*enemies[x].xspeed = (2+randomz(3,0)) * (enemies[x].type == 0 ? 1 : 0) * (enemies[x].xspeed <= 0 ? -1 : 1) * (enemies[x].lunge==1? -1: 1);*/
                        enemies[x].xspeed = enemies[x].speedbase * ((enemies[x].type == 0 || enemies[x].type == 3 || enemies[x].boss == 1) ? 1 : 0) * (enemies[x].xspeed <= 0 ? -1 : 1) * (enemies[x].lunge==1? -1: 1);
                        enemies[x].lunge = 0;
                    }
                        
                        
                    if((enemies[x].boss == 1 || enemies[x].type == 0 || enemies[x].type == 3) && enemies[x].keio == 1)
                    {enemies[x].yspeed -= 2;}
                    if(enemies[x].type == 1 && enemies[x].keio == 1)
                    {
                        enemies[x].counter++;
                        if(enemies[x].direction == 1)
                        {enemies[x].yspeed += 1;}
                        if(enemies[x].direction == 0)
                        {enemies[x].yspeed -= 1;}
                            
                        if(enemies[x].yspeed > 5 || enemies[x].yspeed < -5||enemies[x].counter == 12)
                        {
                            //printf("Change dir");
                            enemies[x].direction = (enemies[x].direction+1)&1;
                            enemies[x].counter = 0;
                        }
                    }
                        
                        
                     /*Monkeys and stuff shooting!!!!*/   
                    if(enemies[x].shootset == 1)
                    {
                        enemies[x].shooty += enemies[x].shootyspeed;
                        enemies[x].shootx += enemies[x].shootxspeed;
                        enemies[x].shootcount--;
                            
                        if(enemies[x].shootcount == 0)
                        {
                            enemies[x].shootset = 0;
                        }
                            
                    }
                        
                    if(enemies[x].keio == 1 )
                    {
                            
                        if(enemies[x].type == 1 || enemies[x].type == 3 || enemies[x].boss == 1)
                        {
                            if( ((paulax > enemies[x].posx-150 && paulax < enemies[x].posx-35) || (paulax > enemies[x].posx+enemies[x].width+35 && paulax < enemies[x].posx+enemies[x].width+125))  && randomz(20,0) > 12 && enemies[x].shootset == 0 && paulay < enemies[x].posy+enemies[x].height+100 && paulay > enemies[x].posy-100)
                            {   
                                #ifdef SOUND
                                off = 0;           
                                    
                                if(SoundStatus == 0)
                                {
                                    for(t = 0; t < 24; t++)
                                    {
                                        if(Bufferinuse[t] == 0 && off == 0)
                                        {
                                            if(Bufferused[t] == 1)
                                            {
                                                lpPlayBuffers[t]->lpVtbl->Release(lpPlayBuffers[t]);
                                            }
                                            
                                            if(enemies[x].boss != 1 && !(enemies[x].ospx == 0 && enemies[x].ospy == 96 && enemies[x].width == 24 && enemies[x].height == 24 && enemies[x].type == 3))
                                            {
                                            lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[3], &lpPlayBuffers[t]);
                                            }
                                            else if( (enemies[x].ospx == 0 && enemies[x].ospy == 96 && enemies[x].width == 24 && enemies[x].height == 24 && enemies[x].type == 3))
                                                {
                                                    lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[12], &lpPlayBuffers[t]);
                                                }
                                             else
                                             {lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[10], &lpPlayBuffers[t]);}
                                              lpPlayBuffers[t]->lpVtbl->Play(lpPlayBuffers[t], 0, 0, 0);
                                            Bufferinuse[t] = 1;
                                            Bufferused[t] = 1;
                                            off = 1;
                                        }
                                    }
                                }
                                #endif
                                enemies[x].shootset = 1;
                                enemies[x].shootcount = 50;
                                enemies[x].shootx = enemies[x].posx+(enemies[x].width/2);
                                enemies[x].shooty = enemies[x].posy+(enemies[x].height/2);
                                enemies[x].shootxspeed = (paulax-enemies[x].shootx+4)/25;
                                enemies[x].shootyspeed = (paulay-enemies[x].shooty+4)/25;
                                if(enemies[x].boss == 1)
                                {enemies[x].sprite |= 0x70;}
                            }
                        }
                            
                        for(i = 0; i < MaxObjects; i++)
                        {
                            cact = 0;
                            if(objects[i].keio == 1 && objects[i].solid == 1 && objects[i].posx < (enemies[x].posx+CCR) && objects[i].posx > (enemies[x].posx-CCR) )
                            {
                                if(enemies[x].boss != 1 || objects[i].donly != 1)
                                cact = DetectCollision(enemies[x].posx, enemies[x].posy, enemies[x].posx+enemies[x].width, enemies[x].posy+enemies[x].height, objects[i].posx+1, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height-2, enemies[x].xspeed, -1*enemies[x].yspeed, 0, 0, 0,(enemies[x].posy+enemies[x].height)<=(objects[i].posy+4)?1:0, (enemies[x].posy>=objects[i].posy /*|| (enemies[x].posy <= objects[i].posy && enemies[x].posy >= objects[i].posy+objects[i].height)*/) ? 1 : 0,0,((enemies[x].posy+enemies[x].height)<=(objects[i].posy+6)/*|| (enemies[x].posy <= objects[i].posy && enemies[x].posy >= objects[i].posy+objects[i].height)*/)?1:0);
                                //printf("cact==%d, i==%d", cact, i);
                            }
                                
                            if(cact != 0)
                            {
                                if((cact&1) == 1)
                                {
                                    psy = enemies[x].posy = objects[i].posy+objects[i].height;
                                    psyset = 1;
                                    enemies[x].yspeed = -1;
                                    enemies[x].posx = enemies[x].posx + (enemies[x].xspeed/2);
                                    enemies[x].posy = enemies[x].posy + (-1*enemies[x].yspeed);
                                        
                                }
                                if((cact&2) == 2)
                                {
                                    psy = enemies[x].posy = objects[i].posy-enemies[x].height;
                                    psyset = 1;
                                    enemies[x].yspeed = 1;
                                    enemies[x].posx = enemies[x].posx + enemies[x].xspeed;
                                    enemies[x].posy = enemies[x].posy + (-1*enemies[x].yspeed);
                                    enemies[x].yspeed = -1;
                                    enemies[x].prev = 1;
                                }
                                if((cact&4) == 4)
                                {
                                    psx = enemies[x].posx = objects[i].posx+objects[i].width;
                                    psxset = 1;
                                    enemies[x].posy = enemies[x].posy + (-1*enemies[x].yspeed);
                                    if(enemies[x].boss != 1)
                                    {
                                       enemies[x].xspeed *= -1;
                                    }
                                    else
                                    {
                                        if(enemies[x].cd == 0){enemies[x].sprite = enemies[x].sprite | (128+15); enemies[x].cd = 1;}
                                        //enemies[x].xspeed = -1;
                                    }
                                }
                                if((cact&8) == 8)
                                {
                                    psx = enemies[x].posx = objects[i].posx-enemies[x].width;
                                    psxset = 1;
                                    enemies[x].posy = enemies[x].posy + (-1*enemies[x].yspeed);
                                    printf("\n8 == %d", enemies[x].posx);
                                    if(enemies[x].boss != 1)
                                    {
                                       enemies[x].xspeed *= -1;
                                    }
                                    else
                                    {
                                        if(enemies[x].cd == 0){enemies[x].sprite = enemies[x].sprite | (128+15); enemies[x].cd = 1;}
                                        //enemies[x].xspeed = 0;
                                    }
                                        
                                }
                                monktast = 1;
                            }
                        }
                    }


/*Boss spritage*/






/*RIGHT HERE!*/

                    if(enemies[x].boss == 1)
                    {
                       if( (enemies[x].sprite & 128) == 128) /*Boss is turning*/
                       {
                          enemies[x].sprite = (enemies[x].sprite&240)+((enemies[x].sprite&15)-1);
                          if((enemies[x].sprite&15) == 0)
                          {
                             enemies[x].sprite = enemies[x].sprite&127;
                             enemies[x].xspeed *= -1;
                             enemies[x].cd = 0;                                            
                          }                          
                       }
                       if( (enemies[x].sprite & 64) == 64) /*Boss is shooting*/
                       {
                          enemies[x].sprite = (enemies[x].sprite&(128+15+64))+((enemies[x].sprite&48)-16);
                          if((enemies[x].sprite&48) == 0)
                          {
                             enemies[x].sprite = enemies[x].sprite&191;
                          }                          
                       }                       
                    }                      


                    if(monktast == 0)
                    {
                        if(enemies[x].prev == 1 && enemies[x].boss != 1 && (enemies[x].type == 0 || enemies[x].type == 3))
                        {
                            enemies[x].xspeed *= -1;
                            enemies[x].yspeed = 0;                                
                        }
                        if(enemies[x].boss == 1)
                        {
                           enemies[x].posx = enemies[x].posx + enemies[x].xspeed;
                           enemies[x].posy = enemies[x].posy + -1*enemies[x].yspeed;
                        }
                        enemies[x].posx = enemies[x].posx + enemies[x].xspeed;
                        enemies[x].posy = enemies[x].posy + -1*enemies[x].yspeed;
                        
                    }
                    if(psxset == 1)
                    {enemies[x].posx = psx;}  
                    if(psyset == 1)
                    {enemies[x].posy = psy;}
                        
                    if(enemies[x].type == 0 && enemies[x].fb == 1)
                    {
                        enemies[x].posy = enemies[x].forceposy;
                    }
                        
                    if( enemies[x].boss != 1 && (enemies[x].type == 0 || enemies[x].type == 3))
                    {
                        enemies[x].sprite = (enemies[x].sprite+1)&1;
                        if(enemies[x].xspeed > 0)
                        {enemies[x].sprite += 2;}
                            
                    }
                    if(enemies[x].type == 1)
                    {
                        if(enemies[x].direction == 0)
                        {
                            enemies[x].sprite = 0;
                        }
                        else
                        {
                            enemies[x].sprite = 1;
                        }
                        if(paulax > enemies[x].posx)
                        {
                            enemies[x].sprite += 2;
                        }
                            
                    }
                        
                    if(enemies[x].posy > ysize-enemies[x].height)
                    {enemies[x].posy = ysize-enemies[x].height;}
                }
                    
                    
                                       
                if(paulaspeed == 0 && psxset != 0)
                {
                    paulastatus = 1;
                }
                if(paulay >= ysize-prect.bottom)
                {vertpaula = 0; paulay = ysize-prect.bottom;}
                    
                //........................NYO!
                if(flower == 0 && jump == 1 && (vertpaula == 0 || ((jumpcounter&1) == 1 && vertpaula == 1) || ((jumpcounter&12) == 12 && vertpaula == -5)))
                {
                    paulastatus = 0;
                        
                    if((jumpcounter&12)==12&&vertpaula==-5)
                    {
                        vertpaula = 11; 
                        //paulaspeed = pauladirection * -10;
                    }                                     
                    else
                    {vertpaula = 15;}
                }
                if(flower == 0 && jump == 0 && vertpaula > 5 && jumptimer == 0)
                {
                    paulastatus = 0;
                    vertpaula = 5;
                }

                    
                if(vertpaula != 0 && cactus == 0)
                {  paulastatus = 0; }
                if((vertpaula == 0 || (vertpaula == 1 && cactus == 1)) && paulaspeed == 0)
                {  paulastatus = 1; }
                if(fire == 1)
                {
                    for(i = 0; i < 3; i++)
                    {
                        if(blastset[i] != 1)
                        {
                            blastset[i] = 1;
                            blastx[i] = paulax;
                            blasty[i] = paulay+8;
                            blaststatus[i] = 0;
                            blastdirection[i] = (pauladirection+1)&1;
                            /*PlaySound(MAKEINTRESOURCE(202),apphinst,SND_ASYNC|SND_RESOURCE); */
                            #ifdef SOUND
                            if(SoundStatus == 0)
                            {
                                lpSecondaryBuffer[i]->lpVtbl->SetCurrentPosition(lpSecondaryBuffer[i], 0);
                                if( lpSecondaryBuffer[i]->lpVtbl->Play(lpSecondaryBuffer[i], 0, 0, 0) == DSERR_BUFFERLOST)
                                {
                                    lpSecondaryBuffer[i]->lpVtbl->Restore(lpSecondaryBuffer[i]);
                                    lpSecondaryBuffer[i]->lpVtbl->Play(lpSecondaryBuffer[i], 0,0,0);
                                    //printf("BUFFERLOST!");
                                }
                            }
                            #endif
                                
                            goto exitmakeblast;
                        }
                    }
                        
                }
                exitmakeblast: ;
                    
                fire = 0;
                jump = 0;
                for(i = 0; i < 3; i++)
                {
                    blaststatus[i] = (blaststatus[i]+1)&1;
                    blastx[i] = blastx[i] + (8*((blastdirection[i] == 0) ? 1 : -1 ));
                }
                    
                //printf("Cactus == %d", cactus);
                    
                /*if(paulay < 0)
                        {
                           vertpaula = -2;
                           paulay = 0;
                        }*/
                if(paulax < 0)
                {
                    paulax = 0;
                    paulaspeed = 0;
                }
                if(paulax+prect.right > scrsize)
                {
                    paulax = scrsize-prect.right;
                    paulaspeed = 0;
                }
                    
                if(cactus != 1)
                {
                    paulax = paulax + (paulaspeed*((pauladirection == 1)?1:-1));
                    paulay = paulay + (-1*vertpaula);
                    //printf("CACTUS!");
                }
                    
                if((paulax - scrx) > (rc.right / 2)) /*Relative screen position > Middle of actual?*/
                {
                    //scrx+=((paulax-scrx)-(rc.right/2));
                    scrx = paulax-(rc.right/2);
                    if(scrx > scrsize-(rc.right))
                    {scrx = scrsize-(rc.right);}
                }
                if((paulax - scrx) < (rc.right / 2))
                {
                    scrx-=((scrx-paulax)+(rc.right/2));
                    if(scrx < 0)
                    {scrx = 0;}
                }
                ysize=NumofLines*24-6;
                if((paulay - scry) > (rc.bottom / 2)) /*Relative screen position > Middle of actual?*/
                {
                    scry+=((paulay-scry)-(rc.bottom/2));
                    if(scry > ysize-(rc.bottom))
                    {scry = ysize-(rc.bottom);}
                }
                if((paulay - scry) < (rc.bottom / 2))
                {
                    scry-=((scry-paulay)+(rc.bottom/2));
                    if(scry < 0)
                    {scry = 0;}
                }
                hitOK -= (hitOK == 0 ? 0 : 1);
                if(GhostStatus == 0)
                {
                    for(x = 0; x < MaxObjects; x++)
                    {                                                                                                                                                                                                                                 /*Start*/
                        if( ((enemies[x].keio == 1 ? DetectCollision(enemies[x].posx+enemies[x].collisionshrink, enemies[x].posy+enemies[x].collisionshrink, enemies[x].posx+enemies[x].width-enemies[x].collisionshrink, enemies[x].posy+enemies[x].height-enemies[x].collisionshrink, paulax, paulay, paulax+prect.right, paulay+prect.bottom, 0, 0, 0, 0, 1, 0, 0,0,0) : 0) != 0) || ( ((enemies[x].shootset == 1) ? DetectCollision(enemies[x].shootx+1, enemies[x].shooty-1, enemies[x].shootx+enemies[x].shootimgwidth-1, enemies[x].shooty+enemies[x].shootimgheight-1, paulax+1, paulay+1, paulax+prect.right-1, paulay+prect.bottom-1, 0, 0, 0, 0, 1, 0, 0,0,0) : 0) != 0))
                        {
                            cact = 1;
                        }
                        if(cact != 0 && hitOK == 0)
                        {
                            Health -= (Health == 0 ? 0 : 1 );
                            hitOK = 21;
                            if(Health == 0)
                            {
                                #ifdef SOUND
                                KillTimer(hwnd, 900);
                                if(SoundStatus == 0)
                                {
                                    PlaySound(MAKEINTRESOURCE(206), apphinst, SND_RESOURCE);
                                }
                                else
                                {
                                    Sleep ( 1000 );
                                }
                                SetTimer(hwnd, 900, 55, NULL);
                                #endif    
                                hitOK = 0;
                                if(Lives == 0)
                                {
                                    KillTimer(hwnd, 900);
                                    GeneratePasswordString(level[Levelnum], Levelnum, &PassWord2);
                                    sprintf(buf, "Paula gasps her few last breaths as her sight dims...\nGame Over\nPlay again?\n\nPassword for current level:  %s", PassWord2);    
                                    if( MessageBox(hwnd, buf, "Your story ends here.", MB_TASKMODAL|MB_YESNO) == IDYES)
                                    {
                                        GhostStatus = 0; Health = Healthmax+1; Lives = Livesmax; Levelnum = 0;
                                        LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                                        ChangeBGM();
                                        vertpaula = 0; twait = 10;
                                        SetTimer(hwnd, 900, 55, NULL);
                                    }
                                    else
                                    {   DestroyWindow(hwnd);  }
                                }
                                else
                                {
                                    Lives -= (Lives == 0?0:1); Health = Healthmax+1; /*Prevents underflowage*/ GhostStatus = 0;
                                    LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx =0;
                                    ChangeBGM();
                                    vertpaula = 0; twait = 10;
                                }
                            }
                                
                        }
                    }
                }
                    
                    
                for(i = 0; i < 3; i++)
                {
                    if(blastset[i] == 1)
                    {
                        for(x = 0; x < MaxObjects; x++)
                        {
                            cact = (blastset[i] == 1 ? 1 : 0) * enemies[x].keio == 1 ? DetectCollision(enemies[x].posx+enemies[x].collisionshrink, enemies[x].posy+enemies[x].collisionshrink, enemies[x].posx+enemies[x].width-enemies[x].collisionshrink, enemies[x].posy+enemies[x].height-enemies[x].collisionshrink, blastx[i], blasty[i], blastx[i]+16, blasty[i]+8, 0, 0, 0, 0, 1, 0,0,0,0): 0;
                            if(cact != 0)
                            {
                                blastset[i] = 0;
                                enemies[x].health -= enemies[x].health == 255 ? 0 : 1;
                                /*                                    enemies[x].speedbase += (enemies[x].type == 0 ? 1 : 0 * 1 * (enemies[x].health&1));*/
                                if(enemies[x].health == 0)
                                {
                                        
                                    #ifdef SOUND
                                    off = 0;                    /*   PlaySound(MAKEINTRESOURCE(201),apphinst,SND_ASYNC|SND_RESOURCE); */
                                        
                                    for(t = 0; t < 24; t++)
                                    {
                                        if(Bufferinuse[t] == 0 && off == 0 && SoundStatus == 0)
                                        {
                                            if(Bufferused[t] == 1)
                                            {
                                                lpPlayBuffers[t]->lpVtbl->Release(lpPlayBuffers[t]);
                                            }

                                            if( (enemies[x].ospx == 109 && enemies[x].ospy == 128 && enemies[x].width == 16 && enemies[x].height == 24 && enemies[x].type == 1))
                                            {
                                               lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[11], &lpPlayBuffers[t]);
                                            }
                                            else if( (enemies[x].ospx == 109 && enemies[x].ospy == 96 && enemies[x].width == 16 && enemies[x].height == 16 && enemies[x].type == 0))
                                            {
                                               lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[13], &lpPlayBuffers[t]);
                                            }
                                            else
                                            lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[1], &lpPlayBuffers[t]);
                                            lpPlayBuffers[t]->lpVtbl->Play(lpPlayBuffers[t], 0, 0, 0);
                                            Bufferinuse[t] = 1;
                                            Bufferused[t] = 1;
                                                
                                            off = 1;
                                        }
                                    }
                                    if(enemies[x].boss == 1)
                                    {PlaySound(MAKEINTRESOURCE(209),apphinst,SND_RESOURCE);}
                                    #endif
                                        
                                    enemies[x].deathset = (enemies[x].posx < paulax ? 16: 0)+10;
                                    enemies[x].speedbase = 0;
                                    enemies[x].xspeed = 0;
                                    enemies[x].yspeed = 0;
                                    enemies[x].keio = 0;
                                }
                                else
                                {
                                    #ifdef SOUND
                                    off = 0;                   /*  PlaySound(MAKEINTRESOURCE(203),apphinst,SND_ASYNC|SND_RESOURCE|SND_NOSTOP);*/
                                    for(t = 0; t < 24; t++)
                                    {
                                        if(Bufferinuse[t] == 0 && off != 1 && SoundStatus == 0)
                                        {
                                            if(Bufferused[t] == 1)
                                            {
                                                lpPlayBuffers[t]->lpVtbl->Release(lpPlayBuffers[t]);
                                            }
                                                
                                            if( lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[2], &lpPlayBuffers[t]) == DS_OK)
                                            {
                                                lpPlayBuffers[t]->lpVtbl->Play(lpPlayBuffers[t], 0, 0, 0);
                                            }
                                            Bufferinuse[t] = 1;
                                            Bufferused[t] = 1;
                                            off = 1;
                                                
                                        }
                                    }
                                    #endif
                                        
                                    enemies[x].hitset = (((enemies[x].posx+enemies[x].collisionshrink) >= blastx[i] ? 16: 0)+4)*(enemies[x].health == 0 ? 0 : 1);
                                    enemies[x].hitx = blastx[i]+((enemies[x].posx+enemies[x].collisionshrink) >= blastx[i] ? 1 : 0 *(enemies[x].width+16))+((enemies[x].posx+enemies[x].collisionshrink) >= blastx[i] ? 0 : -1 *(enemies[x].width+5)) ;
                                    enemies[x].hity = blasty[i]+2;
                                }
                            }
                        }
                    }
                        
                }
                    
                for(i = 0; i < MaxObjects; i++)
                {
                    if( (objects[i].keio == 1 && objects[i].kill == 1) || (objects[i].keio == 1 && objects[i].healthpickup == 1) || (objects[i].keio == 1 && objects[i].OneUp == 1) )
                    {
                        cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width-1, objects[i].posy+objects[i].height-3, 1, 0, 0, 0, 0, 0, 0, paulay<(objects[i].posy+objects[i].height)?1:0, 0);
                        if(cact != 0)
                        {
                            if(objects[i].healthpickup == 1)
                            {
                                Health += 1;
                                objects[i].keio = 0;                                          
                            }
                            if(objects[i].OneUp == 1)
                            {
                                Lives++;                
                              #ifdef SOUND
                              if(SoundStatus == 0)                              
                              PlaySound(MAKEINTRESOURCE(208),apphinst,SND_RESOURCE);
                              #endif

                                objects[i].keio = 0;
                            }
                            if(objects[i].kill == 1)
                            {
                                #ifdef SOUND
                                KillTimer(hwnd, 900);
                                if(SoundStatus == 0)
                                {
                                    PlaySound(MAKEINTRESOURCE(206), apphinst, SND_RESOURCE);
                                }
                                else
                                {
                                    sleep(1000);
                                }
                                SetTimer(hwnd, 900, 55, NULL);
                                #endif    
                                if(Lives == 0)
                                {
                                    KillTimer(hwnd, 900);
                                    GeneratePasswordString(level[Levelnum], Levelnum, &PassWord2);
                                    sprintf(buf, "Paula gasps her few last breaths as her sight dims...\nGame Over\nPlay again?\n\nPassword for current level:  %s", PassWord2);    
                                        
                                    if( MessageBox(hwnd, buf, "Your story ends here.", MB_TASKMODAL|MB_YESNO) == IDYES)
                                    {
                                        Health = Healthmax; Lives = Livesmax; GhostStatus = 0; Levelnum = 0;
                                        LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx = 0;
                                        ChangeBGM();
                                        vertpaula = 0; twait = 10;
                                        SetTimer(hwnd, 900, 55, NULL);
                                    }
                                    else
                                    {   DestroyWindow(hwnd);  }
                                }
                                else
                                {
                                    GhostStatus = 0;     Lives -= (Lives == 0?0:1); Health = Healthmax; /*Prevents underflowage*/
                                    LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom); scrx = 0;
                                    ChangeBGM();
                                    vertpaula = 0; twait = 10;
                                }
                                    
                                    
                            }
                        }
                    }                             
                }
            }
	    
	    if(GetActiveWindow() == hwnd && GameStatus == GAME_MENU)
	    {
	        if(GetAsyncKeyState(VK_DOWN) != 0)
		{
		     MenuPosition += MenuPosition < 3 ? 1 : 0;
		}
                if(GetAsyncKeyState('u') != 0 || GetAsyncKeyState('U') != 0)
                {
                    doubleswitch = (doubleswitch + 1)&1;
                    if(doubleswitch == 1)
                    {
                       SetWindowPos(hwnd, HWND_TOP,0,0, 640+GetSystemMetrics(SM_CXBORDER),480+GetSystemMetrics(SM_CYCAPTION),SWP_NOMOVE);
                    }
                    if(doubleswitch == 0)
                    {
                       SetWindowPos(hwnd, HWND_TOP,0,0, 320+GetSystemMetrics(SM_CXBORDER),240+GetSystemMetrics(SM_CYCAPTION),SWP_NOMOVE);                                    
                    }
                    clear_async();
                    Sleep(500);
                    clear_async();
                }
	        
		if(GetAsyncKeyState(VK_UP) != 0)
		{
		     MenuPosition -= MenuPosition > 0 ? 1 : 0;
		}
		
		if(GetAsyncKeyState(32) != 0 || GetAsyncKeyState(13) != 0 )
	        {
		     if(MenuPosition == 0)
		     {
		         Levelnum = 0;
                         LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);
                         ChangeBGM();
                         twait = 10;
                         scrx = 0;		     
                         GameStatus = GAME_PLAY;
                         clear_async();
		     }
		     if(MenuPosition == 1)
		     {
		         Levelnum = 0;
		         KillTimer(hwnd,900);
			 /*Do password stuff here*/
			 PassWord[0] = 0;
             password_window();
             Levelnum = ((PassWord[0]-48)*100)+((PassWord[1]-48)*10)+(PassWord[2]-48);
             
             if(Levelnum <= MaxLvl)
              {
                 GeneratePasswordString(level[Levelnum], Levelnum, &PassWord2);
              }
			 
		 	 if( stricmp(PassWord, "DeathBurgerWithASideOfDecay") == 0)
		 	 {
                 if(Healthmax != 2)
                 {MessageBox(hwnd, "Insane Mortality Activated!", "Alert",MB_OK); Healthmax = 2; Health = 2;}
                 else
                 {MessageBox(hwnd, "Insane Mortality Deactivated!", "Alert", MB_OK); Healthmax = HM; Health = 2;}
             }
             else if( stricmp(PassWord, "IAmCuteAndFluffy") == 0)
             {
                 if(Invincible == 0)
                 {MessageBox(hwnd, "Invincibility Activated!", "Alert", MB_OK); Invincible = 1;}
                 else
                 {MessageBox(hwnd, "Invincibility Deactivated!", "Alert", MB_OK); Invincible = 0;}     
             }
             else if( stricmp(PassWord, "ImmortalityIsPancakes") == 0)
             {
                 if(Ulives == 0)
                 {MessageBox(hwnd, "Unlimited-Lives Activated!", "Alert", MB_OK); Ulives = 1;}
                 else
                 {MessageBox(hwnd, "Unlimited-Lives Deactivated!", "Alert", MB_OK); Ulives = 0;}     
             }
             else if( stricmp(PassWord, "SpiderWafflePaula") == 0)
             {
                 if(Spidey == 0)
                 {MessageBox(hwnd, "Freezing Spidey Paula Activated!", "Alert", MB_OK); Spidey = 1;}
                 else
                 {MessageBox(hwnd, "Freezing Spidey Paula Deactivated!", "Alert", MB_OK); Spidey = 0;}                       
             }
             else if( stricmp(PassWord, "IAreBugslayer") == 0)
             {
                 if(Dkey == 0)
                 {MessageBox(hwnd, "Debug Keys Activated!", "Alert", MB_OK); Dkey = 1;}
                 else
                 {MessageBox(hwnd, "Debug Keys Deactivated!", "Alert", MB_OK); Dkey = 0;}                       
             }
             else if(Levelnum > MaxLvl || stricmp(PassWord, PassWord2) != 0)
   	          {
                 MessageBox(hwnd, "Password invalid...", "ALERT", MB_OK);
              Levelnum = 0;
              }             
              else
              {
		        LoadLevel(&paulax, &paulay, objects, enemies, level[Levelnum], rc.bottom);
		        ChangeBGM();
                GameStatus = GAME_PLAY;
              }
              SetTimer( hwnd, 900, 55, NULL );
              clear_async();
		     }
		     if(MenuPosition == 2)
		     {
                        MessageBox(hwnd, thestory, "Help", MB_OK);
                        //MessageBox(hwnd, helpcntrl , "Help Box: Basic Controls", MB_OK);
                        MessageBox(hwnd, helpadv, "Advanced control and item notes:", MB_OK);
                        MessageBox(hwnd, windexp, "Display And Miscellaneous Info", MB_OK);
                        MessageBox(hwnd, credits, "Credits", MB_OK);
                        //MessageBox(hwnd, finalmessage, "And a word from our sponsors!", MB_OK);
 
		        clear_async();
		     }
		     if(MenuPosition == 3)
		     {
		       LoadCustom(&level);
		       //MessageBox(hwnd, "Not implemented yet.", "sorry, not implemented.", MB_OK);
		       clear_async();
		     }
		}
	    }
	    
	    
	    
            paulaspeed = 0;
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
            break; 
        }
        case WM_DESTROY:
        /* The window is being destroyed, close the application
                         * (the child button gets destroyed automatically). */
        DeleteObject(PaulaBMP);
        DeleteObject(IceBlastBMP);
        DeleteObject(Canvas);
        DeleteObject(BossPumpkin);
        DeleteObject(Tile);
        DeleteObject(FX);
        DeleteObject(Osprites);
        #ifdef SOUND
            
        for(i = 0; i < 24; i++)
        {
            if(Bufferused[i] == 1)
            {
                lpPlayBuffers[i]->lpVtbl->Release(lpPlayBuffers[i]);
            }
        }   
        for(i = 0; i < NumberOfSounds; i++)
        {
            ProtoBuffersStr[i]->lpVtbl->Release(ProtoBuffersStr[i]);
        }
        for(i = 0; i < 3; i++)
        {
            lpSecondaryBuffer[i]->lpVtbl->Release(lpSecondaryBuffer[i]);
        }
        lplpDirectSoundBuffer->lpVtbl->Release(lplpDirectSoundBuffer);
        for(i = 0; i < 10; i++)
        {
            FreeResource(wavemem[i]);
        }
        #endif
        KillTimer(hwnd, 900);
        PostQuitMessage (0);
        return 0;
        break;
            
        case WM_PAINT:
        /* The window needs to be painted (redrawn). */
    if(GameStatus == GAME_PLAY)
    { 
        #ifdef DoubleBuffer
        hdc = BeginPaint (hwnd, &ps);
        mem = CreateCompatibleDC( hdc );
        mem2 = CreateCompatibleDC( hdc );
            
            
        SelectObject(mem, FX);
        SelectObject(mem2, Canvas);
        BitBlt(mem, 0,0,320,240,mem2,bkgx,bkgy,SRCCOPY);
        SelectObject(mem2, Tile);
        for(i = 0; i < MaxObjects; i++)
        {
            if(objects[i].keio == 1)
            {
                if( (objects[i].posx+objects[i].width)-scrx > -12 && (objects[i].posx-scrx) < 430)
                {
                    if(objects[i].ani == 0)
                    {
                        BitBlt(mem, objects[i].posx-scrx, objects[i].posy-scry, objects[i].width, objects[i].height, mem2, objects[i].tilemaskx, objects[i].tilemasky, SRCAND);
                        BitBlt(mem, objects[i].posx-scrx, objects[i].posy-scry, objects[i].width, objects[i].height, mem2, objects[i].tilex, objects[i].tiley, SRCPAINT);
                    }
                    else
                    {
                        BitBlt(mem, objects[i].posx-scrx, objects[i].posy-scry, objects[i].width, objects[i].height, mem2, objects[i].tilemaskx+(objects[i].aniframe*objects[i].width), objects[i].tilemasky, SRCAND);
                        BitBlt(mem, objects[i].posx-scrx, objects[i].posy-scry, objects[i].width, objects[i].height, mem2, objects[i].tilex+(objects[i].aniframe*objects[i].width), objects[i].tiley, SRCPAINT);
                        objects[i].framecounter += randomz(2,0)+randomz(2,0);
                        if(objects[i].framecounter >= objects[i].framerate)
                        {                              
                            objects[i].aniframe++;
                            objects[i].framecounter = 0;
                        }
                        if(objects[i].aniframe >= objects[i].numframes)
                        {
                            objects[i].aniframe=0;
                        }
                            
                    }
                    for(q = 0; q < objects[i].PNumber; q++)
                    {
                        objects[i].ParticleData[q*5] = randomz(objects[i].width,0)+randomz(objects[i].width-2,0)+randomz(objects[i].width/4,0);   /*x and y relative positions of particles*/
                        objects[i].ParticleData[(q*5)+1] = randomz(objects[i].height,0)+randomz(objects[i].height-2,0)+randomz(objects[i].height/4,0);   
                        if(randomz(1,0)+randomz(1,0) == 1 || randomz(1,0)+randomz(1,0) == 2)
                        {
                            objects[i].ParticleData[(q*5)+2] = 255*randomz(2,0);
                            objects[i].ParticleData[(q*5)+3] = 255;
                            objects[i].ParticleData[(q*5)+4] = 255;                                                      
                        }
                        else
                        {
                            objects[i].ParticleData[(q*5)+2] = 255;
                            objects[i].ParticleData[(q*5)+3] = 255;
                            objects[i].ParticleData[(q*5)+4] = 0;                                                                    
                        }
                    }
                        
                }
                //                              printf("Draw obj%d %d, %d", i, objects[i].posx-scrx, objects[i].posy);
            }
        }
        //void DrawAlpha(HDC hdestdc, HDC hsrcdc, HDC alphamaskdc, int destx, int desty, int srcx, int srcy, int maskx, int masky, int width, int height)
        //DrawAlpha(mem, mem2, mem2, 0,0, 48,240, 72,240,24,24,3);
        /*Draw Particles*/
        for(i = 0; i < MaxObjects; i++)
        {
            if(objects[i].Particles == 1 && objects[i].keio == 1 && (objects[i].posx > scrx-24 && objects[i].posx < scrx+320) && (objects[i].posy > scry-24 && objects[i].posy < scry+264) )
            {
                for(q = 0; q < objects[i].PNumber; q++)
                {
                    SetPixel(mem, objects[i].posx-objects[i].width/2-scrx+objects[i].ParticleData[q*5], objects[i].posy-objects[i].height/2-scry+objects[i].ParticleData[(q*5)+1],RGB(objects[i].ParticleData[(q*5)+2],objects[i].ParticleData[(q*5)+3],objects[i].ParticleData[(q*5)+4]));
                }                        
            }  
        }
            
            
        /*Draw enemies*/
        SelectObject(mem2, Osprites);
        for(i = 0; i < MaxObjects; i++)
        {
            if(enemies[i].keio == 1)
            {
                if( (enemies[i].posx+enemies[i].width)-scrx > -12 && (enemies[i].posx-scrx) < 430 && enemies[i].boss == 0)
                {BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height,mem2, enemies[i].ospxm+(enemies[i].width * enemies[i].sprite)    , enemies[i].ospym    , SRCPAINT);
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height , mem2, enemies[i].ospx+(enemies[i].width * enemies[i].sprite) , enemies[i].ospy , SRCAND);}
                if( enemies[i].keio == 1 && enemies[i].boss == 1)
                {
                    SelectObject(mem2, BossPumpkin);
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height,mem2, enemies[i].ospxm+(enemies[i].width * (enemies[i].sprite&1)), enemies[i].ospym, SRCPAINT);
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height, mem2, enemies[i].ospx+(enemies[i].width * (enemies[i].sprite&1)) , enemies[i].ospy , SRCAND);                    
                    if((enemies[i].sprite&64) == 64)
                    {
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height, mem2, (enemies[i].xspeed > 0 ? 1 : (enemies[i].xspeed < 0 ? 0:2) )*160, 480, SRCAND);
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height, mem2, (enemies[i].xspeed > 0 ? 1 : (enemies[i].xspeed < 0 ? 0:2) )*160, 320, SRCPAINT);
                    }                    
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height, mem2, (enemies[i].xspeed > 0 ? 1 : (enemies[i].xspeed < 0 ? 0:2) )*160, 642+160, SRCAND);
                    BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height, mem2, (enemies[i].xspeed > 0 ? 1 : (enemies[i].xspeed < 0 ? 0:2) )*160, 642, SRCPAINT);
                    DrawAlpha(mem, mem2,mem2,enemies[i].posx-scrx,enemies[i].posy-scry+85,randomz(512-160, 0), 960+randomz(512-160,0), 320,85,160,160-85,0);                    
                    SelectObject(mem2, Osprites);
                }
                //printf("ENENYDRAW%d %d, %d|||", i, enemies[i].posx-scrx, enemies[i].posy);
            }
            if(enemies[i].shootset == 1)
            {
                BitBlt(mem, enemies[i].shootx-scrx, enemies[i].shooty-scry, enemies[i].shootimgwidth, enemies[i].shootimgheight, mem2, enemies[i].shootimgx+enemies[i].shootimgwidth, enemies[i].shootimgy, SRCPAINT);
                BitBlt(mem, enemies[i].shootx-scrx, enemies[i].shooty-scry, enemies[i].shootimgwidth, enemies[i].shootimgheight, mem2, enemies[i].shootimgx, enemies[i].shootimgy, SRCAND);
            }
            if(enemies[i].deathset != 0)
            {
                /*                              BitBlt(mem, (enemies[i].posx+((enemies[i].deathset&16) == 16?0:1)+ enemies[i].width)-scrx)+( (enemies[i].deathset&15)*-2*( (enemies[i].deathset&16) == 16 ? -1 : 1 )), enemies[i].posy+(enemies[i].type == 1? 16:0), 16,24,mem2, 160, 40+( ( (enemies[x].deathset&16) >>4) *24), SRCPAINT);*/
                BitBlt(mem, ((enemies[i].posx+(enemies[i].deathset>16?0:1 * enemies[i].width))-scrx)+ ( (10-(enemies[i].deathset&15))*2* (enemies[i].deathset >= 16 ? -1 : 1)) , enemies[i].posy+(enemies[i].type == 1? 16:0)-scry, 16,24,mem2, 160, 40+( (enemies[i].deathset>16?1:0) *24), SRCPAINT);
                //printf("Deathset == %d", enemies[i].deathset);
                enemies[i].deathset = (enemies[i].deathset&16)+((enemies[i].deathset&15)-1);
                if((enemies[i].deathset&15) == 0)
                {enemies[i].deathset = 0;}
            }
            if(enemies[i].hitset != 0)
            {
                BitBlt(mem, enemies[i].hitx-scrx+((4-(enemies[i].hitset&7))*7*(enemies[i].hitset > 16 ? 1:-1)),enemies[i].hity-scry,16,16,mem2,160,24,SRCPAINT);
                enemies[i].hitset = (enemies[i].hitset&16)+((enemies[i].hitset&15)-1);
                if((enemies[i].hitset&15) == 0)
                {enemies[i].hitset = 0;}
            }
        }
            
        if(hitOK != 0)
        {
            SelectObject(mem2, Osprites);
            BitBlt(mem, paulax-scrx,paulay-20-scry,15,15,mem2,79,80,SRCPAINT);
            BitBlt(mem, paulax-scrx,paulay-20-scry,15,15,mem2,64,80,SRCAND);
            for(i = 0; i < hitOK>>2; i++)
            {
                BitBlt(mem, i*8, rc.bottom-8, 8,8,mem2,168,0,SRCCOPY);
            }
                
        }
        for(i = 0; i < Health+1; i++)
        {
            BitBlt(mem, rc.right-(i*8), 0, 8,8,mem2, 160, 8, SRCCOPY);
        }
        for(i = 0; i < Lives+1; i++)
        {
            BitBlt(mem, rc.right-(i*8), 8, 8,8,mem2, 168, 8, SRCCOPY);
        }
        for(i = 0; i < abbs(vertpaula)-2; i++)
        {
            BitBlt(mem, (i*4), rc.bottom-16,4,8,mem2, 160, 0, SRCCOPY);
        }
/*        for(i = 0; i < Levelnum+1; i++)
        {
            BitBlt(mem, (i*20)+(15*8), rc.bottom-8, 16, 8, mem2, 160, 88, SRCCOPY);
        }*/
            sprintf(buf, "%d", Levelnum+1);
            x = 0;
            while(1)
            {
                    if(buf[x] == 48)
                    {
                        //DrawAlpha(mem, mem2,mem2, 100+(x*16),rc.bottom-24,160-16,192,160-16,192+24,16,24,0);
                        BitBlt(mem, 100+(x*16), rc.bottom-24, 16, 24, mem2, 160-16,192+48, SRCAND);
                        BitBlt(mem, 100+(x*16), rc.bottom-24, 16, 24, mem2, 160-16,192, SRCPAINT);
                    }
                    if(buf[x] >= 49 && buf[x] <= 57)
                    {
                        //DrawAlpha(mem, mem2, mem2, 100+(x*16), rc.bottom-24, (16*(buf[x]-49)), 192, (16*(buf[x]-49)), 192+24,16,24, 0);     
                        BitBlt(mem, 100+(x*16), rc.bottom-24, 16, 24, mem2, (16*(buf[x]-49)),192+48, SRCAND);
                        BitBlt(mem, 100+(x*16), rc.bottom-24, 16, 24, mem2, (16*(buf[x]-49)),192, SRCPAINT);
                    }
                    if(buf[x] == 0)
                    break;
                    x++;
            }
            
        /*Draw paula*/
            
        SelectObject(mem2, GhostStatus == 0 ? PaulaBMP : Osprites);
        BitBlt(mem, paulax-scrx,paulay-scry,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),prect.top+prect.bottom,SRCPAINT);
        BitBlt(mem, paulax-scrx,paulay-scry,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),0,SRCAND);
        if(flower == 1 && wallage == 1 && vertpaula == 0)
        {
            SelectObject(mem2, Osprites);
            if(pauladirection == 1)
            DrawAlpha(mem, mem2,mem2,paulax-scrx,paulay-scry,0, 168, 16,168,16,24,0);                                
            else
            DrawAlpha(mem, mem2,mem2,paulax-scrx,paulay-scry,32, 168, 48,168,16,24,0);                                
            
        }
        /*Draw shots*/
        SelectObject(mem2, IceBlastBMP);
        for(i = 0; i < 3; i++)
        {
            if(blastset[i] == 1)
            {
                BitBlt(mem,blastx[i]-scrx,blasty[i]-scry,32,16,mem2,blastdirection[i]*32,32+(blaststatus[i]*16),SRCPAINT); 
                BitBlt(mem,blastx[i]-scrx,blasty[i]-scry,32,16,mem2,blastdirection[i]*32,(blaststatus[i]*16),SRCAND);
                if(blastx[i]-scrx > 320 || blastx[i]-scrx < -8)
                {blastset[i] = 0;}
            }
        }
        if(doubleswitch == 0)
           BitBlt(hdc, 0, 0, 320, 240, mem, 0, 0, SRCCOPY);
        else
           {
StretchBlt(hdc, 0, 0, 635, 474, mem, 0,0,315,234,SRCCOPY);
           
           }
        
            
        DeleteDC(mem);
        DeleteDC(mem2);
        EndPaint (hwnd, &ps);
        return 0;
        break;
        #endif
            
        /************************************************************/
            
            
        #ifndef DoubleBuffer
        hdc = BeginPaint (hwnd, &ps);
        mem2 = CreateCompatibleDC( hdc );
            
        SelectObject(mem2, Canvas);
        BitBlt(hdc, 0,0,320,240,mem2,0,0,SRCCOPY);
        SelectObject(mem2, Tile);
        for(i = 0; i < MaxObjects; i++)
        {
            if(objects[i].keio == 1)
            {
                if( (objects[i].posx+objects[i].width)-scrx > -12 && (objects[i].posx-scrx) < 430)
                {
                    BitBlt(hdc, objects[i].posx-scrx, objects[i].posy, objects[i].width, objects[i].height, mem2, objects[i].tilemaskx, objects[i].tilemasky, SRCAND);
                    BitBlt(hdc, objects[i].posx-scrx, objects[i].posy, objects[i].width, objects[i].height, mem2, objects[i].tilex, objects[i].tiley, SRCPAINT);
                        
                }
                //                              printf("Draw obj%d %d, %d", i, objects[i].posx-scrx, objects[i].posy);
            }
        }
        /*Draw enemies*/
        SelectObject(mem2, Osprites);
        for(i = 0; i < MaxObjects; i++)
        {
            if(enemies[i].shootset == 1)
            {
                BitBlt(hdc, enemies[i].shootx-scrx, enemies[i].shooty, 15, 15, mem2, 94+15, 80, SRCPAINT);
                BitBlt(hdc, enemies[i].shootx-scrx, enemies[i].shooty, 15, 15, mem2, 94, 80, SRCAND);
            }
            if(enemies[i].keio == 1)
            {
                if( (enemies[i].posx+enemies[i].width)-scrx > -12 && (enemies[i].posx-scrx) < 430)
                {BitBlt(hdc, enemies[i].posx-scrx, enemies[i].posy,enemies[i].width, enemies[i].height,mem2, enemies[i].ospxm+(enemies[i].width * enemies[i].sprite)    , enemies[i].ospym    , SRCPAINT);
                    BitBlt(hdc, enemies[i].posx-scrx, enemies[i].posy,enemies[i].width, enemies[i].height , mem2, enemies[i].ospx+(enemies[i].width * enemies[i].sprite) , enemies[i].ospy , SRCAND);}
                //printf("ENENYDRAW%d %d, %d|||", i, enemies[i].posx-scrx, enemies[i].posy);
            }
            if(enemies[i].deathset != 0)
            {
                /*                              BitBlt(mem, (enemies[i].posx+((enemies[i].deathset&16) == 16?0:1)+ enemies[i].width)-scrx)+( (enemies[i].deathset&15)*-2*( (enemies[i].deathset&16) == 16 ? -1 : 1 )), enemies[i].posy+(enemies[i].type == 1? 16:0), 16,24,mem2, 160, 40+( ( (enemies[x].deathset&16) >>4) *24), SRCPAINT);*/
                BitBlt(hdc, ((enemies[i].posx+(enemies[i].deathset>16?0:1 * enemies[i].width))-scrx)+ ( (10-(enemies[i].deathset&15))*2* (enemies[i].deathset >= 16 ? -1 : 1)) , enemies[i].posy+(enemies[i].type == 1? 16:0), 16,24,mem2, 160, 40+( (enemies[i].deathset>16?1:0) *24), SRCPAINT);
                //printf("Deathset == %d", enemies[i].deathset);
                enemies[i].deathset = (enemies[i].deathset&16)+((enemies[i].deathset&15)-1);
                if((enemies[i].deathset&15) == 0)
                {enemies[i].deathset = 0;}
            }
            if(enemies[i].hitset != 0)
            {
                BitBlt(hdc, enemies[i].hitx-scrx+((4-(enemies[i].hitset&7))*7*(enemies[i].hitset > 16 ? 1:-1)),enemies[i].hity,16,16,mem2,160,24,SRCPAINT);
                enemies[i].hitset = (enemies[i].hitset&16)+((enemies[i].hitset&15)-1);
                if((enemies[i].hitset&15) == 0)
                {enemies[i].hitset = 0;}
            }
        }
            
        if(hitOK != 0)
        {
            SelectObject(mem2, Osprites);
            BitBlt(hdc, paulax-scrx,paulay-20,15,15,mem2,79,80,SRCPAINT);
            BitBlt(hdc, paulax-scrx,paulay-20,15,15,mem2,64,80,SRCAND);
            for(i = 0; i < hitOK>>2; i++)
            {
                BitBlt(hdc, i*8, rc.bottom-8, 8,8,mem2,168,0,SRCCOPY);
            }
                
        }
        for(i = 0; i < Health+1; i++)
        {
            BitBlt(hdc, rc.right-(i*8), 0, 8,8,mem2, 160, 8, SRCCOPY);
        }
        for(i = 0; i < Lives+1; i++)
        {
            BitBlt(hdc, rc.right-(i*8), 8, 8,8,mem2, 168, 8, SRCCOPY);
        }
        for(i = 0; i < abbs(vertpaula)-2; i++)
        {
            BitBlt(hdc, (i*4), rc.bottom-16,4,8,mem2, 160, 0, SRCCOPY);
        }

            
        /*Draw paula*/
            
        SelectObject(mem2, GhostStatus == 0 ? PaulaBMP : Osprites);
        BitBlt(hdc, paulax-scrx,paulay,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),prect.top+prect.bottom,SRCPAINT);
        BitBlt(hdc, paulax-scrx,paulay,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),0,SRCAND);
        /*Draw shots*/
        SelectObject(mem2, IceBlastBMP);
        for(i = 0; i < 3; i++)
        {
            if(blastset[i] == 1)
            {
                BitBlt(hdc,blastx[i]-scrx,blasty[i],32,16,mem2,blastdirection[i]*32,32+(blaststatus[i]*16),SRCPAINT); 
                BitBlt(hdc,blastx[i]-scrx,blasty[i],32,16,mem2,blastdirection[i]*32,(blaststatus[i]*16),SRCAND);
                if(blastx[i]-scrx > 320 || blastx[i]-scrx < -8)
                {blastset[i] = 0;}
            }
        }
            
        DeleteDC(mem2);
        EndPaint (hwnd, &ps);
        return 0;
        break;
               
            
            
            
        #endif
    }
    if(GameStatus == GAME_MENU)
    {
        hdc = BeginPaint (hwnd, &ps);
        mem = CreateCompatibleDC( hdc );
        mem2 = CreateCompatibleDC( hdc );
        
        SelectObject(mem, FX);
        SelectObject(mem2,Canvas);
        BitBlt(mem, 0,-6, 320, 240, mem2, 320, 480, SRCCOPY);
        SelectObject(mem2,Osprites);
        if(MenuPosition == 0)
        {
           BitBlt(mem, 78,115,15,15, mem2, 130,80, SRCPAINT);
        }          
        if(MenuPosition == 1)
        {
           BitBlt(mem, 30,155,15,15, mem2, 130,80, SRCPAINT);
        }          
        if(MenuPosition == 2)
        {
           BitBlt(mem, 105,192,15,15, mem2, 130,80, SRCPAINT);
        }          
        if(MenuPosition == 3)
        {
           BitBlt(mem, 0,222,15,15, mem2, 130,80, SRCPAINT);
        }          
           if(doubleswitch == 0)
           BitBlt(hdc, 0, 0, 320, 240, mem, 0, 0, SRCCOPY);
        else
           {
StretchBlt(hdc, 0, 0, 635, 474, mem, 0,0,315,234,SRCCOPY);
           
           }
DeleteDC(mem);
        DeleteDC(mem2);
        EndPaint (hwnd, &ps);
        return 0;
        break;                  
    }
            
        case WM_COMMAND:
        /* Check the control ID, notification code and
                         * control handle to see if this is a button click
                         * message from our child button. */
        return 0;
        break;
    }
        
    /* If we don't handle a message completely we hand it to the system
         * provided default window function. */
    return DefWindowProc (hwnd, nMsg, wParam, lParam);
}


int STDCALL
WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{
    //printf("WinMain start");
    HWND         hwndMain;        /* Handle for the main window. */
    MSG          msg;             /* A Win32 message structure. */
    WNDCLASSEX   wndclass;        /* A window class structure. */
    char*        szMainWndClass = "Zombies!";
    /* The name of the main window class */
        
    /*
         * First we create a window class for our main window.
         */
        
    /* Initialize the entire structure to zero. */
    memset (&wndclass, 0, sizeof(WNDCLASSEX));
        
    wndclass.lpszClassName = szMainWndClass;
        
    wndclass.cbSize = sizeof(WNDCLASSEX);
        
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
        
    wndclass.lpfnWndProc = MainWndProc;
    PaulaBMP = LoadBitmap(hInst, MAKEINTRESOURCE(101));
    IceBlastBMP = LoadBitmap(hInst, MAKEINTRESOURCE(102));
    Canvas = LoadBitmap(hInst, MAKEINTRESOURCE(100));
    Tile = LoadBitmap(hInst, MAKEINTRESOURCE(103));
    Osprites = LoadBitmap(hInst, MAKEINTRESOURCE(104));
    BossPumpkin = LoadBitmap(hInst, MAKEINTRESOURCE(1001));
    #ifdef SOUND
    waveres[0] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(202), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[0] = LoadResource(hInst, waveres[0]);
    wavesize[0] = 20513-44;
    waveres[1] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(200), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[1] = LoadResource(hInst, waveres[1]);
    wavesize[1] = 55410-44;
    waveres[2] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(201), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[2] = LoadResource(hInst, waveres[2]);
    wavesize[2] = 31039-44;
    waveres[3] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(203), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[3] = LoadResource(hInst, waveres[3]);
    wavesize[3] = 11284-44;
    waveres[4] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(204), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[4] = LoadResource(hInst, waveres[4]);
    wavesize[4] =  8658-44;
    waveres[5] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(300), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[5] = LoadResource(hInst, waveres[5]);
    wavesize[5] =  826927-44;
    waveres[6] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(301), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[6] = LoadResource(hInst, waveres[6]);
    wavesize[6] =  576110-44;
    waveres[7] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(302), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[7] = LoadResource(hInst, waveres[7]);
    wavesize[7] =  1444232-44;
    waveres[8] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(303), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[8] = LoadResource(hInst, waveres[8]);
    wavesize[8] =  326948-44;
    waveres[9] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(304), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[9] = LoadResource(hInst, waveres[9]);
    wavesize[9] =  297532-44;
    waveres[10] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(305), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[10] = LoadResource(hInst, waveres[10]);
    wavesize[10] =  502220-44;
    waveres[11] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(210), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[11] = LoadResource(hInst, waveres[11]);
    wavesize[11] =  38156-44;
    waveres[12] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(213), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[12] = LoadResource(hInst, waveres[12]);
    wavesize[12] =  24754-44;
    waveres[13] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(214), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[13] = LoadResource(hInst, waveres[13]);
    wavesize[13] =  16596-44;
    waveres[14] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(215), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[14] = LoadResource(hInst, waveres[14]);
    wavesize[14] =  11084-44;
    waveres[15] = FindResourceEx(hInst, "WAVE", MAKEINTRESOURCE(216), MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL));
    wavemem[15] = LoadResource(hInst, waveres[15]);
    wavesize[15] =   6262-44;

    int i;
    for(i = 0; i < NumberOfSounds+1; i++)
    {
        if(wavemem[i] == NULL)
        {wavesize[i] = 5;}          
    }

    #endif
    wndclass.hInstance = hInst;
        
    wndclass.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(9));
    wndclass.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(9));
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
        
    wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        
    RegisterClassEx (&wndclass);
        
    hwndMain = CreateWindow (
    szMainWndClass,             /* Class name */
"ZOMBINANZA!",                    /* Caption */
    WS_POPUP|WS_SYSMENU|WS_VISIBLE|WS_CAPTION|WS_MINIMIZEBOX,        /* Style */
    CW_USEDEFAULT,
    CW_USEDEFAULT,              /* Initial y (use default) */
    320+GetSystemMetrics(SM_CXBORDER),              /* Initial x size (use default) */
    240+GetSystemMetrics(SM_CYCAPTION),              /* Initial y size (use default) */
    NULL,                       /* No parent window */
    NULL,                       /* No menu */
    hInst,                      /* This program instance */
    NULL                        /* Creation parameters */
    );
        
    ShowWindow (hwndMain, nShow);
    UpdateWindow (hwndMain);
    apphinst = hInst;
        
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return msg.wParam;
}

/*
:17) ProgrammerX15: I still use the Factory defaults (well, slightly modified factory defaults, Windows made it bigger for me a while back when I ran on 512 MB of RAM and filled the swapfile to the brim_
(00:03:40) WhiteKnight8103: i use system defaults for the most part, to lazy to mess with most of the
(00:03:42) WhiteKnight8103: them*
(00:03:57) WhiteKnight8103: i have well over half my hard drive to fill up, so i dont mind giving windows a lot of space
(00:04:13) ProgrammerX15: It was funny, Windows raised my limit from 1 GB to 1.1 GB so I could runthe GIMP :)
(00:04:23) WhiteKnight8103: i give it 2 gigs for system recovery (which doesnt work, btw (or i heard it works 1 out of 10, and it didnt work when i tried it a ways back))
(00:04:48) WhiteKnight8103: 1 out of 10 times that is
(00:04:55) ProgrammerX15: hehe I see
(00:04:59) WhiteKnight8103: puedo ir al bano por favor!
(00:05:24) ProgrammerX15: lol, I started compressing this PNG minutes ago
(00:05:27) WhiteKnight8103: ::watches the number of current handles update...::
(00:05:40) ProgrammerX15: hehe :)
(00:05:46) ProgrammerX15: Constantly changing:)
(00:05:50) WhiteKnight8103: si
(00:05:54) ProgrammerX15: 42530
(00:05:55) WhiteKnight8103: god damn spanish homework
(00:05:57) WhiteKnight8103: rofl
(00:05:58) ProgrammerX15: 42522
(00:06:03) ProgrammerX15: 42565
(00:06:07) WhiteKnight8103: 10960
(00:06:15) WhiteKnight8103: damn, nearly made 11000
(00:06:23) WhiteKnight8103: 10894
(00:06:24) WhiteKnight8103: lol
(00:06:28) ProgrammerX15: lol
(00:06:29) WhiteKnight8103: sill goose of a computer
(00:06:43) ProgrammerX15: I don't think I push my computer hard....
(00:07:47) WhiteKnight8103: yeah, right, if it had a gun it would shoot you
(00:07:48) WhiteKnight8103: lol
(00:07:55) WhiteKnight8103: just like mine
(00:07:56) ProgrammerX15: I really should get rid of that Ebates Mo Money Maker program... but I'm too lazy :-P
(00:08:01) WhiteKnight8103: except it would probably shoot me in the foot
*/









