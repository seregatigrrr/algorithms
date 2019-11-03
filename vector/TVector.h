#pragma once

#include <utility>
#include <exception>
#include <stdexcept>

template <typename T>
struct TMemory;

template<typename T>
class TVector{
public:
    TVector() = default;
    explicit TVector(size_t n) : data(n){
            for(size_t i = 0; i < n; ++i){
                Construct(data + i);
            }
            sz = n;
    }
    TVector(const TVector<T>& other): data(other.sz){
        for(size_t i = 0; i < other.sz; ++i){
            Construct(data + i, other[i]);
        }
        sz = other.sz;
    }
    TVector(TVector&& other) noexcept {
        Swap(other);
    }
    ~TVector() {
        for (size_t i = 0; i < sz; ++i) {
            Destroy(data + i);
        }
    }
    TVector(std::initializer_list<T> init): data(init.size()){
        for(size_t i = 0; i < init.size(); ++i){
            Construct(data + i, *(init.begin() + i));
        }
        sz = init.size();
    }

    TVector& operator =(TVector<T>&& other) noexcept{
        Swap(other);
        return *this;
    }
    TVector& operator= (const TVector<T>& other){
        if(other.sz > data.cp) {
            TVector<T> temp(other);
            Swap(temp);
        }
        else{
            for(size_t i = 0; i < std::min(sz, other.sz); ++i){
                 data[i] = other[i];
            }
            if(sz > other.sz){
                for(size_t i = sz; i!= other.sz; --i){
                    Destroy(data + i - 1);
                }
            }
            else if(sz < other.sz){
                for(size_t i = sz; i < other.sz; ++i){
                    Construct(data + i, other[i]);
                }
            }
            sz = other.sz;
        }
        return *this;
    }
    size_t Capacity() const {
        return data.cp;
    }
    size_t Size() const{
        return sz;
    }
    bool Empty() const{
        return ( sz == 0 );
    }

    T& Front(){
        return *data.buf;
    }
    const T& Front() const{
        return *data.buf;
    }
    T& Back(){
        return data[sz - 1];
    }
    const T& Back() const{
        return data[sz - 1];
    }
    T* Data(){
        return &Front();
    }
    const T* Data() const{
        return &Front();
    }
    T& At(const size_t pos){
        if(!(pos > 0 &&  pos < sz)){
            throw std::out_of_range("index is out of range");//здесь можно написать например sstream ss; ss << pos <<" > " << sz, но страшна. что такое поток как не контейнер?А А А
        }
        return data[pos];
    }
    const T& At(size_t pos) const{
        if(!(pos > 0 &&  pos < sz)){
            throw std::out_of_range("index is out of range");//здесь можно написать например sstream ss; ss << pos <<" > " << sz, но страшна. что такое поток как не контейнер? А А А
        }
        return data[pos];
    }
    const T& operator[](size_t i) const{
        return data[i];
    }
    T& operator[] (const size_t i){
        return data[i];
    }
    bool operator== (const TVector& other) const{
        if(other.sz != sz){
            return false;
        }
        for (size_t i = 0; i < sz; ++i){
            if(data[i] != other[i]){
                return false;
            }
        }
        return true;
    }
    T* Begin() { //WARNING, ЖЕРТВА CODESTYLE
        return data.buf;
    }
    T* End(){ //WARNING, ЖЕРТВА CODESTYLE
        return data + sz;
    }
    const T* Begin() const { //WARNING, ЖЕРТВА CODESTYLE
        return data.buf;
    }
    const T* End() const{ //WARNING, ЖЕРТВА CODESTYLE
        return data + sz;
    }
    void Reserve(const size_t n){
        if(n > data.cp ){
            TMemory<T> data2(n);
            for(size_t i = 0; i < sz; ++i){
                Construct(data2 + i, std::move(data[i]));
                Destroy(data + i);
            }
            data.Swap(data2);
        }
    }

    void Clear(){
        for (size_t i = 0; i < sz; ++i) {
            Destroy(data + i);
        }
        sz = 0;
    }
    void PushBack(const T& elem){
        if(sz == data.cp){
            Reserve(sz == 0? 1 : sz * 2);
        }
        Construct(data + sz, elem);
        ++sz;
    }
    void PushBack(T&& elem){
        if(sz == data.cp){
            Reserve(sz == 0? 1 : sz * 2);
        }
        Construct(data + sz, std::move(elem));
        ++sz;
    }
    void PopBack(){
        if(sz > 0) {
            Destroy(data + sz - 1);
        }
        --sz;
    }
    void Resize(size_t n){
        Reserve(n);
        if(sz < n){
            for(size_t i = sz; i < n; ++i){
                Construct(data + sz);
            }
        }
        else{
            for(size_t i = sz; i!= n; --i){
                Destroy(data + i - 1);
            }
        }
        sz = n;
    }
    template <typename ... Args>
    T& EmplaceBack( Args&& ... args ){
        if(sz == data.cp){
            Reserve(sz == 0 ? 1 : sz * 2);
        }
        auto elem = new (data + sz) T(std::forward<Args>(args)...);
        ++sz;
        return *elem;
    }
    void Swap(TVector& other) noexcept{
        data.Swap(other.data);
        std::swap(sz, other.sz);
    }
private:
    TMemory<T> data;
    size_t sz = 0;

    static void Construct(void* buf){
        new (buf) T();
    }
    static void Construct(void* buf, const T& elem) {
        new (buf) T(elem);
    }
    static void  Construct(void* buf, T&& elem){
        new (buf) T(std::move(elem));
    }
    static void Destroy(T* buf){
        buf->~T();
    }
};
template <typename T>
struct TMemory{
    T* buf = nullptr;
    size_t cp = 0;

    TMemory() = default;
    static T* Allocate(size_t n) {
        return static_cast<T*>(operator new(n * sizeof(T)));
    }
    static void Deallocate(T* buf){
        operator delete(buf);
    }

    explicit TMemory(size_t n){
        buf = Allocate(n);
        cp = n;
    }

    ~TMemory(){
        Deallocate(buf);
   }

    T* operator+ (size_t i){
        return buf + i;
    }
    const T* operator+ (size_t i) const{
        return buf + i;
    }
    T& operator[] (size_t i){
        return buf[i];
    }

    const T& operator[] (size_t i) const{
        return buf[i];
    }

    void Swap(TMemory<T>& other) noexcept{
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }

    TMemory(TMemory&& other) noexcept{
        Swap(other);
    }
    TMemory& operator= (TMemory&& other) noexcept{
        Swap(other);
        return *this;
    }
};