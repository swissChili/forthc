%require "3.0"
%language "c++"

%{
#include <string>
#include <sstream>
#include <variant>
#include <iostream>
#include <stdio.h>
#include "asm.h"

extern parser::symbol_type yylex();
extern void yyterminate();
void yyerror(const char *s);
extern FILE* yyin;
extern int yylineno;
%}

%define api.value.type variant

%token<int> INT
%token<std::string> NAME
%token OTYPE_HINT
%token CTYPE_HINT

//%type<std::ostringstream> program
%type<std::ostringstream> function body fragment
// %token<std::ostringstream> function

%%

program:
	%empty
	{
		std::cout << ".text\n\n";
	}
    | program function
	{
		std::cout << $2.str();
	}
    ;

function:
	':' NAME
	{
		$<std::ostringstream>$ = std::ostringstream();
		$<std::ostringstream>$
			<< ".global " << $2 << "\n"
			<< $2 << ":\n";
	}
	body ';'
	{
		$$
			<< $<std::ostringstream>1.str()
			<< "\tpop\t%rax\n"
			<< "\tret\n";
	}
	;

body:
	%empty
	{
		$$ = std::ostringstream();
	}
	| body fragment
	{
		$$ << $2.str();
	}
	;

fragment:
	| INT
	{
		$$ = std::ostringstream();
		$$ << "\tpush\t$" << $1 << std::endl;
	}
	| '+'
	{
		$$ = std::ostringstream();
		$$ << BINARY_OP(add);
	}
	| '-'
	{
		$$ = std::ostringstream();
		$$ << BINARY_OP(sub);
	}
	| '*'
	{
		$$ = std::ostringstream();
		$$ << RAX_OP(mul);
	}
	| '/'
	{
		$$ = std::ostringstream();
		$$ << RAX_OP(div);
	}
	| NAME
	{
		$$ = std::ostringstream();
		$$ << "\tcall\t" << $1 << "\n";
	}
	;

%%


/* Entry point */
int main(int argc, char **argv)
{
	yyin = stdin;
	yy::parser parse;
	return parse();
}

/* Display error messages */
void yyerror(const char *s)
{
	std::cerr << "Error near line " << yylineno << ": " << s << std::endl;
}
