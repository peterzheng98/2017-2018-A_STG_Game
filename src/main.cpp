#include "SDL2_header.h"

#include "UserDefined.h"

#include <unistd.h>
#define _LIMIT_WIDTH SCREEN_WIDTH
#define _LIMIT_HEIGHT SCREEN_HEIGHT
using namespace Game;

int sign[] = {-1,1};

const int Game::SCREEN_WIDTH	= 1024;
const int Game::SCREEN_HEIGHT	= 768;
int imagew, imageh;
int pxt = 0;
bool _game_over = false;
uint64_t lastSPACE = 0;
uint64_t FPSRateCurrent = 0;
uint64_t StopAnimationCount = 0;
uint64_t Skill_CD = 0;
const std::string Game::TitleName = "STG Game(Peter Zheng, ACM Class, 517030910430)";
int Timer;
std::map<int, bool> keyboard;

int score = 0, hp = 5000, bump = 3;

string scoreStr, Hp_bumpStr;
std::vector<Flight> enemy;
std::vector<Bullet> bullet;
std::vector<Bullet> userbullet;

PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
int enemyNumber, imageNumber;
double speedPlayer;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];
Image *GameOver;

void loadPictures()
{
    imagePlayer = loadImage( "player.png"	);
    imageBullet = loadImage( "bullet.png"	);
    imageEnemy	= loadImage( "player_u.png" );
    GameOver = loadImage("red_strip24.png");
}
FILE *dbg;
Message msg;
void initialize()
{
    Timer = 0;
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
    if(!_game_over){
        //Flag = 4
        if(Timer==0 && !_game_over){
            msg.makepair(0,0,"In drawPlayer() process.","",1,"main.cpp",4);
            print_debug(msg,"debug.log");
            /*Flight enemytmp;
            enemytmp.newenemy();
            enemy.push_back(enemytmp);*/
        }
        Timer = (Timer + 1) % (FPS_RATE);
        int p = 0;
        if(score < 20) p = 55;
        else if(score < 40) p =30;
        else if(score < 60) p =10;
        else p = 5;
        if(Timer % p == 0 && !_game_over){
            Flight enemytmp;
            enemytmp.newenemy();
            enemytmp.velocity = 5;
            enemytmp.interval = rand() % 5 + 2;
            enemy.push_back(enemytmp);
            Bullet bullet1;
            bullet1.pos.x = enemytmp.pos.x;
            bullet1.pos.y = enemytmp.pos.y;
            bullet1.velocity = 5;
            bullet1.user = 1;
            bullet.push_back(bullet1);
            bullet1.pos.x = enemytmp.pos.x;
            bullet1.pos.y = enemytmp.pos.y;
            bullet1.velocity = rand() % 12 + 5;
            bullet1.user = 1;
            msg.makepair(0,0,"New enemy: position(" + itos(enemytmp.pos.x) + "," + itos(enemytmp.pos.y) + ")","",1,"main.cpp",4);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"Vector Size:" + itos(enemy.size()),"",1,"main.cpp",5);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"bullet Address:" + itos(&bullet) + " Size = " + itos(bullet.size()),"",1,"main.cpp",184);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"userbullet Address:" + itos(&userbullet) + " Size = " + itos(userbullet.size()),"",1,"main.cpp",184);
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


}
void drawBackground()
{
    Rect rect = {0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT};

    //	Pay attention:
    //		(Color){255,255,0} means (Color){255,255,0,0}
    //		and means you will draw nothing
    setPenColor((Color){0, 0,0, 0});


    drawRect( rect, true );

}
void drawHint()
{
    Image *text = textToImage( Hp_bumpStr );
    Image *text2 = textToImage( scoreStr);
    int w,h;
    getImageSize( text, w, h );
    drawImage( text, 0 , SCREEN_HEIGHT-h );
    cleanup(text);

    getImageSize( text2, w, h );
    drawImage( text2, 0 , SCREEN_HEIGHT-2*h );
    cleanup(text2);
}
/*void bump(){
    //TODO: minus User HP;
    std::vector<Bullet> bullet1;
    bullet = bullet1;
    std::vector<Flight> enemy1;
    enemy = enemy1;
    std::vector<Bullet> bullet3;
    userbullet = bullet3;
}*/
void drawBullet()
{
    if(!_game_over){
        int w,h;
        getImageSize( imageBullet, w, h );
        //画子弹
        int len = bullet.size();
        for (int i = 0; i < len; ++i) {
            drawImage(imageBullet, bullet[i].pos.x - w / 2 , bullet[i].pos.y - h / 2);
        }

        int len2 = userbullet.size();
        for (int j = 0; j < len2; ++j) {
            drawImage(imageBullet, userbullet[j].pos.x-w/2, userbullet[j].pos.y-h/2);
        }
        len = bullet.size();
        for(int i = 0;i < len; i++){
            bullet[i].pos.y += bullet[i].velocity;
            if(score > 40 && i %  20 ==0) bullet[i].pos.x += sign[rand()%2]*rand()%50;
            bullet[i].getAreaCode();

            try{
                if(bullet[i].pos.y > Game::SCREEN_HEIGHT) {
                    bullet.erase(bullet.begin()+i);
                    i--;
                    len--;
                }
            } catch (std::exception e){
                std::cout << e.what() << std::endl;
            }

        }
        len2 = userbullet.size();
        for (int i = 0; i < len2; ++i) {
            userbullet[i].pos.y += userbullet[i].velocity;
            userbullet[i].getAreaCode();

            try {
                if(userbullet[i].pos.y > Game::SCREEN_HEIGHT) {
                    userbullet.erase(userbullet.begin()+i);
                    i--;
                    len2--;
                }
            } catch (std::exception ee){
                std::cout << ee.what() << std::endl;
            }
        }
    }
}
void drawEnemy()
{
    if(!_game_over){
        int w,h;
        getImageSize( imageEnemy, w, h );
        //画飞机
        int len = enemy.size();
        for (int i = 0; i < len; ++i) {
            drawImage(imageEnemy, enemy[i].pos.x - w/2, enemy[i].pos.y - h / 2);
        }

        len = enemy.size();
        for(int i = 0;i < len; i++){
            enemy[i].pos.y += enemy[i].velocity;
            enemy[i].getAreaCode();
            if(enemy[i].pos.y > Game::SCREEN_HEIGHT) {
                enemy.erase(enemy.begin()+i);
                i--;
            }
        }

        //敌方飞机打子弹

        len = enemy.size();
        for (int j = 0; j < len; ++j) {
            if((duration_i - enemy[j].OccurTime) % enemy[j].interval == 0 ){
                Bullet bullet1;
                bullet1.pos = enemy[j].pos;
                bullet1.velocity = rand() % 10 + enemy[j].velocity;
                bullet.push_back(bullet1);
            }
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
    if(!_game_over){
        bool move = false;
        //Calculate velocity
        if( keyboard['q'] && bump > 0 && Skill_CD < duration){
            std::vector<Bullet> bullet1;
            bullet = bullet1;
            std::vector<Flight> enemy1;
            enemy = enemy1;
            std::vector<Bullet> bullet3;
            userbullet = bullet3;
            bump--;
            Skill_CD = duration + 2;
            return;
        }
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
        if( keyboard[KEY_SPACE] || keyboard['z'] || keyboard[KEY_ENTER]){
            if(FPSRateCurrent % (FPS_RATE / 10) == 0){
                Bullet bullet1;
                bullet1.velocity = -5;
                bullet1.user = 0;
                bullet1.pos = posPlayer;
                userbullet.push_back(bullet1);
                //lastSPACE = duration_i;
            }
        }

        //查找碰撞问题
        getImageSize( imagePlayer, imagew, imageh );
        /*double posPlayerL = posPlayer.x - imagew/2, posPlayerR = posPlayerL + imagew;
        double posPlayerU = posPlayer.y - imageh/2, posPlayerD = posPlayerU + imageh;
        for (int i = 0; i < enemy.size(); ++i) {
            double enemyL = enemy[i].pos.x - imagew/2, enemyR = enemyL + imagew;
            double enemyU = enemy[i].pos.y - imageh/2, enemyD = enemyU + imageh;
            if(enemyD < posPlayerU && enemyD > posPlayerD && enemyL < posPlayerR && enemyL > posPlayerL) {
                bump(); break;
            }

            if(enemyD < posPlayerU && enemyD > posPlayerD && enemyR < posPlayerR && enemyR > posPlayerL) {
                bump(); break;
            }

            if(enemyU < posPlayerU && enemyU > posPlayerD && enemyL < posPlayerR && enemyL > posPlayerL) {
                bump(); break;
            }

            if(enemyU < posPlayerU && enemyU > posPlayerD && enemyR < posPlayerR && enemyR > posPlayerL) {
                bump(); break;
            }

        }
        //int p;
        //std :: cin >> p;*/

        for(int i = 0; i < userbullet.size();++i){
            for(int j = 0;j < enemy.size();++j){
                if((abs(enemy[j].pos.x - userbullet[i].pos.x)) < (imagew/4) && abs(enemy[j].pos.y - userbullet[i].pos.y) < (imageh/4)){
                    score++;
                    userbullet.erase(userbullet.begin() + i);
                    i--;
                    enemy.erase(enemy.begin() + j);
                    j--;
                }
            }
        }
        for (int i = 0; i < bullet.size(); ++i) {
            if((abs(posPlayer.x - bullet[i].pos.x)) < (imagew/4) && abs(posPlayer.y-bullet[i].pos.y) < (imageh/4)){
                hp--;bump = 3;
                std::vector<Bullet> bullet1;
                bullet = bullet1;
                std::vector<Flight> enemy1;
                enemy = enemy1;
                std::vector<Bullet> bullet3;
                userbullet = bullet3; break;
            }
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
}
void Animation_GameOver(PointD point, uint64_t start){

    /*for(int i = 1;i <= 24; ++i){
        drawImage(GameOver,point.x,point.y,1,1,0,NULL,FLIP_NONE,&rect);
        rect.x = 32*i + 1;
    }*/
}
int work( bool &quit )
{
    drawPlayer();
    FPSRateCurrent ++;
    if(hp>0){
        Hp_bumpStr = "HP: " + itos(hp) + "       Skill : " + itos(bump);
        scoreStr = "Your Score: " + itos(score);
    } else {
        Hp_bumpStr = "Game Over";
        scoreStr = "Your Score: " + itos(score);
    }


    //Calculate sth.
    deal();

    /*FILE *file;
    file = fopen("monitor.log","a+");
    fprintf(file,"Enemy Count:%lld\n Bullet Count:%lld\n User-Position:%f,%f\n",enemy.size(),bullet.size(),posPlayer.x,posPlayer.y);
    fclose(file);*/

    if(hp<=0){
        //ToDo Game Over part
        std::vector<Bullet> bullet1;
        bullet = bullet1;
        std::vector<Flight> enemy1;
        enemy = enemy1;
        std::vector<Bullet> bullet3;
        userbullet = bullet3;
        drawBackground();
        _game_over = true;
        PointD start = posPlayer;
        if(hp==0) {
            msg.makepair(0,0,"Game Over! All button locked!","",1,"main.cpp",397);
            print_debug(msg,"debug.log");
            double start = duration;
            //int i = 0;
            //while(i<24){
                if(abs(start-duration)<1e-5){
                    Rect rect;
                    rect.x = 32 * pxt; rect.y = 0; rect.w = rect.h = 32;
                    msg.makepair(0,0,"Lead in Image:"+itos(&GameOver) + " with rect:"+itos(&rect),"",1,"main.cpp",389);
                    print_debug(msg,"debug.log");
                    drawImage(GameOver,posPlayer.x,posPlayer.y,1,1,0,NULL,FLIP_NONE,&rect);
                    rect.x = 32 * pxt;
                    pxt++;
                    start += 1/24;
                }
                //StopAnimationCount = duration_i - start;
                //uint64_t Last = StopAnimationCount;
                //if(Last!=StopAnimationCount){

                //sleep(1);
                //}
                //StopAnimationCount++;
            //}
            if(pxt>=24) hp--;
        }
    }

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
    cleanup( imagePlayer, imageBullet, imageEnemy, GameOver );
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

