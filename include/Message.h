//
// Created by Peter Zheng on 2017/10/24.
//
#include "CommonInclude.h"
/*#ifndef STG_GAME_MESSAGE_H
#define STG_GAME_MESSAGE_H

#endif //STG_GAME_MESSAGE_H*/

const char *ID2Note[4] = {"INFO","WARN","Exception","FatalError"};
struct Message{
    int type;
    int id;
    string text;
    string fataltext;
    int Fileid;
    string FileName;
    int line;
    tm Time;

    void print(){
        time_t timer;
        time(&timer);
        struct tm *nowTime = localtime(&timer);
        Time = *nowTime;
        printf("[%d-%d-%d %d:%d:%d]:[%s at File(%s) id(%d) Flag %d]%s",Time.tm_year+1900,Time.tm_mon+1
        ,Time.tm_mday,Time.tm_hour,Time.tm_min,Time.tm_sec,ID2Note[type],FileName.c_str(),Fileid,line,text.c_str());
        if(id==3) printf(" %s\n",fataltext.c_str()); else printf("\n");
    }

    void print2file(FILE *fq){
        time_t timer;
        time(&timer);
        struct tm *nowTime = localtime(&timer);
        Time = *nowTime;
        fprintf(fq,"[%d-%d-%d %d:%d:%d]:[%s at File(%s) id(%d) Flag %d]%s",Time.tm_year+1900,Time.tm_mon+1
                ,Time.tm_mday,Time.tm_hour,Time.tm_min,Time.tm_sec,ID2Note[type],FileName.c_str(),Fileid,line,text.c_str());
        if(id==3) fprintf(fq," %s\n",fataltext.c_str()); else fprintf(fq,"\n");
    }

    void makepair(int Type,int Id, string Text, string Fataltext, int fileid, string filename, int Line){
        type = Type;
        id = Id;
        text = Text;
        fataltext = Fataltext;
        Fileid = fileid;
        FileName = filename;
        line = Line;
        //Time = time;
    }
};

void print_debug(Message msg, string filename){
    try{
        FILE *file;
        const char* p = filename.data();
        file = fopen(p,"a+");
        msg.print2file(file);
        fclose(file);
    } catch (exception e){

    }
}



