//
// Created by Administrator on 2024/3/17.
//

#include "stropt.h"
#include "iostream"
#include "string"
#include "cstring"
using namespace std;

string deleteCh(string &str, const char ch, bool bloop) {
    size_t size = str.size();
    if (!size)
        return str;
    int i = 0;
    while (i < size) {
        if (str[i] == ch)
            break;
        i++;
    }
    if (i == size)
        return str;
    str.replace(i, 1, "");
    if (bloop)
        deleteCh(str, ch, bloop);
    return str;
}

string deleteLeft(string &str, const char ch) {
    size_t size = str.size();
    if (!size)
        return str;
    if (str[0] != ch)
        return str;
    int i = 0;
    while (str[i] == ch)
        i++;
    str.replace(0, i, "");
    return str;
}

string deleteRight(string &str, const char ch) {
    size_t size = str.size();
    if (!size)
        return str;
    int i = str.size() - 1;
    if (str[i] != ch)
        return str;
    while (str[i] == ch)
        i--;
    str.replace(i + 1, str.size() - i, "");
    return str;
}

string deleteLR(string &str, const char ch) {
    deleteLeft(str, ch);
    deleteRight(str, ch);
    return str;
}

string &toupper(string &str) {
    for (auto &cc: str) {
        if ((cc >= 'a') && (cc <= 'z')) cc = cc - 32;
    }
    return str;
}

string &tolower(string &str) {
    for (auto &cc: str) {
        if ((cc >= 'A') && (cc <= 'Z')) cc = cc + 32;
    }
    return str;
}

string &replaceStr(string &str, string &strSubOld, string &strSubNew, bool bloop) {
    if (!str.length() * strSubOld.length() * strSubNew.length()) //如果其中一个字符串为空，那么没有意义
    {
        cout << "其中一个字符串为空" << endl;
        return str;
    }
    if (strSubNew.find(strSubOld)) //防止死循环
    {
        cout << "非法操作" << endl;
        return str;
    }
    int pos, ppos = 0;
    while (true) {
        if (str.find(strSubOld, ppos) == std::string::npos)
            return str;
        pos = (int) str.find(strSubOld, ppos);
        ppos = (int) strSubNew.length() + pos;
        str.replace(pos, strSubOld.length(), "");
        str.insert(pos, strSubNew);
        if (!bloop)
            return str;
    }
}
string& picknumber(const string &src,string &dest,const bool bsigned,const bool bdot)
{
    // 为了支持src和dest是同一变量的情况，定义str临时变量。
    string str;

    for (char cc:src)
    {
        // 判断是否提取符号。
        if ( (bsigned==true) && ( (cc == '+') || (cc == '-') ))
        {
            str.append(1,cc); continue;
        }

        // 判断是否提取小数点。
        if ( (bdot==true) && (cc == '.') )
        {
            str.append(1,cc); continue;
        }

        // 提取数字。
        if (isdigit(cc)) str.append(1,cc);
    }

    dest=str;

    return dest;
}

string picknumber(const string &src,const bool bsigned,const bool bdot)
{
    string dest;
    picknumber(src,dest,bsigned,bdot);
    return dest;
}

///////////////////////////////////// /////////////////////////////////////
bool ccmdstr::getvalue(const int ii, string &value, const int ilen) const {
    if (ii >= m_cmdstr.size()) return false;

    // 从xml中截取数据项的内容。
    // 视频中是以下代码：
    // value=m_cmdstr[ii];
    // 改为：
    int itmplen = m_cmdstr[ii].length();
    if ((ilen > 0) && (ilen < itmplen)) itmplen = ilen;
    value = m_cmdstr[ii].substr(0, itmplen);

    return true;
}

bool ccmdstr::getvalue(const int ii, int &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoi(picknumber(m_cmdstr[ii], true));  // stoi有异常，需要处理异常。
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}


bool ccmdstr::getvalue(const int ii, unsigned int &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoi(picknumber(m_cmdstr[ii]));  // stoi有异常，需要处理异常。不提取符号 + -
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, long &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stol(picknumber(m_cmdstr[ii], true));  // stol有异常，需要处理异常。
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, unsigned long &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoul(picknumber(m_cmdstr[ii]));  // stoul有异常，需要处理异常。不提取符号 + -
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, double &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stod(picknumber(m_cmdstr[ii], true, true));  // stod有异常，需要处理异常。提取符号和小数点。
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, float &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stof(picknumber(m_cmdstr[ii], true, true));  // stof有异常，需要处理异常。提取符号和小数点。
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, bool &value) const {
    if (ii >= m_cmdstr.size()) return false;

    string str = m_cmdstr[ii];
    toupper(str);     // 转换为大写来判断。

    if (str == "TRUE") value = true;
    else value = false;

    return true;
}

ccmdstr::ccmdstr(const string &buffer, const string &sepstr, const bool bdelspace)
{
    splittocmd(buffer, sepstr, bdelspace);
}

void ccmdstr::splittocmd(const string &buffer, const string &sepstr, const bool bdelspace)
{
    int sepstrLen = sepstr.size();
    int pos = 0, ppos = 0, i=0;
    while(pos < buffer.size())
    {
        pos = buffer.find(sepstr, pos);
        if(pos > 0)
        {
            m_cmdstr.push_back(buffer.substr(i, pos-i));
            pos = pos + sepstrLen;
            i = pos;
        }
        else
        {
            m_cmdstr.push_back(buffer.substr(i));
            pos = pos + sepstrLen;
            i = pos;
            break;
        }
    }
}
ostream& operator<<(ostream& out, const ccmdstr& cmdstr)
{
    for (int ii=0;ii<cmdstr.size();ii++)
        out << "[" << ii << "]=" << cmdstr[ii] << endl;

    return out;
}

ccmdstr::~ccmdstr()
{
    m_cmdstr.clear();
}

///////////////////////////////////// /////////////////////////////////////
bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,string  &value,const int ilen)
{
    string start="<"+fieldname+">";            // 数据项开始的标签。
    string end="</"+fieldname+">";            // 数据项结束的标签。
    int startp=xmlbuffer.find(start);               // 在xml中查找数据项开始的标签的位置。
    if (startp==string::npos) return false;
    int endp=xmlbuffer.find(end);                 // 在xml中查找数据项结束的标签的位置。
    if (endp==string::npos) return false;
    // 从xml中截取数据项的内容。
    int itmplen=endp-startp-start.length();
    if ( (ilen>0) && (ilen<itmplen) ) itmplen=ilen;
    value=xmlbuffer.substr(startp+start.length(),itmplen);
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,char *value,const int len)
{
    if (value==nullptr) return false;
    if (len>0) memset(value,0,len+1);   // 调用者必须保证value的空间足够，否则这里会内存溢出。
    string str;
    getxmlbuffer(xmlbuffer,fieldname,str);

    if ( (str.length()<=(unsigned int)len) || (len==0) )
    {
        str.copy(value,str.length());
        value[str.length()]=0;    // string的copy函数不会给C风格字符串的结尾加0。
    }
    else
    {
        str.copy(value,len);
        value[len]=0;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,bool &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    toupper(str);    // 转换为大写来判断（也可以转换为小写，效果相同）。
    if (str=="TRUE") value=true;
    else value=false;
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,int &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stoi(picknumber(str,true));  // stoi有异常，需要处理异常。
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,unsigned int &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stoi(picknumber(str));  // stoi有异常，需要处理异常。不提取符号 + -
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,long &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stol(picknumber(str,true));  // stol有异常，需要处理异常。
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,unsigned long &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stoul(picknumber(str));  // stoul有异常，需要处理异常。不提取符号 + -
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,double &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stod(picknumber(str,true,true));  // stod有异常，需要处理异常。提取符号和小数点。
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,float &value)
{
    string str;
    if (!getxmlbuffer(xmlbuffer, fieldname, str)) return false;
    try
    {
        value = stof(picknumber(str,true,true));  // stof有异常，需要处理异常。提取符号和小数点。
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}





