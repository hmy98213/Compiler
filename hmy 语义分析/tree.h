extern int yylineno;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

struct treenode {
	struct treenode* bro;
	struct treenode* chd;
	char* type;
	char* content;	
	int lineno;
	int is_token;
	int _type;
	int dim;
	struct struct_node* st;
};

struct treenode* new_node(char* type, int lineno, char* text){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->type=type;
	node->chd=NULL;
	node->bro=NULL;
	node->lineno=lineno;
	int len=strlen(text);
	node->content=(char*) malloc(sizeof(char)*len);
	strcpy(node->content,text);
	node->is_token=1;
	return node;
}

struct treenode* new_tree(struct treenode* n[],int t_num,char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	struct treenode* ori[100];
	node->chd=n[0];
	node->bro=NULL;
	for (int i = 0; i < t_num-1; ++i){
		n[i]->bro=n[i+1];
	}
	node->type=type;
	node->lineno=yylineno;
	node->is_token=0;
	return node;
}


void tree_display(struct treenode* rt,int dep){
	if(rt==NULL) return ;
	if(strcmp(rt->type,"null")!=0){
		int i=0;
		for(i=0;i<dep;i++){
			printf("  ");
		}
		if(strcmp(rt->type,"TYPE")==0){
			printf("TYPE: %s\n",rt->content);
		}
		else if(strcmp(rt->type,"ID")==0){
			printf("ID: %s\n",rt->content);			
		}
		else if(strcmp(rt->type,"FLOAT")==0){
			printf("FLOAT: %s\n",rt->content);
		}
		else if(strcmp(rt->type,"INT")==0){
			printf("INT: %s\n", rt->content);
		}
		else if(rt->is_token){
			printf("%s\n",rt->type);
		}
		else{
			printf("%s (%d)\n",rt->type,rt->lineno);
		}

	} 
	if(rt->chd!=NULL){
		tree_display(rt->chd,dep+1);
	}
	if(rt->bro!=NULL){
		tree_display(rt->bro,dep);
	}
	

}

