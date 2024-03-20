//
// Created by Administrator on 2024/3/18.
//

#include <fstream>
#include <atomic>
#include "timeopt.h"

#ifndef PROJECT1_LOGBUILD_H
#define PROJECT1_LOGBUILD_H

#endif //PROJECT1_LOGBUILD_H


///////////////////////////////////////////////////////////////////////////////////////////////////
// 自旋锁。
class spinlock_mutex
{
private:
    atomic_flag flag{};

    spinlock_mutex(const spinlock_mutex&) = delete;
    spinlock_mutex& operator=(const spinlock_mutex) = delete;
public:
    spinlock_mutex()
    {
        flag.clear();
    }
    void lock()           // 加锁。
    {
        while (flag.test_and_set())
            ;
    }
    void unlock()      // 解锁。
    {
        flag.clear();
    }
};

///////////////////////////////////// /////////////////////////////////////
// 日志文件。
class clogfile
{
    ofstream fout;                       // 日志文件对象。
    string   m_filename;              // 日志文件名，建议采用绝对路径。
    ios::openmode m_mode;      // 日志文件的打开模式。
    bool     m_backup{};                // 是否自动切换日志。
    int        m_maxsize;               // 当日志文件的大小超过本参数时，自动切换日志。
    bool     m_enbuffer{};              // 是否启用文件缓冲区。
    spinlock_mutex m_splock;    // 自旋锁，用于多线程程序中给写日志的操作加锁。

public:
    // 构造函数，日志文件的大小缺省100M。
    clogfile(int maxsize=100):m_maxsize(maxsize){}

    // 打开日志文件。
    // filename：日志文件名，建议采用绝对路径，如果文件名中的目录不存在，就先创建目录。
    // openmode：日志文件的打开模式，缺省值是ios::app。
    // bbackup：是否自动切换（备份），true-切换，false-不切换，在多进程的服务程序中，如果多个进程共用一个日志文件，bbackup必须为false。
    // benbuffer：是否启用文件缓冲机制，true-启用，false-不启用，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件，缺省是不启用。
    // 注意，在多进程的程序中，多个进程往同一日志文件写入大量的日志时，可能会出现小混乱，但是，多线程不会。
    // 1）多个进程往同一日志文件写入大量的日志时，可能会出现小混乱，这个问题并不严重，可以容忍；
    // 2）只有同时写大量日志时才会出现混乱，在实际开发中，这种情况不多见。
    // 3）如果业务无法容忍，可以用信号量加锁。
    bool open(const string &filename,const ios::openmode mode=ios::app,const bool bbackup=true,const bool benbuffer=false);

    // 把日志内容以文本的方式格式化输出到日志文件，并且，在日志内容前面写入时间。
    template< typename... Args >
    bool write(const char* fmt, Args... args)
    {
        if (!fout.is_open()) return false;

        backup();                   // 判断是否需要切换日志文件。

        m_splock.lock();        // 加锁。
        fout << ltime1() << " " << sformat(fmt,args...);      // 把当前时间和日志内容写入日志文件。
        m_splock.unlock();    // 解锁。

        return fout.good();
    }

    // 重载<<运算符，把日志内容以文本的方式输出到日志文件，不会在日志内容前面写时间。
    // 注意：内容换行用\n，不能用endl。
    template<typename T>
    clogfile& operator<<(const T &value)
    {
        m_splock.lock();
        fout << value;
        m_splock.unlock();

        return *this;
    }

private:
    // 如果日志文件的大小超过m_maxsize的值，就把当前的日志文件名改为历史日志文件名，再创建新的当前日志文件。
    // 备份后的文件会在日志文件名后加上日期时间，如/tmp/log/filetodb.log.20200101123025。
    // 注意，在多进程的程序中，日志文件不可切换，多线的程序中，日志文件可以切换。
    bool backup();
public:
    void close() { fout.close(); }

    ~clogfile() { close(); };
};