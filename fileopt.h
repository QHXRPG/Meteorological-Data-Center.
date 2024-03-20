//
// Created by Administrator on 2024/3/18.
//

#ifndef PROJECT1_FILEOPT_H
#define PROJECT1_FILEOPT_H

#endif //PROJECT1_FILEOPT_H

#include "iostream"
#include "fstream"
#include "vector"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////

// 根据绝对路径的文件名或目录名逐级的创建目录。
// pathorfilename：绝对路径的文件名或目录名。
// bisfilename：指定pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，缺省值为true。
// 返回值：true-成功，false-失败，如果返回失败，原因有大概有三种情况：
// 1）权限不足；2）pathorfilename参数不是合法的文件名或目录名；3）磁盘空间不足。
bool newdir(const string &pathorfilename,bool bisfilename=true);

///////////////////////////////////// /////////////////////////////////////
// 文件操作相关的函数

// 重命名文件，类似Linux系统的mv命令。
// srcfilename：原文件名，建议采用绝对路径的文件名。
// dstfilename：目标文件名，建议采用绝对路径的文件名。
// 返回值：true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够，如果原文件和目标文件不在同一个磁盘分区，重命名也可能失败。
// 注意，在重命名文件之前，会自动创建dstfilename参数中包含的目录。
// 在应用开发中，可以用renamefile()函数代替rename()库函数。
bool renamefile(const string &srcfilename,const string &dstfilename);

// 复制文件，类似Linux系统的cp命令。
// srcfilename：原文件名，建议采用绝对路径的文件名。
// dstfilename：目标文件名，建议采用绝对路径的文件名。
// 返回值：true-成功；false-失败，失败的主要原因是权限不足或磁盘空间不够。
// 注意：
// 1）在复制文件之前，会自动创建dstfilename参数中的目录名。
// 2）复制文件的过程中，采用临时文件命名的方法，复制完成后再改名为dstfilename，避免中间状态的文件被读取。
// 3）复制后的文件的时间与原文件相同，这一点与Linux系统cp命令不同。
bool copyfile(const string &srcfilename,const string &dstfilename);

// 获取文件的大小。
// filename：待获取的文件名，建议采用绝对路径的文件名。
// 返回值：如果文件不存在或没有访问权限，返回-1，成功返回文件的大小，单位是字节。
int filesize(const string &filename);

// 获取文件的时间。
// filename：待获取的文件名，建议采用绝对路径的文件名。
// mtime：用于存放文件的时间，即stat结构体的st_mtime。
// fmt：设置时间的输出格式，与ltime()函数相同，但缺省是"yyyymmddhh24miss"。
// 返回值：如果文件不存在或没有访问权限，返回false，成功返回true。
bool filemtime(const string &filename,string &mtime,const string &fmt="yyyymmddhh24miss");

// 重置文件的修改时间属性。
// filename：待重置的文件名，建议采用绝对路径的文件名。
// mtime：字符串表示的时间，格式不限，但一定要包括yyyymmddhh24miss，一个都不能少，顺序也不能变。
// 返回值：true-成功；false-失败，失败的原因保存在errno中。
bool setmtime(const string &filename,const string &mtime);
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////// /////////////////////////////////////
// 获取某目录及其子目录中的文件列表的类。
class cdir
{
private:
    vector<string> m_filelist;  // 存放文件列表的容器（绝对路径的文件名）。
    int m_pos;                          // 从文件列表m_filelist中已读取文件的位置。
    string m_fmt;                     // 文件时间格式，缺省"yyyymmddhh24miss"。

    cdir(const cdir &) = delete;                      // 禁用拷贝构造函数。
    cdir &operator=(const cdir &) = delete;  // 禁用赋值函数。
public:
    // /project/public/_public.h
    string m_dirname;        // 目录名，例如：/project/public
    string m_filename;       // 文件名，不包括目录名，例如：_public.h
    string m_ffilename;      // 绝对路径的文件，例如：/project/public/_public.h
    int      m_filesize;          // 文件的大小，单位：字节。
    string m_mtime;           // 文件最后一次被修改的时间，即stat结构体的st_mtime成员。
    string m_ctime;            // 文件生成的时间，即stat结构体的st_ctime成员。
    string m_atime;            // 文件最后一次被访问的时间，即stat结构体的st_atime成员。

    cdir():m_pos(0),m_fmt("yyyymmddhh24miss") {}  // 构造函数。

    // 设置文件时间的格式，支持"yyyy-mm-dd hh24:mi:ss"和"yyyymmddhh24miss"两种，缺省是后者。
    void setfmt(const string &fmt);

    // 打开目录，获取目录中文件的列表，存放在m_filelist容器中。
    // dirname，目录名，采用绝对路径，如/tmp/root。
    // rules，文件名的匹配规则，不匹配的文件将被忽略。
    // maxfiles，本次获取文件的最大数量，缺省值为10000个，如果文件太多，可能消耗太多的内存。
    // bandchild，是否打开各级子目录，缺省值为false-不打开子目录。
    // bsort，是否按文件名排序，缺省值为false-不排序。
    // 返回值：true-成功，false-失败。
    bool opendir(const string &dirname,const string &rules,const int maxfiles=10000,const bool bandchild=false,bool bsort=false);

private:
    // 这是一个递归函数，被opendir()的调用，在cdir类的外部不需要调用它。
    bool _opendir(const string &dirname,const string &rules,const int maxfiles,const bool bandchild);

public:
    // 从m_filelist容器中获取一条记录（文件名），同时获取该文件的大小、修改时间等信息。
    // 调用opendir方法时，m_filelist容器被清空，m_pos归零，每调用一次readdir方法m_pos加1。
    // 当m_pos小于m_filelist.size()，返回true，否则返回false。
    bool readdir();

    unsigned int size() { return m_filelist.size(); }

    ~cdir();  // 析构函数。
};
///////////////////////////////////// /////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 写文件的类。
class cofile  // class out file
{
private:
    ofstream fout;                  // 写入文件的对象。
    string   m_filename;         // 文件名，建议采用绝对路径。
    string   m_filenametmp;  // 临时文件名，在m_filename后面加".tmp"。
public:
    cofile() {}
    bool isopen() const { return fout.is_open(); }     // 文件是否已打开。

    // 打开文件。
    // filename，待打开的文件名。
    // btmp，是否采用临时文件的方案。
    // mode，打开文件的模式。
    // benbuffer，是否启用文件缓冲区。
    bool open(const string &filename,const bool btmp=true,const ios::openmode mode=ios::out,const bool benbuffer=true);

    // 把数据以文本的方式格式化输出到文件。
    template< typename... Args >
    bool writeline(const char* fmt, Args... args)
    {
        if (fout.is_open()==false) return false;

        fout << sformat(fmt,args...);

        return fout.good();
    }

    // 重载<<运算符，把数据以文本的方式输出到文件。
    // 注意：换行只能用\n，不能用endl。
    template<typename T>
    cofile& operator<<(const T &value)
    {
        fout << value; return *this;
    }

    // 把二进制数据写入文件。
    bool write(void *buf,int bufsize);

    // 关闭文件，并且把临时文件名改为正式文件名。
    bool closeandrename();

    // 关闭文件，如果有临时文件，则删除它。
    void close();

    ~cofile() { close(); };
};
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// 读取文件的类。
class cifile    // class in file
{
private:
    ifstream fin;                     // 读取文件的对象。
    string   m_filename;         // 文件名，建议采用绝对路径。
public:
    cifile() {}

    // 判断文件是否已打开。
    bool isopen() const { return fin.is_open(); }

    // 打开文件。
    // filename，待打开的文件名。
    // mode，打开文件的模式。
    bool open(const string &filename,const ios::openmode mode=ios::in);

    // 以行的方式读取文本文件，endbz指定行的结尾标志，缺省为空，没有结尾标志。
    bool readline(string &buf,const string& endbz="");

    // 读取二进制文件，返回实际读取到的字节数。
    int read(void *buf,const int bufsize);

    // 关闭并删除文件。
    bool closeandremove();

    // 只关闭文件。
    void close();

    ~cifile() { close(); }
};
///////////////////////////////////////////////////////////////////////////////////////////////////