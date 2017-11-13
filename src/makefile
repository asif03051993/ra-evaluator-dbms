#Declaring variables. accessed with $ sign in later use. 
#CC will be the compiler I am using. 
#CFLAGS are  compiler flags.
#YACCFLAGS are yacc flags.
CC = g++ 
PARGEN = yacc
PARGENFLAGS = -d -v -l
LEXER = lex

all:
		$(LEXER) lex.l
		$(PARGEN) $(PARGENFLAGS) yacc.y
		$(CC) $(CFLAGS) lex.yy.c y.tab.c -o a.out
.PHONY : clean
clean:
		rm lex.yy.c y.tab.c y.tab.h y.output a.out
.PHONY : build1 run1 build2 run2

runexample1: 
		./a.out < test1
runexample2:
		./a.out < test2
runexample3: 
		./a.out < test3
runexample4:
		./a.out < test4

