#ifndef PATTERNS
#define PATTERNS
#include<iostream>
#include<cstdlib>
#include<map>
#include<string>
#include<cstdio>
#include<vector>
#include<queue>
#include<time.h>
#include<cmath>
#include<stack>
#include<typeinfo>
#define NUM_CARD 15
#define NUM_NUM 4
#define PAIR_LENGTH 2
#define TRI_LENGTH 3
#define QUA_LENGTH 4
#define TWO 12
#define joker 13
#define JOKER 14
using namespace std;

static double alpha = 100.0;
static double beta = 0.1;

double Power(int p){
    return alpha * exp(-beta * p);
}

/** the pattern class that has the basic characteristic of a pattern**/
/** variables: String name: the name of the pattern
               int len: the length of the pattern                   **/
class pattern{
private:
    string name;
    int len;
public:
    pattern(string nam, int l){
        this->name = nam;
        this->len = l;
    }

    string getName() const{
        return name;
    }

    int getSize() const{
        return len;
    }

    void setSize(int length){
        len = length;
    }

    virtual vector<int> toVec() = 0;
};


/** the class representing single pattern

    variable: int num: the number value of the single card
    constructor: (int number): the number value             **/
class Single: public pattern{
    int num;
public:
    Single(int number):pattern("S", 1){
        this->num = number;
    }

    int getNum() const{
        return num;
    }

    bool operator >(const Single& s){
        return num > s.getNum();
    }

    static bool ifExist(int card[], int index){
        return card[index] > 0;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = num + 1; i != NUM_CARD; i++)
            if(counter[i] > 0)
                p++;
        return Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        vec.push_back(num);
        return vec;
    }
};


/** the class representing pair pattern

    variable: int num: the number value of the pair
    constructor: (int number): the number value **/
class Pair: public pattern{
    int num;
public:
    Pair(int number):pattern("P", PAIR_LENGTH){
        this->num = number;
    }

    int getNum() const{
        return num;
    }

    bool operator >(const Pair& p){
        return num > p.getNum();
    }

    static bool ifExist(int card[], int index){
        return card[index] > 1;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = num + 1; i != NUM_CARD; i++)
            if(counter[i] >= PAIR_LENGTH)
                p++;
        return getSize() * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        vec.push_back(num);
        vec.push_back(num);
        return vec;
    }

};


/** the class representing triple pattern

    variable: int tri: the number value of the triple part
              int appd: the number value of the appendix part
    constructor: (int triple, int appendix, int length) **/
class Triple:public pattern{
    int tri, appd;
public:
    Triple(int triple, int appendix, int length):pattern("T", length){
        this->tri = triple;
        this-> appd = appendix;
    }

    int getTriple() const{
        return tri;
    }

    int getAppendix() const{
        return appd;
    }

    bool operator >(Triple& t){
        if(this->getSize() != t.getSize())
            return false;
        else
            return this->tri > t.getTriple();
    }

    static bool ifExist(int card[], int tri_index, int appd_index, int appd_length){
        return (card[tri_index] >= TRI_LENGTH) && (card[appd_index] >= appd_length);
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = tri + 1; i != NUM_CARD; i++)
            if(counter[i] >= TRI_LENGTH)
                p++;
        return getSize() * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        for (int i = 0; i != TRI_LENGTH; i++)
            vec.push_back(tri);
        for (int i = 0; i != getSize() - TRI_LENGTH; i++)
            vec.push_back(appd);
        return vec;
    }
};


/** the class representing straight pattern

    variable: int st: the number value of the start number of a straight
    constructor: (int start, int length): the number value and the length **/
class Straight:public pattern{
    int st;
public:
    Straight(int start, int length):pattern("ST", length){
        this->st = start;
    }

    int getStart() const{
        return st;
    }

    bool operator >(const Straight& st){
        if(this->getSize() != st.getSize())
            return false;
        else
            return this->st > st.getStart();
    }

    static bool ifExist(int card[], int start, int length){
        for (int i = start; i != start + length; i++)
            if (card[i] == 0)
                return false;
        return true;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = st + 1; i <= TWO - this->getSize(); i++){
            bool sw = true;
            for (int j = 0; j != this->getSize(); j++){
                if(counter[i+j] == 0){
                    sw = false;
                    break;
                }
            }
            if (sw)
                p++;
        }
        return getSize() * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        for (int i = st; i != st + getSize(); i++)
            vec.push_back(i);
        return vec;
    }
};


/** the class representing straight pairs pattern

    variable: int start: the number value of the start card
    constructor: (int start, int length): the number value and the length **/
class StraightPair:public pattern{
    int st;
public:
    StraightPair(int start, int length):pattern("SP", length){
        this->st = start;
    }

    int getStart() const{
        return st;
    }

    bool operator >(const StraightPair& sp){
        if(this->getSize() != sp.getSize())
            return false;
        else
            return this->st > sp.getStart();
    }

    static bool ifExist(int card[], int start, int length){
        for (int i = start; i != start + length; i++)
            if (card[i] < PAIR_LENGTH)
                return false;
        return true;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = st; i <= TWO - this->getSize(); i++){
            bool sw = true;
            for (int j = 0; j != this->getSize(); j++){
                if(counter[i+j] < PAIR_LENGTH){
                    sw = false;
                    break;
                }
            }
            if (sw)
                p++;
        }
        return getSize() * 2 * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        for (int i = st; i != st + getSize(); i++){
            vec.push_back(i);
            vec.push_back(i);
        }
        return vec;
    }
};


/** the class representing plane pattern. **/
class Plane:public pattern{
    int st, tri_num;
    vector<Triple> tri_set;
public:
    Plane(int startTriple, int triples_num, vector<Triple> TripleSet, int length): pattern("PL", length){
        this->st = startTriple;
        this->tri_num = triples_num;
        this->tri_set = TripleSet;
    }

    int getStartTriple() const{
        return st;
    }

    vector<Triple> getTripleSet() const{
        return tri_set;
    }

    bool operator >(const Plane& plane){
        if (this->getSize() != plane.getSize())
            return false;
        else{
            return this->st > plane.getStartTriple();
        }
    }

    static bool ifExist(int card[], int st, int length){
        for (int i = st; i != st + length; i++)
            if (card[i] < TRI_LENGTH)
                return false;
        return true;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = st + 1; i < TWO - tri_num; i++){
            if(counter[i] >= TRI_LENGTH){
                for (int j = 1; j != tri_num; j++){
                    if (counter[i + j] < TRI_LENGTH)
                        break;
                    if (j == tri_num - 1)
                        p++;
                }
            }
        }
        return getSize() * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        for (int i = 0; i != tri_set.size(); i++){
            vector<int> temp = tri_set[i].toVec();
            for (int j = 0; j != temp.size(); j++)
                vec.push_back(temp[j]);
        }
        return vec;
    }

};


/** the class representing bomb pattern

    variable: int num: the number value of the bomb
    constructor: (int number, int length): the number value and length **/
class Bomb:public pattern{
    int num;
public:
    Bomb(int number, int length):pattern("B", length){
        num = number;
    }

    int getNum() const{
        return num;
    }

    bool operator >(const Bomb& b){
        return num > b.getNum();
    }

    static bool ifExist(int card[], int index){
        if(index == JOKER || index == joker)
            return card[joker] && card[JOKER];
        else
            return card[index] == QUA_LENGTH;
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = num + 1 ; i != NUM_CARD - 1; i++){
            if(i == joker && counter[joker] && counter[JOKER])
                p++;
            else if (counter[i] == QUA_LENGTH)
                p++;
        }
        return getSize() * 2 * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        if (num == joker){
            vec.push_back(joker);
            vec.push_back(JOKER);
        }
        else{
            for (int i = 0; i != QUA_LENGTH; i++)
                vec.push_back(num);
        }
        return vec;
    }
};


/** the class representing quadruple pattern

    variable: int quad: the number value of the quadruple part
              int appd1: the number value of the appendix part 1
              int appd2: the number value of the appendix part 2 **/
class Quadruple:public pattern{
    int quad, appd1, appd2;
public:
    Quadruple(int quadruple, int appd1, int appd2, int length):pattern("Q", length){
        this->quad = quadruple;
        this->appd1 = appd1;
        this->appd2 = appd2;
    }

    int getQuad() const{
        return quad;
    }

    bool operator >(const Quadruple& qua){
        if (this->getSize() != qua.getSize())
            return false;
        else
            return quad > qua.getQuad();
    }

    static bool ifExist(int card[], int qua_index, int appd1_index, int appd2_index,
                        int appd_length){
        return (card[qua_index] == QUA_LENGTH) && (card[appd1_index] >= appd_length) &&
                (card[appd2_index] >= appd_length);
    }

    double getPow(const int counter[]){
        int p = 0;
        for (int i = quad + 1 ; i != NUM_CARD; i++){
            if (counter[i] == QUA_LENGTH)
                p++;
        }
        return getSize() * 2 * Power(p);
    }

    vector<int> toVec(){
        vector<int> vec;
        for (int i = 0; i != QUA_LENGTH; i++)
            vec.push_back(quad);
        for (int i = 0; i != (getSize() - QUA_LENGTH) / 2; i++)
            vec.push_back(appd1);
        for (int i = 0; i != (getSize() - QUA_LENGTH) / 2; i++)
            vec.push_back(appd2);
        return vec;
    }
};

#endif // PATTERNS
