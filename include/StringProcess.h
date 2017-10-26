//
// Created by Peter Zheng on 2017/10/24.
//
#include "CommonInclude.h"
#ifndef STG_GAME_STRINGPROCESS_H
#define STG_GAME_STRINGPROCESS_H

#endif //STG_GAME_STRINGPROCESS_H

template <class T>
string itos(T a){
    T p = a;
    stringstream ss;
    ss << p;
    string string1;
    ss >> string1;
    return string1;
}