all: code

sintax.tab.c sintax.tab.h: sintax.y
	bison -d sintax.y
lex.yy.c: lex.l sintax.tab.h
	flex lex.l
code: lex.yy.c sintax.tab.c sintax.tab.h
	g++ sintax.tab.c lex.yy.c -lfl -o code

clean:
	rm sintax.tab.c sintax.tab.h lex.yy.c code

