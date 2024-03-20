//
// Created by Administrator on 2024/3/18.
//

#include "logbuild.h"
#include "fileopt.h"

bool clogfile::open(const string &filename,const ios::openmode mode,const bool bbackup,const bool benbuffer)
{
    // 如果日志文件是打开的状态，先关闭它。
    if (fout.is_open()) fout.close();

    m_filename=filename;        // 日志文件名。
    m_mode=mode;                 // 打开模式。
    m_backup=bbackup;          // 是否自动备份。
    m_enbuffer=benbuffer;      // 是否启用文件缓冲区。

    newdir(m_filename,true);                              // 如果日志文件的目录不存在，创建它。

    fout.open(m_filename,m_mode);                  // 打开日志文件。

    if (m_enbuffer==false) fout << unitbuf;       // 是否启用文件缓冲区。

    return fout.is_open();
}

bool clogfile::backup()
{
    // 不备份
    if (m_backup == false) return true;

    if (fout.is_open() == false) return false;

    // 如果当前日志文件的大小超过m_maxsize，备份日志。
    if (fout.tellp() > m_maxsize*1024*1024)
    {
        m_splock.lock();       // 加锁。

        fout.close();              // 关闭当前日志文件。

        // 拼接备份日志文件名。
        string bak_filename=m_filename+"."+ltime1("yyyymmddhh24miss");

        rename(m_filename.c_str(),bak_filename.c_str());   // 把当前日志文件改名为备份日志文件。

        fout.open(m_filename,m_mode);              // 重新打开当前日志文件。

        if (m_enbuffer==false) fout << unitbuf;   // 判断是否启动文件缓冲区。

        m_splock.unlock();   // 解锁。

        return fout.is_open();
    }

    return true;
}