//
// Created by Administrator on 2024/3/17.
//

#ifndef PROJECT1_STROPT_H
#define PROJECT1_STROPT_H

#endif //PROJECT1_STROPT_H

#include "string"
#include "vector"
using namespace std;
string deleteCh(string& str, const char ch, bool bloop= false); // 删除字符串指定的字符，缺省删除空格。
string deleteLeft(string& str, const char ch = ' '); //删除字符串左边的字符，缺省删除空格。
string deleteRight(string& str, const char ch = ' ');  //删除字符串右边的字符，缺省删除空格。
string deleteLR(string& str, const char ch=' ');  //删除字符串左右边的字符，缺省删除空格。
string& toupper(string &str);
string& tolower(string &str);
string& replaceStr(string& str, string& strSubOld, string& strSubNew, bool bloop = false);
string& picknumber(const string &src,string &dest,const bool bsigned=false,const bool bdot=false);
string picknumber(const string &src,const bool bsigned=false,const bool bdot=false);

///////////////////////////////////// /////////////////////////////////////
// ccmdstr类用于拆分有分隔符的字符串。demo20.cpp
// 字符串的格式为：字段内容1+分隔符+字段内容2+分隔符+字段内容3+分隔符+...+字段内容n。
// 例如："messi,10,striker,30,1.72,68.5,Barcelona"，这是足球运动员梅西的资料。
// 包括：姓名、球衣号码、场上位置、年龄、身高、体重和效力的俱乐部，字段之间用半角的逗号分隔。
class ccmdstr
{
private:
    vector<string> m_cmdstr;  // 存放拆分后的字段内容。

    ccmdstr(const ccmdstr &) = delete;                      // 禁用拷贝构造函数。
    ccmdstr &operator=(const ccmdstr &) = delete;  // 禁用赋值函数。
public:
    ccmdstr()  = default; // 构造函数。
    ccmdstr(const string &buffer,const string &sepstr,const bool bdelspace=false);

    const string& operator[](int ii) const     // 重载[]运算符，可以像访问数组一样访问m_cmdstr成员。
    {
        return m_cmdstr[ii];
    }

    // 把字符串拆分到m_cmdstr容器中。
    // buffer：待拆分的字符串。
    // sepstr：buffer中采用的分隔符，注意，sepstr参数的数据类型不是字符，是字符串，如","、" "、"|"、"~!~"。
    // bdelspace：拆分后是否删除字段内容前后的空格，true-删除；false-不删除，缺省不删除。
    void splittocmd(const string &buffer, const string &sepstr, const bool bdelspace=false);

    // 获取拆分后字段的个数，即m_cmdstr容器的大小。
    int size() const
    {
        return m_cmdstr.size();
    }
    int cmdcount() const
    {
        return m_cmdstr.size();
    }      // 兼容以前的项目。
    // 从m_cmdstr容器获取字段内容。
    // ii：字段的顺序号，类似数组的下标，从0开始。
    // value：传入变量的地址，用于存放字段内容。
    // 返回值：true-成功；如果ii的取值超出了m_cmdstr容器的大小，返回失败。
    bool getvalue(const int ii,string &value,const int ilen=0) const;      // C++风格字符串。视频中没有第三个参数，加上第三个参数更好。
    bool getvalue(const int ii,int  &value) const;                                    // int整数。
    bool getvalue(const int ii,unsigned int &value) const;                     // unsigned int整数。
    bool getvalue(const int ii,long &value) const;                                  // long整数。
    bool getvalue(const int ii,unsigned long &value) const;                  // unsigned long整数。
    bool getvalue(const int ii,double &value) const;                              // 双精度double。
    bool getvalue(const int ii,float &value) const;                                  // 单精度float。
    bool getvalue(const int ii,bool &value) const;                                  // bool型。
    ~ccmdstr(); // 析构函数。
};
ostream& operator<<(ostream& out, const ccmdstr& cc);

///////////////////////////////////// /////////////////////////////////////
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,string &value,const int ilen=0);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,char *value,const int ilen=0);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,bool &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,int  &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,unsigned int &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,long &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,unsigned long &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,double &value);
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,float &value);

///////////////////////////////////// /////////////////////////////////////
// C++格式化输出函数模板。 demo23.cpp
template< typename... Args >
bool sformat(string &str,const char* fmt, Args... args )
{
    int len = snprintf( nullptr, 0, fmt, args... );      // 得到格式化输出后字符串的总长度。
    if (len < 0) return false;                                  // 如果调用snprintf失败，返回-1。
    if (len == 0) { str.clear(); return true; }            // 如果调用snprintf返回0，表示格式化输出的内容为空。

    str.resize(len);                                                 // 为string分配内存。
    snprintf(&str[0], len + 1, fmt, args... );           // linux平台第二个参数是len+1，windows平台是len。
    return true;
}
template< typename... Args >
string sformat(const char* fmt, Args... args )
{
    string str;

    int len = snprintf( nullptr, 0, fmt, args... );      // 得到格式化后字符串的长度。
    if (len < 0) return str;              // 如果调用snprintf失败，返回-1。
    if (len == 0) return str;           // 如果调用snprintf返回0，表示格式化输出的内容为空。;

    str.resize(len);                                                // 为string分配内存。
    snprintf(&str[0], len + 1, fmt, args... );          // linux平台第二个参数是len+1，windows平台是len。
    return str;
}


