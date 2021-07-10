#include<bits/stdc++.h>
using namespace std;
class node{
    public:
        int* data;
        node** child;
        bool is_leaf;
        int num;
        node(){
            data=new int[3];
            for(int i=0;i<3;i++) data[i]=INT_MAX;
            child=new node*[4];
            for(int i=0;i<4;i++) child[i]=NULL;
            is_leaf=true;
            num=0;
        }
};
struct twin{
    node* c1;
    node* c2;
    int val;
    twin(node* a,node* b,int v){
        c1=a;
        c2=b;
        val=v;
    }
};
node* root=new node();
int* merge(node* x,int data){
    int* m=new int[4];
    for(int i=0;i<3;i++) m[i]=x->data[i];
    m[3]=data;
    sort(m,m+4);
    return m;
}
twin* findleaf(node* x,int data){
    if(x->is_leaf==true){
        if(x->num<3){
            x->data[x->num]=data;
            x->num++;
            sort(x->data,x->data+3);
            twin* temp=new twin(x,NULL,INT_MAX);
            return temp;
        }
        else{
            int *merged=merge(x,data);
            node* s=new node();
            node* temp=x->child[3];
            x->child[3]=s;
            s->child[3]=temp;
            s->num=2;
            x->num=2;
            for(int i=2;i<3;i++) x->data[i]=INT_MAX;
            for(int i=0;i<=1;i++){
                x->data[i]=merged[i];
                s->data[i]=merged[i+2];
            }
            x->is_leaf=true;
            s->is_leaf=true;
            twin* temp1=new twin(x,s,s->data[0]);
            return temp1;
         }    
    }
    else{
        int pos=0;
        twin* temp;
        if(data<x->data[0]) {
            temp=findleaf(x->child[0],data);
            pos=0;
        }
        else if(data>=x->data[x->num-1]){ 
            temp=findleaf(x->child[x->num],data);
            pos=x->num;
        }
        else{
            int i;
            for(i=1;i<x->num-1;i++){
                // if(data==x->data[i]) break;
                if(data>=x->data[i-1] && data<x->data[i]) break;
            }
            temp=findleaf(x->child[i],data);
            pos=i;
        }
        if(temp->c2==NULL) {
            return new twin(x,NULL,INT_MAX);
        }
        else{
            if(x->num<3){
                for(int i=x->num;i>pos;i--){
                    x->data[i]=x->data[i-1];
                }
                x->data[pos]=temp->val;
                x->num++;
                node** c=new node*[x->num+1];
                for(int i=0;i<=pos;i++) c[i]=x->child[i];
                c[pos+1]=temp->c2;
                for(int i=pos+2;i<=x->num;i++) c[i]=x->child[i-1];
                for(int i=0;i<=x->num;i++) x->child[i]=c[i];
                return new twin(x,NULL,INT_MAX);
            }
            else{
                node* c2=new node();
                int* merged=merge(x,temp->val);
                node** c=new node*[x->num+2];
                for(int i=0;i<=pos;i++) c[i]=x->child[i];
                c[pos+1]=temp->c2;
                for(int i=pos+2;i<=x->num+1;i++) c[i]=x->child[i-1];
                for(int i=0;i<2;i++){
                    x->data[i]=merged[i];
                }
                c2->data[0]=merged[3]; 
                for(int i=0;i<=2;i++) x->child[i]=c[i];
                x->child[3]=NULL;
                c2->child[0]=c[3];
                c2->child[1]=c[4]; 
                x->data[2]=INT_MAX;
                x->num=2;
                c2->num=1;
                c2->is_leaf=false;
                return new twin(x,c2,merged[2]);
            }
        }
    }
    return NULL;
}
void insert(int data){
    twin* t=findleaf(root,data);
    if(t->c2!=NULL){
        if(t->c1==root){
            node* temp=new node();
            temp->num=1;
            temp->is_leaf=false;
            temp->data[0]=t->val;
            temp->child[0]=t->c1;
            temp->child[1]=t->c2;
            root=temp;
        }
    }
}
node* traverse(node* temp,int data){
    if(temp->is_leaf) return temp;
    else{
        if(data<temp->data[0]) traverse(temp->child[0],data);
        else if(data>temp->data[temp->num-1]) traverse(temp->child[temp->num],data);
        else{
        int i;
        for(i=0;i<temp->num-1;i++){
            if(data==temp->data[i]) break;
            else if(data>temp->data[i] && data<temp->data[i+1]) break;
        }
        traverse(temp->child[i],data);
        }
    }
}
void  find(int data){
    node* l=traverse(root,data);
    bool there=false;
    while(l!=NULL && there==false){
    for(int i=0;i<l->num;i++){
        if(l->data[i]==data) {there=true;break;}
    }
    l=l->child[3];
    }
    if(there==true)
    cout<<"YES"<<endl;
    else
    cout<<"NO"<<endl;
}
void count(int data){
    node* l=traverse(root,data);
    int count=0;
    int fin=0;
    while(l!=NULL && fin==0){
    for(int i=0;i<l->num;i++){
        if(l->data[i]==data) {count++;}
        if(count>0 && l->data[i]!=data) fin=1;
    }
    l=l->child[3];
    }
    cout<<count<<endl;
    
}
void range(int x,int y){
    node* l=traverse(root,x);
    while(l!=NULL){
    for(int i=0;i<l->num;i++){
        if(l->data[i]>=x && l->data[i]<=y) {cout<<l->data[i]<<" ";}
    }
    l=l->child[3];
    }
    cout<<endl;
}
int main(int argc, char* argv[]){
    // int n=1000000;
    // insert(1);
    ifstream word(argv[1]);
    string line;
    string a;
    int b,c;
    while(getline(word, line)) {
        istringstream iss(line);
        if(iss>>a >>b)
            (iss>>c);
        else {
            continue;
        }
        if(a=="INSERT") {
            insert(b);
        }
        else if(a=="FIND") {
            find(b);
        }
        else if(a == "COUNT") {
            count(b);
        }
        else if(a =="RANGE") {
            range(b,c);
        }
    }
    return 0;
}