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
struct treenode* new_tree1(struct treenode* n1,char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	node->type=type;
	//node->lineno=n1->lineno;
	node->lineno=yylineno;

	node->is_token=0;
	
	return node;
}
struct treenode* new_tree2(struct treenode* n1, struct treenode* n2, char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	n1->bro=n2;
	node->type=type;
	//node->lineno=n1->lineno;
	node->lineno=yylineno;

	node->is_token=0;
	
	return node;
}
struct treenode* new_tree3(struct treenode* n1, struct treenode* n2, struct treenode*n3, char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	n1->bro=n2;
	n2->bro=n3;
	node->type=type;
	//node->lineno=n1->lineno;
	node->lineno=yylineno;
	node->is_token=0;
	
	return node;
}
struct treenode* new_tree4(struct treenode* n1, struct treenode* n2, struct treenode* n3, struct treenode* n4,char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	n1->bro=n2;
	n2->bro=n3;
	n3->bro=n4;
	node->type=type;
	node->lineno=yylineno;
//node->lineno=n1->lineno;
	node->is_token=0;
	
	return node;
}
struct treenode* new_tree5(struct treenode* n1, struct treenode* n2, struct treenode* n3, struct treenode* n4, struct treenode* n5,char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	n1->bro=n2;
	n2->bro=n3;
	n3->bro=n4;
	n4->bro=n5;
	node->type=type;
	//node->lineno=n1->lineno;
	node->lineno=yylineno;
	node->is_token=0;
	
	return node;
}
struct treenode* new_tree7(struct treenode* n1, struct treenode* n2, struct treenode* n3, struct treenode* n4, struct treenode* n5, struct treenode* n6, struct treenode* n7,char* type){
	struct treenode* node =(struct treenode*)malloc(sizeof(struct treenode));
	node->chd=n1;
	node->bro=NULL;
	n1->bro=n2;
	n2->bro=n3;
	n3->bro=n4;
	n4->bro=n5;
	n5->bro=n6;
	n6->bro=n7;
	node->type=type;
	//node->lineno=n1->lineno;
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

