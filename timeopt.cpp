//
// Created by Administrator on 2024/3/17.
//
#include <cstring>
#include "timeopt.h"
#include "stropt.h"
#include "time.h"

// 把整数表示的时间转换为字符串表示的时间。
// ttime：整数表示的时间。
// strtime：字符串表示的时间。
// fmt：输出字符串时间strtime的格式，与ttime函数的fmt参数相同，如果fmt的格式不正确，strtime将为空。
string& timetostr(const time_t ttime,string &strtime,const string &fmt)
{
    //struct tm sttm = *localtime ( &ttime );        // 非线程安全。
    struct tm sttm;
//    localtime_r ((tm *) &ttime, &sttm);   // 线程安全。
    sttm.tm_year=sttm.tm_year+1900;                // tm.tm_year成员要加上1900。
    sttm.tm_mon++;                                            // sttm.tm_mon成员是从0开始的，要加1。

    // 缺省的时间格式。
    if ( (fmt=="") || (fmt=="yyyy-mm-dd hh24:mi:ss") )
    {
        strtime=sformat("%04u-%02u-%02u %02u:%02u:%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,\
                   sttm.tm_hour,sttm.tm_min,sttm.tm_sec);
        return strtime;
    }

    if (fmt=="yyyy-mm-dd hh24:mi")
    {
        strtime=sformat("%04u-%02u-%02u %02u:%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,\
                   sttm.tm_hour,sttm.tm_min);
        return strtime;
    }

    if (fmt=="yyyy-mm-dd hh24")
    {
        strtime=sformat("%04u-%02u-%02u %02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,sttm.tm_hour);
        return strtime;
    }

    if (fmt=="yyyy-mm-dd")
    {
        strtime=sformat("%04u-%02u-%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday);
        return strtime;
    }

    if (fmt=="yyyy-mm")
    {
        strtime=sformat("%04u-%02u",sttm.tm_year,sttm.tm_mon);
        return strtime;
    }

    if (fmt=="yyyymmddhh24miss")
    {
        strtime=sformat("%04u%02u%02u%02u%02u%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,\
                   sttm.tm_hour,sttm.tm_min,sttm.tm_sec);
        return strtime;
    }

    if (fmt=="yyyymmddhh24mi")
    {
        strtime=sformat("%04u%02u%02u%02u%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,\
                   sttm.tm_hour,sttm.tm_min);
        return strtime;
    }

    if (fmt=="yyyymmddhh24")
    {
        strtime=sformat("%04u%02u%02u%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday,sttm.tm_hour);
        return strtime;
    }

    if (fmt=="yyyymmdd")
    {
        strtime=sformat("%04u%02u%02u",sttm.tm_year,sttm.tm_mon,sttm.tm_mday);
        return strtime;
    }

    if (fmt=="hh24miss")
    {
        strtime=sformat("%02u%02u%02u",sttm.tm_hour,sttm.tm_min,sttm.tm_sec);
        return strtime;
    }

    if (fmt=="hh24mi")
    {
        strtime=sformat("%02u%02u",sttm.tm_hour,sttm.tm_min);
        return strtime;
    }

    if (fmt=="hh24")
    {
        strtime=sformat("%02u",sttm.tm_hour);
        return strtime;
    }

    if (fmt=="mi")
    {
        strtime=sformat("%02u",sttm.tm_min);
        return strtime;
    }

    return strtime;
}

char* timetostr(const time_t ttime,char *strtime,const string &fmt)
{
    if (strtime==nullptr) return nullptr;    // 判断空指针。

    string str;
    timetostr(ttime,str,fmt);           // 直接调用string& timetostr(const time_t ttime,string &strtime,const string &fmt="");
    str.copy(strtime,str.length());
    strtime[str.length()]=0;           // string的copy函数不会给C风格字符串的结尾加0。

    return strtime;
}

string timetostr1(const time_t ttime,const string &fmt)
{
    string str;
    timetostr(ttime,str,fmt);           // 直接调用string& timetostr(const time_t ttime,string &strtime,const string &fmt="");
    return str;
}

string& ltime(string &strtime,const string &fmt,const int timetvl)
{
    time_t  timer;
    time(&timer );                          // 获取系统当前时间。

    timer=timer+timetvl;              // 加上时间的偏移量。

    timetostr(timer,strtime,fmt);   // 把整数表示的时间转换为字符串表示的时间。

    return strtime;
}

char* ltime(char *strtime,const string &fmt,const int timetvl)
{
    if (strtime==nullptr) return nullptr;    // 判断空指针。

    time_t  timer;
    time(&timer );                          // 获取系统当前时间。

    timer=timer+timetvl;              // 加上时间的偏移量。

    timetostr(timer,strtime,fmt);   // 把整数表示的时间转换为字符串表示的时间。

    return strtime;
}

string ltime1(const string &fmt,const int timetvl)
{
    string strtime;

    ltime(strtime,fmt,timetvl);   // 直接调用string& ltime(string &strtime,const string &fmt="",const int timetvl=0);

    return strtime;
}

bool addtime(const string &in_stime,string &out_stime,const int timetvl,const string &fmt)
{
    time_t  timer;

    // 把字符串表示的时间转换为整数表示的时间，方便运算。
    if ( (timer=strtotime(in_stime))==-1) { out_stime=""; return false; }

    timer=timer+timetvl;  // 时间运算。

    // 把整数表示的时间转换为字符串表示的时间。
    timetostr(timer,out_stime,fmt);

    return true;
}

bool addtime(const string &in_stime,char *out_stime,const int timetvl,const string &fmt)
{
    if (out_stime==nullptr) return false;    // 判断空指针。

    time_t  timer;

    // 把字符串表示的时间转换为整数表示的时间，方便运算。
    if ( (timer=strtotime(in_stime))==-1) { strcpy(out_stime,""); return false; }

    timer=timer+timetvl;  // 时间运算。

    // 把整数表示的时间转换为字符串表示的时间。
    timetostr(timer,out_stime,fmt);

    return true;
}