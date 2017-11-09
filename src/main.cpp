#include "SDL2_header.h"

#include "UserDefined.h"

#include <unistd.h>

//#define DEBUG_MODE
#define _LIMIT_WIDTH SCREEN_WIDTH
#define _LIMIT_HEIGHT SCREEN_HEIGHT
using namespace Game;
double rate = 0;
int HitAll = 0;
int HitGet = 0;
bool __lock = false;
bool debug_mode = false;
bool start = false;
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
std::map<int, bool> keyboard;
#ifdef DEBUG_MODE
int hp = 5000;
#endif
int score = 0, bump = 3;
#ifndef DEBUG_MODE
int hp = 5;
#endif
Uint8 *audio_chunk, *audio_pos;
int audio_len;

string scoreStr, Hp_bumpStr, debugStr1, debugStr2, debugStr3, debugStr4;
std::vector<Flight> enemy;
std::vector<Bullet> bullet;
std::vector<Bullet> userbullet;

PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
int enemyNumber, imageNumber;
double speedPlayer;
int cTimer = 0, Timer = 0;
Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];
Image *GameOver;
bool 打炮了 = false;
void loadPictures()
{
    imagePlayer = loadImage( "player.png"	);
    imageBullet = loadImage( "bullet.png"	);
    imageEnemy	= loadImage( "player_u.png" );
    GameOver = loadImage("red_strip24.png");
}
FILE *dbg;
Message msg;
pthread_t bgmthread, timerthread;
void fill_audio(void *udata,Uint8 *stream,int len){
    //SDL 2.0
    SDL_memset(stream, 0, len);
    if(audio_len==0)
        return;
    len=(len>audio_len?audio_len:len);

    SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

void *pcmplay(void *arg){
    pthread_detach(pthread_self());
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = fill_audio;
    if(SDL_OpenAudio(&spec, NULL) < 0){
        msg.makepair(2,0,"In THREAD: PCMPLAYING: Audio Device Binding Failed", " Can't open Audio!", 1,__FILE__, __LINE__);
        print_debug(msg,"debug.log");
        return ( (void*) 100);
    }
    FILE* wav;
    wav = fopen("bgm2.wav","rb+");
    if(wav == NULL){
        msg.makepair(2,0,"In THREAD: PCMPLAYING: Can't open Audio File!", " Can't open Audio File!", 1,__FILE__, __LINE__);
        print_debug(msg,"debug.log");
        return ( (void*) 100);
    }

    int pcm_buffer_size = 8192;
    char *pcm_buffer = (char *) malloc(pcm_buffer_size);
    int data_count = 0;

    SDL_PauseAudio(0);

    while(true){
        if(start && !__lock){
           wav = fopen("bgm.wav","rb+");
            int pcm_buffer_size = 8192;
            char *pcm_buffer = (char *) malloc(pcm_buffer_size);
            int data_count = 0;
            __lock = true;
        }
        if(fread(pcm_buffer,1, pcm_buffer_size, wav)!= pcm_buffer_size){
            fseek(wav,0,SEEK_SET);
            fread(pcm_buffer,1,pcm_buffer_size,wav);
            data_count = 0;
        }
        msg.makepair(0,0,"In THREAD: PCMPLAYING: Now Playing  " + itos(data_count) + "  Bytes Data.", "", 1,__FILE__, __LINE__);
        print_debug(msg,"debug.log");
        data_count+=pcm_buffer_size;
        audio_chunk = (Uint8 *) pcm_buffer;
        audio_len = pcm_buffer_size;
        audio_pos = audio_chunk;
        while(audio_len>0)
            SDL_Delay(1);
    }
    free(pcm_buffer);
    SDL_Quit();
    return ( (void*) 100);

}

void *TimeCountThread(void *arg){
    pthread_detach(pthread_self());
    while (1){
        cTimer++;
        sleep(1);
    }
}
void initialize()
{
    int err = pthread_create(&bgmthread,NULL,pcmplay,NULL);
    if(err!=0){
        msg.makepair(3,0,"Can't create pthread BGMplaying For Error detected.",strerror(err),1,__FILE__, __LINE__);
        print_debug(msg,"debug.log");
    }

    err = pthread_create(&timerthread,NULL,TimeCountThread,NULL);
    if(err!=0){
        msg.makepair(3,0,"Can't create pthread TimeCountThread For Error detected.",strerror(err),1,__FILE__, __LINE__);
        print_debug(msg,"debug.log");
    }
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
        msg.makepair(3,0,e.what(),e.what(),1,__FILE__, __LINE__);
        msg.print();
    }

    //Flag = 2;Display FPS
    FPS_DISPLAY = true;
    //msg传递
    time_t timer;
    time(&timer);
    struct tm *nowTime = localtime(&timer);
    //Message msg;
    msg.makepair(0,0,"FPS Display = True","",1,__FILE__, __LINE__);
    print_debug(msg,"debug.log");

    //Initialize vairables
    //Flag = 3;
    posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
    /*msg.makepair(0,0,"posPlayer =(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")","",1,"main.cpp",3);
    print_debug(msg,"debug.log");*/

    posEnemy[0] = posPlayer;
    enemyNumber = 1;
    speedPlayer = 5;
    canvasColor = {0, 0, 0, 255};

    //Load pictures from files
    loadPictures();

    _game_over = false;

    std::vector<Bullet> bullet1;
    bullet = bullet1;
    std::vector<Flight> enemy1;
    enemy = enemy1;
    std::vector<Bullet> bullet3;
    userbullet = bullet3;
    hp = 5; bump = 3;
    score = 0;
}

void drawPlayer()
{
    if(!_game_over){
        //Flag = 4
        /*if(Timer==0 && !_game_over){
            msg.makepair(0,0,"In drawPlayer() process.","",1,"main.cpp",4);
            print_debug(msg,"debug.log");
            Flight enemytmp;
            enemytmp.newenemy();
            enemy.push_back(enemytmp);
        }*/
        Timer = (Timer + 1) % (FPS_RATE);
        int p = 0;
        if(score < 20) p = 55;
        else if(score < 40) p =30;
        else if(score < 60) p =20;
        else p = 10;
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
            msg.makepair(0,0,"New enemy: position(" + itos(enemytmp.pos.x) + "," + itos(enemytmp.pos.y) + ")","",1,__FILE__, __LINE__);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"Vector Size:" + itos(enemy.size()),"",1,__FILE__, __LINE__);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"bullet Address:" + itos(&bullet) + " Size = " + itos(bullet.size()),"",1,__FILE__, __LINE__);
            print_debug(msg,"debug.log");
            msg.makepair(0,0,"userbullet Address:" + itos(&userbullet) + " Size = " + itos(userbullet.size()),"",1,__FILE__, __LINE__);
            print_debug(msg,"debug.log");
        }


        try {
            int w,h;
            getImageSize( imagePlayer, w, h );
            drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2);
        } catch (exception e){
            msg.makepair(3,0,e.what(),e.what(),1,__FILE__, __LINE__);
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
    if(debug_mode){
        int w,h;
        Image *text3 = textToImage( debugStr1 );
        Image *text4 = textToImage( debugStr2 );
        Image *text5 = textToImage( debugStr3 );
        Image *text6 = textToImage( debugStr4 );
        getImageSize( text3, w, h );
        drawImage( text3, 0 , SCREEN_HEIGHT-3*h );
        cleanup(text3);

        getImageSize( text4, w, h );
        drawImage( text4, 0 , SCREEN_HEIGHT-4*h );
        cleanup(text4);

        getImageSize( text5, w, h );
        drawImage( text5, 0 , SCREEN_HEIGHT-7*h );
        cleanup(text5);
        getImageSize( text6, w, h );
        drawImage( text6, 0 , SCREEN_HEIGHT-5.5*h );
        cleanup(text6);
    }
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
                if(userbullet[i].pos.y < 0) {
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
        //rate = HitGet/HitAll;
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
                HitAll++;
                /*if(!打炮了){
                    打炮了 = true;
                    thread 播放子弹轰击音乐(pcmplay, 3);
                    播放子弹轰击音乐.detach();
                    打炮了 = false;
                }*/

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
                    HitGet++;
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
            msg.makepair(0,0,"Player pos(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")","",1,__FILE__, __LINE__);
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
    if(!start){
        string Welcome = "Welcome to Thunder Game!";
        string wel2 = "Press [R] To start the game.";
        string wel3 = "Press [D] To debug the game.";
        string wel4 = "Press [ESC] To Exit the game.";
        Image *txt1 = textToImage(Welcome);
        Image *txt2 = textToImage(wel2);
        Image *txt3 = textToImage(wel3);
        Image *txt4 = textToImage(wel4);
        int w,h;
        getImageSize( txt1, w, h );
        drawImage( txt1, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 - h / 2 );
        cleanup(txt1);

        getImageSize( txt2, w, h );
        drawImage( txt2, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + h / 2 );
        cleanup(txt2);

        getImageSize( txt3, w, h );
        drawImage( txt3, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + 3 * h / 2 );
        cleanup(txt3);

        getImageSize( txt4, w, h );
        drawImage( txt4, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + 5 * h / 2 );
        cleanup(txt4);

        if( keyboard['r'])
        {
            start = true;
        }
        if(keyboard['d']){
            debug_mode = true;
            hp = 5000;
            start = true;
        }
        if( keyboard[KEY_ESC] )
            quit = true;
        return 0;
    }
    if(start){
        drawPlayer();
        FPSRateCurrent ++;
        if(hp>0){
            Hp_bumpStr = "HP: " + itos(hp) + "       Skill : " + itos(bump) + "     ";
            if(debug_mode){
                debugStr1 = "运行时间(线程返回): "+ itos(cTimer) + "  秒" + "  (变量返回): {1}"+itos(SDL_GetTicks())+ "秒 {2} "+itos(duration)+" 秒  (系统返回): " + itos(cTimer) + "秒"
                    /*    BGM Sound : ON     Sound : Error(com.apple.audiokit)"*/;
                debugStr2 = "用户坐标("+itos(posPlayer.x)+","+itos(posPlayer.y)+")     敌机计数:"+itos(enemy.size())+"  子弹数: "+itos(bullet.size())+
                            "  用户子弹数: " + itos(userbullet.size());
                debugStr3 = "线程监控系统：启动（简易模式）  总发出子弹：" + itos(HitAll) + "  打中子弹：" + itos(HitGet);
                debugStr4 = "背景音效:正常|音效:ERROR(11,EXC_BAD_ACCESS Crash:com.apple.audio.IOThread.client)";
            }
            scoreStr = "Your Score: " + itos(score)/* + "      Running Time(Thread): "+ itos(cTimer) + "  Seconds" + "  (Variable)duration: "+itos(duration)+"  Seconds." + "     广告：澳门首家线上赌场上线了！"*/;
            if(score > 20 && score < 40) Hp_bumpStr += "   是不是觉得有些简单？我们再来一些！";
            else if(score >= 40 && score <=60) Hp_bumpStr += "   是不是觉得游戏难度更大了？充钱可以让你获得更多有用的道具！";
            else if(score > 60) Hp_bumpStr += "   是不是觉得太难了，赶紧充钱吧！充钱能让你变得更强！";
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
            rate = (HitAll == 0 ? 0 : (HitGet+0.000001)/HitAll) * 100;
            string Welcome = "Game Over!";
            string wel2 = scoreStr;
            string wel3 = "Hitting Rate: " + itos(rate) + "%";
            string wel4 = "Press [ESC] To Exit the game.";
            string wel5 = "Press [R] To Restart the game.";
            Image *txt1 = textToImage(Welcome);
            Image *txt2 = textToImage(wel2);
            Image *txt3 = textToImage(wel3);
            Image *txt4 = textToImage(wel4);
            Image *txt5 = textToImage(wel5);
            int w,h;
            getImageSize( txt1, w, h );
            drawImage( txt1, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 - h / 2 );
            cleanup(txt1);

            getImageSize( txt2, w, h );
            drawImage( txt2, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + h / 2 );
            cleanup(txt2);

            getImageSize( txt3, w, h );
            drawImage( txt3, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + 3 * h / 2 );
            cleanup(txt3);

            getImageSize( txt4, w, h );
            drawImage( txt4, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + 5 * h / 2 );
            cleanup(txt4);

            getImageSize( txt5, w, h );
            drawImage( txt5, SCREEN_WIDTH / 2 - w / 2 , SCREEN_HEIGHT / 2 + 7 * h / 2 );
            cleanup(txt5);

            if( keyboard['r'] )
                initialize();


            if(hp==0) {
                msg.makepair(0,0,"Game Over! All button locked!","",1,__FILE__, __LINE__);
                print_debug(msg,"debug.log");
                double start = duration;
                //int i = 0;
                //while(i<24){
                if(abs(start-duration)<1e-5){
                    Rect rect;
                    rect.x = 32 * pxt; rect.y = 0; rect.w = rect.h = 32;
                    msg.makepair(0,0,"Lead in Image:"+itos(&GameOver) + " with rect:"+itos(&rect),"",1,__FILE__, __LINE__);
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

}

void mousePress()
{
    if(score>40) {
        for (int j = 0; j < enemy.size(); ++j) {
            if ((abs(enemy[j].pos.x - mouseX)) < (imagew / 1.5) &&
                abs(enemy[j].pos.y - mouseY) < (imageh / 1.5)) {
                score++;
                enemy.erase(enemy.begin() + j);
                j--;
            }
        }
    }
}

void mouseMove()
{

}

void mouseRelease()
{
    /*if(score>40) {
        for (int j = 0; j < enemy.size(); ++j) {
            if ((abs(enemy[j].pos.x - mouseX)) < (imagew / 2) &&
                abs(enemy[j].pos.y - mouseY) < (imageh / 2)) {
                score++;
                enemy.erase(enemy.begin() + j);
                j--;
            }
        }
   /// }*/
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

