#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <queue>
#include <memory>

using TAlpha = unsigned long long int;
struct TVertex{
    TVertex() = default;
    TAlpha alpha = 0;
    int level = 0;
    int numeric = 0;
    bool terminate = false;
    std::weak_ptr<TVertex> parent;
    std::weak_ptr<TVertex>  suffLink;
    std::weak_ptr<TVertex>  up;
    std::map<TAlpha, std::shared_ptr<TVertex>> to;
};

class TTrie{
public:
    TTrie() = default;
    std::shared_ptr<TVertex> root = std::make_shared<TVertex>();
    std::stringstream ss;
    void MakeGo(std::shared_ptr<TVertex>& v, TAlpha& alpha){
        v->to[alpha] =  std::make_shared<TVertex>();
        v->to[alpha]->parent = v;
        v->to[alpha]->level = v->level + 1;
        v->to[alpha]->alpha = alpha;
    }
    void Insert(std::string& s){
        static int count = 0;
        ss.clear();
        ss << s;
        std::shared_ptr<TVertex> curr = root;
        TAlpha alpha;
        while( ss >> alpha){
            if(curr->to.count(alpha) == 0){
                MakeGo(curr, alpha);
            }
            curr = curr->to[alpha];
        }
        ++count;
        curr->numeric = count;
        curr->terminate = true;
    }
    void GetSuffLink(std::shared_ptr<TVertex>& v){
        if(v == root || v->parent.lock() == root) {
            v->suffLink = root;
        }
        else {
            std::shared_ptr<TVertex> p(v->parent.lock()->suffLink);
            while (p != root && p->to.count(v->alpha) == 0) {
                p = p->suffLink.lock();
            }
            if (p->to.count(v->alpha) == 0) {
                v->suffLink = root;
            } else {
                v->suffLink = p->to[v->alpha];
            }
        }
    }
    void GetUp(std::shared_ptr<TVertex>& v) {
        if (v == root || v->parent.lock() == root) {
            v->up.lock() = nullptr;
        } else if (v->suffLink.lock()->terminate) {
            v->up = v->suffLink;
        } else {
            v->up = v->suffLink.lock()->up;
        }
    }
    std::shared_ptr<TVertex>& go(std::shared_ptr<TVertex>& state, TAlpha alpha){
        if(state->to.count(alpha) > 0){
            return state->to[alpha];
        }
        else {
            for (; state->to.count(alpha) == 0 && state != root; state = state->suffLink.lock())
                ;
            if(state->to.count(alpha) > 0){
                return state->to[alpha];
            }
            else{
                return root;
            }
        }
    }
};


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TTrie trie;
    std::string s;
    std::getline(std::cin,s);
    while(!s.empty()){
        trie.Insert(s);
        std::getline(std::cin, s);
    }

    std::queue<std::shared_ptr<TVertex>> q;
    q.push(trie.root);

    while(!q.empty()){
        std::shared_ptr<TVertex> temp =  q.front();
        q.pop();
        trie.GetSuffLink(temp);
        trie.GetUp(temp);

        for(std::pair<const TAlpha, std::shared_ptr<TVertex>>& elem: temp->to){
            q.push(elem.second);
        }
    }

    std::shared_ptr<TVertex> state = trie.root;
    int pos,currLine = 0;
    TAlpha alpha;
    std::vector<int> nSymbols;
    std::stringstream ss;
    while(std::getline(std::cin,s)){
        ++currLine;
        ss.clear();
        ss << s;
        pos = 0;
        while(ss >> alpha){
            ++pos;
            state = trie.go(state, alpha);
            if(state->terminate) {
                int line = currLine;
                int nSym = state->level - pos;
                while (nSym > 0) {
                    --line;
                    nSym -= nSymbols[line - 1];
                }
                std::cout << line << ", " << abs(nSym) + 1 << ", " << state->numeric << '\n';
            }
            if(state->up.lock()){
                std::shared_ptr<TVertex> up = state->up.lock();
                while(up){
                    int line = currLine;
                    int nSym = up->level - pos;
                    while (nSym > 0) {
                        --line;
                        nSym -= nSymbols[line - 1];
                    }
                    std::cout << line << ", " << abs(nSym) + 1 << ", " << up->numeric << '\n';
                    up = up->up.lock();
                }
            }

        }
        nSymbols.push_back(pos);
    }
    return 0;
}

