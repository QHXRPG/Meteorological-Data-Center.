//
// Created by Administrator on 2024/3/18.
//

#include <sys/utime.h>
#include <dirent.h>
#include <algorithm>
#include "fileopt.h"
#include "iostream"
#include "sys/stat.h"
#include "timeopt.h"
#include "stropt.h"


using namespace std;

bool copyfile(const string &srcfilename,const string &dstfilename)
{
    // 创建目标文件的目录。
    if (newdir(dstfilename,true) == false) return false;

    cifile ifile;
    cofile ofile;
    int ifilesize=filesize(srcfilename);

    int  total_bytes=0;
    int  onread=0;
    char buffer[5000];

    if (ifile.open(srcfilename,ios::in|ios::binary)==false) return false;

    if (ofile.open(dstfilename,ios::out|ios::binary)==false) return false;

    while (true)
    {
        if ((ifilesize-total_bytes) > 5000) onread=5000;
        else onread=ifilesize-total_bytes;

        memset(buffer,0,sizeof(buffer));
        ifile.read(buffer,onread);
        ofile.write(buffer,onread);

        total_bytes = total_bytes + onread;

        if (total_bytes == ifilesize) break;
    }

    ifile.close();
    ofile.closeandrename();

    // 更改文件的修改时间属性
    string strmtime;
    filemtime(srcfilename,strmtime);
    setmtime(dstfilename,strmtime);

    return true;
}


bool cifile::open(const string &filename,const ios::openmode mode)
{
    // 如果文件是打开的状态，先关闭它。
    if (fin.is_open())
        fin.close();

    m_filename=filename;

    fin.open(m_filename, mode);

    return fin.is_open();
}

int cifile::read(void *buf,const int bufsize)
{
    // fin.read((char *)buf,bufsize);
    fin.read(static_cast<char *>(buf),bufsize);

    return fin.gcount();          // 返回读取的字节数。
}

bool cifile::closeandremove()
{
    if (fin.is_open()==false) return false;

    fin.close();

    if (remove(m_filename.c_str())!=0) return false;

    return true;
}

void cifile::close()
{
    if (fin.is_open()==false) return;

    fin.close();
}

bool cifile::readline(string &buf,const string& endbz)
{
    buf.clear();            // 清空buf。

    string strline;        // 存放从文件中读取的一行。

    while (true)
    {
        getline(fin, strline);    // 从文件中读取一行。

        if (fin.eof())  // 如果文件已读完。
            break;

        buf=buf + strline;      // 把读取的内容拼接到buf中。

        if (endbz == "")
            return true;          // 如果行没有结尾标志。
        else
        {
            // 如果行有结尾标志，判断本次是否读到了结尾标志，如果没有，继续读，如果有，返回。
            if (buf.find(endbz,buf.length()-endbz.length()) != string::npos)
                return true;
        }

        buf = buf + "\n";        // getline从文件中读取一行的时候，会删除\n，所以，这里要补上\n，因为这个\n不应该被删除。
    }

    return false;
}

bool cofile::open(const string &filename,const bool btmp,const ios::openmode mode,const bool benbuffer)
{
    // 如果文件是打开的状态，先关闭它。
    if (fout.is_open())
        fout.close();

    m_filename=filename;

    newdir(m_filename,true);     // 如果文件的目录不存在，创建目录。

    if (btmp==true)  // 采用临时文件的方案。
    {
        m_filenametmp = m_filename + ".tmp";
        fout.open(m_filenametmp, mode);
    }
    else
    {   // 不采用临时文件的方案。
        m_filenametmp.clear();
        fout.open(m_filename,mode);
    }

    // 不启用文件缓冲区。
    if (benbuffer==false) fout << unitbuf;

    return fout.is_open();
}

bool cofile::write(void *buf,int bufsize)
{
    if (fout.is_open()==false) return false;

    // fout.write((char *)buf,bufsize);
    fout.write(static_cast<char *>(buf),bufsize);

    return fout.good();
}

// 关闭文件，并且把临时文件名改为正式文件名。
bool cofile::closeandrename()
{
    if (fout.is_open()==false) return false;

    fout.close();

    //  如果采用了临时文件的方案。
    if (m_filenametmp.empty()==false)
        if (rename(m_filenametmp.c_str(),m_filename.c_str())!=0) return false;

    return true;
}

// 关闭文件，删除临时文件。
void cofile::close()
{
    if (fout.is_open()==false)
        return;

    fout.close();

    //  如果采用了临时文件的方案。
    if (m_filenametmp.empty()==false)
        remove(m_filenametmp.c_str());
}

bool newdir(const string &pathorfilename,bool bisfilename)
{
    // /tmp/aaa/bbb/ccc/ddd    /tmp    /tmp/aaa    /tmp/aaa/bbb    /tmp/aaa/bbb/ccc

    // 检查目录是否存在，如果不存在，逐级创建子目录
    int pos=1;          // 不要从0开始，0是根目录/。

    while (true)
    {
        int pos1=pathorfilename.find('/',pos);
        if (pos1==string::npos)
            break;

        string strpathname=pathorfilename.substr(0,pos1);      // 截取目录。

        pos=pos1+1;       // 位置后移。
        if (access(strpathname.c_str(),F_OK) != 0)  // 如果目录不存在，创建它。
        {
            // 0755是八进制，不要写成755。
            if (mkdir(strpathname.c_str(),0755) != 0) return false;  // 如果目录不存在，创建它。
        }
    }

    // 如果pathorfilename不是文件，是目录，还需要创建最后一级子目录。
    if (bisfilename==false)
    {
        if (access(pathorfilename.c_str(),F_OK) != 0)
        {
            if (mkdir(pathorfilename.c_str(),0755) != 0) return false;
        }
    }

    return true;
}

int filesize(const string &filename)
{
    struct stat st_filestat;      // 存放文件信息的结构体。
    // 获取文件信息，存放在结构体中。
    if (stat(filename.c_str(), &st_filestat) < 0) return -1;

    return st_filestat.st_size;   // 返回结构体的文件大小成员。
}

bool setmtime(const string &filename,const string &mtime)
{
    struct utimbuf stutimbuf;

    stutimbuf.actime=stutimbuf.modtime=strtotime(mtime);

    if (utime(filename.c_str(),&stutimbuf)!=0) return false;

    return true;
}

time_t strtotime(const string &strtime)
{
    string strtmp,yyyy,mm,dd,hh,mi,ss;

    picknumber(strtime,strtmp,false,false);    // 把字符串中的数字全部提取出来。
    // 2021-12-05 08:30:45
    // 2021/12/05 08:30:45
    // 20211205083045

    if (strtmp.length() != 14) return -1;           // 如果时间格式不是yyyymmddhh24miss，说明时间格式不正确。

    yyyy=strtmp.substr(0,4);
    mm=strtmp.substr(4,2);
    dd=strtmp.substr(6,2);
    hh=strtmp.substr(8,2);
    mi=strtmp.substr(10,2);
    ss=strtmp.substr(12,2);

    struct tm sttm;

    try
    {
        sttm.tm_year = stoi(yyyy) - 1900;
        sttm.tm_mon = stoi(mm) - 1;
        sttm.tm_mday = stoi(dd);
        sttm.tm_hour = stoi(hh);
        sttm.tm_min = stoi(mi);
        sttm.tm_sec = stoi(ss);
        sttm.tm_isdst = 0;
    }
    catch(const std::exception& e)
    {
        return -1;
    }

    return mktime(&sttm);
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

bool addtime(const string &in_stime, string out_stime, const int timetvl, const string &fmt)
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

bool filemtime(const string &filename,string &mtime,const string &fmt)
{
    struct stat st_filestat;      // 存放文件信息的结构体。

    // 获取文件信息，存放在结构体中。
    if (stat(filename.c_str(),&st_filestat) < 0) return false;

    // 把整数表示的时间转换成字符串表示的时间。
    timetostr(st_filestat.st_mtime,mtime,fmt);

    return true;
}

bool filemtime(const string &filename, string mtime, const string &fmt)
{
    struct stat st_filestat;      // 存放文件信息的结构体。

    // 获取文件信息，存放在结构体中。
    if (stat(filename.c_str(),&st_filestat) < 0) return false;

    // 把整数表示的时间转换成字符串表示的时间。
    timetostr(st_filestat.st_mtime, mtime, fmt);

    return true;
}

void cdir::setfmt(const string &fmt)
{
    m_fmt=fmt;
}

bool cdir::opendir(const string &dirname,const string &rules,const int maxfiles,const bool bandchild,bool bsort)
{
    m_filelist.clear();    // 清空文件列表容器。
    m_pos=0;              // 从文件列表中已读取文件的位置归0。

    // 如果目录不存在，创建它。
    if (newdir(dirname,false) == false) return false;

    // 打开目录，获取目录中的文件列表，存放在m_filelist容器中。
    bool ret=_opendir(dirname,rules,maxfiles,bandchild);

    if (bsort==true)    // 对文件列表排序。
    {
        sort(m_filelist.begin(), m_filelist.end());
    }

    return ret;
}

// 这是一个递归函数，在opendir()中调用，cdir类的外部不需要调用它。
bool cdir::_opendir(const string &dirname,const string &rules,const int maxfiles,const bool bandchild)
{
    DIR *dir;   // 目录指针。

    // 打开目录。
    if ( (dir=::opendir(dirname.c_str())) == nullptr ) return false; // opendir与库函数重名，需要加::

    string strffilename;            // 全路径的文件名。
    struct dirent *stdir;            // 存放从目录中读取的内容。

    // 用循环读取目录的内容，将得到目录中的文件名和子目录。
    while ((stdir=::readdir(dir)) != 0) // readdir与库函数重名，需要加::
    {
        // 判断容器中的文件数量是否超出maxfiles参数。
        if ( m_filelist.size() >= maxfiles ) break;

        // 文件名以"."打头的文件不处理。.是当前目录，..是上一级目录，其它以.打头的都是特殊目录和文件。
        if (stdir->d_name[0]=='.') continue;

        // 拼接全路径的文件名。
        strffilename = dirname + '/' + stdir->d_name;

        // 如果是目录，处理各级子目录。
        if (stdir->d_type==4)
        {
            if (bandchild == true)      // 打开各级子目录。
            {
                if (_opendir(strffilename,rules,maxfiles,bandchild) == false)   // 递归调用_opendir函数。
                {
                    closedir(dir);
                    return false;
                }
            }
        }

        // 如果是普通文件，放入容器中。
        if (stdir->d_type==8)
        {
            // 把能匹配上的文件放入m_filelist容器中。
            if (matchstr(stdir->d_name, rules) == false) continue;

            m_filelist.push_back(std::move(strffilename));
        }
    }

    closedir(dir);   // 关闭目录。

    return true;
}

bool cdir::readdir()
{
    // 如果已读完，清空容器
    if (m_pos >= m_filelist.size())
    {
        m_pos=0; m_filelist.clear(); return false;
    }

    // 文件全名，包括路径
    m_ffilename=m_filelist[m_pos];

    // 从绝对路径的文件名中解析出目录名和文件名。
    int pp=m_ffilename.find_last_of("/");
    m_dirname=m_ffilename.substr(0,pp);
    m_filename=m_ffilename.substr(pp+1);

    // 获取文件的信息。
    struct stat st_filestat;
    stat(m_ffilename.c_str(), &st_filestat);
    m_filesize=st_filestat.st_size;                                     // 文件大小。
    m_mtime=timetostr1(st_filestat.st_mtime,m_fmt);   // 文件最后一次被修改的时间。
    m_ctime=timetostr1(st_filestat.st_ctime,m_fmt);      // 文件生成的时间。
    m_atime=timetostr1(st_filestat.st_atime,m_fmt);      // 文件最后一次被访问的时间。

    m_pos++;       // 已读取文件的位置后移。

    return true;
}

cdir::~cdir()
{
    m_filelist.clear();
}

bool renamefile(const string &srcfilename,const string &dstfilename)
{
    // 如果原文件不存在，直接返回失败。
    if (access(srcfilename.c_str(),R_OK) != 0) return false;  //R_OK是一个宏，表示检查文件是否可读取

    // 创建目标文件的目录。
    if (newdir(dstfilename,true) == false) return false;

    // 调用操作系统的库函数rename重命名文件。 mv
    if (rename(srcfilename.c_str(),dstfilename.c_str()) == 0) return true;

    return false;
}

// 忽略关闭全部的信号、关闭全部的IO，缺省只忽略信号，不关IO。
// 不希望后台服务程序被信号打扰，需要什么信号可以在程序中设置。
// 实际上关闭的IO是0、1、2。
void closeioandsignal(bool bcloseio)
{
    int ii=0;

    for (ii=0;ii<64;ii++)
    {
        if (bcloseio==true) close(ii);

        signal(ii,SIG_IGN);
    }
}