#include <vector>

template<typename T>
class THeapArray{
public:
    THeapArray() = default;
    explicit THeapArray(const std::vector<T>& v){
        data = v;
        for(int i = v.size() - 1; i >= 0; --i){
            BubbleDown(i);
        }
    }
    size_t Size() const{
        return data.size();
    }
    T GetMin() const{
        return data[0];
    }
    void Insert(const T& newValue){
        data.push_back(newValue);
        BubbleUp(Size() - 1);
    }
    void RemoveMin(){
        data[0] = data.back();
        data.pop_back();
        BubbleDown(0);
    }
private:
    std::vector<T> data;
    size_t Parent(size_t i) const{
        return (i - 1) / 2;
    }
    size_t Left(size_t i) const{
        return 2*i + 1;
    }
    size_t Right(size_t i) const{
        return 2*i + 2;
    }
    void BubbleDown(size_t i){
        size_t min = i;
        if(Left(i) < data.size() && data[Left(i)] < data[min]){
            min = Left(i);
        }
        if(Right(i) < data.size() && data[Right(i)] < data[min]){
            min = Right(i);
        }
        if(min != i){
            std::swap(data[i], data[min]);
            BubbleDown(min);
        }
    }
    void BubbleUp(size_t i){
        if(i > 0 && data[Parent(i)] > data[i]){
            std::swap(data[Parent(i)], data[i]);
            BubbleUp(Parent(i));
        }
    }
};
