%token BREAK DOUBLE ELSE FOR IF INT RETURN VOID WHILE
%token IDENTIFIER CLASS STRING BOOLEAN CASE
%token CHAR CONTINUE DEFAULT FLOAT INSTANCEOF LONG NEW SWITCH
%token NOT_RESERVED PUBLIC STATIC

%token INTEGER_LITERAL FLOAT_LITERAL STRING_LITERAL BOOLEAN_LITERAL NULLVAL
%token CHARACTER_LITERAL INCREMENT DECREMENT TYPE_CAST

%token LESS_EQUAL GREATER_EQUAL
%token EQUAL NOT_EQUAL LOGIC_AND LOGIC_OR



%token CHAR_ERROR INT_ERROR UNKNOWN


%{
	#include <stdio.h>
	extern int yylex();
	extern int yyerror(char*s);
	//Stuff taken out of j0gram.y and not replaced. CLASSNAME
	#define YYDEBUG 1
%}
%%

ClassDecl: PUBLIC CLASS IDENTIFIER ClassBody;
ClassBody: '{' ClassBodyDecls '}' | '{' '}' ;
ClassBodyDecls: ClassBodyDecl | ClassBodyDecls ClassBodyDecl ;
ClassBodyDecl: FieldDecl | MethodDecl | ConstructorDecl ;
FieldDecl: Type VarDecls ';' ;
Type: INT | DOUBLE | BOOLEAN | STRING | Name ;

Name: IDENTIFIER | QualifiedName ;
QualifiedName: Name '.' IDENTIFIER ;

VarDecls: VarDeclarator | VarDecls ',' VarDeclarator ;
VarDeclarator: IDENTIFIER | VarDeclarator '[' ']' ;

MethodReturnVal : Type | VOID ;
MethodDecl: MethodHeader Block ;
MethodHeader: PUBLIC STATIC MethodReturnVal MethodDeclarator ;
MethodDeclarator: IDENTIFIER '(' FormalParmListOpt ')' ;
FormalParmListOpt: FormalParmList | ;
FormalParmList: FormalParm | FormalParmList ',' FormalParm ;
FormalParm: Type VarDeclarator ;

ConstructorDecl: ConstructorDeclarator Block ;
ConstructorDeclarator: IDENTIFIER '(' FormalParmListOpt ')' ;
ArgListOpt:  ArgList | ;

Block: '{' BlockStmtsOpt '}' ;
BlockStmtsOpt: BlockStmts | ;
BlockStmts:  BlockStmt | BlockStmts BlockStmt ;
BlockStmt:   LocalVarDeclStmt | Stmt ;

LocalVarDeclStmt: LocalVarDecl ';' ;
LocalVarDecl: Type VarDecls ;

Stmt: Block | ';' | ExprStmt | BreakStmt | ReturnStmt |
      | IfThenStmt | IfThenElseStmt | IfThenElseIfStmt
      | WhileStmt | ForStmt ;

ExprStmt: StmtExpr ';' ;

StmtExpr: Assignment | MethodCall | InstantiationExpr ;

IfThenStmt: IF '(' Expr ')' Block ;
IfThenElseStmt: IF '(' Expr ')' Block ELSE Block ;
IfThenElseIfStmt: IF '(' Expr ')' Block ElseIfSequence
       |  IF '(' Expr ')' Block ElseIfSequence ELSE Block ;

ElseIfSequence: ElseIfStmt | ElseIfSequence ElseIfStmt ;
ElseIfStmt: ELSE IfThenStmt ;
WhileStmt: WHILE '(' Expr ')' Stmt ;

ForStmt: FOR '(' ForInit ';' ExprOpt ';' ForUpdate ')' Block ;
ForInit: StmtExprList | LocalVarDecl | ;
ExprOpt: Expr |  ;
ForUpdate: StmtExprList | ;

StmtExprList: StmtExpr | StmtExprList ',' StmtExpr ;

BreakStmt: BREAK ';' | BREAK IDENTIFIER ';' ;
ReturnStmt: RETURN ExprOpt ';' ;

Primary:  Literal | '(' Expr ')' | FieldAccess | MethodCall ;
Literal: INTEGER_LITERAL	| FLOAT_LITERAL | BOOLEAN_LITERAL | STRING_LITERAL | NULLVAL ;

InstantiationExpr: Name '(' ArgListOpt ')' ;
ArgList: Expr | ArgList ',' Expr ;
FieldAccess: Primary '.' IDENTIFIER ;

MethodCall: Name '(' ArgListOpt ')'
	| Name '{' ArgListOpt '}'
	| Primary '.' IDENTIFIER '(' ArgListOpt ')'
	| Primary '.' IDENTIFIER '{' ArgListOpt '}' ;

PostFixExpr: Primary | Name ;
UnaryExpr:  '-' UnaryExpr | '!' UnaryExpr | PostFixExpr ;
MulExpr: UnaryExpr | MulExpr '*' UnaryExpr
    | MulExpr '/' UnaryExpr | MulExpr '%' UnaryExpr ;
AddExpr: MulExpr | AddExpr '+' MulExpr | AddExpr '-' MulExpr ;
RelOp: LESS_EQUAL | GREATER_EQUAL | '<' | '>' ;
RelExpr: AddExpr | RelExpr RelOp AddExpr ;

EqExpr: RelExpr | EqExpr EQUAL RelExpr | EqExpr NOT_EQUAL RelExpr ;
CondAndExpr: EqExpr | CondAndExpr LOGIC_AND EqExpr ;
CondOrExpr: CondAndExpr | CondOrExpr LOGIC_OR CondAndExpr ;

Expr: CondOrExpr | Assignment ;
Assignment: LeftHandSide AssignOp Expr ;
LeftHandSide: Name | FieldAccess ;
AssignOp: '=' | INCREMENT | DECREMENT ;
