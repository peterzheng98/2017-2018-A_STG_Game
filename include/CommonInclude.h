//
// Created by Peter Zheng on 2017/10/24.
//

#ifndef STG_GAME_COMMONINCLUDE_H
#define STG_GAME_COMMONINCLUDE_H

enum flight{
    Enemy,
    Boss,
    Friend,
    MainFlight
};

//定义飞机的基础血量
const int _HP_Boss = 100;
const int _HP_Enemy = 10;
const int _HP_Friend = 10;

#endif //STG_GAME_COMMONINCLUDE_H


#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include <stdexcept>
#include <stack>
#include <queue>
#include <deque>
#include <sstream>
#include <cstdlib>
#include <vector>
//#include <pthread>
#include <unistd.h>
using namespace std;

