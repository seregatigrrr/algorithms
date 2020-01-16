#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>

struct TSMemory{
    using size_type = size_t;
    char* data = nullptr;
    size_type cp = 0;
    size_t Capacity(){
        return cp;
    }
    static char* Allocate(size_type n) {
        return new char[n + 1];
    }
    explicit TSMemory(size_type n = 0){
        data = Allocate(n);
        cp = n;
        data[cp] = '\0';
    }

    TSMemory(const TSMemory& other){
        data = Allocate(other.cp);
        strcpy(data, other.data);
        cp = other.cp;
        data[cp] = '\0';
    }

    TSMemory(const char* other, const size_t sz){
        data = Allocate(sz);
        memcpy(data, other, sz);
    }

    TSMemory(const char* other){
        size_t otherSz = strlen(other);
        data = Allocate(otherSz);
        strcpy(data, other);
    }
    TSMemory(TSMemory&& other) noexcept {
        Swap(other);
    }

    ~TSMemory(){
        Deallocate(data);
    }
    static void Deallocate(const char* buf){
        delete[] buf;
    }
    void Reallocate(size_t n){
        Deallocate(data);
        data = Allocate(n);
        cp = n;
        data[cp] = '\0';
    }
    void Resize(size_t n){
        TSMemory temp(n);
        strcpy(temp.data, data);
        Swap(temp);
    }

    char* operator+ (size_type i){
        return data + i;
    }
    const char* operator+ (size_type i) const{
        return data + i;
    }
    char& operator[] (size_type i){
        return data[i];
    }

    const char& operator[] (size_type i) const{
        return data[i];
    }

    void Swap(TSMemory& other) noexcept{
        std::swap(data, other.data);
        std::swap(cp, other.cp);
    }


    TSMemory& operator= (TSMemory&& other) noexcept{
        Swap(other);
        return *this;
    }
    TSMemory& operator= (const TSMemory& other){
        if(cp < other.cp){
            Reallocate(other.cp);
        }
        strcpy(data, other.data);
        return *this;
    }
};

class TString{
public:
    static bool IsEscape(const int& ch){
        return ch == ' ' || ch == '\n' || ch == EOF || ch == '\r';
    }
    ~TString() = default;
    explicit TString(int newCp = 0): buf(newCp) {
        buf[0] = '\0';
        sz = 0;
    }
    TString(const char* other){
        sz = strlen(other);
        buf = TSMemory(other,sz);
    }
    TString(const TString& other) : buf(other.buf){
        sz = other.sz;
    }
    TString(TString&& other) noexcept{
        Swap(other);
        sz = other.sz;
    }
    char* GetData(){
        return buf.data;
    }
    TString& Append(size_t count, const char& ch){
        size_t newSize = sz + count;
        if(newSize > buf.Capacity()) {
            size_t newCp = 1;
            while (newCp < sz) {
                newCp *= 2;
            }
            buf.Resize(newCp);
        }
        memset(buf.data + sz, ch, count);
        sz = newSize;
        buf[sz] = '\0';
        return *this;
    }
    void PushBack(const char ch){
        Append(1, ch);
    }
    char& operator[] (size_t n) {
        return buf[n];
    }
    const char& operator[] (size_t n) const{
        return buf[n];
    }
    bool operator ==(const TString& other) const{
        return  strcmp(buf.data, other.buf.data) == 0;
    }
    bool operator !=(const TString& other) const{
        return strcmp(buf.data, other.buf.data) != 0;
    }
    bool operator <(const TString& other) const{
        return strcmp(buf.data, other.buf.data) < 0;
    }
    bool operator <=(const TString& other) const{
        return strcmp(buf.data, other.buf.data) <= 0;
    }
    bool operator >(const TString& other) const{
        return strcmp(buf.data, other.buf.data) > 0;
    }
    bool operator >= (const TString& other) const{
        return strcmp(buf.data, other.buf.data) >= 0;
    }
    TString& operator= (const char* other){
        sz = strlen(other);
        buf = other;
        return *this;
    }
    TString& operator= (const TString& other) = default;
    friend std::ostream& operator<<(std::ostream& out, const TString& str){
        out << str.buf.data;
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
        return buf.data;
    }
    char* end(){
        return buf.data + sz;
    }
private:
    TSMemory buf;
    size_t sz;
};
