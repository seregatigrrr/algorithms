#pragma once

#include <utility>
#include <exception>

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
        sz =  other.sz;
    }
    TVector(TVector&& other) noexcept {
        Swap(other);
    }
    ~TVector() {
        for (size_t i = 0; i < sz; ++i) {
            Destroy(data + i);
        }
    }

    TVector& operator =(TVector<T>&& other) noexcept{
        Swap(other);
        return *this;
    }
    TVector& operator= (const TVector<T>& other){ //красиво, но можно эффективней, если размер нынешнего вектора больше размера вектора справа. такой же принип у resize, потом напишу.
        TVector<T> temp(other);
        Swap(temp);
        return *this;
    }
    size_t Capacity() const {
        return data.cp;
    }
    size_t Size() const{
        return sz;
    }
    bool Empty() const{
        return sz == 0;
    }

    T& Front(){
        return *data.buf; //везде вместо *data.buf было бы правильней и красивей писать data + 0, но в последнее время я такой заботливый.. было очень сложно устоять.
        // это не точно, но возможно, ещё можно было бы определить преобразование data в T*, но это вроде мета-программирование, что-то страшное, хрен знает как это делать,
        // может позже.
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
    T* Data(){ //возможно работает не по стандарту для пустого вектора, ну а шо поделать. потом затещу.
        return &Front();
    }

    T& At(size_t pos){
        if(!(pos > 0 &&  pos < sz)){
            throw std::out_of_range("");//здесь можно написать например sstream ss; ss << pos <<" > " << sz, но страшна. что такое поток как не контейнер?А А А
        }
        return data[pos];
    }
    const T& At(size_t pos) const{
        if(!(pos > 0 &&  pos < sz)){
            throw std::out_of_range("");//здесь можно написать например sstream ss; ss << pos <<" > " << sz, но страшна. что такое поток как не контейнер? А А А
        }
        return data[pos];
    }
    const T& operator[](size_t i) const{
        return data[i];
    }
    T& operator[] (size_t i){
        return data[i];
    }
    T* Begin() { //WARNING, ЖЕРТВА CODESTYLE
        return data.buf;//WARNING, ЖЕРТВА CODESTYLE
    }//WARNING, ЖЕРТВА CODESTYLE
    T* End(){ //WARNING, ЖЕРТВА CODESTYLE
        return data + sz;//WARNING, ЖЕРТВА CODESTYLE
    }//WARNING, ЖЕРТВА CODESTYLE
    const T* Begin() const { //WARNING, ЖЕРТВА CODESTYLE
        return data.buf;//WARNING, ЖЕРТВА CODESTYLE
    }//WARNING, ЖЕРТВА CODESTYLE
    const T* End() const{ //WARNING, ЖЕРТВА CODESTYLE
        return data + sz; //WARNING, ЖЕРТВА CODESTYLE
    } //WARNING, ЖЕРТВА CODESTYLE
    void Reserve(size_t n){
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
        TVector newV(data.cp);
        Swap(std::move(newV));
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

    void Swap(TMemory<T>& other) noexcept{ //Для вызова std::swap нужен оператор присваивания, конструктор копирования или перемещения. но они нигде кроме этой строчки не пригодятся.
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }

    TMemory(TMemory&& other) noexcept{
        Swap(other);
    }
    TMemory& operator = (TMemory&& other) noexcept{
        Swap(other);
        return *this;
    }
};
