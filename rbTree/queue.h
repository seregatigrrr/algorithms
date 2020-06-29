#pragma once

template <typename T>
struct TQueue{
    struct TItem{
        TItem(const T& node) : data(node){}
        T data;
        TItem* next = nullptr;
    };
    TItem* first = nullptr;
    TItem* last = nullptr;
    size_t size = 0;
    TQueue() = default;
    explicit TQueue(const T& node){
        last = first = new TItem(node);
        size = 1;
    }
    T Front(){
        return first->data;
    }
    void Pop( ){
        if (first != nullptr){
            TItem* temp = first;;
            first = first->next;
            --size;
            delete temp;
        }
    }
    void Insert(const T& node) {
        if (Empty()) {
            first = last = new TItem(node);
        } else {
            last->next = new TItem(node);
            last = last->next;
        }
        ++size;
    }
    bool Empty(){
        return first == nullptr;
    }
};
