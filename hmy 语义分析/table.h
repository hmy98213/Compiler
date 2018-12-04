// standard lib
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
// define
/*#define INT 0
#define FLOAT 1
#define STRUCT 3
*/
// charactor table
struct var_node{
    int type;
    char* name;
    int dim;
    struct struct_node* struct_type;
    struct var_node* nxt;
};

struct func_node{
    int return_type;
    struct struct_node* return_struct_type;
    char* func_name;
    struct var_node* begin_param;
    int param_num;
    struct func_node* nxt;
};

struct struct_node{
    char* name;
    struct var_node* var_list;
    struct struct_node* nxt;
};

// variable
struct var_node* Tvar;
struct var_node* var;

struct struct_node* Tstruct;
struct struct_node* stru;

struct func_node* Tfunc;
struct func_node* func;

//function
void init(){
    Tvar=(struct var_node* )malloc(sizeof(struct var_node));
    Tvar->name=NULL;
    var=Tvar;

    Tstruct=(struct struct_node* )malloc(sizeof(struct struct_node));
    stru=Tstruct;

    Tfunc=(struct func_node* )malloc(sizeof(struct func_node));
    func=Tfunc;
}

int get_type(char* t){
    if(strcmp(t,"INT")) return 0;
    if(strcmp(t,"FLOAT")) return 1;
    if(strcmp(t,"STRUCT")) return 3;
    return -1;
}

// display charactor table
void display_Tvar(){
    struct var_node* p=Tvar->nxt;
    while(p!=NULL){
        printf("%s %d %d\n",p->name,p->type,p->dim);
        p=p->nxt;
    }
}
void display_Tstruct(){
    struct struct_node* st=Tstruct->nxt;

    while(st!=NULL){
        printf("struct: %s \n",st->name);
        struct var_node* p=st->var_list->nxt;
        while(p){
            printf("    %s %d %d\n",p->name,p->type,p->dim);
            p=p->nxt;
        }
        st=st->nxt;
    }
}

void display_Tfunc(){
    struct func_node* p=Tfunc->nxt;
    while(p){
        printf("%s  \n",p->func_name);
        p=p->nxt;
    }
}

// find in table by name
struct var_node* find_in_Tvar(char* name){
    struct var_node* p=Tvar->nxt;
    while(p!=NULL && strcmp(p->name,name)!=0){
        p=p->nxt;
    }
    if(p==Tvar){
        return NULL;
    }
    return p;
}

struct struct_node* find_in_Tstruct(char* name){
    struct struct_node* p=Tstruct->nxt;
    while(p!=NULL && strcmp(p->name,name)!=0){
        p=p->nxt;
    }
    if(p==Tstruct){
        return NULL;
    }
    return p;
}

struct func_node* find_in_Tfunc(char* name){
    struct func_node* p=Tfunc->nxt;
    while(p!=NULL && strcmp(p->func_name,name)!=0){
        p=p->nxt;
    }
    if(p==Tfunc){
        return NULL;
    }
    return p;
}
struct var_node* find_struct_var(char* name, char* st_name){
    struct struct_node* node=find_in_Tstruct(st_name);
    if(node!=NULL){
        struct var_node* p=node->var_list->nxt;
        while(p!=NULL&&strcmp(p->name,name)!=0){
            p=p->nxt;
        }
        return p;
    }
    return NULL;
}

// insert in charactor table
void insert_Tvar(int type, char* name, struct struct_node* st,int dim){
    struct var_node* tmp=(struct var_node*)malloc(sizeof(struct var_node));
    struct struct_node* s=NULL;
    tmp->nxt=NULL;
    tmp->dim=dim;
    tmp->name=name;
    tmp->type=type;
    tmp->struct_type=st;
    
    var->nxt=tmp;
    var=tmp;
}

void insert_struct_var(int type, char* name, struct struct_node* st, int dim, char* st_name){
    struct var_node* tmp=(struct var_node*)malloc(sizeof(struct var_node));
    struct struct_node* node=find_in_Tstruct(st_name);
    struct var_node* p=node->var_list;
    while(p->nxt!=NULL){
        p=p->nxt;
    }
    tmp->type=type;
    tmp->name=name;
    tmp->struct_type=st;
    tmp->dim=dim;
    p->nxt=tmp;
}

void insert_Tstruct(char* name){
    struct struct_node* tmp=(struct struct_node*)malloc(sizeof(struct struct_node));
    tmp->name=name;
    tmp->var_list=(struct var_node* )malloc(sizeof(struct var_node));
    stru->nxt=tmp;
    stru=tmp;
}

void insert_Tfunc(char* name){
    struct func_node* tmp=(struct func_node*)malloc(sizeof(struct func_node));
    tmp->func_name=name;
    func->nxt=tmp;
    func=tmp;
}

void insert_func_return(char* name, int return_type, struct struct_node* return_struct_type){
    struct func_node* node=find_in_Tfunc(name);
    if(node!=NULL){
        node->return_type=return_type;
        node->return_struct_type=return_struct_type;
    }
}

void insert_func_param(int type, char* name, struct struct_node* st, int dim, char* fname){
    struct var_node* tmp=(struct var_node*)malloc(sizeof(struct var_node));
    struct struct_node* s=NULL;
    struct func_node* f=find_in_Tfunc(fname);
    tmp->nxt=NULL;
    tmp->dim=dim;
    tmp->name=name;
    tmp->type=type;
    tmp->struct_type=st;
    var->nxt=tmp;
    var=tmp;
    if(f->begin_param==NULL){
        f->begin_param=tmp;
    }
    f->param_num+=1;
}


