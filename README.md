# ra-evaluator-dbms

This repo contains relational algebra evaluator. Relational Algebra is procedural query language, which takes Relation as input and generate relation as output. Relational algebra mainly provides theoretical foundation for relational databases and SQL.

This also provides usage of lex and yacc for relational algebra evaluator.

Steps: (using g++)

lex lex.l

yacc -d -v -l yacc.y

g++ y.tab.c lex.yy.c

./a.out "filename containing relational algebra expression"
