#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>

class TString{
public:
    static bool IsEscape(const int& ch){
        return ch == ' ' || ch == '\n' || ch == EOF || ch == '\r';
    }
    ~TString(){
        delete[] buf;
    };
    TString(const size_t newCp = 1){
        buf = new char[newCp];
        sz = 0;
        cp = newCp;
        buf[0] = '\0';
    }
    TString(const char* other){
        sz = strlen(other);
        buf = new char[sz + 1];
        cp = sz + 1;
        strcpy(buf, other);
    }
    TString(const TString& other){
        sz = other.sz;
        buf = new char[other.cp];
        cp = other.cp;
        strcpy(buf, other.buf);
    }
    void Reserve(size_t newCp){
        if(newCp > cp){
            char temp[cp];
            strcpy(temp, buf);
            delete[] buf;
            buf = new char[newCp];
            strcpy(buf, temp);
            cp = newCp;
        }
    }
    char* GetData(){
        return buf;
    }
    size_t Size(){
        return sz;
    }
    TString& Append(const size_t& count, const char& ch){
        size_t newSize = sz + count;
        if(newSize >= cp) {
            size_t newCp = 1;
            while (newCp <= sz) {
                newCp *= 2;
            }
            Reserve(newCp);
        }
        memset(buf + sz, ch, count);
        sz = newSize;
        buf[sz] = '\0';
        return *this;
    }
    void PushBack(const char& ch){
        Append(1, ch);
    }
    char& operator[] (const size_t& n) {
        return buf[n];
    }
    const char& operator[] (const size_t& n) const{
        return buf[n];
    }
    bool operator ==(const TString& other) const{
        return  strcmp(buf, other.buf) == 0;
    }
    bool operator !=(const TString& other) const{
        return strcmp(buf, other.buf) != 0;
    }
    bool operator <(const TString& other) const{
        return strcmp(buf, other.buf) < 0;
    }
    bool operator <=(const TString& other) const{
        return strcmp(buf, other.buf) <= 0;
    }
    bool operator >(const TString& other) const{
        return strcmp(buf, other.buf) > 0;
    }
    bool operator >= (const TString& other) const{
        return strcmp(buf, other.buf) >= 0;
    }
    TString& operator= (const char* other){
        sz = strlen(other);
        if(cp < sz){
            Reserve(sz + 1);
        }
        strcpy(buf, other);
        return *this;
    }
    TString& operator= (const TString& other) = default;
    friend std::ostream& operator<<(std::ostream& out, const TString& str){
        out << str.buf;
        return out;
    }
    friend std::istream& operator>>(std::istream& in, TString& str){
        std::basic_istream<char>::sentry s(in);
        if(in) {
            str = "";
            in >> std::noskipws;
            char ch = 0;
            while (in >> ch) {
                if (IsEscape(ch)) {
                    break;
                }
                str.PushBack(ch);
            }
            in >> std::skipws;
        }
        return in;
    }
    void Swap(TString& other){
        std::swap(buf, other.buf);
        std::swap(sz, other.sz);
    }
    char* begin(){
        return buf;
    }
    char* end(){
        return buf + sz;
    }
private:
    char* buf;
    size_t sz;
    size_t cp;
};