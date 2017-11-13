%{
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include "header.h"
#include <vector>
#include <stack>
int errorCount = 0;
extern FILE *yyin;
extern "C" int yylex();
void yyerror(const char *);
using namespace std;
Relation *r1;
Relation *r2;
class filename{
public:
	string name; 
};
filename file;
vector <filename> file_stack;
class attribute{
public:
	vector <string> value;
};
attribute attr;
vector <string> operands;
vector <attribute> attr_stack;
stack <Relation> input;
class expcont{
public:
	vector<terms> expressions;
};
expcont current_exp;
stack <expcont> express_stack;
terms test;
unsigned int length;
%}
%union {
	char string[100];
	char number[100];
}
%token 	SIGMA PI UNION DIFFERENCE PRODUCT RENAME  
%token	RB LB GE LE NE EQ GT LT SRB SLB CRB CLB COMMA AND OR NOT QUOTE
%token  <number> NUMBER
%token  <string> STRING
%start  begin
%%
begin : {length = 0;r1 = new Relation();r2 = new Relation();}exp{cout<<"Parsing started\n";}
       ;
Pr : STRING {
	string str($1);
	str.append(".dat");
	operands.push_back(str);	
	Relation r(str.data());
	input.push(r);
	length++;
	}
     ;
exp_term : sig 
	  |pi 
	  |rename 
	  |Pr 
	  ;
exp : exp UNION exp_term {
		union_ *u = new union_(); 
		*r1 = input.top();
		input.pop();
		*r2 = input.top();
		input.pop(); 
		input.push( u->func(*r1,*r2) );
		u->r.printtupples();	
	}
	|exp DIFFERENCE exp_term {
		difference *d = new difference();
		*r1 = input.top();
		input.pop();
		*r2 = input.top();
		input.pop();
		input.push( d->func(*r1,*r2) );
		d->r.printtupples();
	}
	|exp PRODUCT exp_term {
		product *p = new product();
		*r1 = input.top();
		input.pop();
		*r2 = input.top();
		input.pop();
		input.push( p->func(*r1,*r2) );
		p->r.printtupples();				
	}
        |exp_term 
	| LB exp RB
	;
term :  STRING LE STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3; 
		expterm.op = 'L';
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	}
        |STRING LE NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = 'L';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
	|STRING GE STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = 'G';
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	}
        |STRING GE NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = 'G';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
	|STRING LT STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = '<';	
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	}
        |STRING LT NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = '<';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
	|STRING GT STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = '>';
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	}
        |STRING GT NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = '>';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
	|STRING NE STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = 'N';
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	}
        |STRING NE NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = 'N';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
        |STRING NE QUOTE STRING QUOTE {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $4;
		expterm.op = 'N';
		expterm.isRhsConst = true;
		expterm.type = "string";
		current_exp.expressions.push_back(expterm);
	} 
	|STRING EQ STRING {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		expterm.rhs = $3;
		expterm.op = '=';
		expterm.isRhsConst = false;
		current_exp.expressions.push_back(expterm);
	} 
        |STRING EQ QUOTE STRING QUOTE {
		terms expterm;
		test.isTerm = true;
		test.op = '=';
		test.isRhsConst = true;
		test.type = "string";
		test.lhs = $1;test.rhs = $4;
		current_exp.expressions.push_back(test);
	} 
	|STRING EQ NUMBER {
		terms expterm;
		expterm.isTerm = true;
		expterm.lhs = $1;
		
		expterm.rhs = $3;
		expterm.op = '=';
		expterm.isRhsConst = true;
		expterm.type = "int";
		current_exp.expressions.push_back(expterm);
	} 
        ;
predicate : term
	    | predicate AND{
			terms expterm;
			expterm.isTerm = false;
			expterm.op = '^';
			current_exp.expressions.push_back(expterm);
			} term
	    | predicate OR{
			terms expterm;
			expterm.isTerm = false;
			expterm.op = '|';
			current_exp.expressions.push_back(expterm);
		} term 
	    | NOT{
			terms expterm;
			expterm.isTerm = false;
			expterm.op = '!';
			current_exp.expressions.push_back(expterm);
		} CLB{
			terms expterm;
			expterm.isTerm = false;
			expterm.op = '(';
			current_exp.expressions.push_back(expterm);
		} predicate CRB {
			terms expterm;
			expterm.isTerm = false;
			expterm.op = ')';
			current_exp.expressions.push_back(expterm);
		}
	    ;
sig   : SIGMA SLB {current_exp.expressions.clear();} predicate {express_stack.push(current_exp)} SRB LB exp RB {
		select_ *p = new select_();
		*r1 = input.top();
		input.pop();
		input.push( p->func(*r1,express_stack.top().expressions) );
		p->r.printtupples();
		express_stack.pop();
		}
	;
list :  STRING {attr.value.push_back($1);}
       | list COMMA STRING {attr.value.push_back($3);} 
       |
       ;
pi    : PI SLB {attr.value.clear();} list {attr_stack.push_back(attr);} SRB LB exp RB {
		project *p = new project();
		*r1 = input.top();
		input.pop();
		input.push( p->func(attr_stack.back().value,*r1) );
		p->r1.printtupples();
		attr_stack.erase(attr_stack.end()--);		
	} 
	; 
rename : RENAME SLB STRING {file.name.assign($3);file_stack.push_back(file);} SRB SLB {attr.value.clear();} list{attr_stack.push_back(attr);} SRB LB exp RB {
		rename_ *p = new rename_();
		*r1 = input.top();
		input.pop();
		input.push( p->func(*r1, file_stack.back().name.data(), attr_stack.back().value) );
		p->r1.printtupples();
		attr_stack.erase(attr_stack.end()--);
		file_stack.erase(file_stack.end()--);	
	}
	;
%%
void myerror(const char *s, int d) {
	errorCount++;
	printf("Error at line %d: %s\n", d, s);
}
void yyerror(const char *s) {}
int main(int argc,char *argv[]) {
        yyin=fopen(argv[1],"r");
        do {
		yyparse();
	} while (!feof(yyin));
	if(errorCount == 0)
		printf("\nThere are no errors in the file.\n");
	return 0;
}
