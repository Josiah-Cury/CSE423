%{
	#include "ns.tab.h"
%}

%option noyywrap
%option noinput
%option nounput
%%
[a-zA-Z]+ { return NAME; }
[0-9]+ { return NUMBER; }
[ \t\n]+ { }
. { fprintf(stderr, "bad char\n"); }
