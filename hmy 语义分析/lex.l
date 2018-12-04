%{
 #include<stdio.h>
 #include "stdlib.h"
 #include"syntax.tab.h"
 #include<string.h>
 #include<ctype.h>
 #include "tree.h"
 //#include "table.h"

 int flag=0;
 
 char* itoa(int n,char* tmp){
	 int i,j,sign;
	 char* s=malloc(sizeof(char)*strlen(tmp));
		if((sign=n)<0)//记录符号
		 n=-n;//使n成为正数
		 i=0;
		do{

			s[i++]=n%10+'0';//取下一个数字

		 }
		 while ((n/=10)>0);//删除该数字
			 if(sign<0)
				s[i++]='-';
			//生成的数字是逆序的，所以要逆序输出
		     int m=i-1;
			 j=0; 
			for(m=i-1;m>=0;m--){
				tmp[j++]=s[m];
			}
			tmp[j]='\0';
		return tmp;
 }

 void solve_token(char* type, char* text){
	yylval.node=new_node(type,yylineno,text); //!! new_node()
}
%}

%option yylineno
COMMENT  \/\*
LINECOMMENT \/\/
COMMENTERROR \*\/
TYPE int|float
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
HEX 0[Xx][0-9A-Za-z]*
OCTAL 0[0-9A-Za-z]+
SCIENNUM [+-]?[0-9]+(\.[0-9]+)?[eE][+\-0-9a-zA-Z]*
INT [1-9]+[0-9]*|0 
FLOAT [+-]?[0-9]+"."[0-9]+
ID [A-Za-z_][A-Za-z_0-9]*
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS "+"
MINUS -
STAR "*"
DIV "/"
AND &&
OR "||"
DOT "."
NOT "!"
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
ILLEGALNUM  ([0-9]|{FLOAT})[0-9a-zA-Z_\.]+



%%
{COMMENT} {
	char ch,last_ch;
	int i=1;
	while((ch=input())!=EOF){
		if(ch=='/'){
			if((ch=input())=='*'){
				i++;
			}

		}
		if(ch=='*'){
			if((ch=input()) && ch=='/'){
				i--;
				if(i==0) {
					break ;
				}
			}
		}
			if(ch==NULL&&i>0){
			printf("Error type A at line %d:Comment Error:  '/*' lost right one\n",yylineno);
				break;
			}
	}
	
	//return COMMENT;
}
{COMMENTERROR} {
	printf("Error Type A at Line %d: Comment Error: '*/' lost left one\n",yylineno);
	flag=1;
}
{LINECOMMENT} {
	char c=input();while(c!='\n')c=input();
}
{TYPE} {
	solve_token("TYPE",yytext);
	if(strcmp(yylval.node->content,"int")==0){
		yylval.node->_type=0;
	}
	else if(strcmp(yylval.node->content,"float")==0){
		yylval.node->_type=1;
	}
	return TYPE;
}
{STRUCT} {
	solve_token("STRUCT",yytext);
	return STRUCT;
}
{RETURN} {
	solve_token("RETURN",yytext);
	return RETURN;
}
{IF} {
	solve_token("IF",yytext);
	return IF;
}
{ELSE} {
	solve_token("ELSE",yytext);
	return ELSE;
}
{WHILE} {
	solve_token("WHILE",yytext);
	return WHILE;
}
{SCIENNUM} {
	double number=atof(yytext);
	char* p=NULL;
	int is_wrong=0;
	p=strchr(yytext,'e');
	if(p==NULL) {p=strchr(yytext,'E');}
	if(p!=NULL){
		if(*(p+sizeof(char))=='+'||*(p+sizeof(char))=='-'){
			p=p+sizeof(char);
			while(*(p+sizeof(char))!='\0'){
				p=p+sizeof(char);
				if(*p>'9'||*p<'0'){
					is_wrong=1;
					break;
				}
			}
		}
		else{
			while(*(p+sizeof(char))!='\0'){
				p=p+sizeof(char);
				if(*p>'9'||*p<'0'){
					is_wrong=1;
					break;
				}
			}
		
		}
		if(!is_wrong){
			char* tmp=malloc(sizeof(char)*30);
			gcvt(number,8,tmp);
			solve_token("FLOAT",tmp);
		}
	}
	else {
		solve_token("FLOAT",yytext);
		yylval.node->_type=1;
		}
	if(is_wrong){
		flag=1;
		printf("Error Type A at Line %d: cannot identify Science number--%s\n",yylineno,yytext);
	}
	return FLOAT;
}
{HEX} {
	int number;
	sscanf(yytext,"%x",&number);
	int len=strlen(yytext);
	int i=0,is_wrong=0;
	if(len<=2) {is_wrong=1;}
	else if(yytext[0]!='0'||(yytext[1]!='x'&&yytext[1]!='X')) {is_wrong=1;}
	else {
		for(int i=2;i<len;i++){
			if(!isxdigit(yytext[i])){
				is_wrong=1;
				break;}
		}
	}
	if(is_wrong){
		flag=1;
		printf("Error Type A at Line %d: cannot identify HEXADECIMAL number--%s\n",yylineno,yytext);
	}
	else{
		char* tmp=malloc(sizeof(char)*(len*8));
		tmp=itoa(number,tmp);
		solve_token("INT",tmp); 	
	} return INT;
}
{OCTAL} {
	int number;
	sscanf(yytext,"%o",&number);
	int len=strlen(yytext);
	int i,is_wrong=0;
	for(i=0;i<len;i++){
		if(yytext[i]<'0'||yytext[i]>='8'){
			printf("Error Type A At Line %d: Cannot identify OCTAL number -- %s\n",yylineno,yytext);
			is_wrong=1;
			flag=1;
			break;
		}
	}
	if(!is_wrong){
		char* tmp=malloc(sizeof(char)*(len+5));
		tmp=itoa(number,tmp);
		solve_token("INT",tmp);
		
	}return INT;
}

{INT} {
	solve_token("INT",yytext);
	yylval.node->_type=0;
	//printf("token: %s %d\n",yylval.node->content,yylval.node->lineno);
	return INT;
}
{FLOAT} {
	solve_token("FLOAT",yytext);
	yylval.node->_type=1;
	//printf("token: %s %d\n",yylval.node->content,yylval.node->lineno);
	return FLOAT;
}

{ILLEGALNUM} {
	flag=1;
		printf("Error Type A at Line %d: cannot identify the number--%s\n",yylineno,yytext);
	
	return INT;
}

{ID} {
	solve_token("ID",yytext);
	return ID;
}

{SEMI} {
	solve_token("SEMI",yytext);
	return SEMI;
}
{COMMA} {
	solve_token("COMMA",yytext);
	return COMMA;
}
{ASSIGNOP} {
	solve_token("ASSIGNOP",yytext);
	return ASSIGNOP;
}
{RELOP} {
	solve_token("RELOP",yytext);
	return RELOP;
}

{PLUS} {
	solve_token("PLUS",yytext);
	return PLUS;
}
{MINUS} {
	solve_token("MINUS",yytext);
	return MINUS;
}
{STAR} {
	solve_token("STAR",yytext);
	return STAR;
}
{DIV} {
	solve_token("DIV",yytext);
	return DIV;
}
{AND} {
	solve_token("AND",yytext);
	return AND;
}


{OR} {
	solve_token("OR",yytext);
	return OR;
}
{DOT} {
	solve_token("DOT",yytext);
	return DOT;
}
{NOT} {
	solve_token("DOT",yytext);
	return NOT;
}
{LP} {
	solve_token("LP",yytext);
	return LP;
}
{RP} {
	solve_token("RP",yytext);
	return RP;
}
{LB} {
	solve_token("LB",yytext);
	return LB;
}
{RB} {
	solve_token("RB",yytext);
	return RB;
}
{LC} {
	solve_token("LC",yytext);
	return LC;
}
{RC} {
	solve_token("RC",yytext);
	return RC;
}
[ \t\n]+ { }

. {
		flag=1;		
		printf("Error type A at Line %d: mysterious character \"%s\".\n",yylineno,yytext);
		return ID;
}
%%
int yywrap() {

	return 1;
}




