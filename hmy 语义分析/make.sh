#! /bin/bash
# makefil
bison -d syntax.y
flex lex.l
gcc syntax.tab.c tree.h table.h -lfl -ly -o parser

