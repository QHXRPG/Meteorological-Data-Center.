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
    if (!str.length() * strSubOld.length() * strSubNew.length()) //�������һ���ַ���Ϊ�գ���ôû������
    {
        cout << "����һ���ַ���Ϊ��" << endl;
        return str;
    }
    if (strSubNew.find(strSubOld)) //��ֹ��ѭ��
    {
        cout << "�Ƿ�����" << endl;
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
    // Ϊ��֧��src��dest��ͬһ���������������str��ʱ������
    string str;

    for (char cc:src)
    {
        // �ж��Ƿ���ȡ���š�
        if ( (bsigned==true) && ( (cc == '+') || (cc == '-') ))
        {
            str.append(1,cc); continue;
        }

        // �ж��Ƿ���ȡС���㡣
        if ( (bdot==true) && (cc == '.') )
        {
            str.append(1,cc); continue;
        }

        // ��ȡ���֡�
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

    // ��xml�н�ȡ����������ݡ�
    // ��Ƶ�������´��룺
    // value=m_cmdstr[ii];
    // ��Ϊ��
    int itmplen = m_cmdstr[ii].length();
    if ((ilen > 0) && (ilen < itmplen)) itmplen = ilen;
    value = m_cmdstr[ii].substr(0, itmplen);

    return true;
}

bool ccmdstr::getvalue(const int ii, int &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoi(picknumber(m_cmdstr[ii], true));  // stoi���쳣����Ҫ�����쳣��
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}


bool ccmdstr::getvalue(const int ii, unsigned int &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoi(picknumber(m_cmdstr[ii]));  // stoi���쳣����Ҫ�����쳣������ȡ���� + -
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, long &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stol(picknumber(m_cmdstr[ii], true));  // stol���쳣����Ҫ�����쳣��
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, unsigned long &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stoul(picknumber(m_cmdstr[ii]));  // stoul���쳣����Ҫ�����쳣������ȡ���� + -
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, double &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stod(picknumber(m_cmdstr[ii], true, true));  // stod���쳣����Ҫ�����쳣����ȡ���ź�С���㡣
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, float &value) const {
    if (ii >= m_cmdstr.size()) return false;

    try {
        value = stof(picknumber(m_cmdstr[ii], true, true));  // stof���쳣����Ҫ�����쳣����ȡ���ź�С���㡣
    }
    catch (const std::exception &e) {
        return false;
    }

    return true;
}

bool ccmdstr::getvalue(const int ii, bool &value) const {
    if (ii >= m_cmdstr.size()) return false;

    string str = m_cmdstr[ii];
    toupper(str);     // ת��Ϊ��д���жϡ�

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
    string start="<"+fieldname+">";            // �����ʼ�ı�ǩ��
    string end="</"+fieldname+">";            // ����������ı�ǩ��
    int startp=xmlbuffer.find(start);               // ��xml�в��������ʼ�ı�ǩ��λ�á�
    if (startp==string::npos) return false;
    int endp=xmlbuffer.find(end);                 // ��xml�в�������������ı�ǩ��λ�á�
    if (endp==string::npos) return false;
    // ��xml�н�ȡ����������ݡ�
    int itmplen=endp-startp-start.length();
    if ( (ilen>0) && (ilen<itmplen) ) itmplen=ilen;
    value=xmlbuffer.substr(startp+start.length(),itmplen);
    return true;
}

bool getxmlbuffer(const string &xmlbuffer,const string &fieldname,char *value,const int len)
{
    if (value==nullptr) return false;
    if (len>0) memset(value,0,len+1);   // �����߱��뱣֤value�Ŀռ��㹻������������ڴ������
    string str;
    getxmlbuffer(xmlbuffer,fieldname,str);

    if ( (str.length()<=(unsigned int)len) || (len==0) )
    {
        str.copy(value,str.length());
        value[str.length()]=0;    // string��copy���������C����ַ����Ľ�β��0��
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
    toupper(str);    // ת��Ϊ��д���жϣ�Ҳ����ת��ΪСд��Ч����ͬ����
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
        value = stoi(picknumber(str,true));  // stoi���쳣����Ҫ�����쳣��
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
        value = stoi(picknumber(str));  // stoi���쳣����Ҫ�����쳣������ȡ���� + -
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
        value = stol(picknumber(str,true));  // stol���쳣����Ҫ�����쳣��
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
        value = stoul(picknumber(str));  // stoul���쳣����Ҫ�����쳣������ȡ���� + -
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
        value = stod(picknumber(str,true,true));  // stod���쳣����Ҫ�����쳣����ȡ���ź�С���㡣
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
        value = stof(picknumber(str,true,true));  // stof���쳣����Ҫ�����쳣����ȡ���ź�С���㡣
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}





