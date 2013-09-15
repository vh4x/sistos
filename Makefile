parserspec.tab.c parserspec.tab.h: parserspec.y
	bison -d parserspec.y

lex.yy.c: lexspec.l parserspec.tab.h
	flex lexspec.l

exec: lex.yy.c parserspec.tab.c parserspec.tab.h
	g++ parserspec.tab.c lex.yy.c -lfl -o exec
