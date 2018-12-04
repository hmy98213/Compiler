%{
	#include <stdio.h>
	#include <unistd.h>
	#include "lex.yy.c"
	//#include "tree.h"
	//#include "table.h"
	char* struct_name=NULL;
	char* func_name=NULL;
	void login_a_var(struct treenode* declist){
		struct treenode* dec= declist->chd;
		struct treenode* id=dec->chd;
		int dim=-1;
		while(id->chd!=NULL){
			dim+=1;
			id=id->chd;
		}
		if(find_in_Tvar(id->content)!=NULL ||find_in_Tstruct(id->content)!=NULL){
			printf("Error type 3 at line %d: Redefined variable \"%s\".\n",id->lineno, id->content);
		}
		else{
			int type=-1;
			if(declist->st==NULL){
				type=declist->_type;
			}
			else{
				type=3;
			}
			insert_Tvar(type, id->content, declist->st,dim);
		}
		if(dec->bro!=NULL && dec->bro->bro!=NULL){
			dec->bro->bro->_type=declist->_type;
			dec->bro->bro->st=declist->st;
			login_a_var(dec->bro->bro);
		}
	}
	void login_vars(struct treenode* deflist){
		struct treenode* def=deflist->chd;
		if(def!=NULL){
			login_a_var(def->chd->bro);
			login_vars(def->bro);
		}
	}
	void login_struct_var(struct treenode* declist, char* name){
		struct treenode* dec=declist->chd;
		struct treenode* id=dec->chd;
		int dim=-1;
		while(id->chd!=NULL){
			dim+=1;
			id=id->chd;
		}
		if(find_struct_var(id->content,name)!=NULL){
			printf("Eorror type 15 at Line %d: Redefined variable \"%s\" in struct \"%s\". \n",id->lineno, id->content,name);
		}
		else{
			int type=-1;
			if(declist->st==NULL){
				type=declist->_type;
			}
			else
			{
				type=3;
			}
			insert_struct_var(type,id->content,declist->st,dim,name);
		}
		if(dec->bro!=NULL && dec->bro->bro!=NULL){
			dec->bro->bro->_type=declist->_type;
			login_struct_var(dec->bro->bro,name);
		}
	}
	void login_struct_vars(struct treenode* deflist, char* name){
		struct treenode* def=deflist->chd;
		if(def!=NULL){
			login_struct_var(def->chd->bro,name);
			login_struct_vars(def->bro,name);
		}
	}
	void login_param_var(struct treenode* vardec, char* name){
		struct treenode* id=vardec;
		int dim=-1;
		while(id->chd!=NULL){
			id=id->chd;
			dim+=1;
		}
		if(find_in_Tvar(id->content)!=NULL ||find_in_Tstruct(id->content)!=NULL){
			printf("Error type 3 at line %d: Redefined variable \"%s\". \n",id->lineno, id->content);

		}
		else{
			int type=-1;
			if(vardec->st==NULL){
				type=vardec->_type;
			}
			else{
				type=3;
			}
			insert_func_param(type,id->content, vardec->st,dim,name);
		}
	}
	void login_param_vars(struct treenode* varlist, char* name){
		struct treenode* paramdec=varlist->chd;
		if(paramdec!=NULL){
			login_param_var(paramdec->chd->bro,name);
			if(paramdec->bro!=NULL && paramdec->bro->bro!=NULL){
				login_param_vars(paramdec->bro->bro,name);
			}
		}
	}
	void match_param(struct treenode* param, struct func_node* fnode){
		int v=1;
		struct treenode* t=param->chd;
		while(t->bro!=NULL){
			v+=1;
			t=t->bro->bro->chd;
		}
		if(fnode->param_num!=v){
			printf("Error type 9 at line %d: Params in function \"%s\" can't match.\n",param->lineno, fnode->func_name);

		}
		else{
			struct var_node* p=fnode->begin_param;
			struct treenode* a=param;
			while(1){
				struct treenode* exp=a->chd;
				if(exp->_type!=p->type ||exp->st!=p->struct_type ||exp->dim!=p->dim){
					printf("Error type 9 at line %d: params in function \"%s\" can't match.\n",exp->lineno, fnode->func_name);
				}
				if(exp->bro==NULL){
					break;
				}
				else{
					a=exp->bro->bro;
					p=p->nxt;
				}
			}
		}
	}
%}

%union{
	struct treenode* node;
}
%token <node> INT
%token <node> FLOAT
%token <node> ID
%token <node> SEMI
%token <node> COMMA
%token <node> ASSIGNOP
%token <node> RELOP
%token <node> PLUS
%token <node> MINUS
%token <node> STAR
%token <node> DIV
%token <node> AND
%token <node> OR
%token <node> DOT
%token <node> NOT
%token <node> TYPE
%token <node> LP
%token <node> RP
%token <node> LB
%token <node> RB
%token <node> LC
%token <node> RC
%token <node> STRUCT
%token <node> RETURN
%token <node> IF
%token <node> ELSE
%token <node> WHILE

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag 
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> FuncBegin CompStBegin StructSpecifierBegin
%type <node> Exp Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE 
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT
%right LP RP LB RB DOT COMMA

%%
Program:
				ExtDefList {
					if(1){
					printf("Program (%d)\n",$1->lineno);
					tree_display($1,1);
					}
				}
				;
ExtDefList:
				ExtDef ExtDefList {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"ExtDefList");
				}
				|{
					$$=new_node("null", yylineno, "");
				}
				;
ExtDef:
				Specifier ExtDecList SEMI {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"ExtDef");
				}
				|Specifier SEMI {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"ExtDef");
				}
				|FuncBegin CompSt {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"ExtDef");
				}
				;
FuncBegin:
				Specifier FunDec {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"FuncBegin");
					insert_func_return($2->chd->content, $1->_type, $2->st);
					func_name=$2->chd->content;
				}
				;
ExtDecList:
				VarDec {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"ExtDecList");
				}
				|VarDec COMMA ExtDecList {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"ExtDecList");
				}
				;
			
Specifier:
				TYPE {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Specifier");
					$$->_type=$1->_type;
				}
				|StructSpecifier {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Specifier");
					$$->st=$1->st;
					$$->_type=2;
				}
				;
StructSpecifier:
				StructSpecifierBegin DefList RC {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"StructSpecifier");

				}
				|STRUCT Tag {

					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"StructSpecifier");
					struct struct_node* _st=find_in_Tstruct($2->chd->content);
					if(_st==NULL){
						printf("Error type 17 at line %d: Undefined struct \"%s\"\n",$2->lineno,$2->chd->content);
					}
					else{
						$$->st=_st;
					}
				}
				;

StructSpecifierBegin: 
				STRUCT OptTag LC{
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"StructSpecifierBegin");
					char* name=$2->chd->content;
					if(find_in_Tstruct(name)!=NULL ||find_in_Tvar(name)!=NULL){
						printf("Error type 16 at line %d: Redefined struct \"%s\".\n",$2->lineno,name);

					}
					else{
						insert_Tstruct(name);
						struct_name=name;
					}
				}
				;
OptTag:
				ID {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"OptTag");
				}
				|{
					$$=new_node("null",yylineno,"");
				}
				;
Tag:
				ID {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Tag");
				}
				;
VarDec:
				ID {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"VarDec");
				}
				|VarDec LB INT RB {
					struct treenode* n[]={$1,$2,$3,$4};
    				$$=new_tree(n,4,"VarDec");
				}
				;
FunDec:
				ID LP VarList RP {
					struct treenode* n[]={$1,$2,$3,$4};
    				$$=new_tree(n,4,"FunDec");
					if(find_in_Tfunc($1->content)!=NULL){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",$1->lineno, $1->content);
					}
					else{
						insert_Tfunc($1->content);
						login_param_vars($3,$1->content);
					}

				}
				|ID LP RP {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"FunDec");
					if(find_in_Tfunc($1->content)!=NULL){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",$1->lineno, $1->content);
					}
					else{
						insert_Tfunc($1->content);
					}
				}
				;
VarList:
				ParamDec COMMA VarList {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"VarList");
				}
				|ParamDec {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"VarList");
				}
				;
ParamDec:
				Specifier VarDec {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"ParamDec");
					$2->_type=$1->_type;
					$2->st=$1->st;
				}
				;
CompSt:
				CompStBegin DefList StmtList RC {
					struct treenode* n[]={$1,$2,$3,$4};
    				$$=new_tree(n,4,"CompSt");
				}
				;
CompStBegin:
				LC {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"CompStBegin");
					struct_name=NULL;
				}
				;
StmtList:
				Stmt StmtList {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"StmtList");
				}
				|{
					$$=new_node("null",yylineno,"");
				}
				;
Stmt:
				Exp SEMI {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"Stmt");
				}
				|CompSt {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Stmt");
				}
				|RETURN Exp SEMI {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Stmt");
					if(func_name!=NULL){
						struct func_node* f=find_in_Tfunc(func_name);
						if(f->return_type!=$2->_type||f->return_struct_type!=$2->st){
							printf("Error type 8 at line %d: Return value can't match.\n",$2->lineno);
						}
					}
				}
				|IF LP Exp RP Stmt %prec LOWER_THAN_ELSE{
					struct treenode* n[]={$1,$2,$3,$4,$5};
    				$$=new_tree(n,5,"Stmt");
				}
				|IF LP Exp RP Stmt ELSE Stmt {
					struct treenode* n[]={$1,$2,$3,$4,$5,$6,$7};
    				$$=new_tree(n,7,"Stmt");
				}
				|WHILE LP Exp RP Stmt {
					struct treenode* n[]={$1,$2,$3,$4,$5};
    				$$=new_tree(n,5,"Stmt");
				}
				;
DefList:
				Def DefList {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"DefList");
				}
				|{
					$$=new_node("null",yylineno,"");
				}
				;
Def:
				Specifier DecList SEMI {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Def");
					$2->_type=$1->_type;
					$2->st=$1->st;
					if(struct_name==NULL){
						login_a_var($2);
					}
					else{
						login_struct_var($2,struct_name);
					}
				}
				;
DecList:
				Dec {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"DecList");
				}
				|Dec COMMA DecList {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"DecList");
				}
				;
Dec:
				VarDec {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"VarDec");
				}
				|VarDec ASSIGNOP Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Dec");
				}
				;
Exp: 
				Exp ASSIGNOP Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					
					if($1->_type==$3->_type && $1->st==$3->st && $1->dim==$3->dim){
						$$->_type=$1->_type;
						$$->st=$1->st;
						$$->dim=$1->dim;
					}
					else{
						printf("Error type 5 at Line %d: Symbol type left side of \"%s\" can't match the right side.\n",$2->lineno,$2->content);
					}
				}
				|Exp AND Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
				}
				|Exp OR Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
				}
				|Exp RELOP Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
				}
				|Exp PLUS Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					if($1->st==NULL && $3->st==NULL && $1->_type==$3->_type && $1->dim==$3->dim){
						$$->_type=$1->_type;
						$$->st=$1->st;
						$$->dim=$1->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",$2->lineno);		
					}
				}
				|Exp MINUS Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					printf("Exp: %s %d\n",$1->type,$1->lineno);

					if($1->st==NULL && $3->st==NULL && $1->_type==$3->_type && $1->dim==$3->dim){
						$$->_type=$1->_type;
						$$->st=$1->st;
						$$->dim=$1->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched fpr operands.\n",$2->lineno);		
					}
				}
				|Exp STAR Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					if($1->st==NULL && $3->st==NULL && $1->_type==$3->_type && $1->dim==$3->dim){
						$$->_type=$1->_type;
						$$->st=$1->st;
						$$->dim=$1->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",$2->lineno);		
					}
				}
				|Exp DIV Exp {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					if($1->st==NULL && $3->st==NULL && $1->_type==$3->_type && $1->dim==$3->dim){
						$$->_type=$1->_type;
						$$->st=$1->st;
						$$->dim=$1->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",$2->lineno);		
					}
				}
				|LP Exp RP {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
				}
				|MINUS Exp %prec UMINUS {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"Exp");
				}
				|NOT Exp {
					struct treenode* n[]={$1,$2};
    				$$=new_tree(n,2,"Exp");
				}
				|ID LP Args RP {
					struct treenode* n[]={$1,$2,$3,$4};
    				$$=new_tree(n,4,"Exp");
					struct var_node* v=find_in_Tvar($1->content);
					struct func_node* f=find_in_Tfunc($1->content);
					if(f==NULL && v!=NULL){
						printf("Error type 11 at Line %d: Can't use \"()\" at a Variable.\n",$1->lineno);
					}
					else{
						if(f==NULL && v==NULL){
							printf("Error type 2 at line %d: Undefined function \"%s\".\n",$1->lineno,$1->content);
						}
						else{
							match_param($3,f);
						}
					}
				}
				|ID LP RP {

					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					struct var_node* v=find_in_Tvar($1->content);
					struct func_node* f=find_in_Tfunc($1->content);
					if(f==NULL && v!=NULL){
						printf("Error type 11 at Line %d: Can't use \"()\" at a Variable.\n",$1->lineno);
					}
					else{
						if(f==NULL && v==NULL){
							printf("Error type 2 at line %d: Undefined function \"%s\".\n",$1->lineno,$1->content);
						}
						else{
							if(f->begin_param!=NULL)
							{
								printf("Error type 9 at line %d: Params in function \"%s\" can't match.\n",$2->lineno,$1->content);
							}
						}
					}
				}
				|Exp LB Exp RB {
					struct treenode* n[]={$1,$2,$3,$4};
    				$$=new_tree(n,4,"Exp");
					if($3->_type!=0 ||$3->dim!=0){
						printf("Error type 12 at Line %d: number in \"[ ]\" is not an integer.\n",$3->lineno);
					}
					else{
						int dim=$1->dim-1;
						if(dim<0){
							printf("Error type 10 at Line %d: Can't use \"[ ]\" at an not array symbol.\n",$1->lineno);
						}
						else{
							$$->_type=$1->_type;
							$$->st=$1->st;
							$$->dim=dim;
						}
					}
				}
				|Exp DOT ID {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Exp");
					if($1->st==NULL){
						printf("Error type 13 at line %d: Symbol before \".\" is not a struct.\n",$2->lineno);
					}
					else{
						struct var_node* v =find_struct_var($3->content, $1->st->name);
						if(v==NULL){
							printf("Error type 14 at line %d: Non-existent field \"%s\".\n",$3->lineno,$3->content);
						}
						else{
							$$->_type=v->type;
							$$->st=v->struct_type;
							$$->dim=v->dim;
						}
					}
				}
				|ID {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Exp");
					struct var_node* t =find_in_Tvar($1->content);
					if(t==NULL){
						printf("Error type 1 at line %d: Undefined variable \"%s\".\n",$1->lineno,$1->content);
					}else{
						$$->_type=t->type;
						$$->st=t->struct_type;
						$$->dim=t->dim;
					}
				}
				|INT {
					//printf("yes!\n");
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Exp");
					//printf("Exp: %s %d\n",$1->type,$1->lineno);
					$$->_type=$1->_type;
					//printf("yufa: %d\n",$1->_type);
					$$->dim=0;
					$$->st=NULL;
				}
				|FLOAT {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Exp");
					$$->_type=$1->_type;
					$$->dim=0;
					$$->st=NULL;
				}
				;
Args:
				Exp COMMA Args {
					struct treenode* n[]={$1,$2,$3};
    				$$=new_tree(n,3,"Args");
				}
				|Exp {
					struct treenode* n[]={$1};
    				$$=new_tree(n,1,"Args");
				}
				;

%%
yyerror(char *msg) {
	printf("Error type B at Line %d: %s at %s\n",yylineno,msg,yytext);
}

int main(int argc,char** argv){
	init();
	if(argc>1){
		if(!(yyin=fopen(argv[1],"r"))){
			perror(argv[1]);
			return 1;
		}
	}
	yyparse();
	return 0;
}



