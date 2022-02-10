/* -*-bison-*- */
/* Simple wc implementation in Flex */

%option noyywrap
%option noinput
%option nounput
%option yylineno

%{
#include "defs.h"
%}

letter 	[a-zA-Z]
digit 	[0-9]
ident	{letter}({letter}|{digit})*
hex		[0-9A-Fa-f]
escapes [nt'"\]

%%

"/*"([^*]|"*"+[^/*])*"*"+"/" 	{ /* Ignore multiline comments*/ }
"//".*\r?\n                  	{ /* Ignore single-line comments */; }
[ \t\r\f\n]+                   	{ /* Remove whitespace */ }

\"([^"\n]|("\\\""))*\"			{ return init_token(STRING_LITERAL); }
"-"?{digit}+|"0x"{hex}+			{ return init_token(INTEGER_LITERAL); }
{digit}*"."{digit}{1,7}			{ return init_token(FLOAT_LITERAL); }
\'.\'							{ return init_token(CHARACTER_LITERAL); }
\'\\.\'							{ return init_token(CHARACTER_LITERAL); }

\'\'							{ return init_token(CHAR_ERROR); }
\'([^"\n]|("\\\""))*\'			{ return init_token(CHAR_ERROR); }
"-"?{digit}+[^:;A-Fa-f\n]		{ return init_token(INT_ERROR); }

"boolean"           { return init_token(BOOLEAN); }
"break"             { return init_token(BREAK); }
"case"              { return init_token(CASE); }
"char"              { return init_token(CHAR); }
"class"             { return init_token(CLASS); }
"continue"          { return init_token(CONTINUE); }
"default"           { return init_token(DEFAULT); }
"double"            { return init_token(DOUBLE); }
"else"              { return init_token(ELSE); }
"float"             { return init_token(FLOAT); }
"for"               { return init_token(FOR); }
"if"                { return init_token(IF); }
"instanceof"        { return init_token(INSTANCEOF); }
"int"               { return init_token(INT); }
"long"              { return init_token(LONG); }
"new"               { return init_token(NEW); }
"public"            { return init_token(PUBLIC); }
"return"            { return init_token(RETURN); }
"static"            { return init_token(STATIC); }
"switch"            { return init_token(SWITCH); }
"void"              { return init_token(VOID); }
"while"             { return init_token(WHILE); }

"abstract"          { return init_token(NOT_RESERVED); }
"assert"            { return init_token(NOT_RESERVED); }
"byte"              { return init_token(NOT_RESERVED); }
"catch"             { return init_token(NOT_RESERVED); }
"const"             { return init_token(NOT_RESERVED); }
"do"                { return init_token(NOT_RESERVED); }
"enum"              { return init_token(NOT_RESERVED); }
"exports"           { return init_token(NOT_RESERVED); }
"extends"           { return init_token(NOT_RESERVED); }
"final"             { return init_token(NOT_RESERVED); }
"finally"           { return init_token(NOT_RESERVED); }
"goto"              { return init_token(NOT_RESERVED); }
"implements"        { return init_token(NOT_RESERVED); }
"import"            { return init_token(NOT_RESERVED); }
"interface"         { return init_token(NOT_RESERVED); }
"module"            { return init_token(NOT_RESERVED); }
"native"            { return init_token(NOT_RESERVED); }
"package"           { return init_token(NOT_RESERVED); }
"protected"         { return init_token(NOT_RESERVED); }
"requires"          { return init_token(NOT_RESERVED); }
"short"             { return init_token(NOT_RESERVED); }
"strictfp"          { return init_token(NOT_RESERVED); }
"super"             { return init_token(NOT_RESERVED); }
"synchronized"      { return init_token(NOT_RESERVED); }
"this"              { return init_token(NOT_RESERVED); }
"throw"             { return init_token(NOT_RESERVED); }
"throws"            { return init_token(NOT_RESERVED); }
"transient"         { return init_token(NOT_RESERVED); }
"try"               { return init_token(NOT_RESERVED); }
"var"               { return init_token(NOT_RESERVED); }
"volatile"          { return init_token(NOT_RESERVED); }
"private"           { return init_token(NOT_RESERVED); }

"="             	{ return init_token(ASSIGN); }
"+"             	{ return init_token(ADD); }
"-"             	{ return init_token(SUB); }
"*"             	{ return init_token(MULTIPLY); }
"/"             	{ return init_token(DIVIDE); }
"%"             	{ return init_token(MODULO); }
"++"            	{ return init_token(INCREMENT); }
"--"            	{ return init_token(DECREMENT); }
	/* "-"             	{ return init_token(NEGATION); }*/
"=="            	{ return init_token(EQUAL); }
"!="            	{ return init_token(NOT_EQUAL); }
">"             	{ return init_token(GREATER_THAN); }
"<"             	{ return init_token(LESS_THAN); }
">="            	{ return init_token(GREATER_EQUAL); }
"<="            	{ return init_token(LESS_EQUAL); }
"&&"            	{ return init_token(LOGIC_AND); }
"||"            	{ return init_token(LOGIC_OR); }
"!"             	{ return init_token(LOGIC_NOT); }
	/*"instanceof"    	{ return init_token(INSTANCEOF); }*/
"["             	{ return init_token(LEFT_SUBSCRIPT); }
"]"             	{ return init_token(RIGHT_SUBSCRIPT); }
"."             	{ return init_token(DOT); }
"(type)"        	{ return init_token(TYPE_CAST); }
"("             	{ return init_token(LEFT_PARENTHESIS); }
")"             	{ return init_token(RIGHT_PARENTHESIS); }
","             	{ return init_token(COMMA); }
";"             	{ return init_token(SEMI_COLON); }
"{"             	{ return init_token(LEFT_BRACKET); }
"}"             	{ return init_token(RIGHT_BRACKET); }
":"             	{ return init_token(COLON); }

[A-Za-z0-9]+		{ return init_token(WORD_CODE); }
.					{ return init_token(UNKNOWN); }

%%
