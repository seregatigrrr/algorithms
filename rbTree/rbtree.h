#pragma once
#include <cstring>
#include <utility>
#include <fstream>
#include "tstring.h"
#include "queue.h"

using ULL = unsigned long;

enum  class TColor{
    red,
    black,
};

template<typename T1, typename T2>
struct TRBNode;

template<typename T1, typename T2>
struct TRB{
     TRBNode<T1,T2>* nil;
    TRBNode<T1,T2>* root;
    TRB(){
	nil = new TRBNode<T1,T2>();
        root = nil;
    };
    void Destroy(TRBNode<T1,T2>* node){
        if(node == nil){
            return;
        }
        Destroy(node->left);
        Destroy(node->right);
        delete node;
    }
    ~TRB(){
        Destroy(root);
        delete nil;
    }
    void LeftRotate(TRBNode<T1,T2>* x){
        TRBNode<T1,T2>* y = x->right;
        x->right = y->left;

        if(y->left != nil){
            y->left->p = x;
        }
        y->p = x->p;
        if(x->p == nil){
            root = y;
        }
        else if(x == x->p->left){
            x->p->left = y;
        }
        else{
            x->p->right = y;
        }
        y->left = x;

        x->p = y;
    }
    void RightRotate(TRBNode<T1,T2>* x){
        TRBNode<T1,T2>* y = x-> left;
        x->left = y->right;

        if(y->right != nil){
            y->right->p = x;
        }
        y->p = x->p;
        if(x->p == nil){
            root = y;
        }
        else if(x == x->p->right){
            x->p->right = y;
        }
        else{
            x->p->left = y;
        }
        y->right = x;

        x->p = y;
    }
    bool Insert(T1& key, T2& value){
        TRBNode<T1,T2>* z = new TRBNode<T1,T2>(key,value);
        TRBNode<T1,T2>* y = nil;
        TRBNode<T1,T2>* x = root;
        while(x!= nil){
            y = x;
            if(key < x->key){
                x = x->left;
            }
            else if (x->key < key){
                x = x->right;
            }
            else{
                delete z;
                return false;
            }
        }
        z->p = y;
        if(y == nil) {
            root = z;
        }
        else if(z->key < y->key){
            y->left = z;
        }
        else{
            y->right = z;
        }
        z->left = nil;
        z->right = nil;
        z->color = TColor::red;
        InsertFixup(z);
        return true;
    }

    void InsertFixup(TRBNode<T1,T2>* z){
        while(z->p->color == TColor::red){
            if(z->p == z->p->p->left){
                TRBNode<T1,T2>* y = z->p->p->right;
                if(y->color == TColor::red){
                    z->p->color = TColor ::black;
                    y->color = TColor::black;
                    z->p->p->color = TColor::red;
                    z = z->p->p;
                }
                else {
                    if( z == z->p->right){
                        z = z->p;
                        LeftRotate(z);
                    }
                    z->p->color = TColor::black;
                    z->p->p->color = TColor ::red;
                    RightRotate(z->p->p);
                }
            }
            else{
                TRBNode<T1,T2>* y = z->p->p->left;
                if(y->color == TColor::red){
                    z->p->color = TColor ::black;
                    y->color = TColor::black;
                    z->p->p->color = TColor::red;
                    z = z->p->p;
                }
                else {
                    if( z == z->p->left){
                        z = z->p;
                        RightRotate(z);
                    }
                    z->p->color = TColor::black;
                    z->p->p->color = TColor ::red;
                    LeftRotate(z->p->p);
                }
            }
        }
        root->color = TColor::black;
    }
    void Save(TString& to){
        TQueue<TRBNode<T1,T2>**> q = TQueue<TRBNode<T1,T2>**>(&root);
        TRBNode<T1,T2>* temp;
        std::fstream out(to.GetData(), std::ios::binary | std::ios::out);
	if(!out){
	std::cerr << "can't open file\n";
	        out.close();
	return;
	}
        char red = 'r';
        char black = 'b';
        char space = '_';
        while(!q.Empty()){
            temp = *q.Front();
            q.Pop();
            if(temp == nil){
                out.write(&space, 1);
                continue;
            }
            char color = temp->color == TColor ::red? red : black;
            out.write(&color, 1);
            size_t size = temp->key.Size();
            out.write((char*)&size, sizeof(size));
            out.write(temp->key.GetData(), size);
            out.write((char*)&temp->value, sizeof(temp->value));

            q.Insert(&temp->left);
            q.Insert(&temp->right);
        }
        out.close();
    }
    TRBNode<T1,T2>* ScanNode(std::fstream& from){
        char space = '_';
        char red = 'r';
        char black = 'b';

        char color;
        from.read(&color, 1);
        if(color == space){
            return nil;
        }
        else {
            TColor newColor = color == red ? TColor::red : TColor::black;
            size_t size;
            from.read((char *)&size, sizeof(size));
            TString key = TString(size + 1);
            from.read(key.GetData(), size);
            key[size] = '\0';
            ULL value;
            from.read((char *) &value, sizeof(value));
            return new TRBNode<T1,T2>(newColor,key,value);
        }
    }
    bool Load(TString& from){
        std::fstream in(from.GetData(), std::ios::binary | std::ios::in);
        if(!in){
            std::cerr << "can't open file\n";
            in.close();
            return false;
        }
        Destroy(root);
        root = ScanNode(in);
        if(root == nil){
            return true;
        }
	root->p = nil;
        TQueue<TRBNode<T1,T2>*> q;
        q.Insert(root);
        while(!q.Empty()){
            TRBNode<T1,T2>* p = q.Front();
            q.Pop();
            p-> left = ScanNode(in);
            if(p->left != nil){
                p->left->p = p;
                q.Insert(p->left);
            }
            p->right = ScanNode(in);
            if(p->right!= nil){
                p->right->p = p;
                q.Insert(p->right);
            }
        }
        in.close();
    }

    void Transplant(TRBNode<T1,T2>* u, TRBNode<T1,T2>* v) {
        if (u->p == nil)
            root = v;
        else if (u->p->left == u)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }

    void Delete(TRBNode<T1,T2>* z) {
        TRBNode<T1,T2>* y = z;
        TColor yOriginalColor = y->color;
	    TRBNode<T1,T2>* x;
        if (z->left == nil) {
            x = z->right;
            Transplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            Transplant(z, z->left);
        } else {
            y = TreeMinimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->p == z)
                x->p = y;
            else {
                Transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            Transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        if (yOriginalColor ==TColor::black) {
            DeleteFix(x);
        }
        delete z;
    }

    void DeleteFix(TRBNode<T1,T2>* x){
        while(x != root && x->color == TColor::black) {
            if (x == x->p->left) {
                TRBNode<T1, T2> *w = x->p->right;
                if (w->color == TColor::red) {
                    w->color = TColor::black;
                    x->p->color = TColor::red;
                    LeftRotate(x->p);
                    w = x->p->right;
                }
                if (w->left->color == TColor::black && w->right->color == TColor::black) {
                    w->color = TColor::red;
                    x = x->p;
                } else {
                    if (w->right->color == TColor::black) {
                        w->left->color = TColor::black;
                        w->color = TColor::red;
                        RightRotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = TColor::black;
                    w->right->color = TColor::black;
                    LeftRotate(x->p);
                    x = root;
                }
            } else if (x == x->p->right) {
                TRBNode<T1, T2> *w = x->p->left;
                if (w->color == TColor::red) {
                    w->color = TColor::black;
                    x->p->color = TColor::red;
                    RightRotate(x->p);
                    w = x->p->left;
                }
                if (w->right->color == TColor::black && w->left->color == TColor::black) {
                    w->color = TColor::red;
                    x = x->p;
                } else {
                    if (w->left->color == TColor::black) {
                        w->right->color = TColor::black;
                        w->color = TColor::red;
                        LeftRotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = TColor::black;
                    w->left->color = TColor::black;
                    RightRotate(x->p);
                    x = root;
                }
            }
        }
		x->color = TColor::black;
	}
    TRBNode<T1,T2>* TreeMinimum(TRBNode<T1,T2>* x){
        while(x->left != nil){
            x = x->left;
        }
        return x;
    }
    TRBNode<T1,T2>* Search(TRBNode<T1,T2>* x,const T1& key){
        while(x!= nil && x->key != key){
            if(x->key < key){
                x = x->right;
            }
            else{
                x = x->left;
            }
        }
        return x;
    }
};

template<typename T1, typename T2>
struct TRBNode{
    TRBNode() = default;

    explicit TRBNode(const T1& newKey,const T2& newValue): key(newKey), value(newValue) {
        color = TColor ::red;
    }
    explicit TRBNode(const TColor& newColor, const T1& newKey, const T2& newValue): key(newKey), value(newValue){
        color = newColor;
    }

    TColor color = TColor::black;
    T1 key;
    T2 value;
    TRBNode* p = nullptr;
    TRBNode* left ;
    TRBNode* right;
};
