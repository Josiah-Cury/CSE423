/* -*-bison-*- */
/* Simple wc implementation in Flex */

%option noyywrap
%option noinput
%option nounput

%{
#include "defs.h"

//int rows = 0, words = 0, chars = 0;
%}

letter 	[a-zA-Z]
digit 	[0-9]
ident	{letter}({letter}|{digit})*

%%
	//\n		rows++;

"/*"([^*]|"*"+[^/*])*"*"+"/" 	{ /* Ignore multiline comments*/ }
"//".*\r?\n                  	{ /* Ignore single-line comments */; }
[ \t\r\f\n]+                   	{ /* Remove whitespace */ }

\"([^"\n]|("\\\""))*\"			{ return STRING_LITERAL; }
"-"?[0-9]+|"0x"[0-9A-Fa-f]+		{ return INTEGER_LITERAL; }
[0-9]*"."[0-9]{1,7}				{ return FLOAT_LITERAL; }
\'.\'							{ return CHARACTER_LITERAL; }

\'\'							{ return CHAR_ERROR; }
\'([^"\n]|("\\\""))*\'			{ return CHAR_ERROR; }

"boolean"           { return BOOLEAN; }
"break"             { return BREAK; }
"case"              { return CASE; }
"char"              { return CHAR; }
"class"             { return CLASS; }
"continue"          { return CONTINUE; }
"default"           { return DEFAULT; }
"double"            { return DOUBLE; }
"else"              { return ELSE; }
"float"             { return FLOAT; }
"for"               { return FOR; }
"if"                { return IF; }
"instanceof"        { return INSTANCEOF; }
"int"               { return INT; }
"long"              { return LONG; }
"new"               { return NEW; }
"public"            { return PUBLIC; }
"return"            { return RETURN; }
"static"            { return STATIC; }
"switch"            { return SWITCH; }
"void"              { return VOID; }
"while"             { return WHILE; }

"abstract"          { return NOT_RESERVED; }
"assert"            { return NOT_RESERVED; }
"byte"              { return NOT_RESERVED; }
"catch"             { return NOT_RESERVED; }
"const"             { return NOT_RESERVED; }
"do"                { return NOT_RESERVED; }
"enum"              { return NOT_RESERVED; }
"exports"           { return NOT_RESERVED; }
"extends"           { return NOT_RESERVED; }
"final"             { return NOT_RESERVED; }
"finally"           { return NOT_RESERVED; }
"goto"              { return NOT_RESERVED; }
"implements"        { return NOT_RESERVED; }
"import"            { return NOT_RESERVED; }
"interface"         { return NOT_RESERVED; }
"module"            { return NOT_RESERVED; }
"native"            { return NOT_RESERVED; }
"package"           { return NOT_RESERVED; }
"protected"         { return NOT_RESERVED; }
"requires"          { return NOT_RESERVED; }
"short"             { return NOT_RESERVED; }
"strictfp"          { return NOT_RESERVED; }
"super"             { return NOT_RESERVED; }
"synchronized"      { return NOT_RESERVED; }
"this"              { return NOT_RESERVED; }
"throw"             { return NOT_RESERVED; }
"throws"            { return NOT_RESERVED; }
"transient"         { return NOT_RESERVED; }
"try"               { return NOT_RESERVED; }
"var"               { return NOT_RESERVED; }
"volatile"          { return NOT_RESERVED; }
"private"           { return NOT_RESERVED; }

"="             	{ return ASSIGN; }
"+"             	{ return ADD; }
"-"             	{ return SUB; }
"*"             	{ return MULTIPLY; }
"/"             	{ return DIVIDE; }
"%"             	{ return MODULO; }
"++"            	{ return INCREMENT; }
"--"            	{ return DECREMENT; }
	/* "-"             	{ return NEGATION; }*/
"=="            	{ return EQUAL; }
"!="            	{ return NOT_EQUAL; }
">"             	{ return GREATER_THAN; }
"<"             	{ return LESS_THAN; }
">="            	{ return GREATER_EQUAL; }
"<="            	{ return LESS_EQUAL; }
"&&"            	{ return LOGIC_AND; }
"||"            	{ return LOGIC_OR; }
"!"             	{ return LOGIC_NOT; }
	/*"instanceof"    	{ return INSTANCEOF; }*/
"["             	{ return LEFT_SUBSCRIPT; }
"]"             	{ return RIGHT_SUBSCRIPT; }
"."             	{ return DOT; }
"(type)"        	{ return TYPE_CAST; }
"("             	{ return LEFT_PARENTHESIS; }
")"             	{ return RIGHT_PARENTHESIS; }
","             	{ return COMMA; }
";"             	{ return SEMI_COLON; }
"{"             	{ return LEFT_BRACKET; }
"}"             	{ return RIGHT_BRACKET; }
":"             	{ return COLON; }

[A-Za-z0-9]+		{ return WORD_CODE; }
.					{ return UNKNOWN; }

%%
