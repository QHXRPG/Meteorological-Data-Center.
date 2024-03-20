//
// Created by Administrator on 2024/3/17.
//

#ifndef PROJECT1_TIMEOPT_H
#define PROJECT1_TIMEOPT_H

#endif //PROJECT1_TIMEOPT_H
#include "string"
using namespace std;
/* strtime：用于存放获取到的时间。
 * timetvl：时间的偏移量，单位：秒，0是缺省值，表示当前时间
 * fmt：输出时间的格式，fmt每部分的含义：yyyy-年份；mm-月份；dd-日期；hh24-小时；mi-分钟；ss-秒
 * ttime：整数表示的时间。
 * strtime：字符串表示的时间
 * */
string& ltime(string &strtime,const string &fmt="",const int timetvl=0);
string ltime1(const string &fmt="",const int timetvl=0);
string& timetostr(const time_t ttime,string &strtime,const string &fmt="");    // 把整数表示的时间转换为字符串表示的时间。
string timetostr1(const time_t ttime,const string &fmt="");
time_t strtotime(const string &strtime);  // 把字符串表示的时间转换为整数表示的时间。

// 把字符串表示的时间加上一个偏移的秒数后得到一个新的字符串表示的时间。
// in_stime：输入的字符串格式的时间，格式不限，但一定要包括yyyymmddhh24miss
// out_stime：输出的字符串格式的时间。
// timetvl：需要偏移的秒数，正数往后偏移，负数往前偏移。
// fmt：输出字符串时间out_stime的格式，与ltime()函数的fmt参数相同。
bool addtime(const string &in_stime, string out_stime, const int timetvl, const string &fmt="");
bool addtime(const string &in_stime, string &out_stime, const int timetvl, const string &fmt="");

//微妙计数器
class ctimer
{
private:
    struct timeval m_start;    // 计时开始的时间点。
    struct timeval m_end;     // 计时结束的时间点。
public:
    ctimer();          // 构造函数中会调用start方法。

    void start();     // 开始计时。

    // 计算已逝去的时间，单位：秒，小数点后面是微秒。
    // 每调用一次本方法之后，自动调用start方法重新开始计时。
    double elapsed();
};