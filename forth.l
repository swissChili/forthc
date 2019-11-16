%{
#include <string>
#include <variant>
#include "B.hh"
%}

%option noyywrap
%option yylineno
%option c++

%%

[ \t\r\n]     // skip
\\[^\n]*      // skip comment
[0-9]+        { return yy::parser::make_INT(std::stoi( yytext )); }
[+\-\*/:;]    { return *yytext; }
[a-z]+        { return parser::make_NAME(yytext); }
\(\(          { return parser::make_OTYPE_HINT(); }
\)\)          { return parser::make_CTYPE_HINT(); }
\([^\)]*\)    // skip comment, must be after type-hinted comments (( type name ))

%%
