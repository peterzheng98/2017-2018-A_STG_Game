//
// Created by Peter Zheng on 2017/11/20.
//

#ifndef STG_GAME_TIMERINTERVAL_H
#define STG_GAME_TIMERINTERVAL_H

class TimerInterval {
public:
    void New(int p, bool current) {
        TSec = p;
        Current = current;
    }

    void SetTime(int curSec) {
        CurSec = curSec;
        Delta = CurSec - TSec;
    }

    void SetInterval(int interval) {
        Interval = interval;
    }

    bool Check() {
        if (Delta % Interval == 0)
            return true;
        else
            return false;
    }

private:
    int CurSec, TSec, Delta, Interval;
    bool Current;
};

class CountingInterval {
public:
    void Start(int p) {
        Tsec = p;
    }

    bool Check() {

    }

private:
    int CurSec, Tsec;
    bool Current;
};

#endif //STG_GAME_TIMERINTERVAL_H
