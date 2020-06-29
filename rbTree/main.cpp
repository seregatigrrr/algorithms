#include <iostream>
#include "rbtree.h"
#include <cstring>
#include "tstring.h"
enum  class TCmd{
    add,
    check,
    del,
    load,
    save,
};

void ToLower(TString& str);
bool ReadLine(TCmd& cmd, TString& key, ULL& value, char* word);
int main() {
    std::ios::sync_with_stdio(0);
    TRB<TString, ULL> trb;
    TCmd cmd;
    TString key(257);
    ULL value;
    char word[257];
    while(ReadLine(cmd, key, value, word)){
        switch (cmd) {
            case TCmd::add: {
                if (trb.Insert(key,value)) {
                    std::cout << "OK\n";
                } else {
                    std::cout << "Exist\n";
                }
                break;
            }
            case TCmd::check: {
                TRBNode<TString,ULL> *node = trb.Search(trb.root, key);
                if (node != trb.nil) {
                    std::cout << "OK: " << node->value << '\n';
                } else {
                    std::cout << "NoSuchWord\n";
                }
                break;
            }
            case TCmd::del: {
                TRBNode<TString,ULL> *temp = trb.Search(trb.root, key);
                if (temp != trb.nil) {
                    trb.Delete(temp);
                    std::cout << "OK\n";
                } else {
                    std::cout << "NoSuchWord\n";
                }
                break;
            }
            case TCmd::save: {
                trb.Save(key);
                std::cout << "OK\n";
                break;
            }
            case TCmd::load:
                if(trb.Load(key)) { ;
                    std::cout << "OK\n";
                }
                break;
        }
    }

    return 0;
}
void ToLower(TString& str){
    for(size_t i = 0; i < str.Size(); ++i){
        str[i] = tolower(str[i]);
    }
}
bool ReadLine(TCmd& cmd, TString& key, ULL& value, char* word){
    if(std::cin >> word) {
        if (word[0] == '+'){
            cmd = TCmd::add;
            	
            std::cin >> word;
		    key = word;
            ToLower(key);
            std::cin >> value;
        }
        else if(word[0] == '-'){
            cmd = TCmd ::del;
            std::cin >> word;
		key = word;
            ToLower(key);
        }
        else if(word[0] == '!'){
            char temp[5];
            std::cin >> temp;
            if(temp[0] == 'S'){
                cmd = TCmd::save;
            }
            else if(temp[0] == 'L'){
                cmd = TCmd::load;
            }
            std::cin >> word;
		key = word;
		
        }
        else{
            cmd = TCmd::check;
            key = word;
            ToLower(key);
        }
        return true;
    }
    else{
        return false;
    }
}
