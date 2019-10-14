/* Levels are to be loaded here as it takes forever to compile in zombattle.c */
#include <windows.h>
#include <wingdi.h>
#include <stdio.h>
#include <mem.h>
int MaxLvl = 25;
#define MaxLevels 35
#define Length 3600
#define MaxObjects 600

unsigned char level[36][3600]={

"0\
....................q.....................................G..........W\n\
.........~..........q..M............................*.....g......R...W\n\
......*.............qM...................M......wwwww.....G....wwww..W\n\
.....+++......>.*...q.M.......M.......M..........H........G.-........W\n\
---.+-.....++++++...B............................H........G..........W\n\
..-..-....................13..................x..H.....WWWW..........W\n\
..-..-...................+46..........*.*...WWW..H............-......W\n\
..-+.-......##...>..W..............wwwwwww.......H...................W\n\
.S..>-...x..#####W.....W.......x................H.R*x.............E*.W\n\
__________________^^^^^________\\^^^^^^^^^^^^^^^^wwwww^^^^^^/__________QWelcome to The Zombieverse:\nThe great pumpkin speaks to you:\n\n\"Paula, you must use my magical exit pumpkins to leave this place.\nI can not help you in any other way, go forth, and find your way home!\" ",
"0vvvvvvvvv.................H........Hvvvvvvvvvv.vvvvvvvvvvv\n\
W...........*.............H........H.............M.......W\n\
W...........WWWW.....wwwwzWWWW.....H...........M.......M.W\n\
W.......*.........W..<..GGW....W^^WWW............-M.WM...W\n\
W......WWW......M...WWWWGG......WW.....w.....M-..........W\n\
W..........................*R.......*R..www...-...#....W.W\n\
W.................#*....xxwww.......www....GGGGGGGGGG....W\n\
W.S..>....##..++++++....WWW.......M.............#*..*...EW\n\
WWWWWW...wwww.........WWW...................wwwwwwwwwwwwww\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Q\"Zombies, Spikes and Monkeys, oh my!\":\nWe think we just summed this place up pretty well.",
"0vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv....................vv.vvvvvv.vv.vvvvvv\n\
....................................M.................................\n\
...S..................................................................\n\
...wwwww.........................W...R.....M.....ww.wwwwwwww.........\n\
..................M..M.~........wwwwwwwww........G...........w........\n\
.....M.............M..M......................x..-.R..M..Ww..www...M...\n\
........GGGGGG............*...#.....*......ww....Gww...........w...M.,\n\
................G.GG.G.G.GGGGGGGG..wwwwwwwww..kkGw...w.w...x.........E\n\
................................................www........wwwwwwwwwww\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^QConstruction anyone?:\nWe zombies aren't too good at building....",
"0Wvvvvvvvvvvvvvvvvvvvvvvvvvv...vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvW\n\
W............................................................W\n\
W....M.M...................<..S..>...............M...M.......W\n\
W........................M.+++++++.M.........................W\n\
W........M.................GGGGGGG................~..........W\n\
WRx......................xW.E.....Wx.......................xRW\n\
WWW......................GWWW..WWWWG.......................WWW\n\
W...++..................W...GG.GGGGGW............##......+...W\n\
W........x..*..*..*-..-.G.R*..x..*.*G..*..-..*...*.*.x.......W\n\
W_\\^^^^^/__________\\^^122222222222222223^^122222222223^^^^^/_WQMolehill Mountain:\nYep, we did it, so sue us, if you can MAKE IT THROUGH DINNER!",
"0................WWWWWWWWWWWWWWWW....vvvvvvvv...-......................\n\
................W.....vvvvv..M.................-......M............E..\n\
..S........M....W..R....*..M..*.....M......M...-...-...............ppp\n\
...>.....M......W..-WWWWWWWWWWWW.............M.-...-....M........p....\n\
.ppp............W>.GGGGGGGGGGGGW.-.....-.......-..p-..................\n\
M......<>.**....WGGGGGGGGGGGGGGW...............-...-ppp.......p.......\n\
...-..pppppp....WWWWWWWWWWWW-GGW.....-...-.....-...-..................\n\
M.R-.............GGGGGGGGGGGGGGW.............-.-...--......p..........\n\
.ppp..M......-...GGGGGGGGGGGGGGW..x....M........*.x*..................\n\
^^^^^^^13^^^^^^12222222222222222223zzzzzzzzz++++++++^^^^^^^^^^^^^^^^^^QGhost Tunneler:\nBeing a pseudo-ghost is fun!",
        
"0WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n\
W..............................S......................-vvvv...v.....vW\n\
W.**WWWWWW......M..M..................................-..............W\n\
WWWWW....................++++++++++++++++..p.p.p.p....-..............W\n\
W.....WWWWW...W..p...pp..Wvvv...........WzzzzzzzzzW...-..WWWzWWWzW...W\n\
W....W.*...p..WzzzzzzzzzzW........~.....W+++++++++W...-..W.WWW.WWWGGGW\n\
W....WWWWWWWWWWWWWWWWWWWWW....W.......M.W.............-..W...........W\n\
W..p....M......M............p.W.p.......W...+++++++++++..Wp..........W\n\
W....*..................*.....W....R....W^..............xWR...*!#.E#.W\n\
wwwwww^ww^ww^ww^www^wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwQZombinanza Factory: \nYou are nearing the end of The Zombieverse it would seem....",               
"0\
....-wwwwwwwwwwwwwwwwwwwwwwwww9w\n\
....-........................wwW............................\n\
...............................W............................\n\
...............................W............................\n\
..........................9....Gx...........................\n\
...x...........................G....................GGG.....\n\
.......x...x....x...x..........W....................w9w.....\n\
...............................W....................www.....\n\
www..........9.................Wx............-..............\n\
89wS...........................W.................x..........\n\
.ww............................Wx...........................\n\
w......R............x.......E..W............-...8.......E...\n\
_______________________________\\.>..>....-----------------.........9Q00000000000000000240Bonus Area: Find the lives!",
/*End primary Zombie-levels, begin Computeropolis*/
"2\
BooooooooooooooooooooooooooooooooooooooooooooB\n\
O...G..............-9....vvvvvvvv............O\n\
ORS.G............M.-........................RO\n\
Owwww....M...M..r..-.......-..............BBBO\n\
O...............r..-M..........~.............O\n\
O......r........r..-......*............x*....O\n\
O..............Mr..-.M...---.r..r..r..rrr....O\n\
O...r.....r.....r............................O\n\
OE..*^^^^....*..r.....*..^^^^^^^^^#.*..^.*...O\n\
BooooooooooooooooooooooooooooooooooooooooooooBQTight Binary Quarters:\nHmmm.... it seems you have left The Zombieverse,\nbut you've been taken to Computeropolis instead of your own world!\nAND THE ZOMBIES HAVE ALREADY INFECTED THIS WORLD!",

"2\
............................O......O.................................B\n\
............................O..!.*.O.................................O\n\
..r.B.........B.............BooooooB....................M.....M......O\n\
r...O.M.....C.BoooooooB.................M.........................c..O\n\
r...O.........q.......O...............................r......BooooB..O\n\
..r.O..C..C...q.....M.O.......r....r...............*..*...r..q.......O\n\
..r.O.......M.q..M....BooooB.....#*#*..........CBooooooB...*.q..M....O\n\
S...O...C.....q.......O....g..M.BoooooBM....C...O......BoooooB....R.UO\n\
....O.........q.......OER..g..............C.....O............O....BBBO\n\
ooooBeeeeeeeeeB.......BooooBeeeeeeeeeeeeeeeeeeeeB............BeeeeeeeBQComputeropolis officialy welcomes you:\nOf course, you probably don't want us to welcome you to heartily...",

"2......................................................................\n\
.......................................................................\n\
..................r..........r...........S................r............\n\
................r..........M..........r.....r..........................\n\
......E................................................r.......r...r...\n\
.....BoB.r.......M....M........r........rrr...M.............BeeeeeeeB..\n\
..............M..........r......................U....*.................\n\
.....................U.........M................rrrrrr....U............\n\
...........r.....................r...........................r..r..r...\n\
eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQSpringy thingy:\nUse the green jumper-springs to launch yourself to new, yet old, heights.",


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
q................MBBBBBBB.................O..q\n\
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
BeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQElectrodeath chamber:\nGravity + Electroshocking traps of doom = bad.",

"2\
BooooooooBBBBBBBBBBBBBBBBBBoooooooooooooooBBBBBBBBBBBBBBBooooooooooooB\n\
OS...................................................................O\n\
O.............9...................*................................M.O\n\
B.....BeBoo..BBeBooooooBeBBBBBBBBBBeBB..rBooBeBBBBBBBBBeBooooBr......O\n\
O....................................................................O\n\
O..............M.............................M..............h....q...O\n\
O.....rBoooBeBBBBBBBBBBBBBBBBeBBBBBBBBBBBBBBBBBBeBoooooooooooooooBe..B\n\
O..........q................q........................................q\n\
O..............................*.................M..............M....q\n\
BBBBBBBBBBBBBBBeBoooooBeBBBBBBBBBeBooooBeBBBBBBBBBBBBBBBeBoooooBr....B\n\
O.............q.......q................q.................q..........gO\n\
O.......M.........M....M.........................................C.ggO\n\
O.R....*......*.M.......*......M..............B......r......r.r.r.BREO\n\
BoooooooBeeBBBBBBBeeBoooooooBeeBBBBBBBBBBBBBeeeeeBBeeeeeBrBeeeeeeeeBBBBQViral Discourse:\nTarget ascertain\178d in sec\202..or at 24 Cx 2..4 beginn.ing i.nt.ervnti.!n pR06a..ammm..02jci3j92sj",

"2\
ggg.....................q............BeeeeeeeeeeeeeeeeeeeeeeeeeeeB.....\n\
REgr....................q......r...........................G.....q.....\n\
rrr................r....q...r....##*.........M.......C.....G....Rq.....\n\
............B........B..qMr..BoooooooB...................M.GCCCCCq.....\n\
.....r......q........q.rq...rq.........r....r.....r.M...r........q.....\n\
........C.C.q...U....q..q.r..q................h.*.....r.......r..q.....\n\
............q........q..B...rq................CCC...........M....q.....\n\
...r.....M..BeeeeeeeeBr.O.r..q.......q.r..r......M...r.....r.....q.....\n\
............q...........O...rB.......q.............U.............q.....\n\
.S....M.....q.....CCCCCCB............BeeeeeeeeeeeeeeeeeeeeeeeeeeeB.....\n\
.r..........q.......*...*.*.....r..U...................................\n\
eeeeeeeeeeeeBooooooooooooooooBeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeBQZombie Infestation:\nYou get a +6 Electronics bane sword!  Too bad it won't help you.\nThe final area of Computeropolis you must traverse is crawling with zombies... not computer virii.",
/*Computeropolis levels donage!*/

"1\
..............................IJJi....................................j\n\
..............................IJJi.............9......................j\n\
...........>*....M............IJJi........x...........................j\n\
...ay.S....ay...-.............IJJi.......pp...........................j\n\
...Ii......Ii.................IJJi...M.-..............................j\n\
...IJttttttJi.........#.......IJJi........................M....M......j\n\
...ATTTJJTTTY............#....IJJi.................-..................j\n\
.......AY........x.<.*..*.....IJJi......-................*.*..*.......j\n\
.................atttttttty...IJJi..M.................pppppppppp......j\n\
.................ATTTTTTTTY...ATTY....................................j\n\
.......................................-..........................-...j\n\
................x.......M...................ppp...x...................j\n\
.............jjjj..>..........>.......>.............................E.j\n\
...................j......j...j...j...jj...........................wwwj\n\
.......................j..............................................j\n\
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^jQGeran-err...umm That Falling Word:\nThat pesky gravity can be your friend or foe.  Which shall it be?  Oh, and welcome to the Ice Realm",
"1\
......ay..............................................................\n\
.....aJJty........www......#..*#......*...........h...................\n\
....aJTTJi.....-..wMw..wwwwwwwwww...wwwwwwwwwww...>...M..<............\n\
y...AY..Ii.......wwwwww...............vv......w...p......j............\n\
i.....S.Ii.......wMwwMw...M.....M.............w...............>..M.<.a\n\
i...atttJi....-..wwwwww.......!.........*.....w............M..-....j.I\n\
i...IJJJJi....h.............-^^-......-----...w......................I\n\
i.-jjjjjjjjj----------------wwww---...........w.........h....h...M...I\n\
i................vv..........................w...............-.......I\n\
i.....................M................M....w.....M.................MI\n\
ipppp.........M.*>...........M.............w............p.........E..I\n\
i............*.-----..*..*.......M........w^^^^w............j.....j..I\n\
i-.....--------.....^------^^--..---..----.jjjj.........^............I\n\
i..................---....----..........................j......9.....I\n\
i..............................................................-.....I\n\
i.-........................................-.........................I\n\
i...............................M....................................I\n\
i........<.......*.........*...<...*.........*.....<.*.......*.......I\n\
JttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttJQLook out above?:\nWatch those last steps, they can be a doozy.\n",
"1\
......................................................................Ii\n\
...............................................WWWWWWWW...............Ii\n\
......................................#...#...#w......W...............Ii\n\
........................................#...#..w......G........#......Ii\n\
..........................................#..#9wR.....G..........#..h.Ii\n\
...........................WWW...WWWWWWWWWWWWWWWWppppWWWWGGGWWWWWWWWWWWW\n\
...........................W........w...........w..-....w.p...........Ii\n\
.....h....................-W........w...M.......w....M..w^^...........Ii\n\
.....j...j..j..............w....-............w..w.......wwwwww....-...Ii\n\
.S......................-..W...**..*..W...*..w..w.......h...........M.Ii\n\
..............-............wwwwwwwwwwwwwwwwwww..w................-....Ii\n\
...........................W......w.............w.x....*.-............Ii\n\
.....................WWWwwww.....M............-.wGGGGGGGGw............Ii\n\
...............-....-w................w...w..*..w........w..www...-...Ii\n\
.....................w...M....h...w..*..*.wwwwwww.R.....Ew.*w.......M.Ii\n\
.....................w......attyWWWWWWWWWWW...w.wWWWWWWWWWwww.........Ii\n\
................wWWWWw.....-IJJi..............w..........w.......-....Ii\n\
................w...........ATTY..............w.......................Ii\n\
.............x..w....*.......*..*..w......!.w....w...*w.....w.........Ii\n\
ttttttttttttttttttttttt^ttttttttttttttttttttttttttttttttttttttttttttttJiQIce Fortress:\nWhat kind of game would this be without a poorly constructed fortress of some sort?",
"1\
ivvvvvvvvvvvvvvvvvvvvvvvvvwvvvvvvvvwvvwvvvvvvIJi\n\
i.........................w........w..v......IJi\n\
i.........................w.M..M...v.........IJi\n\
i..............M..........w...............M..IJi\n\
i....S....*...............w9..*..*...........IJi\n\
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
i-..w.......ay-.!...ay............ay^^^^^Ii..IJi\n\
i...w.......Ii^^^-^^Ii........-...IJjjjjjTY..IJi\n\
i..Ew...x...IJttttttJi............Ii.........IJi\n\
i.ppw...-...ATTTTTTTTY.x....x.....GG.........IJi\n\
i...w..................-....-....-GG.R.*....-IJi\n\
i^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^IJttty^^-^^IJiQI'm sorry, but the princess is in another castle.\n\nAlpha Beta Spikey:\nBeware of Spikes.",


"1\
............W.........................................................\n\
........Mw9hW.........................................................\n\
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
...#h.W.p...W..M........................M..........-...........w......\n\
++++..W..h..Wx.......M...........M.........M...........*.......w......\n\
......W..p...........................................---....-.........\n\
...M..W.....M*..................M........................-.....M...E..\n\
..E.*.W....ppp..................................M................pppp.\n\
..++++W...............................................................\n\
......W...............................................................\n\
^^^^^^W^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^QThe Gauntlet:\nRUN! RUN IF YOU WANT TO LIVE!",
/*Ice realm done*/
"1\
....-wwwwwwwwwwwwwwwwwwwwwwwww9w\n\
....-........................wwW............................\n\
...............................W............................\n\
...............................W............................\n\
..........................9....Gx...........................\n\
...x...........................G....................GGG.....\n\
.......x...x....x...x..........W....................w9w.....\n\
...............................W....................www.....\n\
www..........9.................Wx............-..............\n\
89wS...........................W.................x..........\n\
.ww............................Wx...........................\n\
w......R............x.......E..W............-...8.......E...\n\
tttttttttttttttttttttttttttttttY.>..>....-----------------.........9Q00000000000000000240Bonus Area: Find the lives, you get a second shot at it!",

"4\
......................................................................\n\
......................................................................\n\
......................................................................\n\
...........................M..........................................\n\
...............................M.......9..............................\n\
..........................**x.........................................\n\
............M.........-wwwwww.......-................M................\n\
......................-.....G..........G...................M.........E\n\
...M..........M.......-.....G.....#....G.................p........p.pp\n\
......................-.....G...M..-...G..............p.......*.......\n\
.....................h-.....G........M.G...........p.......p..pww.....\n\
.................p...w-.....G.R..!*....G........C.....................\n\
..............p..........hh.Gwwwwww**..G..*R*p........................\n\
........*..p............www........wwwwwwwwww.........................\n\
.....S..p.............................................................\n\
.....W................................................................\n\
...............M......................................................\n\
......................................................................\n\
......................................................................\n\
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzQLavatrocious:\nOnce again, we prove we shouldn't be building things.",
"4\
.....................................................wzzzw.......9....\n\
..............................pppppppppppppppppwww...wwwww.......-....\n\
..............h..-.--..........................vv-...w.....M.....-....\n\
..............-.............-....................-w..W#........M.-....\n\
..............-zzzzzzz-............-.!.-.........-.~.w---...w....-....\n\
............wwwwwwwwwww..###.*..-zzzzzzzz-.......-..wW......w-...-....\n\
....................vvwwwwwwwwwwwwwwwwwwwwwwwww..-...W.M..-.w...E-....\n\
.S................M.M.w.v..R..vv......Gv..v......-w..w....-.wWWWww....\n\
...............-..........pp.......M..G......~...-...wwz-...w.........\n\
...............-!...........-....M....G..~......R-..wwww-...W.........\n\
.............M.-zz--zzw-.......h..-!..G.....~.wWww..........w.........\n\
........M...w..WWwwWwWW........WzzzzzzW....-..w...w.......wWW.........\n\
w..w.....w.....Ww-zzzzW..-..-..wWwWwwWw..-....w.M.....w^^w............\n\
wwww..w........wW----WW.......................w.....wWwWw.............\n\
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzQMolten Cavern: Can you survive this cavern?",
"4\
-v...........vv...vvv.................................................\n\
.S....................................................................\n\
......................................................................\n\
................-.....................................................\n\
......................................................................\n\
......................................................................\n\
...........M.......**...-..........M...h...M..........................\n\
..................WWWWWW-..............-..............................\n\
............-..x........-.............................................\n\
.WWWW...................-.........M...........M........M..............\n\
.vvvv...........M.......-......................xx.....................\n\
.......W...-............-...#*.............h............h.............\n\
.......................WWWWWWW..x....xx....xx.......x...x.....h.......\n\
................................................................**E*..\n\
-.........^........................................9.........--------.\n\
..........-........................................x..................\n\
zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz-zzzzzzzzzzzzzzzzzzQLava: The monkey frontier!",
/*End lava realm*/
"3\
.---k----------..WWkk-kk-kk-kk---------kk-kkW9.......................k\n\
.............8-.M.W..G......................W--..-.....M.............k\n\
8.....8--------...WR.G...................,.MW......M.....-......M.,..k\n\
-.S...--..........WppM.....k..M.-...M-......W........-......M.-......k\n\
..8*...-...M.R...*W.....pp.kpp...........-..W...M.-........-..-.....Mk\n\
--------.....WWWWWW........k......h.........W.....h...*-..***.-......k\n\
..........-..W...G........!k.-..M.-...-...M.W.....WWWWWWWWWWWWWWWWWWWW\n\
.......M.....W...G.x.h.WkkkkkkkkkkkkkkkkkkkkW....W...MM..............W\n\
...,.........W...WWWWWWWWWWWWWWWWWWWWWWWWWWWW...W.R.h.****........,..W\n\
.........-.-.W....R......*...#.....*............WGGGGGGGGGGGGGGGGGGGGW\n\
...-.........WWWWWWWWWWWWWWWWWWWWWWWWW.........xW....................W\n\
..-..*...M................M....G.............WWWW---------------.....W\n\
...---..-...-....M....M........G...............................G.....W\n\
.............-......k..........G.R......E........M.....M....M..G..h..W\n\
kkkkkkkkkkkkkkkk----k--k--k--k-kk---kkWWWWkkWWWWWWkWWkWWWWkWWWWWWWWWWWQ0158400048000720019201584001920098400048Teleportal Fun:\nPick your poison.",
      
        
"3\
....9-.Wkkkk.........WWWWWWWWWWWWWWWWWWWW.............................\n\
...WW-..WW................G.............W.....*............-..........\n\
..S..-..WR................G...*.*..R*8..W.....W...................,...\n\
.....-..-------...WWWWWWWWWWWWWWWWWWWWWWW...M.W.-....#................\n\
.....-......,...G.........W...................W....*--M-.......M......\n\
.....-...........-........W........M..........W...------..............\n\
ppp..-...............MMMM.W.......R....M....-.W-............x..-------\n\
.....-......W......-......Wkkkk..k--kk-kk-kkkkkkkkkkkkkkkkkkkkk.......\n\
.....-........k.-.....-...W...W........kh.............................\n\
....^-.-..........-...........W........kh......M......M...............\n\
..ppp-........................WWWWWWGGGW8...R...............k.........\n\
.....-....kkkkkkkkkkkkk-kkW.-..W.M...M.Wh..k-......q.-.....-.-.....x..\n\
.....-....WWWWWWWWWWWWWWWWW....Wh*RE.*.Wh....-k-k..q!..........B......\n\
^....-........O...............WWWWWWWWWWBeeeeeeeBooBooBeeeeeeeeeeeB...\n\
ppp..-........O............-..O........W..............................\n\
.....-........O...............O........W................-..........x..\n\
.....-........O..rWeeWWeWWWWeeO........W..............M...M.....-.....\n\
.....-........O...............O........W..........M.-........-M.......\n\
.....-........O...!...........O........W..............................\n\
..............BooooooooooB.R.kO........W..........x......x........-x..\n\
.........................O....O........W....BooooooeeeeoooeeeeoooooooB\n\
WWWW.....................WxxxxO........W.x............................\n\
.........................O....O........W..............,...............\n\
.....x...................Ok,..O........W.......................#......\n\
....WWWW.................O....O........B.BB.x..BooooooooooooooooB.....\n\
.........8.#8.#8#........O..##O........W..BeeeeB.......M...........r..\n\
........WWWWWWWWWWW......OxxxxO........W.........M.....-M....M........\n\
.........................BooooB........W...R,.....-...................\n\
.......................................W...BoooooooooooooooooooooooooB\n\
kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkQ01584000480158400048002880009600648005760132000528Space-age:\nOnce again, many roads, one destination...",
"0\
......................................................................\n\
.......+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++..\n\
.......W...................................................).......B..\n\
.......W.........M..............M..................................B..\n\
.......W...................p..h...........M......p.................B..\n\
.......W.........p..h...p.....p........h.............S.............B..\n\
.......W...p..p.....p............p..p..p...p.........p..!..........B..\n\
.......W..............................!.......pp.!p.........!......B..\n\
.......W................^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^B..\n\
.......W................+++++++++++++++++++++++++++++++++++++++++++B..\n\
.......W---.......................................................B...\n\
.......W..............................h.......................9..EB...\n\
.......W..............................p....................h..----B...\n\
.......W..p..............h...p......!.........h......h.....p......B...\n\
.......W............p....p......ppppppp.p..p..p..p...p..p.........B...\n\
.......W......p............................................p......B...\n\
.......Wp..............p...p...p......p.......p....p.........x....B...\n\
.......W....p....x........................p..................p....B...\n\
.......W.........p....p......p...p..p............p..............p.B...\n\
.......W^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^B...\n\
.......W----------------------------------------------------------B...QPrepare for epic combat!\nThe Great Pumpkin speaks...MENACINGLY!\n\n\"MUHAHHAHAHA! Foolish mortal! You have fallen into my trap!  Prepare yourself!\""
/*Space realm*/
};



/*
   GetAsyncKeyState(32);  
   GetAsyncKeyState('p');  
   GetAsyncKeyState('h');  
   GetAsyncKeyState('H');  
   GetAsyncKeyState('P');  
   GetAsyncKeyState(VK_DOWN);  
   GetAsyncKeyState(VK_UP);  
   GetAsyncKeyState(10);  
   GetAsyncKeyState(VK_LEFT);  
   GetAsyncKeyState(VK_RIGHT);  
   GetAsyncKeyState('a');       
*/


int LoadCustom(unsigned char *lvl)
{
    OPENFILENAME op;
    FILE *in;
    int ch;
    char buf[90];
    char *infile;
    char filters[40] = "Text Files *.txt Lvl Files *.lvl      ";
    char wavext[4] = "txt";
    char possarg[2048];
    char stitle[] = "Please Select Level Set to load.";
    int arc;
    sprintf(possarg, "level.txt");
     filters[10] = 0;
     filters[16] = 0;
     filters[26] = 0;
     filters[32] = 0;
     filters[33] = 0;
     
memset((LPOPENFILENAME)&op,0,sizeof(op));    
     op.lStructSize = sizeof(op);

     op.hwndOwner = NULL;
     op.hInstance = NULL;
     op.lpstrFilter = (LPSTR)filters;
     op.lpstrCustomFilter = NULL;
     op.lpstrTitle = (LPSTR)stitle;
     op.lpstrFile = (LPSTR)possarg;
     op.nFilterIndex = 1;
     op.lpstrDefExt = (LPSTR)wavext;    
     op.nMaxFile = 2048;
     op.lpstrFileTitle = NULL;
     op.lpstrInitialDir = NULL;
     op.lpstrTitle = NULL;
     op.lpstrTitle = (LPSTR)stitle;
     op.Flags = 0; 
     arc = GetOpenFileName(&op);   
     if( arc != TRUE) 
     {
            sprintf(buf, "No file selected or error, returning to menu. %d", CommDlgExtendedError());
            MessageBox(NULL, buf, "Notice", MB_OK|MB_TASKMODAL);
            return arc;
     }
     if( (in = fopen(possarg, "rb")) == NULL)
     {
        sprintf(buf, "Failed to open file: %s\nMaybe another program is using it right now?", possarg);
        MessageBox(NULL, buf, "Error", MB_OK|MB_TASKMODAL);
        return 3;
     }
     ch = fgetc(in);
     int currentempty = 0;
     int currentposition = 0;
     int currentlevel = 0;
     while(ch != EOF)
     {
          if( currentempty == 0)
          {
             if(ch == 32 || ch == 10 || ch == 13 || ch == '\t' || ch == 0)
             {
                /*Do nothing*/   
             }
             else
             {
               currentempty = 1;
               lvl[currentposition+(currentlevel*Length)] = (unsigned char)ch;               
               //currentposition++;
             }
          }
          if( currentempty == 1)
          {
             lvl[currentposition+(currentlevel*Length)] = (unsigned char)ch; 
             if(ch == '\%')
             {
                 lvl[currentposition+(currentlevel*Length)] = 0;   
                 currentposition = -1;
                 currentlevel++;
                 currentempty = 0;
                 if(currentlevel >= 30)
                 break;                 
             }
             currentposition += currentposition < 3199 ? 1 : 0;
             
          }   
          ch = fgetc(in);   
     }
     MaxLvl = currentlevel;
     MessageBox(NULL, "Custom Game loaded successfully", "Notice",MB_OK|MB_TASKMODAL);
     fclose(in);
     return 0;
}

int CustomGet24DIBits(HBITMAP hbit, int *BitCount, int *width, int *height, unsigned char *Bits)
{
     BITMAP bmp;
     
     GetObject(hbit, sizeof(BITMAP), &bmp);
     
     *BitCount = bmp.bmBitsPixel;
     *width = bmp.bmWidth;
     *height = bmp.bmHeight;
     if(Bits == NULL)
     {
        /*Do nothing more*/     
     }
     else
     {
         if(*BitCount == 24 || *BitCount == 32)
         {             
             Bits = bmp.bmBits;             
         }
     }                    
}

int CustomSet24DIBits(HBITMAP hbit, int BitCount, int width, int height, unsigned char *Bits)
{
     BITMAP bmp;
     
     GetObject(hbit, sizeof(BITMAP), &bmp);
     
     if(Bits == NULL)
     {
        /*Do nothing more*/     
     }
     else
     {
        bmp.bmBits = Bits;             
     }                    
}

/*Graphics routine! GRAPHICS!*/
void DrawAlpha(HDC hdestdc, HDC hsrcdc, HDC alphamaskdc, int destx, int desty, int srcx, int srcy, int maskx, int masky, int width, int height, int drawflag)
{
     HBITMAP temp[3];
     HDC tempdc[4];
     HGDIOBJ old[4];
     LPBITMAPINFO bmpinfoheader[3]; /*bmpinfoheaders for the bitmaps*/
     BYTE FAR* bmpbits1;
     BYTE FAR* bmpbits2;
     BYTE FAR* bmpbits3;
     BYTE FAR* bmpbits4;
     BYTE FAR* bmpbits5;
     BYTE FAR* bmpbits6;
     int x;
     int BitCount = 24;
 
     width = width * (width > 0 ? 1 : -1 );
     height = height * (height > 0 ? 1 : -1 );
        
     /*temp[0] = CreateCompatibleBitmap(hsrcdc, width, height);
     temp[1] = CreateCompatibleBitmap(hsrcdc, width, height);
     temp[2] = CreateCompatibleBitmap(hsrcdc, width, height);*/
     
     BITMAPINFOHEADER generalheader;
     BITMAPINFO bmiinfo;
     
     generalheader.biWidth = width;
     generalheader.biHeight = height;
     generalheader.biSize = sizeof(BITMAPINFOHEADER);
     generalheader.biCompression = BI_RGB;
     generalheader.biBitCount = 24;
     generalheader.biPlanes = 1;
     generalheader.biSizeImage = 0;
     generalheader.biXPelsPerMeter = 0;
     generalheader.biYPelsPerMeter = 0;
     generalheader.biClrUsed = 0;
     generalheader.biClrImportant = 0;   
     
     tempdc[0] = CreateCompatibleDC(hsrcdc);
     tempdc[1] = CreateCompatibleDC(hsrcdc);
     tempdc[2] = CreateCompatibleDC(hsrcdc);
     
     /*Allocate memory for image bits*/ 
     bmpbits1 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits2 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits3 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits4 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits5 = malloc( sizeof(BYTE)*4*width*height);
     bmpbits6 = malloc( sizeof(BYTE)*4*width*height);

     memcpy(&bmiinfo.bmiHeader, &generalheader, sizeof(BITMAPINFOHEADER));
     
     if(bmpbits1 != NULL)
        temp[0] = CreateDIBitmap(tempdc[0], &generalheader, 2, bmpbits1, &bmiinfo, DIB_RGB_COLORS);
     if(bmpbits2 != NULL)
        temp[1] = CreateDIBitmap(tempdc[1], &generalheader, 2, bmpbits2, &bmiinfo, DIB_RGB_COLORS);
     if(bmpbits3 != NULL)
        temp[2] = CreateDIBitmap(tempdc[2], &generalheader, 2, bmpbits3, &bmiinfo, DIB_RGB_COLORS);
     
    
     old[0] = SelectObject(tempdc[0] ,temp[0]);
     old[1] = SelectObject(tempdc[1] ,temp[1]);
     old[2] = SelectObject(tempdc[2] ,temp[2]);
     
     
     BitBlt(tempdc[0], 0,0,width,height,hdestdc, destx, desty, SRCCOPY);
     BitBlt(tempdc[1], 0,0,width,height,hsrcdc, srcx, srcy, SRCCOPY);
     BitBlt(tempdc[2], 0,0,width,height,alphamaskdc, maskx, masky, SRCCOPY);



     SelectObject(tempdc[0], old[0]);
     SelectObject(tempdc[1], old[1]);
     SelectObject(tempdc[2], old[2]);
     
     if(bmpbits1 == NULL || bmpbits2 == NULL || bmpbits3 == NULL)
     {
        /*If malloc fails, clean up and abort*/
        if(bmpbits1 != NULL)
           free(bmpbits1);         
        if(bmpbits2 != NULL)
           free(bmpbits2);         
        if(bmpbits3 != NULL)
           free(bmpbits3);         
        if(bmpbits4 != NULL)
           free(bmpbits4);         
        if(bmpbits5 != NULL)
           free(bmpbits5);         
        if(bmpbits6 != NULL)
           free(bmpbits6);         
        
        DeleteObject(temp[0]);
        DeleteObject(temp[1]);
        DeleteObject(temp[2]);
        
        DeleteDC(tempdc[0]);
        DeleteDC(tempdc[1]);
        DeleteDC(tempdc[2]);        
     }
     else
     {
         /*GetDIBits(tempdc[0], temp[0], 0, height, bmpbits1, bmpinfoheader[0], DIB_RGB_COLORS);
         GetDIBits(tempdc[1], temp[1], 0, height, bmpbits2, bmpinfoheader[1], DIB_RGB_COLORS);
         GetDIBits(tempdc[2], temp[2], 0, height, bmpbits3, bmpinfoheader[2], DIB_RGB_COLORS);*/
         //CustomGet24DIBits(temp[0],&BitCount ,&width, &height, bmpbits1);
         //CustomGet24DIBits(temp[1],&BitCount ,&width, &height, bmpbits2);
         //CustomGet24DIBits(temp[2],&BitCount ,&width, &height, bmpbits3);
         GetBitmapBits(temp[0], width*height*4, bmpbits4);
         GetBitmapBits(temp[1], width*height*4, bmpbits5);
         GetBitmapBits(temp[2], width*height*4, bmpbits6);
          
         for(x = 0; x < (4*width*height); x++ )
         { 
             /*Dst.Red 	= Src.Red * (SCA/255.0) 	+ Dst.Red * (1.0 - (SCA/255.0))*/
             //bmpbits4[x] = (BYTE)((float)bmpbits4[x] + ((float)bmpbits5[x]*((float)bmpbits6[x]/255.0))>255 ? 255 : (float)bmpbits4[x] + ((float)bmpbits5[x]*((float)bmpbits6[x]/255.0))  ) + 30;
             if(drawflag == 0)             
                bmpbits4[x] = (BYTE)((float)bmpbits4[x]*(1.0 - ((float)bmpbits6[x]/255.0)) + (float)bmpbits5[x]*((float)bmpbits6[x]/255.0));
             if(drawflag == 1)
                bmpbits4[x] = (BYTE)((bmpbits4[x]+bmpbits5[x])>255?255:(bmpbits4[x]+bmpbits5[x]));
	     if(drawflag == 2)
	        bmpbits4[x] = (BYTE)((bmpbits4[x]-bmpbits5[x])<0?0:(bmpbits4[x]-bmpbits5[x]));
	     if(drawflag == 3)
	        bmpbits4[x] = bmpbits4[x] > bmpbits5[x] ? bmpbits4[x] : bmpbits5[x];
	     if(drawflag == 4)
	        bmpbits4[x] = bmpbits4[x] < bmpbits5[x] ? bmpbits4[x] : bmpbits5[x];
	     if(drawflag == 5)
	        bmpbits4[x] = bmpbits4[x] == 0 ? bmpbits5[x] : bmpbits4[x];
		
         }
         /*SetDIBits(tempdc[0], temp[0], 0, height, bmpbits1, bmpinfoheader[0], DIB_RGB_COLORS);*/

     SetBitmapBits(temp[0], width*height*4, bmpbits4);        
    
     
     old[0] = SelectObject(tempdc[0] ,temp[0]);
     BitBlt(hdestdc,destx, desty, width, height, tempdc[0], 0,0,SRCCOPY );
     SelectObject(tempdc[0], old[0]);

     DeleteObject(temp[0]);
     DeleteObject(temp[1]);
     DeleteObject(temp[2]);

     if(bmpbits1 != NULL)
       free(bmpbits1);         
     if(bmpbits2 != NULL)
       free(bmpbits2);         
     if(bmpbits3 != NULL)
       free(bmpbits3);         
        if(bmpbits4 != NULL)
           free(bmpbits4);         
        if(bmpbits5 != NULL)
           free(bmpbits5);         
        if(bmpbits6 != NULL)
           free(bmpbits6);         
                
     DeleteDC(tempdc[0]);
     DeleteDC(tempdc[1]);
     DeleteDC(tempdc[2]);            
     }
}