/*OneSweetLM: Think file header with comments. Call-outs, called from, purpose, variable definitions and explainations. Makes things a lot easier.*/

#define MaxObjects 600
#include <windows.h> 
#include <mmsystem.h> 
#include <string.h> 
//#define SOUND 9
#define debugkeys 9
#define CCR 64
#define exp1 8
//#define OmegaCollisionSystem 9 /*memo to self: delete this collision code, it don't work.*/
//The CCR is the collision check range, the smaller the value, the fewer the objects checked for collision status.
#define DoubleBuffer 9
#define RUNTIME_PSEUDO_RELOC_SUPPORT
//I'll just globalize these variables so I don't have to change every load level call.
int bkgx = 0;
int bkgy = 0;

#ifdef SOUND
#include <dsound.h>  /*define SOUND to enable sound code*/
#endif
/*
This program had troubles running on some computer setups for some reason, including my old 700 mhz machine, this seems to fix the problem
I got gdb to tell me it was something wrong with _pei386_runtime_relocator, the error Windows told me was 0197:00408928
obtained from: http://sources.redhat.com/ml/binutils/2002-07/msg00035/crtest.c
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
DSBUFFERDESC bufferdesc, bufferdesc2, ProtoBuffers[5];
LPDIRECTSOUNDBUFFER lplpDirectSoundBuffer, lpSecondaryBuffer[3], ProtoBuffersStr[5];
LPDIRECTSOUNDBUFFER lpPlayBuffers[24];
BYTE Bufferinuse[24];
BYTE Bufferused[24];
#endif

#ifdef SOUND
DWORD rdl, rdl2;
DWORD off = 0;
WAVEFORMATEX WaveFormat, BGMFormat;
VOID *RawData, *RawData2;
BYTE *MemPtr;
HRSRC waveres[6];  int wavesize[6];
HGLOBAL wavemem[6];
BYTE destroycount[24];
#endif

int jumpcounter = 0;

typedef struct tagitemject{
int posx;
int posy;
int tilex, tiley;
int tilemaskx, tilemasky;
int width;
int height;
BYTE healthpickup;
BYTE solid;
BYTE keio;
BYTE ghost;
BYTE ghostt;
BYTE exit;
BYTE kill;
BYTE jumper;
BYTE donly;
BYTE climber;
BYTE ani;   /*Animation adding I am!*/
BYTE numframes;
BYTE aniframe;
BYTE framecounter; /*To slow them down:-P*/
BYTE framerate;    /*Wellll... delay*/
BYTE Particles; /* Some sort of particle effect? You bet :)*/
BYTE PType;     /*Hard coded type of particle, 1 = random, jumpy type*/
BYTE PNumber;   /*Number of Particles 1 - 9*/
short int ParticleData[45]; /*Array for Particle Data, 5 bytes per particle*/
} ObjectStruct;


typedef struct tagenemyobject{
int posx,posy;
int ospx, ospy;
int ospxm, ospym;
int forceposy;
BYTE fb;
BYTE counter;
int width, height;
int xspeed, yspeed;
BYTE direction, prev;
BYTE keio;
BYTE type, sprite;
BYTE lunge;
BYTE health;
BYTE speedbase;
int shootx, shooty, shootset, shootxspeed, shootyspeed, shootcount;
BYTE deathset, hitset;
int hitx, hity;
} EnemyStruct;
/*RNG*/

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
unsigned char level[20][3000]={      
"4\
......................................................................\n\
......................................................................\n\
......................................................................\n\
...........................M..........................................\n\
...............................M......................................\n\
..........................**..........................................\n\
............M.........-wwwwww.......-................M................\n\
......................-.....G..........G...................M.........E\n\
...M..........M.......-.....G..........G.................p........p.pp\n\
......................-.....G...M..-...G..............p.......*.......\n\
......................-.....G........M.G...........p.......p..pww.....\n\
.................p...w-.....G.R..**....G........C.....................\n\
..............p..........hh.Gwwwwww**..G..*R*p........................\n\
...........p............www........wwwwwwwwww.........................\n\
.....S..p.............................................................\n\
.....W................................................................\n\
...............M......................................................\n\
......................................................................\n\
......................................................................\n\
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzQ",

"4\
-k...........kk...kkkk................................................\n\
.S....................................................................\n\
......................................................................\n\
................-.....................................................\n\
......................................................................\n\
......................................................................\n\
...........M.......**...-..........M...h...M..........................\n\
..................WWWWWW-..............-..............................\n\
............-..x........-.............................................\n\
.WWWW...................-.........M...........M........M..............\n\
................M.......-......................xx.....................\n\
.......W..k-............-...**.............h............h.............\n\
...........k...........WWWWWWW..x....xx....xx.......x...x.....h.......\n\
................................................................**E*..\n\
-............................................................--------.\n\
......................................................................\n\
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzQLava , the monkey frontier!",
"1\
............W.........................................................\n\
........Mw..W.........................................................\n\
.........wwwW....M.....................M....................M.........\n\
....M.......WxM.......................................................\n\
............W..................M...........-.......M......h...........\n\
.........M..W....pp..M.....................M..............-..w........\n\
....M.......Wh......M......M.........M.........M...............M.....\n\
............W.......p.....................-............M.h.....w......\n\
...........MWx........pp..h.......S.........M............-.....w......\n\
.......h....W...M.........-..*M...W......h...xh.....M.......M..w......\n\
......Mp....Wh........M......----.....W..-....-..-........h....w......\n\
..++++W.....W...............M......................h...M..-..-.w......\n\
...*h.W.p...W..M........................M..........-...........w......\n\
++++..W..h..Wx.......M...........M.........M...........*.......w......\n\
......W..p...........................................---....-.........\n\
...M..W.....M*..................M........................-.....M...E..\n\
..E.*.W....ppp..................................M................pppp.\n\
..++++W...............................................................\n\
......W...............................................................\n\
^^^^^^W^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^QThe Gauntlet:\nRUN! RUN IF YOU WANT TO LIVE!",
"3\
......ay..............................................................\n\
.....aJJty........www.........*.......*...............................\n\
....aJTTJi.....-..wMw..wwwwwwwwww...wwwwwwwwwww...>...M..<............\n\
y...AY..Ii.......wwwwww...............vv......w...p......j............\n\
i.....S.Ii.......wMwwMw...M.....M.............w...............>..M.<.a\n\
i...atttJi....-..wwwwww.................*.....w............M..-....j.I\n\
i...IJJJJi....h.............-kk-......-----...w......................I\n\
i.-jjjjjjjjj----------------wwww---...........w.........h........M...I\n\
i................vv..........................w...............-.......I\n\
i.....................M................M....w.....M.................MI\n\
ipppp.........M.*>...........M.............w............p.........E..I\n\
i............*.-----..*..*.......M........w^^^^w............j.....j..I\n\
i-.....--------.....^------^^--..---..----.jjjj.........^............I\n\
i..................---....----..........................j............I\n\
i....................................................................I\n\
i.-........................................-.........................I\n\
i...............................M....................................I\n\
i........<.......*.........*...<...*.........*.....<.*.......*.......I\n\
JttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttJQLook out above?:\nWatch those last steps, they can be a doozy.\n",
"1\
..............................IJJi....................................j\n\
..............................IJJi....................................j\n\
...........>*....M............IJJi........x...........................j\n\
...ay.S....ay...-.............IJJi.......pp...........................j\n\
...Ii......Ii.................IJJi...M.-..............................j\n\
...IJttttttJi.................IJJi........................M....M......j\n\
...ATTTJJTTTY.................IJJi.................-..................j\n\
.......AY........x.<.*..*.....IJJi......-................*.*..*.......j\n\
.................atttttttty...IJJi..M.................pppppppppp......j\n\
.................ATTTTTTTTY...ATTY....................................j\n\
.......................................-..........................-...j\n\
................x.......M...................ppp...x...................j\n\
.............jjjj..>..........>.......>.............................E.j\n\
...................j......j...j...j...jj...........................wwwj\n\
.......................j..............................................j\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^jQGeran-err...umm That Falling Word:\nThat pesky gravity can be your friend or foe.  Which shall it be?",
"1\
......................................................................Ii\n\
...............................................WWWWWWWW...............Ii\n\
...............................................w......W...............Ii\n\
...............................................w......G...............Ii\n\
...............................................wR.....G...............Ii\n\
...........................WWW...WWWWWWWWWWWWWWWWppppWWWWGGGWWWWWWWWWWWW\n\
...........................W........w...........w..-....w.p...........Ii\n\
..........................-W........w...M.......w....M..w^^...........Ii\n\
.....j...j..j..............w....-............w..w.......wwwwww....-...Ii\n\
.S......................-..W...**..*..W...*..w..w...................M.Ii\n\
..............-............wwwwwwwwwwwwwwwwwww..w................-....Ii\n\
...........................W......w.............w.x....*.-............Ii\n\
.....................WWWwwww.....M............-.wGGGGGGGGw............Ii\n\
...............-....-w................w...w..*..w........w..www...-...Ii\n\
.....................w...M........w..*..*.wwwwwww.R.....Ew.*w.......M.Ii\n\
.....................w......attyWWWWWWWWWWW...w.wWWWWWWWWWwww.........Ii\n\
................wWWWWw.....-IJJi..............w..........w.......-....Ii\n\
................w...........ATTY..............w.......................Ii\n\
.............x..w....*.......*..*..w....*.*.w....w...*w.....w.........Ii\n\
ttttttttttttttttttttttt^ttttttttttttttttttttttttttttttttttttttttttttttJiQIce Fortress:\nWhat kind of game would this be without a poorly constructed fortress of some sort?",
"1\
ivvvvvvvvvvvvvvvvvvvvvvvvvwvvvvvvvvwvvwvvvvvvIJi\n\
i.........................w........w..v......IJi\n\
i.........................w.M..M...v.........IJi\n\
i..............M..........w...............M..IJi\n\
i....S....*...............w...*..*...........IJi\n\
i..atttttty...............wppppppppp...--....IJi\n\
i..IJJJJJJi.wwww..........w..................IJi\n\
i..ATTJJTTY...........M...w.......-......ay..IJi\n\
i.....AY..........-.......w..............Ii..IJi\n\
i.........................w...........*.*Ii..IJi\n\
i.........................w.M...h..ppppppIi..IJi\n\
i................-........w.....-........Ii..IJi\n\
i............M............w..............Ii..IJi\n\
iGG-w.....................w............M.Ii..IJi\n\
i...w..M...M.......-.......M...-.........Ii..IJi\n\
iR..w.-...........................^^.....Ii-.IJi\n\
i-..w.......ay-.....ay............ay^^^^^Ii..IJi\n\
i...w.......Ii^^^-^^Ii........-...IJjjjjjJi..IJi\n\
i..Ew...x...IJttttttJi............Ii.........IJi\n\
i.ppw...-...ATTTTTTTTY.x....x.....GG.........IJi\n\
i...w..................-....-....-GG.R.*....-IJi\n\
i^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^IJttty^^-^^IJiQAlpha Beta Spikey:\nBeware of Spikes.",
"2\
BooooooooooooooooooooooooooooooooooooooooooooB\n\
O...G..............-.....vvvvvvvv............O\n\
ORS.G............M.-........................RO\n\
Owwww....M...M..r..-.......-..............BBBO\n\
O...............r..-M..........M.............O\n\
O......r........r..-......*............x*....O\n\
O..............Mr..-.M...---.r..r..r..rrr....O\n\
O...r.....r.....r............................O\n\
OE..*^^^^....*..r.....*..^^^^^^^^^..*..^.*...O\n\
BooooooooooooooooooooooooooooooooooooooooooooBQTight Binary Quarters:\nI always was a close combat sort.",
"2\
BeeeeeeeeeeeBeeeeeeeeeBeeeeeeeeeeeeeeeeeeeeeeB\n\
q...........q.........q......................q\n\
q.S.B.................q......................q\n\
q...BR...........r....q.............r....R...q\n\
q...CCC..CMC......BeM......M.........r..BoB..q\n\
q.............*...O.....B.................O..q\n\
q.......M....CCC..O.....O......M...r......OM.q\n\
q.........*.......O.....O..U........*.....O..q\n\
q........CCC......BgggggB..C.......CCC....O..q\n\
q.............r...O.....O.....BoB.........O..q\n\
q.....M...........OR...EO................UO..q\n\
q.................BBBBBBB.................O..q\n\
q........r......r..q.q.q..r...............O..q\n\
q..................q.q.q..................O..q\n\
q..................q...q............M.....O..q\n\
q....CCC.....CCC...q.....................UO..q\n\
q.....................................C.BoB..q\n\
qB.B....................M............M.......q\n\
qO.O................................C........q\n\
qOUO.....CCC........*.......M..*..C..........q\n\
qBoB..........CCC..CCC..CCC..CCCC............q\n\
q............................................q\n\
q............................................q\n\
BeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeB\n\
BeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQ",

"2\
BooooooooBBBBBBBBBBBBBBBBBBoooooooooooooooBBBBBBBBBBBBBBBooooooooooooB\n\
OS...................................................................O\n\
O.................................*................................M.O\n\
B.....BeBoo..BBeBooooooBeBBBBBBBBBBeB...eBooBeBBBBBBBBBeBooooBr......O\n\
O....................................................................O\n\
O..............M.............................M...................q...O\n\
O.....rBoooBeBBBBBBBBBBBBBBBBeBBBBBBBBBBBBBBBBBBeBoooooooooooooooBe..B\n\
O..........q................q........................................q\n\
O..............................*.................M..............M....q\n\
BBBBBBBBBBBBBBBeBoooooBeBBBBBBBBBeBooooBeBBBBBBBBBBBBBBBeBoooooBr....B\n\
O.............q.......q................q.................q..........gO\n\
O.......M.........M....M.........................................C.ggO\n\
O.R....*......*.M.......*......M..............B......r......r.r.r.BREO\n\
BoooooooBeeBBBBBBBeeBoooooooBeeBBBBBBBBBBBBBeeeeeBBeeeeeBrBeeeeeeeeBBBBQ",
"2\
ggg.....................q............BeeeeeeeeeeeeeeeeeeeeeeeeeeeB.....\n\
REgr....................q......r...........................G.....q.....\n\
rrr................r....q...r....*.*.........M.......C.....G....Rq.....\n\
............B........B..qMr..BoooooooB...................M.GCCCCCq.....\n\
.....r......q........q.rq...rq.........r....r.....r.M...r........q.....\n\
........C.C.q...U....q..q.r..q..................*.....r.......r..q.....\n\
............q........q..B...rq................CCC...........M....q.....\n\
...r.....M..BeeeeeeeeBr.O.r..q.......q.r..r......M...r.....r.....q.....\n\
............q...........O...rB.......q.............U.............q.....\n\
.S....M.....q.....CCCCCCB............BeeeeeeeeeeeeeeeeeeeeeeeeeeeB.....\n\
.r..........q.......*...*.*.....r..U...................................\n\
eeeeeeeeeeeeBooooooooooooooooBeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQ",         

"2......................................................................\n\
.......................................................................\n\
..................r..........r...........S................r............\n\
................r..........M..........r.....r..........................\n\
......E................................................r.......r...r...\n\
.....BoB.r.......M....M........r........rrr...M.............BeeeeeeeB..\n\
..............M..........r......................U....*.................\n\
.....................U.........M................rrrrrr....U............\n\
...........r.....................r...........................r..r..r...\n\
eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQ",
         "2\
............................O......O.................................B\n\
............................O....*.O.................................O\n\
..r.B.........B.............BooooooB....................M.....M......O\n\
r...O.M.....C.BoooooooB.................M.........................c..O\n\
r...O.........q.......O...............................r......BooooB..O\n\
..r.O..C..C...q.....M.O.......r....r...............*..*...r..q.......O\n\
..r.O.......M.q..M....BooooB......*.*..........CBooooooB...*.q..M....O\n\
S...O...C.....q.......O....g..M.BoooooBM....C...O......BoooooB....R.UO\n\
....O.........q.......OER..g..............C.....O............O....BBBO\n\
ooooBeeeeeeeeeB.......BooooBeeeeeeeeeeeeeeeeeeeeB............BeeeeeeeBQ\n",
"0\
....................q.....................................G..........W\n\
....................q..M............................*.....g......R...W\n\
......*.............qM...................M......wwwww.....G....wwww..W\n\
.....+++......>.*...q.M.......M.......M..........H........G.-........W\n\
---.+-.....++++++...B............................H........G..........W\n\
..-..-....................13..................x..H.....WWWW..........W\n\
..-..-...................+46..........*.*...WWW..H............-......W\n\
..-+.-...........>..W..............wwwwwww.......H...................W\n\
.S..>-...x.......W.....W.......x................H.R*x.............E*.W\n\
__________________^^^^^________\\^^^^^^^^^^^^^^^^wwwww^^^^^^/__________Q",
"1vvvvvvvvv.................H........Hvvvvvvvvvv.vvvvvvvvvvv\n\
W...........*.............H........H.............M.......W\n\
W...........WWWW.....wwwwzWWWW.....H...........M.......M.W\n\
W.......*.........W..<..GGW....W^^WWW............-M.WM...W\n\
W......WWW......M...WWWWGG......WW.....w.....M-..........W\n\
W..........................*R.......*R..www...-........W.W\n\
W..................*....xxwww.......www....GGGGGGGGGG....W\n\
W.S..>........++++++....WWW.......M..............*..*...EW\n\
WWWWWW...wwww.........WWW...................wwwwwwwwwwwwww\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Q",
"0vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv....................vv.vvvvvv.vv.vvvvvv\n\
....................................M.................................\n\
...S..................................................................\n\
...wwwww...M.....................W...R.....M.....ww.wwwwwwww.........\n\
..................M..M.M........wwwwwwwww........G...........w........\n\
...................M..M......................x..-.R..M..Ww..www...M...\n\
........GGGGGG............*.........*......ww....Gww...........w...M.,\n\
................G.GG.G.G.GGGGGGGG..wwwwwwwww..kkGw...w.w...x.........E\n\
................................................www........wwwwwwwwwww\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Q",
"0WvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvW\n\
W............................................................W\n\
W....M.M...................<..S..>...............M...M.......W\n\
W........................M.+++++++.M.........................W\n\
W........M.................GGGGGGG................M..........W\n\
WRx......................xW.E.....Wx.......................xRW\n\
WWW......................GWWW..WWWWG.......................WWW\n\
W...++..................W...GG.GGGGGW....................+...W\n\
W........x..*..*..*-..-.G.R*..x..*.*G..*..-..*...*.*.x.......W\n\
W_\\^^^^^/__________\\^^122222222222222223^^122222222223^^^^^/_WQ",
"0................WWWWWWWWWWWWWWWW....vvvvvvvv...-......................\n\
................W.....vvvvv..M.................-......M............E..\n\
..S........M....W..R....*..M..*.....M......M...-...-...............ppp\n\
...>.....M......W..-WWWWWWWWWWWW.............M.-...-....M........p....\n\
.ppp............W>.GGGGGGGGGGGGW.-.....-.......-..p-..................\n\
M......<>.**....WGGGGGGGGGGGGGGW...............-...-ppp.......p.......\n\
...-..pppppp....WWWWWWWWWWWW-GGW.....-...-.....-...-..................\n\
M.R-.............GGGGGGGGGGGGGGW.............-.-...--......p..........\n\
.ppp..M......-...GGGGGGGGGGGGGGW..x....M........*.x*..................\n\
^^^^^^^13^^^^^^12222222222222222223zzzzzzzzz++++++++^^^^^^^^^^^^^^^^^^Q",

"0WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n\
W..............................S......................-vvvv...v.....vW\n\
W.**WWWWWW......M..M..................................-..............W\n\
WWWWW....................++++++++++++++++..p.p.p.p....-..............W\n\
W.....WWWWW...W..p...pp..Wvvv...........WzzzzzzzzzW...-..WWWzWWWzW...W\n\
W....W.*...p..WzzzzzzzzzzW........M.....W+++++++++W...-..W.WWW.WWWGGGW\n\
W....WWWWWWWWWWWWWWWWWWWWW....W.......M.W.............-..W...........W\n\
W..p....M......M............p.W.p.......W...+++++++++++..Wp..........W\n\
W....*..................*.....W....R....W^..............xWR...*.*.E*.W\n\
wwwwww^ww^ww^ww^www^wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwQ"};



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

void LoadLevel(int *startx, int *starty, ObjectStruct *objs, EnemyStruct *ens, unsigned char *level, int height)
{
   int point = 1, posx = 0, posy = 0, i;
   int o = 0, e = 0, line = 0, col = 0;
   int startdefheight = 240;
   int q, m, z, j;
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
      objs[o].PType = 0;
      objs[o].PNumber = 0;
      objs[o].framerate = 1;
      objs[o].framecounter = 0;
      e = o;
      ens[e].keio = 0;
      ens[e].type = 0;
      ens[e].width = 16;
      ens[e].height = 24;
      ens[e].sprite = 0;
      ens[e].xspeed = -4;
      ens[e].yspeed = 0;
      ens[e].direction = 0;
      ens[e].counter = 8;
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
   }
   e = 0;
   o = 0;
   while(1)
   {
      if(level[point] == 'Q')
      {NumofLines = line+1; 
      MessageBox(NULL, &level[point+1], "Level", MB_OK|MB_TASKMODAL);
      break;}
      if(level[point] == 10)
      {
         posx = -24;
         posy += 24;
         SolidityMap[line][col] = 10;
         line++;
      }
      objs[o].posx = posx;
      objs[o].posy = posy;
      ens[e].posx = posx;
      ens[e].posy = posy;
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
        SolidityMap[line][col] = 1;
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
        SolidityMap[line][col] = 3;
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
        SolidityMap[line][col] = 1;
        if(level[0] == '1')
        {
           objs[o].tilex = 24;
           objs[o].tiley =  48;        
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
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '_')
      {
        objs[o].keio = 1;
        objs[o].tilex = 48;
        objs[o].tiley = 0;
        objs[o].tilemaskx = 120;
        objs[o].tilemasky = 72;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'a')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 120;
        objs[o].tilemaskx = 264;
        objs[o].tilemasky = 120;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'A')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 144;
        objs[o].tilemaskx = 264;
        objs[o].tilemasky = 144;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 't')
      {
        objs[o].keio = 1;
        objs[o].tilex = 96;
        objs[o].tiley = 120;
        objs[o].tilemaskx = 288;
        objs[o].tilemasky = 120;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'T')
      {
        objs[o].keio = 1;
        objs[o].tilex = 96;
        objs[o].tiley = 144;
        objs[o].tilemaskx = 288;
        objs[o].tilemasky = 144;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'y')
      {
        objs[o].keio = 1;
        objs[o].tilex = 120;
        objs[o].tiley = 120;
        objs[o].tilemaskx = 312;
        objs[o].tilemasky = 120;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'Y')
      {
        objs[o].keio = 1;
        objs[o].tilex = 120;
        objs[o].tiley = 144;
        objs[o].tilemaskx = 312;
        objs[o].tilemasky = 144;
        SolidityMap[line][col] = 1;
        o++;
      }

      if(level[point] == 'i')
      {
        objs[o].keio = 1;
        objs[o].tilex = 144;
        objs[o].tiley = 120;
        objs[o].tilemaskx = 336;
        objs[o].tilemasky = 120;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'I')
      {
        objs[o].keio = 1;
        objs[o].tilex = 144;
        objs[o].tiley = 144;
        objs[o].tilemaskx = 336;
        objs[o].tilemasky = 144;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'j')
      {
        objs[o].keio = 1;
        objs[o].tilex = 48;
        objs[o].tiley = 120;
        objs[o].tilemaskx = 192;
        objs[o].tilemasky = 0;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'J')
      {
        objs[o].keio = 1;
        objs[o].tilex = 48;
        objs[o].tiley = 144;
        objs[o].tilemaskx = 192;
        objs[o].tilemasky = 0;
        SolidityMap[line][col] = 1;
        o++;
      }

      if(level[point] == '\\')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 0;
        objs[o].tilemaskx = 144;
        objs[o].tilemasky = 72;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'L')
      {
        objs[o].keio = 1;
        objs[o].tilex = 24;
        objs[o].tiley = 24;
        objs[o].tilemaskx = 96;
        objs[o].tilemasky = 96;
        SolidityMap[line][col] = 1;
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
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == 'V')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 24;
        objs[o].tilemaskx = 144;
        objs[o].tilemasky = 96;
        SolidityMap[line][col] = 1;
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
        SolidityMap[line][col] = 3;
        if(level[0] == '1')
        {
           objs[o].tilex = 144;
           objs[o].tiley = 48;         
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
         objs[o].framerate = 10;
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
        SolidityMap[line][col] = 4;
        if(level[0] == '1')
        {
           objs[o].tilex = 240;
           objs[o].tiley = 24;         
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
        SolidityMap[line][col] = 1;
        
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
        e++;
      }
      if(level[point] == '*')
      {
        ens[e].keio = 1;
        ens[e].ospx = 0;
        ens[e].ospy = 48;
        ens[e].ospxm = 0;
        ens[e].ospym = 72;
        ens[e].health += (1 + randomz(2,0));
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
        SolidityMap[line][col] = 3;
        o++;
      }
      if(level[point] == 'W')
      {
        objs[o].keio = 1;
        objs[o].tilex = 192;
        objs[o].tiley = 48;
        objs[o].tilemaskx = 168+24;
        objs[o].tilemasky = 0;
        SolidityMap[line][col] = 1;
        if(level[0] == '1')
        {
           objs[o].tilex = 120;
           objs[o].tiley =  48;        
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
        SolidityMap[line][col] = 1;
        if(level[0] == '1')
        {
           objs[o].tilex = 192;
           objs[o].tiley = 120;         
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
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '2')
      {
        objs[o].keio = 1;
        objs[o].tilex = 48;
        objs[o].tiley = 72;
        objs[o].tilemaskx = 120;
        objs[o].tilemasky = 72;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '3')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 72;
        objs[o].tilemaskx = 144;
        objs[o].tilemasky = 72;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '4')
      {
        objs[o].keio = 1;
        objs[o].tilex = 24;
        objs[o].tiley = 24+72;
        objs[o].tilemaskx = 96;
        objs[o].tilemasky = 96;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '5')
      {
        objs[o].keio = 1;
        objs[o].tilex = 48;
        objs[o].tiley = 24+72;
        objs[o].tilemaskx = 120;
        objs[o].tilemasky = 96;
        SolidityMap[line][col] = 1;
        o++;
      }
      if(level[point] == '6')
      {
        objs[o].keio = 1;
        objs[o].tilex = 72;
        objs[o].tiley = 24+72;
        objs[o].tilemaskx = 144;
        objs[o].tilemasky = 96;
        SolidityMap[line][col] = 1;
        o++;
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
        static ObjectStruct objects[MaxObjects];
        int q;
        static EnemyStruct enemies[MaxObjects];
        static RECT          rc;              /* A rectangle used during drawing */
        
        static int blastx[3], blasty[3], blastset[3], paulax=6, paulay=6;
        static BYTE paulastatus=1, paulaspeed=0, pauladirection=1, blastdirection[3], blaststatus[3];
        static int fire=0, jump=0, vertpaula=0, i, x;
        static int scrx = 0, scry = 0, scrsize=1680, psx=0, psy=0;
        static RECT prect = {0,0,16,24}; /*Defines image locations and sizes for Paula*/
        static BYTE cact, cactus, psxset=0, psyset=0;
        static BYTE GhostStatus= 0, ghostpause=0, sprite = 0;
        static BYTE Health = 4, Healthmax = 4, Livesmax = 9, Lives = 9, hitOK = 0, bcol =0, ShotOK=1;
        static BYTE SoundStatus = 0, SoundPause = 0, HelpPause = 0;
        static BYTE Levelnum = 0, t, twait = 10;
        static char thestory[1000] = "An evil zombie-spirit has transported Paula to a strange zombie-infested world.  However, the Great Pumpkin has come to her aid by giving her \"Exit Pumpkins\" she can use to progress through this zombie-world and eventually back to our own, but the problem is that she must find her way to the Exit Pumpkins on her own.";
        static char helpcntrl[500] = "Controls:\np\t\tPause\nUp/ctrl\t\tJump\nDown\t\tAction Button (aka Use object)\nLeft/Right\t\tMove left and right\nS\t\tEnable/Disable Sound\nh\t\tView Help Dialogs\nSpace\t\tPSI FREEZE!";
        static char helpadv[950] = "Climbing: If you jump toward certain blocks and hold up, it may\n allow you to climb them.\nTunnel Sliding: Hold up or ctrl to move down a tunnel slightly faster than you would by walking.\n\nUse (press down on) Life-Runes (Gray with red marking) to become a pseudo-ghost that can pass through certain kinds of walls and is invunerable to damage from enemies. (Can't shoot, You are NOT invunerable to spikes and the like as a ghost!)\n\nTo exit the level, get to the Exit pumpkin and use it.  (Doesn't work as ghost, you must be in Human form to win)\n\nUse (press down) the Golden Mushroom to jump high into the sky!\n\nBeware spikes, ecto-plasmic ooze, and few the spirit vortices out there as they cause instant death on contact.";
        static char windexp[650] = "Display Explanations:\nThe Red bar (Upper-right):\tThis bar describes your Health status\nThe Green bar (Upper-right):\tNumber of lives remaining\nThe black and orange bar (Lower-left):\tVertical speed\nBlue Bar (Lower-left):\tWhen you get hit, this bar will appear, as long as the bar is there, you are immune to additional damage.\nBlack/Orange blocks (Bottom, Center):\tThis indicates the level you are on (eg. 2 blocks is level 2).\n\nThere are 6 levels.\nStarting Lives: 9\nStarting Health: 4\nSpritit Vortices only appear in level 3, They're brownish colored.\nYes, this game is beatable.";
        static char credits[550] = "Tileset/Programming/non-character sprites/background:\n\tEvilPuffBall\nSound Effects:\n\tWhat Evar\nSprites and Background music:\n\tEarthBound\nOriginal moon photo:\n\tNASA\nExcessive Testing:\n\tWhat Evar, BB, B. Solem, and Odis\n(For the curious) APIs Utilized:\n\tWin32 API, DirectX 7.0 (DirectSound) API\n(UPX Compressed Executable)";
        static char finalmessage[120] = "Know when to run! Know when to fight!  Good Luck!";
        int ysize; /*ysize, describes total y-spanning of level, no need for static*/
        int monktast;

        /*
         * Perform processing based on what kind of message we got.
         */
        switch (nMsg)
        {
                case WM_CREATE:
                {
                        if( MessageBox(hwnd, "Is this your first time playing Paula's Halloween Zombinanza?", "!!!", MB_YESNO) == IDYES)
                        {
                           MessageBox(hwnd, thestory, "The story thus far", MB_OK);
                           MessageBox(hwnd, helpcntrl , "Help Box: Basic Controls", MB_OK);
                           MessageBox(hwnd, helpadv, "Advanced control and item notes:", MB_OK);
                           MessageBox(hwnd, windexp, "Display And Miscellaneous Info", MB_OK);
                           MessageBox(hwnd, credits, "Credits", MB_OK);
                           MessageBox(hwnd, finalmessage, "And a word from our sponsors!", MB_OK);
                        }
                        GetClientRect( hwnd, &rc );
                        for(i = 0; i < MaxObjects; i++)
                        {
                           objects[i].keio = 0;
                           enemies[i].keio = 0;
                        }
                        randomz(-1, randomz(2000,0));
                        LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);
                        twait = 10;
                        scrx = 0;

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
                           MessageBox(hwnd, "I can't allocate a timer for the game to run on!\nTry restarting your computer and running me again.","Timer allocation error", MB_OK);
                        }
                        blastset[0] = 0;
                        blastset[1] = 0;
                        blastset[2] = 0;
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
                        bufferdesc.dwSize = sizeof(DSBUFFERDESC);
                        bufferdesc.dwFlags = DSBCAPS_CTRLVOLUME|DSBCAPS_PRIMARYBUFFER;
                        bufferdesc.dwBufferBytes = 0;
                        bufferdesc.dwReserved = 0;
                        bufferdesc.lpwfxFormat = NULL;
                        printf("%s", lpds->lpVtbl->CreateSoundBuffer(lpds, &bufferdesc, &lplpDirectSoundBuffer, NULL) == DS_OK ? "CDSB == TRUE" : "CDSB == FALSE" );
                        lplpDirectSoundBuffer->lpVtbl->SetFormat(lplpDirectSoundBuffer, &WaveFormat);
                        for(i = 0; i < 5; i++)
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
                           lpds->lpVtbl->CreateSoundBuffer(lpds, &ProtoBuffers[i], &ProtoBuffersStr[i], NULL);
                           ProtoBuffersStr[i]->lpVtbl->Lock(ProtoBuffersStr[i], 0, wavesize[i+1],&RawData, &rdl,&RawData2,&rdl2,0);
                                 MemPtr = wavemem[i+1];
                                 memcpy(RawData,&MemPtr[44],rdl);
                                 if(rdl2 != 0 || RawData2 != NULL)
                                 {
                                 //printf("!");
                                 memcpy(RawData2,&MemPtr[44+rdl], rdl2);
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
                                 memcpy(RawData,&MemPtr[44],rdl);
                                 if(rdl2 != 0 || RawData2 != NULL)
                                 {
                                 printf("!");
                                 memcpy(RawData2,&MemPtr[44+rdl], rdl2);
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
                                 ProtoBuffersStr[4]->lpVtbl->Play(ProtoBuffersStr[4], 0,0, DSBPLAY_LOOPING);
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

                      psxset = 0;
                      psyset = 0;

                      if(GetActiveWindow() == hwnd)
                      {
                        #ifdef debugkeys
                        if(GetAsyncKeyState(VK_F10) != 0)
                        {
                             Levelnum += 1;
                             Health = Healthmax; Lives = Livesmax; scry = 0;
                             LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                             vertpaula = 0; twait = 10;
                        }
                        if(GetAsyncKeyState(VK_F11) != 0)
                        {
                             Levelnum--;
                             Health = Healthmax; Lives = Livesmax; scry = 0;
                             LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                             vertpaula = 0; twait = 10;
                        }
                        if(GetAsyncKeyState(VK_F12) != 0)
                        {                             
                             Health = Healthmax;
                        }
                        
                        #endif
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
                              ProtoBuffersStr[4]->lpVtbl->Stop(ProtoBuffersStr[4]);
                              SoundPause = 20;
                              SoundStatus = 1;
                              goto monk;
                           }
                           if(SoundStatus == 1 && SoundPause == 0)
                           {
                              ProtoBuffersStr[4]->lpVtbl->Play(ProtoBuffersStr[4], 0,0, DSBPLAY_LOOPING);
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
                           MessageBox(hwnd, thestory, "The story thus far", MB_OK);
                           MessageBox(hwnd, helpcntrl , "Help Box: Basic Controls", MB_OK);
                           MessageBox(hwnd, helpadv, "Advanced control and item notes:", MB_OK);
                           MessageBox(hwnd, windexp, "Display And Miscellaneous Info", MB_OK);
                           MessageBox(hwnd, credits, "Credits", MB_OK);
                           MessageBox(hwnd, finalmessage, "And a word from our sponsors!", MB_OK);

                           }
                        }

                        HelpPause -= (HelpPause>0 ? 1 : 0);

                        SoundPause -= ((SoundPause > 0) ? 1 : 0);
                        jumpcounter -= ((jumpcounter > 0 )?1:0);

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
                           MessageBox(hwnd, "Game is paused, press OK to continue", "PAUSE", MB_OK);
                           SetTimer(hwnd, 900, 55, NULL);
                        }

                        if(GetAsyncKeyState(VK_DOWN) != 0)
                        {
                          for(i = 0; i < MaxObjects; i++)
                          {
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
                                          }
                                       }

                                       if(objects[i].exit == 1 && GhostStatus == 0)
                                       {
                                         /*Do win stuff here*/
                                         Levelnum++;
                                         if(Levelnum == 6)
                                         {
                                            KillTimer(hwnd, 900);
                                            if( MessageBox(hwnd, "YAY!  You manged to defeat the zombie hordes over treacherous terrain!  What will you do next?\nPlay Again?", "[SSB Voice]Congratulations![/voice]", MB_YESNO) == IDYES)
                                            {
                                          Health = Healthmax; Lives = Livesmax; Levelnum = 0;
                                          LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                                          vertpaula = 0; twait = 10;SetTimer(hwnd, 900, 55, NULL);

                                            }
                                            else
                                            {
                                               DestroyWindow(hwnd);
                                            }


                                         }
                                         Health = Healthmax;
                                         LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
                                         vertpaula = 0; twait = 10;
                                         goto monkey;

                                       }

                                       if(objects[i].ghost == 1)
                                       {
                                       GhostStatus=(GhostStatus+1)&1;
                                       KillTimer(hwnd, 900);
                                       if(SoundStatus == 0)
                                       {
                                       PlaySound(MAKEINTRESOURCE(205), apphinst, SND_RESOURCE);
                                       }
                                       else
                                       {
                                          Sleep( 1000 );
                                       }
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
                        {jump=1; jumpcounter = 16/*Jump velocity+1*//2;}

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

                        if(jump == 1 && (vertpaula == 0 || (vertpaula == 1 && cactus == 1)))
                        {
                           paulastatus = 0;
                           vertpaula = 15;
                        } /* Leave in for stuff */
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
                              if((StopperRectType[i]&4) == 4)
                              {jumpcounter = 12;}
                              paulay = paulay + (-1*vertpaula);
                           }
                           if((cact&8) == 8)
                           {
                              psx = paulax = StopperRect[i].left-prect.right;
                              paulaspeed = 0;
                              psxset = 1;
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

        if(paulay+prect.bottom <= enemies[x].posy+enemies[x].height+15 && paulay+prect.bottom >= enemies[x].posy+enemies[x].height-15 && paulax > enemies[x].posx - 80 && paulax < enemies[x].posx + 80 && enemies[x].type == 0 && enemies[x].keio == 1)
        {
           if(enemies[x].lunge == 0 && enemies[x].type == 0 && enemies[x].keio == 1)
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
           enemies[x].xspeed = enemies[x].speedbase * (enemies[x].type == 0 ? 1 : 0) * (enemies[x].xspeed <= 0 ? -1 : 1) * (enemies[x].lunge==1? -1: 1);
           enemies[x].lunge = 0;
        }


                        if(enemies[x].type == 0 && enemies[x].keio == 1)
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

                        if(enemies[x].keio == 1)
                        {

                        if(enemies[x].type == 1)
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
                      lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[3], &lpPlayBuffers[t]);
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
                           }
                        }

                        for(i = 0; i < MaxObjects; i++)
                        {
                        cact = 0;
                           if(objects[i].keio == 1 && objects[i].solid == 1 && objects[i].posx < (enemies[x].posx+CCR) && objects[i].posx > (enemies[x].posx-CCR) )
                           {
                               cact = DetectCollision(enemies[x].posx, enemies[x].posy, enemies[x].posx+enemies[x].width, enemies[x].posy+enemies[x].height, objects[i].posx+1, objects[i].posy+1, objects[i].posx+objects[i].width, objects[i].posy+objects[i].height-2, enemies[x].xspeed, -1*enemies[x].yspeed, 0, 0, 0,(enemies[x].posy+enemies[x].height)<=(objects[i].posy+4)?1:0, enemies[x].posy>=objects[i].posy ? 1 : 0,0,(enemies[x].posy+enemies[x].height)<=(objects[i].posy+6)?1:0);
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
                              enemies[x].xspeed *= -1;
                           }
                           if((cact&8) == 8)
                           {
                              psx = enemies[x].posx = objects[i].posx-enemies[x].width;
                              psxset = 1;
                              enemies[x].posy = enemies[x].posy + (-1*enemies[x].yspeed);
                              //printf("\n8 == %d", enemies[x].posx);
                              enemies[x].xspeed *= -1;

                           }
                           monktast = 1;
                        }
                        }
                        }
                        if(monktast == 0)
                        {
                           if(enemies[x].prev == 1 && enemies[x].type == 0)
                           {
                              enemies[x].xspeed *= -1;
                              enemies[x].yspeed = 0;

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

                        if(enemies[x].type == 0)
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
                        if(jump == 1 && (vertpaula == 0 || ((jumpcounter&1) == 1 && vertpaula == 1) || ((jumpcounter&12) == 12 && vertpaula == -5)))
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
                              if( ((enemies[x].keio == 1 ? DetectCollision(enemies[x].posx, enemies[x].posy, enemies[x].posx+enemies[x].width, enemies[x].posy+enemies[x].height, paulax, paulay, paulax+prect.right, paulay+prect.bottom, 0, 0, 0, 0, 1, 0, 0,0,0) : 0) != 0) || ( ((enemies[x].shootset == 1) ? DetectCollision(enemies[x].shootx+1, enemies[x].shooty+1, enemies[x].shootx+14, enemies[x].shooty+14, paulax+1, paulay+1, paulax+prect.right-1, paulay+prect.bottom-1, 0, 0, 0, 0, 1, 0, 0,0,0) : 0) != 0))
                              {
                                 cact = 1;
                              }
                              if(cact != 0 && hitOK == 0)
                              {
                                 Health -= (Health == 0 ? 0 : 1 );
                                 hitOK = 21;
                                 if(Health == 0)
                                 {
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
                                    
                                    hitOK = 0;
                                    if(Lives == 0)
                                    {
                                       KillTimer(hwnd, 900);
                                       
                                       if( MessageBox(hwnd, "Paula gasps her few last breaths as her sight dims...\nGame Over\nPlay again?", "Your story ends here.", MB_TASKMODAL|MB_YESNO) == IDYES)
                                       {
                                          GhostStatus = 0; Health = Healthmax+1; Lives = Livesmax; Levelnum = 0;
                                          LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx=0;
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
                                 cact = (blastset[i] == 1 ? 1 : 0) * enemies[x].keio == 1 ? DetectCollision(enemies[x].posx, enemies[x].posy, enemies[x].posx+enemies[x].width, enemies[x].posy+enemies[x].height, blastx[i], blasty[i], blastx[i]+16, blasty[i]+8, 0, 0, 0, 0, 1, 0,0,0,0): 0;
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

                      lpds->lpVtbl->DuplicateSoundBuffer(lpds, ProtoBuffersStr[1], &lpPlayBuffers[t]);
                      lpPlayBuffers[t]->lpVtbl->Play(lpPlayBuffers[t], 0, 0, 0);
                      Bufferinuse[t] = 1;
                      Bufferused[t] = 1;

                      off = 1;
                   }
                }
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

                                    enemies[x].hitset = (((enemies[x].posx >= blastx[i] ? 16: 0)+4)*(enemies[x].health == 0 ? 0 : 1));
                                    enemies[x].hitx = blastx[i]+(enemies[x].posx >= blastx[i] ? 1 : 0 *(enemies[x].width+16))+(enemies[x].posx >= blastx[i] ? 0 : -1 *(enemies[x].width+5)) ;
                                    enemies[x].hity = blasty[i]+2;
                                    }
                                 }
                              }
                           }

                        }

                        for(i = 0; i < MaxObjects; i++)
                        {
                                if( (objects[i].keio == 1 && objects[i].kill == 1) || (objects[i].keio == 1 && objects[i].healthpickup == 1) )
                                {
                                   cact = DetectCollision(paulax, paulay, paulax+prect.right, paulay+prect.bottom, objects[i].posx, objects[i].posy+1, objects[i].posx+objects[i].width-1, objects[i].posy+objects[i].height-3, 1, 0, 0, 0, 0, 0, 0, paulay<(objects[i].posy+objects[i].height)?1:0, 0);
                                   if(cact != 0)
                                   {
                                       if(objects[i].healthpickup == 1)
                                       {
                                          Health += 1;
                                          objects[i].keio = 0;                                          
                                       }
                                       if(objects[i].kill == 1)
                                       {
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

                                    if(Lives == 0)
                                    {
                                       KillTimer(hwnd, 900);
                                       
                                       if( MessageBox(hwnd, "Paula gasps her few last breaths as her sight dims...\nGame Over\nPlay again?", "Your story ends here.", MB_TASKMODAL|MB_YESNO) == IDYES)
                                       {
                                          Health = Healthmax; Lives = Livesmax; GhostStatus = 0; Levelnum = 0;
                                          LoadLevel(&paulax, &paulay,objects, enemies, level[Levelnum], rc.bottom);scrx = 0;
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
                        vertpaula = 0; twait = 10;
                                    }


                                       }
                                   }
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
                        for(i = 0; i < 5; i++)
                        {
                          ProtoBuffersStr[i]->lpVtbl->Release(ProtoBuffersStr[i]);
                        }
                        for(i = 0; i < 3; i++)
                        {
                          lpSecondaryBuffer[i]->lpVtbl->Release(lpSecondaryBuffer[i]);
                        }
                        lplpDirectSoundBuffer->lpVtbl->Release(lplpDirectSoundBuffer);
                        for(i = 0; i < 6; i++)
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
                              if(enemies[i].shootset == 1)
                              {
                                 BitBlt(mem, enemies[i].shootx-scrx, enemies[i].shooty-scry, 15, 15, mem2, 94+15, 80, SRCPAINT);
                                 BitBlt(mem, enemies[i].shootx-scrx, enemies[i].shooty-scry, 15, 15, mem2, 94, 80, SRCAND);
                              }
                           if(enemies[i].keio == 1)
                           {
                              if( (enemies[i].posx+enemies[i].width)-scrx > -12 && (enemies[i].posx-scrx) < 430)
                              {BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height,mem2, enemies[i].ospxm+(enemies[i].width * enemies[i].sprite)    , enemies[i].ospym    , SRCPAINT);
                              BitBlt(mem, enemies[i].posx-scrx, enemies[i].posy-scry,enemies[i].width, enemies[i].height , mem2, enemies[i].ospx+(enemies[i].width * enemies[i].sprite) , enemies[i].ospy , SRCAND);}
                              //printf("ENENYDRAW%d %d, %d|||", i, enemies[i].posx-scrx, enemies[i].posy);
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
                        for(i = 0; i < Levelnum+1; i++)
                        {
                          BitBlt(mem, (i*20)+(15*8), rc.bottom-8, 16, 8, mem2, 160, 88, SRCCOPY);
                        }

                        /*Draw paula*/

                        SelectObject(mem2, GhostStatus == 0 ? PaulaBMP : Osprites);
                        BitBlt(mem, paulax-scrx,paulay-scry,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),prect.top+prect.bottom,SRCPAINT);
                        BitBlt(mem, paulax-scrx,paulay-scry,prect.right,prect.bottom,mem2,prect.left+(pauladirection*32)+(paulastatus*16),0,SRCAND);
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
                        BitBlt(hdc, 0, 0, 320, 240, mem, 0, 0, SRCCOPY);

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
                        for(i = 0; i < Levelnum+1; i++)
                        {
                          BitBlt(hdc, (i*20)+(15*8), rc.bottom-8, 16, 8, mem2, 160, 88, SRCCOPY);
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
                        #endif
        wndclass.hInstance = hInst;

        wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
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









