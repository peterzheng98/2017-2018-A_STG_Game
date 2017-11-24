#include "SDL2_header.h"

#include "UserDefined.h"

#include <unistd.h>
//#define DEBUG_MODE
#define _LIMIT_WIDTH SCREEN_WIDTH
#define _LIMIT_HEIGHT SCREEN_HEIGHT
#define PI acos(-1)
using namespace Game;
double rate = 0;
int HitAll = 0;
int HitGet = 0;
bool __lock = false;
bool debug_mode = false;
bool start = false;
int sign[] = {-1, 1};
const int Game::SCREEN_WIDTH = 1024;
const int Game::SCREEN_HEIGHT = 768;
int imagew, imageh;
int pxt = 0;
bool _game_over = false;
uint64_t lastSPACE = 0;
uint64_t FPSRateCurrent = 0;
uint64_t StopAnimationCount = 0;
uint64_t Skill_CD = 0;
const std::string Game::TitleName = "STG Game(Peter Zheng, ACM Class, 517030910430)";
std::map<int, bool> keyboard;
//#ifdef DEBUG_MODE
//int hp = 5000;
//#endif
int score = 0, bump = 3;
//#ifndef DEBUG_MODE
int hp = 5;
//#endif
Uint8 *audio_chunk, *audio_pos;
int audio_len;
int data_count = 0;
string scoreStr, Hp_bumpStr, debugStr1, debugStr2, debugStr3, debugStr4;
std::vector<Flight> enemy;
std::vector<Bullet> bullet;
std::vector<Bullet> userbullet;
std::vector<BulletCurve> bulletCurve;
std::vector<UserBulletCurve> userBulletCurve;
PointD posPlayer, velocityPlayer;
PointD posEnemy[10];
int enemyNumber, imageNumber;
double speedPlayer;
int cTimer = 0, Timer = 0;
Image *imagePlayer, *imageBullet, *imageBulletCurve, *imageBulletCurveUser, *imageEnemy, *imageCur, *images[100];
Image *GameOver;

struct StopAnimation {
    PointD pos;
    int frame;
    double NextStart;
    bool firstLoad;
};

vector<StopAnimation> stopAnimation;

inline void ClearScreenObject() {
    std::vector<Bullet> bullet1;
    bullet = bullet1;
    std::vector<Flight> enemy1;
    enemy = enemy1;
    std::vector<Bullet> bullet3;
    userbullet = bullet3;
    std::vector<BulletCurve> bullet4;
    bulletCurve = bullet4;
    std::vector<UserBulletCurve> bullet5;
    userBulletCurve = bullet5;
}

void sigABRTHandler(int signum) {

    _game_over = true;
    std::cerr << "Executive Error: Receive Signal SIGABRT." << std::endl;
    //std::cerr << "Program will STOP in 3 seconds.........." << std::endl;

    Message msg2p;
    msg2p.makepair(3, 0, "Executive Error: Receive Signal SIGABRT.", "SIGABRT", 1, __FILE__, __LINE__);
    print_debug(msg2p, "debug.log");
    print_debug(msg2p, "monitor.log");

    SDL_Quit();

    std::cout << "\nYour Score is " << score << std::endl;
    std::cout << std::endl;
    std::cout << "Press Number 1 and Enter To Exit." << std::endl;
    int __exit;
    std::cin >> __exit;


    //finale();
    exit(signum);
}

void sigINTHandler(int signum) {

    _game_over = true;
    std::cerr << "Executive Error: Receive Signal SIGINT : " << signum << std::endl;
    //std::cerr << "Program will STOP in 3 seconds.........." << std::endl;

    Message msg2p;
    msg2p.makepair(3, 0, "Executive Error: Receive Signal SIGINT:" + itos(signum), "SIGINT" + itos(signum), 1, __FILE__,
                   __LINE__);
    print_debug(msg2p, "debug.log");

    print_debug(msg2p, "monitor.log");

    SDL_Quit();

    std::cout << "\nYour Score is " << score << std::endl;
    std::cout << std::endl;
    std::cout << "Press Number 1 and Enter To Exit." << std::endl;
    int __exit;
    std::cin >> __exit;
    //finale();
    exit(signum);
}

void sigSEGVHandler(int signum) {

    _game_over = true;
    std::cerr << "Executive Error: Receive Signal SIGSEGV." << std::endl;
    //std::cerr << "Program will STOP in 3 seconds.........." << std::endl;

    Message msg2p;
    msg2p.makepair(3, 0, "Executive Error: Receive Signal SIGSEGV.", "SIGSEGV", 1, __FILE__, __LINE__);
    print_debug(msg2p, "debug.log");

    print_debug(msg2p, "monitor.log");

    SDL_Quit();

    std::cout << "\nYour Score is " << score << std::endl;
    std::cout << std::endl;
    std::cout << "Press Number 1 and Enter To Exit." << std::endl;
    int __exit;
    std::cin >> __exit;
    //finale();
    exit(signum);
}

void sigFPEHandler(int signum) {

    _game_over = true;
    std::cerr << "Executive Error: Receive Signal SIGFPE." << std::endl;
    //std::cerr << "Program will STOP in 3 seconds.........." << std::endl;

    Message msg2p;
    msg2p.makepair(3, 0, "Executive Error: Receive Signal SIGFPE.", "SIGFPE", 1, __FILE__, __LINE__);
    print_debug(msg2p, "debug.log");

    print_debug(msg2p, "monitor.log");

    SDL_Quit();

    std::cout << "\nYour Score is " << score << std::endl;
    std::cout << std::endl;
    std::cout << "Press Number 1 and Enter To Exit." << std::endl;
    int __exit;
    std::cin >> __exit;
    //finale();
    exit(signum);
}

void sigILLHandler(int signum) {

    _game_over = true;
    std::cerr << "\nExecutive Error: Receive Signal SIGILL." << std::endl;
    //std::cerr << "Program will STOP in 3 seconds.........." << std::endl;

    Message msg2p;
    msg2p.makepair(3, 0, "Executive Error: Receive Signal SIGILL.", "SIGILL", 1, __FILE__, __LINE__);
    print_debug(msg2p, "debug.log");

    print_debug(msg2p, "monitor.log");

    SDL_Quit();

    std::cout << "\nYour Score is " << score << std::endl;
    std::cout << std::endl;
    std::cout << "Press Number 1 and Enter To Exit." << std::endl;
    int __exit;
    std::cin >> __exit;
    //finale();
    exit(signum);
}

void loadPictures() {
    imagePlayer = loadImage("player.png");
    imageBullet = loadImage("bullet.png");
    imageBulletCurve = loadImage("imagebulletcurve.png");
    imageEnemy = loadImage("player_u.png");
    imageCur = loadImage("cur.png");
    imageBulletCurveUser = loadImage("imagebulletcurveuser.png");
    GameOver = loadImage("red_strip24.png");
}

FILE *dbg;
bool firstStart = true;
bool PCMStatus = false;
Message msg;
pthread_t bgmthread, timerthread, fileMonitor, threadMonitor, animationThread;

void fill_audio(void *udata, Uint8 *stream, int len) {
    //SDL 2.0
    SDL_memset(stream, 0, len);
    if (audio_len == 0)
        return;
    len = (len > audio_len ? audio_len : len);

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    audio_pos += len;
    audio_len -= len;
}

void *pcmplay(void *arg) {
    Message msg;
    PCMStatus = true;
    pthread_detach(pthread_self());
    try {
        SDL_Init(SDL_INIT_AUDIO);
        SDL_AudioSpec spec;
        spec.freq = 44100;
        spec.format = AUDIO_S16SYS;
        spec.channels = 2;
        spec.silence = 0;
        spec.samples = 1024;
        spec.callback = fill_audio;
        if (SDL_OpenAudio(&spec, NULL) < 0) {
            msg.makepair(3, 0, "In Thread: PCMPLAYING: Audio Device Binding Failed", " Can't open Audio!", 1, __FILE__,
                         __LINE__);
            print_debug(msg, "debug.log");
            return ((void *) 100);
        }
        FILE *wav;
        wav = fopen("bgm2.wav", "rb+");
        if (wav == NULL) {
            msg.makepair(3, 0, "In Thread: PCMPLAYING: Can't open Audio File!", " Can't open Audio File!", 1, __FILE__,
                         __LINE__);
            print_debug(msg, "debug.log");
            return ((void *) 100);
        }

        int pcm_buffer_size = 8192;
        char *pcm_buffer = (char *) malloc(pcm_buffer_size);
        data_count = 0;

        SDL_PauseAudio(0);

        while (true) {
            if (start && !__lock) {
                wav = fopen("bgm.wav", "rb+");
                int pcm_buffer_size = 8192;
                char *pcm_buffer = (char *) malloc(pcm_buffer_size);
                data_count = 0;
                __lock = true;
                msg.makepair(0, 0, "In Thread: PCMPLAYING: Start Game!(Locked!)", "", 1, __FILE__, __LINE__);
                print_debug(msg, "debug.log");
            }
            if (fread(pcm_buffer, 1, pcm_buffer_size, wav) != pcm_buffer_size) {
                fseek(wav, 0, SEEK_SET);
                fread(pcm_buffer, 1, pcm_buffer_size, wav);
                data_count = 0;
            }
            msg.makepair(0, 0, "In Thread: PCMPLAYING: Now Playing  " + itos(data_count) + "  Bytes Data.", "", 1,
                         __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            data_count += pcm_buffer_size;
            audio_chunk = (Uint8 *) pcm_buffer;
            audio_len = pcm_buffer_size;
            audio_pos = audio_chunk;
            while (audio_len > 0)
                SDL_Delay(1);
        }
        free(pcm_buffer);
        SDL_Quit();
        return ((void *) 100);
    } catch (exception e) {
        PCMStatus = false;
        msg.makepair(3, 0, "In Thread: PCMPlaying: Error Detected!", e.what(), 1, __FILE__, __LINE__);
        print_debug(msg, "debug.log");
    }


}

void *threadMonitor_debug(void *arg) {
    pthread_detach(pthread_self());
    while (1) {
        //pthread_mutex_lock(&__mutex);
        if (debug_mode) {
            debugStr1 =
                    "Running Time(Thread): " + itos(cTimer) + "sec" + " Variable:" + itos(SDL_GetTicks()) + "s {2} " +
                    itos(duration);
            debugStr2 =
                    "UserPos(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")   EnemyCount:" + itos(enemy.size()) +
                    "  BulletCount: " + itos(bullet.size()) +
                    "  UserBullet: " + itos(userbullet.size());
            debugStr3 = "ThreadMonitoring: ON   All Bullet:" + itos(HitAll) + "  Hitten: " + itos(HitGet);
            if (PCMStatus) debugStr4 = "背景音效：正常"; else debugStr4 = "背景音效：异常";
            int w, h;
            Image *text3 = textToImage(debugStr1);
            Image *text4 = textToImage(debugStr2);
            Image *text5 = textToImage(debugStr3);
            Image *text6 = textToImage(debugStr4);
            getImageSize(text3, w, h);
            drawImage(text3, 0, SCREEN_HEIGHT - 3 * h);
            cleanup(text3);

            getImageSize(text4, w, h);
            drawImage(text4, 0, SCREEN_HEIGHT - 4 * h);
            cleanup(text4);

            getImageSize(text5, w, h);
            drawImage(text5, 0, SCREEN_HEIGHT - 7 * h);
            cleanup(text5);
            getImageSize(text6, w, h);
            drawImage(text6, 0, SCREEN_HEIGHT - 5.5 * h);
            cleanup(text6);
            sleep(1);
        }
        //pthread_mutex_unlock(&__mutex);
    }
}

void *TimeCountThread(void *arg) {
    pthread_detach(pthread_self());
    while (1) {
        cTimer++;
        sleep(1);
    }
}

void *FileMonitoring(void *arg) {
    pthread_detach(pthread_self());
    while (1) {
        Message msg2;
        msg2.makepair(0, 0, "=====================New Information==========================", "", 1, "", 0);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "Variable Information:", "", 1, __FILE__, __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "Vector Size{1}enemy:" + itos(enemy.size()) + " {2}bullet:" + itos(bullet.size()) +
                            " {3}userbullet:" + itos(userbullet.size()) + "  {4}stopAnimation:" +
                            itos(stopAnimation.size()) + "  {5}userBulletCurve:" + itos(userBulletCurve.size()) +
                            "  {6}BulletCurve:" + itos(bulletCurve.size()), "", 1, __FILE__, __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "Vector Address{1}enemy:" + itos(&enemy) + " {2}bullet:" + itos(&bullet) +
                            " {3}userbullet:" + itos(&userbullet) + "  {4}stopAnimation:" +
                            itos(&stopAnimation) + "  {5}userBulletCurve:" + itos(&userBulletCurve) +
                            "  {6}BulletCurve:" + itos(&bulletCurve), "", 1, __FILE__, __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "Player Point{1}x:" + itos(posPlayer.x) + " {2}y:" + itos(posPlayer.y), "", 1, __FILE__,
                      __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "PCMPlaying: Status " + (PCMStatus ? itos("Normal") : itos("Error")), "", 1, __FILE__,
                      __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "PCMPlaying: NowPlaying " + itos(data_count) + " Bytes Data.", "", 1, __FILE__, __LINE__);
        print_debug(msg2, "monitor.log");

        msg2.makepair(0, 0, "=====================Information End==========================", "", 1, "", 0);
        print_debug(msg2, "monitor.log");
        sleep(1);
    }
}

void initialize() {
    if (firstStart) {
        signal(SIGINT, sigINTHandler);
        signal(SIGABRT, sigABRTHandler);
        signal(SIGFPE, sigFPEHandler);
        signal(SIGILL, sigILLHandler);
        signal(SIGSEGV, sigSEGVHandler);
        firstStart = false;
    }


    HitAll = 0;
    HitGet = 0;
    int err = pthread_create(&bgmthread, NULL, pcmplay, NULL);
    if (err != 0) {
        msg.makepair(3, 0, "Can't create pthread BGMplaying For Error detected.", strerror(err), 1, __FILE__, __LINE__);
        print_debug(msg, "debug.log");
    }

    err = pthread_create(&timerthread, NULL, TimeCountThread, NULL);
    if (err != 0) {
        msg.makepair(3, 0, "Can't create pthread TimeCountThread For Error detected.", strerror(err), 1, __FILE__,
                     __LINE__);
        print_debug(msg, "debug.log");
    }

    err = pthread_create(&fileMonitor, NULL, FileMonitoring, NULL);
    if (err != 0) {
        msg.makepair(3, 0, "Can't create pthread FileMonitoring For Error detected.", strerror(err), 1, __FILE__,
                     __LINE__);
        print_debug(msg, "debug.log");
    }
    srand((unsigned int) (time(NULL)));
    try {
        prog_start();
        FILE *option;
        option = fopen("debug.ini", "r");

    } catch (std::exception e) {
        msg.makepair(3, 0, e.what(), e.what(), 1, __FILE__, __LINE__);
        msg.print();
    }

    FPS_DISPLAY = true;
    //Message msg;
    msg.makepair(0, 0, "FPS Display = True", "", 1, __FILE__, __LINE__);
    print_debug(msg, "debug.log");

    //Initialize vairables
    //Flag = 3;
    posPlayer = PointD(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
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
    hp = 5;
    bump = 3;
    score = 0;
}

void drawPlayer() {
    if (!_game_over) {
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
        if (score < 20) p = 55;
        else if (score < 40) p = 30;
        else if (score < 60) p = 20;
        else p = 10;
        if (Timer % p == 0 && !_game_over) {
            Flight enemytmp;
            enemytmp.newenemy();
            enemytmp.velocity = 5;
            enemytmp.interval = rand() % 10 + 2;
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
            if (score > 20) {
                BulletCurve bulletCurve1;
                bulletCurve1.pos.x = enemytmp.pos.x;
                bulletCurve1.pos.y = enemytmp.pos.y;
                bulletCurve1.velocity = 8;
                bulletCurve1.angle = atan((enemytmp.pos.y - posPlayer.y) / (enemytmp.pos.x - posPlayer.x));
            }
            msg.makepair(0, 0, "New enemy: position(" + itos(enemytmp.pos.x) + "," + itos(enemytmp.pos.y) + ")", "", 1,
                         __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            msg.makepair(0, 0, "Vector Size:" + itos(enemy.size()), "", 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            msg.makepair(0, 0, "bullet Address:" + itos(&bullet) + " Size = " + itos(bullet.size()), "", 1, __FILE__,
                         __LINE__);
            print_debug(msg, "debug.log");
            msg.makepair(0, 0, "userbullet Address:" + itos(&userbullet) + " Size = " + itos(userbullet.size()), "", 1,
                         __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            msg.makepair(0, 0, "CurveBullet Address:" + itos(&bulletCurve), "", 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
        }


        try {
            int w, h;
            getImageSize(imagePlayer, w, h);
            drawImage(imagePlayer, posPlayer.x - w / 2, posPlayer.y - h / 2);
        } catch (exception e) {
            msg.makepair(3, 0, e.what(), e.what(), 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
        }
    }


}

void drawBackground() {
    Rect rect = {0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT};
    //	Pay attention:
    //		(Color){255,255,0} means (Color){255,255,0,0}
    //		and means you will draw nothing
    setPenColor((Color) {0, 0, 0, 0});


    drawRect(rect, true);

}

void drawStopAnimation() {
    for (int i = 0; i < stopAnimation.size(); i++) {
        if (stopAnimation[i].firstLoad) {
            Rect rect;
            rect.x = 32 * stopAnimation[i].frame, rect.y = 0, rect.w = rect.h = 32;
            msg.makepair(0, 0, "StopAnimation Drawing in Image:" + itos(&GameOver) + " with rect:" + itos(&rect) +
                               " with frame " + itos(stopAnimation[i].frame), "", 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            drawImage(GameOver, stopAnimation[i].pos.x, stopAnimation[i].pos.y, 4, 4, 0, NULL, FLIP_NONE, &rect);
            stopAnimation[i].frame++;
            stopAnimation[i].NextStart = duration + 1 / 24;
            stopAnimation[i].firstLoad = false;
            continue;
        }
        if (stopAnimation[i].NextStart < duration) {
            Rect rect;
            rect.x = 32 * stopAnimation[i].frame, rect.y = 0, rect.w = rect.h = 32;
            msg.makepair(0, 0, "StopAnimation Drawing in Image:" + itos(&GameOver) + " with rect:" + itos(&rect) +
                               " with frame " + itos(stopAnimation[i].frame), "", 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            drawImage(GameOver, stopAnimation[i].pos.x, stopAnimation[i].pos.y, 4, 4, 0, NULL, FLIP_NONE, &rect);
            stopAnimation[i].frame++;
            stopAnimation[i].NextStart += 1 / 24;
        }

        if (stopAnimation[i].frame >= 24) {
            stopAnimation.erase(stopAnimation.begin() + i);
            msg.makepair(0, 0, "StopAnimation Erasing:" + itos(i), "", 1, __FILE__, __LINE__);
            print_debug(msg, "debug.log");
            i--;
        }
    }
}

void drawHint() {
    Image *text = textToImage(Hp_bumpStr);
    Image *text2 = textToImage(scoreStr);
    if (debug_mode) {
        int w, h;
        Image *text3 = textToImage(debugStr1);
        Image *text4 = textToImage(debugStr2);
        Image *text5 = textToImage(debugStr3);
        Image *text6 = textToImage(debugStr4);
        getImageSize(text3, w, h);
        drawImage(text3, 0, SCREEN_HEIGHT - 3 * h);
        cleanup(text3);

        getImageSize(text4, w, h);
        drawImage(text4, 0, SCREEN_HEIGHT - 4 * h);
        cleanup(text4);

        getImageSize(text5, w, h);
        drawImage(text5, 0, SCREEN_HEIGHT - 7 * h);
        cleanup(text5);
        getImageSize(text6, w, h);
        drawImage(text6, 0, SCREEN_HEIGHT - 5.5 * h);
        cleanup(text6);
    }
    int w, h;
    getImageSize(text, w, h);
    drawImage(text, 0, SCREEN_HEIGHT - h);
    cleanup(text);

    getImageSize(text2, w, h);
    drawImage(text2, 0, SCREEN_HEIGHT - 2 * h);
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
void drawBullet() {
    if (!_game_over) {
        int w, h;
        getImageSize(imageBullet, w, h);
        //画子弹
        int len = bullet.size();
        for (int i = 0; i < len; ++i) {
            drawImage(imageBullet, bullet[i].pos.x - w / 2, bullet[i].pos.y - h / 2);
        }

        int len2 = userbullet.size();
        for (int j = 0; j < len2; ++j) {
            drawImage(imageBullet, userbullet[j].pos.x - w / 2, userbullet[j].pos.y - h / 2);
        }
        int len3 = bulletCurve.size();
        for (int j = 0; j < len3; j++)
            drawImage(imageBulletCurve, bulletCurve[j].pos.x - w / 2, bulletCurve[j].pos.y - h / 2);

        int len4 = userBulletCurve.size();
        for (int j = 0; j < len4; j++) {
            drawImage(imageBulletCurveUser, userBulletCurve[j].pos.x - w / 2, userBulletCurve[j].pos.y - h / 2);
        }
        len = bullet.size();
        for (int i = 0; i < len; i++) {
            bullet[i].pos.y += bullet[i].velocity;
            if (score > 40 && i % 20 == 0) bullet[i].pos.x += sign[rand() % 2] * rand() % 50;
            bullet[i].getAreaCode();

            try {
                if (bullet[i].pos.y > Game::SCREEN_HEIGHT) {
                    bullet.erase(bullet.begin() + i);
                    i--;
                    len--;
                }
            } catch (std::exception e) {
                std::cout << e.what() << std::endl;
            }

        }

        len = bulletCurve.size();
        for (int i = 0; i < len; i++) {
            bulletCurve[i].pos.x += sin(bulletCurve[i].angle) * bulletCurve[i].velocity;
            bulletCurve[i].pos.y += cos(bulletCurve[i].angle) * bulletCurve[i].velocity;
        }
        len = userBulletCurve.size();
        for (int i = 0; i < len; i++) {
            userBulletCurve[i].pos.x += sin(userBulletCurve[i].angle) * userBulletCurve[i].velocity;
            userBulletCurve[i].pos.y += cos(userBulletCurve[i].angle) * userBulletCurve[i].velocity;
        }
        len2 = userbullet.size();
        for (int i = 0; i < len2; ++i) {
            userbullet[i].pos.y += userbullet[i].velocity;
            //userbullet[i].getAreaCode();
            try {
                if (userbullet[i].pos.y < 0) {
                    userbullet.erase(userbullet.begin() + i);
                    i--;
                    len2--;
                }
            } catch (std::exception ee) {
                std::cout << ee.what() << std::endl;
            }
        }

        len3 = bulletCurve.size();
        for (int i = 0; i < len3; i++) {
            if (bulletCurve[i].pos.x < 0 || bulletCurve[i].pos.y < 0 || bulletCurve[i].pos.x > Game::SCREEN_WIDTH ||
                bulletCurve[i].pos.y > Game::SCREEN_HEIGHT) {
                bulletCurve.erase(bulletCurve.begin() + i);
                i--;
                len3--;
            }
        }

        len4 = userBulletCurve.size();
        for (int i = 0; i < len4; i++) {
            if (userBulletCurve[i].pos.x < 0 || userBulletCurve[i].pos.x > Game::SCREEN_WIDTH ||
                userBulletCurve[i].pos.y < 0 || userBulletCurve[i].pos.y > Game::SCREEN_HEIGHT) {
                userBulletCurve.erase(userBulletCurve.begin() + i);
                i--;
                len4--;
            }
        }


    }
}

void drawEnemy() {
    if (!_game_over) {
        int w, h;
        getImageSize(imageEnemy, w, h);
        //画飞机
        int len = enemy.size();
        for (int i = 0; i < len; ++i) {
            drawImage(imageEnemy, enemy[i].pos.x - w / 2, enemy[i].pos.y - h / 2);
        }

        len = enemy.size();
        for (int i = 0; i < len; i++) {
            enemy[i].pos.y += enemy[i].velocity;
            enemy[i].getAreaCode();
            if (enemy[i].pos.y > Game::SCREEN_HEIGHT) {
                enemy.erase(enemy.begin() + i);
                i--;
            }
        }

        //敌方飞机打子弹

        len = enemy.size();
        for (int j = 0; j < len; ++j) {
            if ((duration_i - enemy[j].OccurTime) % enemy[j].interval == 0) {
                Bullet bullet1;
                bullet1.pos = enemy[j].pos;
                bullet1.velocity = rand() % 10 + enemy[j].velocity;
                bullet.push_back(bullet1);
            }
        }

        if (score > 10) {
            for (int j = 0; j < len; ++j) {
                if ((duration_i - enemy[j].OccurTime) % (enemy[j].interval * 2) == 0) {
                    BulletCurve bulletCurvetmp;
                    bulletCurvetmp.pos = enemy[j].pos;
                    bulletCurvetmp.velocity = 5;
                    bulletCurvetmp.angle = atan((enemy[j].pos.y - posPlayer.y) / (enemy[j].pos.x - posPlayer.x));
                    bulletCurve.push_back(bulletCurvetmp);
                }
            }
        }
    }
}

void draw() {
    drawBackground();
    drawPlayer();
    drawBullet();
    drawEnemy();
    drawHint();
    drawStopAnimation();
}

void deal() {
    if (!_game_over) {
        int w2, h2;
        getImageSize(imageCur, w2, h2);
        drawImage(imageCur, mouseX - w2 / 2, mouseY - h2 / 2);
        //rate = HitGet/HitAll;
        bool move = false;
        //Calculate velocity
        if (keyboard['q'] && bump > 0 && Skill_CD < duration) {
            for (int i = 0; i < enemy.size(); i++) {
                StopAnimation sa;
                sa.pos = enemy[i].pos;
                sa.pos.x -= imagew / 2;
                sa.pos.y -= imageh / 2;
                sa.frame = 0;
                sa.NextStart = duration;
                sa.firstLoad = true;
                stopAnimation.push_back(sa);
            }
            score += enemy.size();
            ClearScreenObject();
            bump--;
            Skill_CD = duration + 2;
            return;
        }
        if (keyboard[KEY_UP] || keyboard['w']) {
            velocityPlayer = velocityPlayer + PointD(0, -1) * speedPlayer;
            move = true;
        }
        if (keyboard[KEY_DOWN] || keyboard['s']) {
            velocityPlayer = velocityPlayer + PointD(0, +1) * speedPlayer;
            move = true;
        }
        if (keyboard[KEY_LEFT] || keyboard['a']) {
            velocityPlayer = velocityPlayer + PointD(-1, 0) * speedPlayer;
            move = true;
        }
        if (keyboard[KEY_RIGHT] || keyboard['d']) {
            velocityPlayer = velocityPlayer + PointD(+1, 0) * speedPlayer;
            move = true;
        }
        if (keyboard[KEY_SPACE] || keyboard['z'] || keyboard[KEY_ENTER]) {
            if (FPSRateCurrent % (FPS_RATE / 10) == 0) {
                Bullet bullet1;
                bullet1.velocity = -5;
                bullet1.user = 0;
                bullet1.pos = posPlayer;
                userbullet.push_back(bullet1);
                HitAll++;
                if (score > 40) {
                    UserBulletCurve userBulletCurve1;
                    userBulletCurve1.velocity = -7;
                    userBulletCurve1.pos = posPlayer;
                    userBulletCurve1.angle = PI / 4;
                    userBulletCurve.push_back(userBulletCurve1);
                    HitAll++;
                    userBulletCurve1.velocity = -7;
                    userBulletCurve1.pos = posPlayer;
                    userBulletCurve1.angle = (-PI / 4);
                    userBulletCurve.push_back(userBulletCurve1);
                    HitAll++;
                }
            }
        }

        //查找碰撞问题
        getImageSize(imagePlayer, imagew, imageh);
        for (int j = 0; j < enemy.size(); ++j) {
            if ((abs(enemy[j].pos.x - posPlayer.x)) < (imagew / 2) &&
                abs(enemy[j].pos.y - posPlayer.y) < (imageh / 2)) {
                StopAnimation sa;
                sa.pos = posPlayer;
                sa.pos.x -= imagew / 2;
                sa.pos.y -= imageh / 2;
                sa.frame = 0;
                sa.NextStart = duration;
                sa.firstLoad = true;
                stopAnimation.push_back(sa);
                score++;
                hp--;
                bump = 3;
                ClearScreenObject();
                break;
            }
        }
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

        for (int i = 0; i < userbullet.size(); ++i) {
            for (int j = 0; j < enemy.size(); ++j) {
                if ((abs(enemy[j].pos.x - userbullet[i].pos.x)) < (imagew / 4) &&
                    abs(enemy[j].pos.y - userbullet[i].pos.y) < (imageh / 4)) {
                    score++;
                    StopAnimation sa;
                    sa.pos = enemy[j].pos;
                    sa.pos.x -= imagew / 2;
                    sa.pos.y -= imageh / 2;
                    sa.frame = 0;
                    sa.NextStart = duration;
                    sa.firstLoad = true;
                    stopAnimation.push_back(sa);
                    userbullet.erase(userbullet.begin() + i);
                    HitGet++;
                    i--;
                    enemy.erase(enemy.begin() + j);
                    j--;
                }
            }
        }

        for (int i = 0; i < userBulletCurve.size(); i++) {
            for (int j = 0; j < enemy.size(); ++j) {
                if ((abs(enemy[j].pos.x - userBulletCurve[i].pos.x)) < (imagew / 4) &&
                    abs(enemy[j].pos.y - userBulletCurve[i].pos.y) < (imageh / 4)) {
                    score++;
                    StopAnimation sa;
                    sa.pos = enemy[j].pos;
                    sa.pos.x -= imagew / 2;
                    sa.pos.y -= imageh / 2;
                    sa.frame = 0;
                    sa.NextStart = duration;
                    sa.firstLoad = true;
                    stopAnimation.push_back(sa);
                    userBulletCurve.erase(userBulletCurve.begin() + i);
                    HitGet++;
                    i--;
                    enemy.erase(enemy.begin() + j);
                    j--;
                }
            }
        }
        for (int i = 0; i < bullet.size(); ++i) {
            if ((abs(posPlayer.x - bullet[i].pos.x)) < (imagew / 4) &&
                abs(posPlayer.y - bullet[i].pos.y) < (imageh / 4)) {
                StopAnimation sa;
                sa.pos = posPlayer;
                sa.pos.x -= imagew / 2;
                sa.pos.y -= imageh / 2;
                sa.frame = 0;
                sa.NextStart = duration;
                sa.firstLoad = true;
                stopAnimation.push_back(sa);
                hp--;
                bump = 3;
                ClearScreenObject();
                break;
            }
        }


        for (int i = 0; i < bulletCurve.size(); ++i) {
            if ((abs(posPlayer.x - bulletCurve[i].pos.x)) < (imagew / 4) &&
                abs(posPlayer.y - bulletCurve[i].pos.y) < (imageh / 4)) {
                StopAnimation sa;
                sa.pos = posPlayer;
                sa.pos.x -= imagew / 2;
                sa.pos.y -= imageh / 2;
                sa.frame = 0;
                sa.NextStart = duration;
                sa.firstLoad = true;
                stopAnimation.push_back(sa);
                hp--;
                bump = 3;
                ClearScreenObject();
                break;
            }
        }
        //Limit player's speed
        double len = velocityPlayer.length();
        if (len > speedPlayer) {
            velocityPlayer = velocityPlayer / len * speedPlayer;
        }

        //Calculate new position
        posPlayer = posPlayer + velocityPlayer;

        //判定是否边界。
        int w, h;
        getImageSize(imagePlayer, w, h);
        posPlayer.x = posPlayer.x + w / 2 > _LIMIT_WIDTH ? _LIMIT_WIDTH - w / 2 : posPlayer.x;
        posPlayer.y = posPlayer.y + h / 2 > _LIMIT_HEIGHT ? _LIMIT_HEIGHT - h / 2 : posPlayer.y;
        posPlayer.x = posPlayer.x - w / 2 < 0 ? w / 2 : posPlayer.x;
        posPlayer.y = posPlayer.y - h / 2 < 0 ? h / 2 : posPlayer.y;
        if (move) {
            msg.makepair(0, 0, "Player pos(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")", "", 1, __FILE__,
                         __LINE__);
            print_debug(msg, "debug.log");
        }
        //Stop player
        if (!move) {
            velocityPlayer = velocityPlayer * 0.8;
            if (velocityPlayer.length() < 0.1)
                velocityPlayer = PointD();
        }
    }
}

void Animation_GameOver(PointD point, uint64_t start) {

    /*for(int i = 1;i <= 24; ++i){
        drawImage(GameOver,point.x,point.y,1,1,0,NULL,FLIP_NONE,&rect);
        rect.x = 32*i + 1;
    }*/
}

int work(bool &quit) {
    if (!start) {
        string Welcome = "Welcome to Thunder Game!";
        string wel2 = "Press [R] To start the game.";
        string wel3 = "Press [D] To debug the game.";
        string wel4 = "Press [ESC] To Exit the game.";
        Image *txt1 = textToImage(Welcome);
        Image *txt2 = textToImage(wel2);
        Image *txt3 = textToImage(wel3);
        Image *txt4 = textToImage(wel4);
        int w, h;
        getImageSize(txt1, w, h);
        drawImage(txt1, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2);
        cleanup(txt1);

        getImageSize(txt2, w, h);
        drawImage(txt2, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + h / 2);
        cleanup(txt2);

        getImageSize(txt3, w, h);
        drawImage(txt3, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + 3 * h / 2);
        cleanup(txt3);

        getImageSize(txt4, w, h);
        drawImage(txt4, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + 5 * h / 2);
        cleanup(txt4);

        if (keyboard['r']) {
            start = true;
        }
        if (keyboard['d']) {
            debug_mode = true;
            hp = 5000;
            start = true;
        }
        if (keyboard[KEY_ESC])
            quit = true;
        return 0;
    }
    if (start) {
        drawPlayer();
        FPSRateCurrent++;
        if (hp > 0) {
            Hp_bumpStr = "HP: " + itos(hp) + "       Skill : " + itos(bump) + "     ";
            if (debug_mode) {
                debugStr1 = "运行时间(线程返回): " + itos(cTimer) + "  秒" + "  (变量返回): {1}" + itos(SDL_GetTicks()) + "秒 {2} " +
                            itos(duration) + " 秒  (系统返回): " + itos(cTimer) + "秒"
                    /*    BGM Sound : ON     Sound : Error(com.apple.audiokit)"*/;
                debugStr2 = "用户坐标(" + itos(posPlayer.x) + "," + itos(posPlayer.y) + ")     敌机计数:" + itos(enemy.size()) +
                            "  子弹数: " + itos(bullet.size()) +
                            "  用户子弹数: " + itos(userbullet.size());
                debugStr3 = "监控系统：启动（简易监控）  总发出子弹：" + itos(HitAll) + "  打中子弹：" + itos(HitGet);
                debugStr4 = "BGM:" + (PCMStatus ? itos("正常") : itos("异常"));
            }

            if (score > 20 && score < 40) Hp_bumpStr += "   是不是觉得有些简单？我们再来一些！";
            else if (score >= 40 && score <= 60) Hp_bumpStr += "   是不是觉得游戏难度更大了？充钱可以让你获得更多有用的道具！";
            else if (score > 60) Hp_bumpStr += "   是不是觉得太难了，赶紧充钱吧！充钱能让你变得更强！";
            if (score > 40)scoreStr = "Your Score: " + itos(score) + "    鼠标技能：已激活  您可以使用鼠标击落飞机！";
            else scoreStr = "Your Score: " + itos(score);
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

        if (hp <= 0) {
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
            rate = (HitAll == 0 ? 0 : (HitGet + 0.000001) / HitAll) * 100;
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
            int w, h;
            getImageSize(txt1, w, h);
            drawImage(txt1, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - h / 2);
            cleanup(txt1);

            getImageSize(txt2, w, h);
            drawImage(txt2, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + h / 2);
            cleanup(txt2);

            getImageSize(txt3, w, h);
            drawImage(txt3, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + 3 * h / 2);
            cleanup(txt3);

            getImageSize(txt4, w, h);
            drawImage(txt4, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + 5 * h / 2);
            cleanup(txt4);

            getImageSize(txt5, w, h);
            drawImage(txt5, SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 + 7 * h / 2);
            cleanup(txt5);

            if (keyboard['r'])
                initialize();


            if (hp == 0) {
                msg.makepair(0, 0, "Game Over! All button locked!", "", 1, __FILE__, __LINE__);
                print_debug(msg, "debug.log");
                double start = duration;
                //int i = 0;
                //while(i<24){
                if (abs(start - duration) < 1e-5) {
                    Rect rect;
                    rect.x = 32 * pxt;
                    rect.y = 0;
                    rect.w = rect.h = 32;
                    msg.makepair(0, 0, "Lead in Image:" + itos(&GameOver) + " with rect:" + itos(&rect), "", 1,
                                 __FILE__, __LINE__);
                    print_debug(msg, "debug.log");
                    drawImage(GameOver, posPlayer.x, posPlayer.y, 4, 4, 0, NULL, FLIP_NONE, &rect);
                    rect.x = 32 * pxt;
                    pxt++;
                    start += 1 / 24;
                }
                //StopAnimationCount = duration_i - start;
                //uint64_t Last = StopAnimationCount;
                //if(Last!=StopAnimationCount){

                //sleep(1);
                //}
                //StopAnimationCount++;
                //}
                if (pxt >= 24) hp--;
            }
        }

        //Draw on the screen
        draw();

        if (keyboard[KEY_ESC])
            quit = true;
        return 0;
    }

}

void mousePress() {
    if (score > 40) {
        for (int j = 0; j < enemy.size(); ++j) {
            if ((abs(enemy[j].pos.x - mouseX)) < (imagew / 1.5) &&
                abs(enemy[j].pos.y - mouseY) < (imageh / 1.5)) {
                score++;
                StopAnimation sa;
                sa.pos = enemy[j].pos;
                sa.pos.x -= imagew / 2;
                sa.pos.y -= imageh / 2;
                sa.frame = 0;
                sa.NextStart = duration;
                sa.firstLoad = true;
                stopAnimation.push_back(sa);
                enemy.erase(enemy.begin() + j);
                j--;
            }
        }
    }
}

void mouseMove() {

}

void mouseRelease() {
}

void keyDown() {
    keyboard[keyValue] = true;
}

void keyUp() {
    keyboard[keyValue] = false;
}

void finale() {

    //Delete all images
    cleanup(imagePlayer, imageBullet, imageEnemy, GameOver, imageCur, imageBulletCurve);
    for (int i = 0; i < imageNumber; ++i)
        cleanup(images[i]);
    try {
        prog_end();
    } catch (std::exception e) {

    }
}

