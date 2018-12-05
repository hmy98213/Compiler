/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "syntax.y" /* yacc.c:339  */

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
			printf("Error type 15 at Line %d: Redefined variable \"%s\" in struct \"%s\". \n",id->lineno, id->content,name);
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
	int match_param(struct treenode* param, struct func_node* fnode, int pt){
		int v=1;
		struct treenode* t=param->chd;
		while(t->bro!=NULL){
			v+=1;
			t=t->bro->bro->chd;
		}
		//printf("%d\n",fnode->param_num);
		if(fnode->param_num!=v){
			if(pt){
				printf("Error type 9 at line %d: Params in function \"%s\" can't match.\n",param->lineno, fnode->func_name);
			}
			return 0;
		}
		else{
			struct var_node* p=fnode->begin_param;
			struct treenode* a=param;
			while(1){
				struct treenode* exp=a->chd;
				//printf("%d %d\n",exp->_type,p->type);
				if(exp->_type!=p->type ||exp->st!=p->struct_type ||exp->dim!=p->dim){
					if(pt){
						printf("Error type 9 at line %d: params in function \"%s\" can't match.\n",exp->lineno, fnode->func_name);
					}
					return 0;
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
		return 1;
	}

#line 212 "syntax.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "syntax.tab.h".  */
#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    ID = 260,
    SEMI = 261,
    COMMA = 262,
    ASSIGNOP = 263,
    RELOP = 264,
    PLUS = 265,
    MINUS = 266,
    STAR = 267,
    DIV = 268,
    AND = 269,
    OR = 270,
    DOT = 271,
    NOT = 272,
    TYPE = 273,
    LP = 274,
    RP = 275,
    LB = 276,
    RB = 277,
    LC = 278,
    RC = 279,
    STRUCT = 280,
    RETURN = 281,
    IF = 282,
    ELSE = 283,
    WHILE = 284,
    LOWER_THAN_ELSE = 285,
    UMINUS = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 147 "syntax.y" /* yacc.c:355  */

	struct treenode* node;

#line 288 "syntax.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 305 "syntax.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   228

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   199,   199,   206,   210,   215,   219,   223,   227,   233,
     241,   249,   253,   260,   265,   273,   278,   293,   308,   312,
     317,   323,   327,   333,   358,   376,   389,   402,   406,   412,
     420,   426,   433,   437,   442,   446,   450,   460,   464,   468,
     474,   478,   483,   497,   501,   507,   511,   517,   530,   534,
     538,   542,   554,   568,   580,   592,   596,   600,   604,   621,
     642,   660,   678,   690,   700,   709,   713
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "ID", "SEMI", "COMMA",
  "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV", "AND", "OR", "DOT",
  "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC", "STRUCT", "RETURN",
  "IF", "ELSE", "WHILE", "LOWER_THAN_ELSE", "UMINUS", "$accept", "Program",
  "ExtDefList", "ExtDef", "FuncBegin", "Func_dec_Begin", "ExtDecList",
  "Specifier", "StructSpecifier", "StructSpecifierBegin", "OptTag", "Tag",
  "VarDec", "FunDec", "Fun_dec_Dec", "VarList", "ParamDec", "CompSt",
  "CompStBegin", "StmtList", "Stmt", "DefList", "Def", "DecList", "Dec",
  "Exp", "Args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286
};
# endif

#define YYPACT_NINF -63

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-63)))

#define YYTABLE_NINF -27

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       5,   -63,    -2,     6,   -63,     5,    -1,    15,    13,   -63,
       5,    11,    35,   -63,   -63,   -63,   -63,   -63,     5,   -63,
      17,   -63,    54,    -6,   -63,   -63,    59,    43,     5,   -63,
      46,    41,   -63,    59,    83,   -63,    -4,    81,    92,   -63,
     -63,   -63,   -63,    95,    51,    51,    51,    51,    96,    98,
     -63,    76,    46,    68,   110,    59,   108,    94,   -63,    80,
      51,   -63,    59,     9,   -14,   -14,   126,    82,    51,    51,
     -63,   -63,   -63,    51,    51,    51,    51,    51,    51,    51,
      51,   124,    51,   109,   125,     5,   -63,   168,   -63,   -63,
      97,   123,   -63,   -63,   140,   154,   168,   201,   207,   207,
     -14,   -14,   194,   181,   -63,   111,   -63,    51,   -63,    46,
      46,   -63,   -63,   116,   -63,    46,   -63
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,    13,    19,     0,     2,     4,     0,     0,     0,    14,
      41,    20,     0,    16,     1,     3,    31,     7,    41,     8,
      21,     6,     0,    11,     9,    10,     0,     0,    41,    17,
      33,     0,     5,     0,     0,    21,    45,     0,    43,    15,
      40,    63,    64,    62,     0,     0,     0,     0,     0,     0,
      35,     0,    33,     0,    24,     0,     0,    28,    12,     0,
       0,    42,     0,     0,    56,    57,     0,     0,     0,     0,
      30,    32,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    23,     0,    22,    46,    44,    59,
      66,     0,    55,    36,     0,     0,    47,    50,    51,    52,
      53,    54,    48,    49,    61,     0,    27,     0,    58,     0,
       0,    60,    65,    37,    39,     0,    38
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -63,   -63,   152,   -63,   -63,   -63,   112,     0,   -63,   -63,
     -63,   -63,   -10,   -63,   -63,    73,   -63,   153,   -63,   119,
     -62,     7,   -63,   134,   -63,   -36,    65
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,    22,    26,     9,    10,
      12,    13,    23,    24,    25,    56,    57,    50,    18,    51,
      52,    27,    28,    37,    38,    53,    91
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       8,    33,    81,    11,    60,     8,    14,    82,    64,    65,
      66,    67,    41,    42,    43,    34,    36,    34,    20,    21,
      44,    19,    16,     1,    87,    30,    45,    90,    46,    89,
       2,    55,    94,    95,   -18,    40,    31,    96,    97,    98,
      99,   100,   101,   102,   103,    83,   105,   113,   114,    41,
      42,    43,    36,   116,    41,    42,    43,    44,    29,     1,
      32,    54,    44,    45,    35,    46,     2,    39,    45,    16,
      46,    90,    47,    48,    72,    49,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    55,    59,    61,    93,    82,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    62,
      70,    85,    86,    82,   107,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    63,    68,   -26,    69,    82,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    84,   104,
      34,   -25,    82,   111,    73,    74,    75,    76,    77,    78,
      79,    80,    81,   108,   115,    58,    92,    82,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    15,   106,    17,
     109,    82,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    71,   112,     0,   110,    82,    73,    74,    75,    76,
      77,    78,    79,    80,    81,     0,     0,     0,     0,    82,
      74,    75,    76,    77,    78,    79,    88,    81,     0,     0,
       0,     0,    82,    74,    75,    76,    77,    78,     0,     0,
      81,    75,    76,    77,    78,    82,     0,    81,     0,    77,
      78,     0,    82,    81,     0,     0,     0,     0,    82
};

static const yytype_int8 yycheck[] =
{
       0,     7,    16,     5,     8,     5,     0,    21,    44,    45,
      46,    47,     3,     4,     5,    21,    26,    21,     5,     6,
      11,     6,    23,    18,    60,    18,    17,    63,    19,    20,
      25,    31,    68,    69,    23,    28,    19,    73,    74,    75,
      76,    77,    78,    79,    80,    55,    82,   109,   110,     3,
       4,     5,    62,   115,     3,     4,     5,    11,    23,    18,
       6,    20,    11,    17,     5,    19,    25,    24,    17,    23,
      19,   107,    26,    27,     6,    29,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    85,     3,     6,     6,    21,
       8,     9,    10,    11,    12,    13,    14,    15,    16,     7,
      24,     7,    22,    21,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    19,    19,     6,    19,    21,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    20,     5,
      21,     6,    21,    22,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    20,    28,    33,    20,    21,     8,     9,
      10,    11,    12,    13,    14,    15,    16,     5,    85,     6,
      20,    21,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    52,   107,    -1,    20,    21,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
       9,    10,    11,    12,    13,    14,    62,    16,    -1,    -1,
      -1,    -1,    21,     9,    10,    11,    12,    13,    -1,    -1,
      16,    10,    11,    12,    13,    21,    -1,    16,    -1,    12,
      13,    -1,    21,    16,    -1,    -1,    -1,    -1,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    18,    25,    33,    34,    35,    36,    37,    39,    40,
      41,     5,    42,    43,     0,    34,    23,    49,    50,     6,
       5,     6,    38,    44,    45,    46,    39,    53,    54,    23,
      53,    19,     6,     7,    21,     5,    44,    55,    56,    24,
      53,     3,     4,     5,    11,    17,    19,    26,    27,    29,
      49,    51,    52,    57,    20,    39,    47,    48,    38,     3,
       8,     6,     7,    19,    57,    57,    57,    57,    19,    19,
      24,    51,     6,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    21,    44,    20,     7,    22,    57,    55,    20,
      57,    58,    20,     6,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,     5,    57,    47,     7,    20,    20,
      20,    22,    58,    52,    52,    28,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    36,
      37,    38,    38,    39,    39,    40,    40,    41,    42,    42,
      43,    44,    44,    45,    45,    46,    46,    47,    47,    48,
      49,    50,    51,    51,    52,    52,    52,    52,    52,    52,
      53,    53,    54,    55,    55,    56,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    58,    58
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     3,     2,     3,     1,     0,
       1,     1,     4,     4,     3,     4,     3,     3,     1,     2,
       4,     1,     2,     0,     2,     1,     3,     5,     7,     5,
       2,     0,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     4,     3,
       4,     3,     1,     1,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 199 "syntax.y" /* yacc.c:1646  */
    {					
					//printf("Program (%d)\n",$1->lineno);
					//tree_display($1,1);
					check_Tfunc();
				}
#line 1486 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 206 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"ExtDefList");
				}
#line 1495 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 210 "syntax.y" /* yacc.c:1646  */
    {
					(yyval.node)=new_node("null", yylineno, "");
				}
#line 1503 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 215 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"ExtDef");
				}
#line 1512 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 219 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"ExtDef");
				}
#line 1521 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 223 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"ExtDef");
				}
#line 1530 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 227 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
					(yyval.node)=new_tree(n,2,"ExtDef");
				}
#line 1539 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 233 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"FuncBegin");
					insert_func_return((yyvsp[0].node)->chd->content, (yyvsp[-1].node)->_type, (yyvsp[0].node)->st);
					func_name=(yyvsp[0].node)->chd->content;
				}
#line 1550 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 241 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"Func_dec_Begin");
					insert_func_return((yyvsp[0].node)->chd->content, (yyvsp[-1].node)->_type, (yyvsp[0].node)->st);
					func_name=(yyvsp[0].node)->chd->content;
				}
#line 1561 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 249 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"ExtDecList");
				}
#line 1570 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 253 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"ExtDecList");
				}
#line 1579 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 260 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Specifier");
					(yyval.node)->_type=(yyvsp[0].node)->_type;
				}
#line 1589 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 265 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Specifier");
					(yyval.node)->st=(yyvsp[0].node)->st;
					(yyval.node)->_type=2;
				}
#line 1600 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 273 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"StructSpecifier");

				}
#line 1610 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 278 "syntax.y" /* yacc.c:1646  */
    {

					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"StructSpecifier");
					struct struct_node* _st=find_in_Tstruct((yyvsp[0].node)->chd->content);
					if(_st==NULL){
						printf("Error type 17 at line %d: Undefined struct \"%s\"\n",(yyvsp[0].node)->lineno,(yyvsp[0].node)->chd->content);
					}
					else{
						(yyval.node)->st=_st;
					}
				}
#line 1627 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 293 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"StructSpecifierBegin");
					char* name=(yyvsp[-1].node)->chd->content;
					if(find_in_Tstruct(name)!=NULL ||find_in_Tvar(name)!=NULL){
						printf("Error type 16 at line %d: Redefined struct \"%s\".\n",(yyvsp[-1].node)->lineno,name);

					}
					else{
						insert_Tstruct(name);
						struct_name=name;
					}
				}
#line 1645 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 308 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"OptTag");
				}
#line 1654 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 312 "syntax.y" /* yacc.c:1646  */
    {
					(yyval.node)=new_node("null",yylineno,"");
				}
#line 1662 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 317 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Tag");
				}
#line 1671 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 323 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"VarDec");
				}
#line 1680 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 327 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"VarDec");
				}
#line 1689 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 333 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"FunDec");
					struct func_node *tmp = find_in_Tfunc((yyvsp[-3].node)->content);
					if(tmp!=NULL && tmp->defined){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",(yyvsp[-3].node)->lineno, (yyvsp[-3].node)->content);
					}
					else if(tmp!=NULL && !tmp->defined){
						//printf("OK\n");
						//printf("--%d\n",match_param($3,tmp,0));
						if(!match_param((yyvsp[-1].node),tmp,0)){
							printf("Error type 19 at line %d: Conflict declared function \"%s\".\n",(yyvsp[-3].node)->lineno, (yyvsp[-3].node)->content);
							//printf("!!!Oops!!!");
							//printf("%s %d",tmp->func_name,tmp->defined);
						}
						delete_Tfunc((yyvsp[-3].node)->content);
						insert_Tfunc((yyvsp[-3].node)->content,1);
						login_param_vars((yyvsp[-1].node),(yyvsp[-3].node)->content);
					}
					else{
						insert_Tfunc((yyvsp[-3].node)->content,1);
						login_param_vars((yyvsp[-1].node),(yyvsp[-3].node)->content);
					}

				}
#line 1719 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 358 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"FunDec");
					struct func_node *tmp = find_in_Tfunc((yyvsp[-2].node)->content);
					if(tmp!=NULL && tmp->defined){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",(yyvsp[-2].node)->lineno, (yyvsp[-2].node)->content);
					}
					else if(tmp!=NULL && !tmp->defined){
						if(!match_param((yyvsp[0].node),tmp,0)){
							printf("Error type 19 at line %d: Conflict declared function \"%s\".\n",(yyvsp[-2].node)->lineno, (yyvsp[-2].node)->content);
						}
					}
					else{
						insert_Tfunc((yyvsp[-2].node)->content,1);
					}
				}
#line 1740 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 376 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"Fun_dec_Dec");
					struct func_node *tmp = find_in_Tfunc((yyvsp[-3].node)->content);
					if(tmp!=NULL && tmp->defined){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",(yyvsp[-3].node)->lineno, (yyvsp[-3].node)->content);
					}
					else{
						insert_Tfunc((yyvsp[-3].node)->content,0);
						login_param_vars((yyvsp[-1].node),(yyvsp[-3].node)->content);
					}

				}
#line 1758 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 389 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"FunDec");
					struct func_node *tmp = find_in_Tfunc((yyvsp[-2].node)->content);
					if(tmp!=NULL && tmp->defined){
						printf("Error type 4 at line %d: Redefined function \"%s\".\n",(yyvsp[-2].node)->lineno, (yyvsp[-2].node)->content);
					}
					else{
						insert_Tfunc((yyvsp[-2].node)->content,0);
					}
				}
#line 1774 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 402 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"VarList");
				}
#line 1783 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 406 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"VarList");
				}
#line 1792 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 412 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"ParamDec");
					(yyvsp[0].node)->_type=(yyvsp[-1].node)->_type;
					(yyvsp[0].node)->st=(yyvsp[-1].node)->st;
				}
#line 1803 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 420 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"CompSt");
				}
#line 1812 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 426 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"CompStBegin");
					struct_name=NULL;
				}
#line 1822 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 433 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"StmtList");
				}
#line 1831 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 437 "syntax.y" /* yacc.c:1646  */
    {
					(yyval.node)=new_node("null",yylineno,"");
				}
#line 1839 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 442 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"Stmt");
				}
#line 1848 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 446 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Stmt");
				}
#line 1857 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 450 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Stmt");
					if(func_name!=NULL){
						struct func_node* f=find_in_Tfunc(func_name);
						if(f->return_type!=(yyvsp[-1].node)->_type||f->return_struct_type!=(yyvsp[-1].node)->st){
							printf("Error type 8 at line %d: Return value can't match.\n",(yyvsp[-1].node)->lineno);
						}
					}
				}
#line 1872 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 460 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,5,"Stmt");
				}
#line 1881 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 464 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,7,"Stmt");
				}
#line 1890 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 468 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,5,"Stmt");
				}
#line 1899 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 474 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"DefList");
				}
#line 1908 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 478 "syntax.y" /* yacc.c:1646  */
    {
					(yyval.node)=new_node("null",yylineno,"");
				}
#line 1916 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 483 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Def");
					(yyvsp[-1].node)->_type=(yyvsp[-2].node)->_type;
					(yyvsp[-1].node)->st=(yyvsp[-2].node)->st;
					if(struct_name==NULL){
						login_a_var((yyvsp[-1].node));
					}
					else{
						login_struct_var((yyvsp[-1].node),struct_name);
					}
				}
#line 1933 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 497 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"DecList");
				}
#line 1942 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 501 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"DecList");
				}
#line 1951 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 507 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"VarDec");
				}
#line 1960 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 511 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Dec");
				}
#line 1969 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 517 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					
					if((yyvsp[-2].node)->_type==(yyvsp[0].node)->_type && (yyvsp[-2].node)->st==(yyvsp[0].node)->st && (yyvsp[-2].node)->dim==(yyvsp[0].node)->dim){
						(yyval.node)->_type=(yyvsp[-2].node)->_type;
						(yyval.node)->st=(yyvsp[-2].node)->st;
						(yyval.node)->dim=(yyvsp[-2].node)->dim;
					}
					else{
						printf("Error type 5 at Line %d: Symbol type left side of \"%s\" can't match the right side.\n",(yyvsp[-1].node)->lineno,(yyvsp[-1].node)->content);
					}
				}
#line 1987 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 530 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
				}
#line 1996 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 534 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
				}
#line 2005 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 538 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
				}
#line 2014 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 542 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					if((yyvsp[-2].node)->st==NULL && (yyvsp[0].node)->st==NULL && (yyvsp[-2].node)->_type==(yyvsp[0].node)->_type && (yyvsp[-2].node)->dim==(yyvsp[0].node)->dim){
						(yyval.node)->_type=(yyvsp[-2].node)->_type;
						(yyval.node)->st=(yyvsp[-2].node)->st;
						(yyval.node)->dim=(yyvsp[-2].node)->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",(yyvsp[-1].node)->lineno);		
					}
				}
#line 2031 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 554 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					//printf("Exp: %s %d\n",$1->type,$1->lineno);

					if((yyvsp[-2].node)->st==NULL && (yyvsp[0].node)->st==NULL && (yyvsp[-2].node)->_type==(yyvsp[0].node)->_type && (yyvsp[-2].node)->dim==(yyvsp[0].node)->dim){
						(yyval.node)->_type=(yyvsp[-2].node)->_type;
						(yyval.node)->st=(yyvsp[-2].node)->st;
						(yyval.node)->dim=(yyvsp[-2].node)->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched fpr operands.\n",(yyvsp[-1].node)->lineno);		
					}
				}
#line 2050 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 568 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					if((yyvsp[-2].node)->st==NULL && (yyvsp[0].node)->st==NULL && (yyvsp[-2].node)->_type==(yyvsp[0].node)->_type && (yyvsp[-2].node)->dim==(yyvsp[0].node)->dim){
						(yyval.node)->_type=(yyvsp[-2].node)->_type;
						(yyval.node)->st=(yyvsp[-2].node)->st;
						(yyval.node)->dim=(yyvsp[-2].node)->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",(yyvsp[-1].node)->lineno);		
					}
				}
#line 2067 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 580 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					if((yyvsp[-2].node)->st==NULL && (yyvsp[0].node)->st==NULL && (yyvsp[-2].node)->_type==(yyvsp[0].node)->_type && (yyvsp[-2].node)->dim==(yyvsp[0].node)->dim){
						(yyval.node)->_type=(yyvsp[-2].node)->_type;
						(yyval.node)->st=(yyvsp[-2].node)->st;
						(yyval.node)->dim=(yyvsp[-2].node)->dim;
					}
					else{
						printf("Error type 7 at Line %d: Type dismatched for operands.\n",(yyvsp[-1].node)->lineno);		
					}
				}
#line 2084 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 592 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
				}
#line 2093 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 596 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"Exp");
				}
#line 2102 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 600 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,2,"Exp");
				}
#line 2111 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 604 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"Exp");
					struct var_node* v=find_in_Tvar((yyvsp[-3].node)->content);
					struct func_node* f=find_in_Tfunc((yyvsp[-3].node)->content);
					if(f==NULL && v!=NULL){
						printf("Error type 11 at Line %d: Can't use \"()\" at a Variable.\n",(yyvsp[-3].node)->lineno);
					}
					else{
						if(f==NULL && v==NULL){
							printf("Error type 2 at line %d: Undefined function \"%s\".\n",(yyvsp[-3].node)->lineno,(yyvsp[-3].node)->content);
						}
						else{
							match_param((yyvsp[-1].node),f,1);
						}
					}
				}
#line 2133 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 621 "syntax.y" /* yacc.c:1646  */
    {

					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					struct var_node* v=find_in_Tvar((yyvsp[-2].node)->content);
					struct func_node* f=find_in_Tfunc((yyvsp[-2].node)->content);
					if(f==NULL && v!=NULL){
						printf("Error type 11 at Line %d: Can't use \"()\" at a Variable.\n",(yyvsp[-2].node)->lineno);
					}
					else{
						if(f==NULL && v==NULL){
							printf("Error type 2 at line %d: Undefined function \"%s\".\n",(yyvsp[-2].node)->lineno,(yyvsp[-2].node)->content);
						}
						else{
							if(f->begin_param!=NULL)
							{
								printf("Error type 9 at line %d: Params in function \"%s\" can't match.\n",(yyvsp[-1].node)->lineno,(yyvsp[-2].node)->content);
							}
						}
					}
				}
#line 2159 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 642 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,4,"Exp");
					if((yyvsp[-1].node)->_type!=0 ||(yyvsp[-1].node)->dim!=0){
						printf("Error type 12 at Line %d: number in \"[ ]\" is not an integer.\n",(yyvsp[-1].node)->lineno);
					}
					else{
						int dim=(yyvsp[-3].node)->dim-1;
						if(dim<0){
							printf("Error type 10 at Line %d: Can't use \"[ ]\" at an not array symbol.\n",(yyvsp[-3].node)->lineno);
						}
						else{
							(yyval.node)->_type=(yyvsp[-3].node)->_type;
							(yyval.node)->st=(yyvsp[-3].node)->st;
							(yyval.node)->dim=dim;
						}
					}
				}
#line 2182 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 660 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Exp");
					if((yyvsp[-2].node)->st==NULL){
						printf("Error type 13 at line %d: Symbol before \".\" is not a struct.\n",(yyvsp[-1].node)->lineno);
					}
					else{
						struct var_node* v =find_struct_var((yyvsp[0].node)->content, (yyvsp[-2].node)->st->name);
						if(v==NULL){
							printf("Error type 14 at line %d: Non-existent field \"%s\".\n",(yyvsp[0].node)->lineno,(yyvsp[0].node)->content);
						}
						else{
							(yyval.node)->_type=v->type;
							(yyval.node)->st=v->struct_type;
							(yyval.node)->dim=v->dim;
						}
					}
				}
#line 2205 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 678 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Exp");
					struct var_node* t =find_in_Tvar((yyvsp[0].node)->content);
					if(t==NULL){
						printf("Error type 1 at line %d: Undefined variable \"%s\".\n",(yyvsp[0].node)->lineno,(yyvsp[0].node)->content);
					}else{
						(yyval.node)->_type=t->type;
						(yyval.node)->st=t->struct_type;
						(yyval.node)->dim=t->dim;
					}
				}
#line 2222 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 690 "syntax.y" /* yacc.c:1646  */
    {
					//printf("yes!\n");
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Exp");
					//printf("Exp: %s %d\n",$1->type,$1->lineno);
					(yyval.node)->_type=(yyvsp[0].node)->_type;
					//printf("yufa: %d\n",$1->_type);
					(yyval.node)->dim=0;
					(yyval.node)->st=NULL;
				}
#line 2237 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 700 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Exp");
					(yyval.node)->_type=(yyvsp[0].node)->_type;
					(yyval.node)->dim=0;
					(yyval.node)->st=NULL;
				}
#line 2249 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 709 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,3,"Args");
				}
#line 2258 "syntax.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 713 "syntax.y" /* yacc.c:1646  */
    {
					struct treenode* n[]={(yyvsp[0].node)};
    				(yyval.node)=new_tree(n,1,"Args");
				}
#line 2267 "syntax.tab.c" /* yacc.c:1646  */
    break;


#line 2271 "syntax.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 719 "syntax.y" /* yacc.c:1906  */

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



