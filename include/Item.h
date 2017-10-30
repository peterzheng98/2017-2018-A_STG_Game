//
// Created by Peter Zheng on 2017/10/28.
//

#ifndef STG_GAME_ITEM_H
#define STG_GAME_ITEM_H

#endif //STG_GAME_ITEM_H

#include "CommonInclude.h"
#include "pointd.h"
struct Flight {
    flight kind;
    int hp;
    int w, h;
    PointD pos;
    bool avail;
    PointD areacode;
    int velocity;
    int OccurTime, interval;
    void getAreaCode(){
        areacode.x = pos.x / 128;
        areacode.y = pos.y / 96;
    }
    void newenemy() {
        kind = Enemy;
        hp = _HP_Enemy;
        //srand((time(NULL)));
        pos.x = rand() % Game::SCREEN_WIDTH;
        pos.y = 0;
    };
};
struct Bullet {
    PointD pos;
    int trace;
    // 1 = 圆心散开  2 = 尾随我方飞机 3 = 竖直飞
    int velocity;
    bool avail;
    PointD areacode;
    void getAreaCode(){
        areacode.x = pos.x / 128;
        areacode.y = pos.y / 96;
    }
};
struct obj {
    int kind;
    int usage;
    // 1 = 我方飞机减速 2 = 我方飞机加速 3 = 敌方飞机加血 4 = 敌方飞机子弹数加倍 5 = 敌方无敌 6 = 敌方飞机增加 7 = 清屏
    PointD pos;
    int areacode;
    bool avail;
};
