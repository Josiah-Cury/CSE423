/* -*-bison-*- */
/* Simple wc implementation in Flex */

%option noyywrap
%option noinput
%option nounput


%{
    int error = 0;
%}
    //%s opencomment
%%
("/*")(("/*")(("/*")([^*]|"*"+[^/*])*("*"+"/")|([^*]|"*"+[^/*]))*("*"+"/")|(("/*")([^*]|"*"+[^/*])*("*"+"/")|([^*]|"*"+[^/*])))*("*"+"/")   { printf("Found multiline comment.\n");
                                    printf("\tThis is what matched: %s\n", yytext); }

%%
/*
"/*"    { BEGIN(opencomment); }
<comment>{
    ([^*]|"*"+[^/*])*"*"+"/" { printf("Single nested comment.\n"); }
    "*"+"/"
}
tripe nested = ("/*")(("/*")(("/*")([^*]|"*"+[^/*])*("*"+"/")|([^*]|"*"+[^/*]))*("*"+"/")|(("/*")([^*]|"*"+[^/*])*("*"+"/")|([^*]|"*"+[^/*])))*("*"+"/")
double nested = ("/*")(("/*")([^*]|"*"+[^/*])*("*"+"/")|([^*]|"*"+[^/*]))*("*"+"/")
 */
