#include<bits/stdc++.h>
using namespace std;
//can be changed if required
int  blocksize=2*sizeof(int);
struct block {
    vector<int> keys;
    block* overflow;
    block() {overflow = NULL;}
    bool check(int data) {
        block* node = this;
        while(node!=NULL) {
            for(int i = 0; i < node->keys.size(); i++) if(node->keys[i] == data) return true;
            //for checking all the overflow blocks
            node = node->overflow;
        }
        return false;
    }
    //inserts element into a block
    void insert(int data) {
        if(keys.size() < blocksize/sizeof(int)) keys.push_back(data);
        else {
            //if place not there in block then overflow is created
            if(overflow == NULL) overflow = new block();
            overflow->insert(data);
        }
    }
    //seletes the block  and stores all the elements in a vector
    void deleteblocks(vector<int> &v) {
        for(int i = 0; i < keys.size(); i++) v.push_back(keys[i]);
        keys.clear();
        //if overflow blocks are present retrieve them
        if(overflow) {
            overflow->deleteblocks(v);
            delete overflow;
            overflow = NULL;
        }
    }
};

class lhash {
    public:
    int numr; 
    int bits;
    vector<block*> buckets;
    lhash() {
        numr = 0;
        bits = 1;
        buckets.push_back(new block());
        buckets.push_back(new block());
    }
};
double fillpercentage(lhash h) {
    //returns how much percentage of hash table is filled
    return h.numr*sizeof(int)/ (h.buckets.size()*blocksize);
}
int hash(lhash h,int key) {
        int m = (1 << h.bits);
        //inserting mod for negative values for positive values it doesnt matter
        return (key % m + m) % m;
}
bool check(lhash h,int data) {
    int k = hash(h,data);
    if(k >= h.buckets.size())  k -= (1 << (h.bits - 1));
    if(h.buckets[k]->check(data)) return true;
    return false;
}
void insert(lhash h,int data) {
    int k = hash(h,data);
    if(k >= h.buckets.size()) k -= (1 << (h.bits - 1));
    h.buckets[k]->insert(data);
    h.numr++;
    while(fillpercentage(h) > 0.85) {
        vector<int> v;
        h.buckets.push_back(new block());
        double temp=log2((double)h.buckets.size());
        h.bits = ceil(temp);
        temp= 1 << (h.bits - 1);
        int i = h.buckets.size() - 1 -temp;
        h.buckets[i]->deleteblocks(v);
        for(i = 0; i < v.size(); i++) {
            h.buckets[hash(h,v[i])]->insert(v[i]);
        }
    }
}
int main(int argc, char *argv[]) {
    lhash h;
    ifstream word(argv[1]);
    string line;
    int a;
    while(getline(word, line)) {
        istringstream iss(line);        
        if(iss>>a)
        if(!check(h,a)){
            cout<<a<<endl;
        insert(h,a);
        }
    }
    // cout<<h.buckets.size()<<" "<<h.numr<<" "<<h.fillpercentage()<<endl;
    return 0;
}