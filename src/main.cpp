#include "SDL2_header.h"

#include "UserDefined.h"

#define _LIMIT_WIDTH SCREEN_WIDTH
#define _LIMIT_HEIGHT SCREEN_HEIGHT
using namespace Game;


const int Game::SCREEN_WIDTH	= 800;
const int Game::SCREEN_HEIGHT	= 600;
const std::string Game::TitleName = "STG Game(Peter Zheng, ACM Class, 517030910430)";

std::map<int, bool> keyboard;


std::vector<Flight> enemy;


PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
int enemyNumber, imageNumber;
double speedPlayer;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

void loadPictures()
{
    imagePlayer = loadImage( "player.png"	);
    imageBullet = loadImage( "bullet.png"	);
    imageEnemy	= loadImage( "player_u.png" );
}
FILE *dbg;
Message msg;
int Timer = 0;
void initialize()
{
    srand((unsigned int)(time(NULL)));
    //Flag = 1  Debug Mode
    try {
        dbg = fopen("debug.log","a");
        time_t timer;
        time(&timer);
        struct tm *nowTime = localtime(&timer);
        fprintf(dbg,"=======================[%d-%d-%d %d:%d:%d] Program Start=========================\n",nowTime->tm_year+1900,
                nowTime->tm_mon+1,nowTime->tm_mday,nowTime->tm_hour,nowTime->tm_min,nowTime->tm_sec);
        fclose(dbg);
        FILE *option;
        option = fopen("debug.ini","r");

    } catch (std::exception e){
        Message msg;
        time_t timer;
        time(&timer);
        struct tm *nowTime = localtime(&timer);
        msg.makepair(3,0,e.what(),e.what(),1,"main.cpp",1);
        msg.print();
    }

    //Flag = 2;Display FPS
    FPS_DISPLAY = true;
    //msg传递
    time_t timer;
    time(&timer);
    struct tm *nowTime = localtime(&timer);
    //Message msg;
    msg.makepair(0,0,"FPS Display = True","",1,"main.cpp",2);
    print_debug(msg,"debug.log");

    //Initialize vairables
    //Flag = 3;
    posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
    msg.makepair(0,0,"posPlayer =(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")","",1,"main.cpp",3);
    print_debug(msg,"debug.log");

    posEnemy[0] = posPlayer;
    enemyNumber = 1;
    speedPlayer = 5;
    canvasColor = {0, 0, 0, 255};

    //Load pictures from files
    loadPictures();

}

void drawPlayer()
{
    //Flag = 4
    if(Timer==0){
        msg.makepair(0,0,"In drawPlayer() process.","",1,"main.cpp",4);
        print_debug(msg,"debug.log");
        /*Flight enemytmp;
        enemytmp.newenemy();
        enemy.push_back(enemytmp);*/
    }
    Timer = (Timer + 1) % (FPS_RATE);
    if(Timer==0){
        Flight enemytmp;
        enemytmp.newenemy();
        enemy.push_back(enemytmp);
        msg.makepair(0,0,"New enemy: position(" + itos(enemytmp.pos.x) + "," + itos(enemytmp.pos.y) + ")","",1,"main.cpp",4);
        print_debug(msg,"debug.log");
        msg.makepair(0,0,"Vector Size:" + itos(enemy.size()),"",1,"main.cpp",5);
        print_debug(msg,"debug.log");
    }


    try {
        int w,h;
        getImageSize( imagePlayer, w, h );
        drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2);
    } catch (exception e){
        msg.makepair(3,0,e.what(),e.what(),1,"main.cpp",4);
        print_debug(msg,"debug.log");
    }

}
void drawBackground()
{
    //Rect rect = {70, 50, 80, 90};

    //	Pay attention:
    //		(Color){255,255,0} means (Color){255,255,0,0}
    //		and means you will draw nothing
    //setPenColor((Color){255, 255, 0, 255});


    //drawRect( rect, true );

}
void drawHint()
{
    /*Image *text = textToImage( "< This is a simple game demo. >" );
    int w,h;
    getImageSize( text, w, h );
    drawImage( text, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT-h );
    cleanup(text);*/
}
void drawBullet()
{

}
void drawEnemy()
{
    int w,h;
    getImageSize( imageEnemy, w, h );
    //画飞机
    int len = enemy.size();
    for (int i = 0; i < len; ++i) {
        drawImage(imageEnemy, enemy[i].pos.x - w/2, enemy[i].pos.y - h / 2);
    }

    len = enemy.size();
    for(int i = 0;i < len; i++){
        enemy[i].pos.y += 5;
        if(enemy[i].pos.y > Game::SCREEN_HEIGHT) {
            enemy.erase(enemy.begin()+i);
            i--;
        }
    }
    //drawImage( imageEnemy, posEnemy[0].x-w/2, posEnemy[0].y-h/2 );
}

void draw()
{
    drawBackground();
    drawPlayer();
    drawBullet();
    drawEnemy();
    drawHint();
}
void deal()
{
    bool move = false;
    //Calculate velocity
    if( keyboard[KEY_UP]	|| keyboard['w'] )
    {
        velocityPlayer = velocityPlayer + PointD(0,-1)*speedPlayer;
        move = true;
    }
    if( keyboard[KEY_DOWN]	|| keyboard['s'] )
    {
        velocityPlayer = velocityPlayer + PointD(0,+1)*speedPlayer;
        move = true;
    }
    if( keyboard[KEY_LEFT]	|| keyboard['a'] )
    {
        velocityPlayer = velocityPlayer + PointD(-1,0)*speedPlayer;
        move = true;
    }
    if( keyboard[KEY_RIGHT] || keyboard['d'] )
    {
        velocityPlayer = velocityPlayer + PointD(+1,0)*speedPlayer;
        move = true;
    }

    //Limit player's speed
    double len = velocityPlayer.length();
    if( len > speedPlayer )
    {
        velocityPlayer = velocityPlayer/len*speedPlayer;
    }

    //Calculate new position
    posPlayer = posPlayer + velocityPlayer;

    //判定是否边界。Flag = 5
    int w,h;
    getImageSize( imagePlayer, w, h );
    posPlayer.x = posPlayer.x+w/2 > _LIMIT_WIDTH ? _LIMIT_WIDTH - w/2 : posPlayer.x;
    posPlayer.y = posPlayer.y+h/2 > _LIMIT_HEIGHT ? _LIMIT_HEIGHT - h/2 : posPlayer.y;
    posPlayer.x = posPlayer.x-w/2 < 0 ? w/2 : posPlayer.x;
    posPlayer.y = posPlayer.y-h/2 < 0 ? h/2 : posPlayer.y;
    if(move){
        msg.makepair(0,0,"Player pos(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")","",1,"main.cpp",5);
        print_debug(msg,"debug.log");
    }
    //Stop player
    if(!move)
    {
        velocityPlayer = velocityPlayer * 0.8;
        if( velocityPlayer.length() < 0.1 )
            velocityPlayer = PointD();
    }
}

int work( bool &quit )
{
    drawPlayer();
    //Calculate sth.
    deal();

    //Draw on the screen
    draw();

    if( keyboard[KEY_ESC] )
        quit = true;
    return 0;
}

void mousePress()
{

}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
    keyboard[keyValue] = true;
}

void keyUp()
{
    keyboard[keyValue] = false;
}

void finale()
{

    //Delete all images
    cleanup( imagePlayer, imageBullet, imageEnemy );
    for( int i = 0; i < imageNumber; ++i )
        cleanup( images[i] );
    try {
        dbg = fopen("debug.log","a");
        time_t timer;
        time(&timer);
        struct tm *nowTime = localtime(&timer);
        fprintf(dbg,"=======================[%d-%d-%d %d:%d:%d] Program Finish========================\n",nowTime->tm_year+1900,
                nowTime->tm_mon+1,nowTime->tm_mday,nowTime->tm_hour,nowTime->tm_min,nowTime->tm_sec);
        fclose(dbg);
    } catch (std::exception e){

    }
}

