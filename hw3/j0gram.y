%{
	#include <stdio.h>
	#include "tree.h"
	#include "prodrules.h"
	extern int yylex();
	extern int yyerror(char*s);
	//Stuff taken out of j0gram.y and not replaced. CLASSNAME
	//#define YYDEBUG 1
%}

%union{
	struct tree *treeptr;
}

%token <treeptr> BREAK DOUBLE ELSE FOR IF INT RETURN VOID WHILE
%token <treeptr> CLASS STRING BOOLEAN CASE
%token <treeptr> CHAR CONTINUE DEFAULT FLOAT INSTANCEOF LONG NEW SWITCH
%token <treeptr> NOT_RESERVED PUBLIC STATIC
%token <treeptr> IDENTIFIER

%token <treeptr> INTEGER_LITERAL FLOAT_LITERAL STRING_LITERAL BOOLEAN_LITERAL NULLVAL
%token <treeptr> CHARACTER_LITERAL INCREMENT DECREMENT TYPE_CAST

%token <treeptr> LESS_EQUAL GREATER_EQUAL
%token <treeptr> EQUAL NOT_EQUAL LOGIC_AND LOGIC_OR

%token <treeptr> CHAR_ERROR INT_ERROR UNKNOWN

%type <treeptr> ClassDecl
%type <treeptr> ClassBody
%type <treeptr> ClassBodyDecls
%type <treeptr> ClassBodyDecl
%type <treeptr> FieldDecl
%type <treeptr> Type
%type <treeptr> Name
%type <treeptr> QualifiedName
%type <treeptr> VarDecls
%type <treeptr> VarDeclarator
%type <treeptr> MethodReturnVal
%type <treeptr> MethodDecl
%type <treeptr> MethodHeader
%type <treeptr> MethodDeclarator
%type <treeptr> FormalParmListOpt
%type <treeptr> FormalParmList
%type <treeptr> FormalParm
%type <treeptr> ConstructorDecl
%type <treeptr> ConstructorDeclarator
%type <treeptr> ArgListOpt
%type <treeptr> Block
%type <treeptr> BlockStmtsOpt
%type <treeptr> BlockStmts
%type <treeptr> BlockStmt
%type <treeptr> LocalVarDeclStmt
%type <treeptr> LocalVarDecl
%type <treeptr> Stmt
%type <treeptr> ExprStmt
%type <treeptr> StmtExpr
%type <treeptr> IfThenStmt
%type <treeptr> IfThenElseStmt
%type <treeptr> IfThenElseIfStmt
%type <treeptr> ElseIfSequence
%type <treeptr> ElseIfStmt
%type <treeptr> WhileStmt
%type <treeptr> ForStmt
%type <treeptr> ForInit
%type <treeptr> ExprOpt
%type <treeptr> ForUpdate
%type <treeptr> StmtExprList
%type <treeptr> BreakStmt
%type <treeptr> ReturnStmt
%type <treeptr> Primary
%type <treeptr> Literal
%type <treeptr> InstantiationExpr
%type <treeptr> ArgList
%type <treeptr> FieldAccess
%type <treeptr> MethodCall
%type <treeptr> PostFixExpr
%type <treeptr> UnaryExpr
%type <treeptr> MulExpr
%type <treeptr> AddExpr
%type <treeptr> RelOp
%type <treeptr> RelExpr
%type <treeptr> EqExpr
%type <treeptr> CondAndExpr
%type <treeptr> CondOrExpr
%type <treeptr> Expr
%type <treeptr> Assignment
%type <treeptr> LeftHandSide
%type <treeptr> AssignOp

%%

ClassDecl:
	PUBLIC CLASS IDENTIFIER ClassBody
		{}
	;

ClassBody:
	'{' ClassBodyDecls '}'
		{}
	| '{' '}'
		{}
	;

ClassBodyDecls:
	ClassBodyDecl
		{}
	| ClassBodyDecls ClassBodyDecl
		{ $$ = alloc_tree(PR_CLASS_BODY_DECLS, "temp", 2, $1, $2); }
	;

ClassBodyDecl:
	FieldDecl
		{}
	| MethodDecl
		{}
	| ConstructorDecl
		{}
	;
FieldDecl:
	Type VarDecls ';'
		{}
	;

Type:
	INT
		{}
	| DOUBLE
		{}
	| BOOLEAN
		{}
	| STRING
		{}
	| Name
		{}
	;

Name:
	IDENTIFIER
		{}
	| QualifiedName
		{}
	;

QualifiedName:
	Name '.' IDENTIFIER
		{}
	;

VarDecls:
	VarDeclarator
		{}
	| VarDecls ',' VarDeclarator
		{ $$ = alloc_tree(PR_VAR_DECLS, "temp", 2, $1, $3); }
	;

VarDeclarator:
	IDENTIFIER
		{}
	| VarDeclarator '[' ']'
		{}
	;

MethodReturnVal:
	Type
		{}
	| VOID
		{}
	;

MethodDecl:
	MethodHeader Block
		{ $$ = alloc_tree(PR_METHOD_DECL, "temp", 2, $1, $2); }
	;

MethodHeader:
	PUBLIC STATIC MethodReturnVal MethodDeclarator
		{ $$ = alloc_tree(PR_METHOD_HEADER, "temp", 2, $3, $4); }
	;

MethodDeclarator:
	IDENTIFIER '(' FormalParmListOpt ')'
		{}
	;

FormalParmListOpt:
	FormalParmList
		{}
	| %empty
		{ $$ = NULL; }
	;

FormalParmList:
	FormalParm
		{}
	| FormalParmList ',' FormalParm
		{ $$ = alloc_tree(PR_FORMAL_PARM_LIST, "temp", 2, $1, $3); }
	;

FormalParm:
	Type VarDeclarator
		{ $$ = alloc_tree(PR_FORMAL_PARM, "temp", 2, $1, $2); }
	;

ConstructorDecl:
	ConstructorDeclarator Block
		{ $$ = alloc_tree(PR_CONSTRUCTOR_DECL, "temp", 2, $1, $2); }
	;

ConstructorDeclarator:
	IDENTIFIER '(' FormalParmListOpt ')'
		{}
	;

ArgListOpt:
	ArgList
		{}
	| %empty
		{ $$ = NULL; }
	;

Block:
	'{' BlockStmtsOpt '}'
		{}
	;

BlockStmtsOpt:
	BlockStmts
		{}
	| %empty
		{ $$ = NULL; }
	;

BlockStmts:
	BlockStmt
		{}
	| BlockStmts BlockStmt
		{ $$ = alloc_tree(PR_BLOCK_STMTS, "temp", 2, $1, $2); }
	;

BlockStmt:
	LocalVarDeclStmt
		{}
	| Stmt
		{}
	;

LocalVarDeclStmt:
	LocalVarDecl ';'
		{}
	;

LocalVarDecl:
	Type VarDecls
		{ $$ = alloc_tree(PR_LOCAL_VAR_DECL, "temp", 2, $1, $2); }
	;

Stmt:
	Block
		{}
	| ';'
		{}
	| ExprStmt
		{}
	| BreakStmt
		{}
	| ReturnStmt
		{}
	| %empty
		{ $$ = NULL; }
	| IfThenStmt
	  	{}
	| IfThenElseStmt
	  	{}
	| IfThenElseIfStmt
	  	{}
	| WhileStmt
	  	{}
	| ForStmt
	  	{}
	;

ExprStmt:
	StmtExpr ';'
		{}
	;

StmtExpr:
	Assignment
		{}
	| MethodCall
		{}
	| InstantiationExpr
		{}
	;

IfThenStmt:
	IF '(' Expr ')' Block
		{ $$ = alloc_tree(PR_IF_THEN_STMT, "temp", 2, $3, $5); }
	;

IfThenElseStmt:
	IF '(' Expr ')' Block ELSE Block
		{ $$ = alloc_tree(PR_IF_THEN_ELSE_STMT, "temp", 3, $3, $5, $7); }
	;

IfThenElseIfStmt:
	IF '(' Expr ')' Block ElseIfSequence
		{ $$ = alloc_tree(PR_IF_ELSEIF_STMT, "temp", 3, $3, $5, $6); }
	|  IF '(' Expr ')' Block ElseIfSequence ELSE Block
		{ $$ = alloc_tree(PR_IF_ELSEIF_ELSE_STMT, "temp", 4, $3, $5, $6, $8); }
	;

ElseIfSequence:
	ElseIfStmt
		{}
	| ElseIfSequence ElseIfStmt
		{ $$ = alloc_tree(PR_ELSE_IF_SEQUENCE, "temp", 2, $1, $2); }
	;

ElseIfStmt:
	ELSE IfThenStmt
		{}
	;

WhileStmt:
	WHILE '(' Expr ')' Stmt
		{ $$ = alloc_tree(PR_WHILE_STMT, "temp", 2, $3, $5); }
	;

ForStmt:
	FOR '(' ForInit ';' ExprOpt ';' ForUpdate ')' Block
		{ $$ = alloc_tree(PR_FOR_STMT, "temp", 4, $3, $5, $7, $9); }
	;

ForInit:
	StmtExprList
		{}
	| LocalVarDecl
		{}
	| %empty
		{ $$ = NULL; }
	;

ExprOpt:
	Expr
		{}
	| %empty
		{ $$ = NULL; }
	;

ForUpdate:
	StmtExprList
		{}
	| %empty
		{ $$ = NULL; }
	;

StmtExprList:
	StmtExpr
		{}
	| StmtExprList ',' StmtExpr
		{ $$ = alloc_tree(PR_STMT_EXPR_LIST, "temp", 2, $1, $3); }
	;

BreakStmt:
	BREAK ';'
		{}
	| BREAK IDENTIFIER ';'
		{}
	;

ReturnStmt:
	RETURN ExprOpt ';'
		{}
	;

Primary:
	 Literal
		{}
	| '(' Expr ')'
		{}
	| FieldAccess
		{}
	| MethodCall
		{}
	;

Literal:
	INTEGER_LITERAL
		{}
	| FLOAT_LITERAL
		{}
	| BOOLEAN_LITERAL
		{}
	| STRING_LITERAL
		{}
	| NULLVAL
		{}
	;

InstantiationExpr:
	NEW Name '(' ArgListOpt ')'
		{ $$ = alloc_tree(PR_INSTANTIATION_EXPR, "temp", 2, $2, $4); }
	;

ArgList:
	Expr
		{}
	| ArgList ',' Expr
		{ $$ = alloc_tree(PR_ARG_LIST, "temp", 2, $1, $3); }
	;

FieldAccess:
	Primary '.' IDENTIFIER
		{}
	;

MethodCall:
	Name '(' ArgListOpt ')'
		{ $$ = alloc_tree(PR_METHODCALL_P, "temp", 2, $1, $3); }
	| Name '{' ArgListOpt '}'
		{ $$ = alloc_tree(PR_METHODCALL_B, "temp", 2, $1, $3); }
	| Primary '.' IDENTIFIER '(' ArgListOpt ')'
		{ $$ = alloc_tree(PR_METHODCALL_PP, "temp", 2, $1, $5); }
	| Primary '.' IDENTIFIER '{' ArgListOpt '}'
		{ $$ = alloc_tree(PR_METHODCALL_BB, "temp", 2, $1, $5); }
	;

PostFixExpr:
	Primary
		{}
	| Name
		{}
	;

UnaryExpr:
	 '-' UnaryExpr
		{}
	| '!' UnaryExpr
		{}
	| PostFixExpr
		{}
	;

MulExpr:
	UnaryExpr
		{}
	| MulExpr '*' UnaryExpr
		{ $$ = alloc_tree(PR_MUL_EXPR_MULT, "temp", 2, $1, $3); }

	| MulExpr '/' UnaryExpr
		{ $$ = alloc_tree(PR_MUL_EXPR_DIV, "temp", 2, $1, $3); }
	| MulExpr '%' UnaryExpr
		{ $$ = alloc_tree(PR_MUL_EXPR_MOD, "temp", 2, $1, $3); }
	;

AddExpr:
	MulExpr
		{}
	| AddExpr '+' MulExpr
		{ $$ = alloc_tree(PR_ADD_EXPR_ADD, "temp", 2, $1, $3); }
	| AddExpr '-' MulExpr
		{ $$ = alloc_tree(PR_ADD_EXPR_SUB, "temp", 2, $1, $3); }
	;

RelOp:
	LESS_EQUAL
		{}
	| GREATER_EQUAL
		{}
	| '<'
		{}
	| '>'
		{}
	;

RelExpr:
	AddExpr
		{}
	| RelExpr RelOp AddExpr
		{ $$ = alloc_tree(PR_REL_EXPR, "temp", 3, $1, $2, $3); }
	;

EqExpr:
	RelExpr
		{}
	| EqExpr EQUAL RelExpr
		{ $$ = alloc_tree(PR_EQ_EXPR, "temp", 2, $1, $3); }
	| EqExpr NOT_EQUAL RelExpr
		{ $$ = alloc_tree(PR_EQ_EXPR_NOT, "temp", 2, $1, $3); }
	;

CondAndExpr:
	EqExpr
		{}
	| CondAndExpr LOGIC_AND EqExpr
		{ $$ = alloc_tree(PR_COND_AND_EXPR, "temp", 2, $1, $3); }
	;

CondOrExpr:
	CondAndExpr
		{}
	| CondOrExpr LOGIC_OR CondAndExpr
		{ $$ = alloc_tree(PR_COND_OR_EXPR, "temp", 2, $1, $3); }
	;

Expr:
	CondOrExpr
		{}
	| Assignment
		{}
	;

Assignment:
	LeftHandSide AssignOp Expr
		{ $$ = alloc_tree(PR_ASSIGNMENT, "temp", 3, $1, $2, $3); }
	;

LeftHandSide:
	Name
		{}
	| FieldAccess
		{}
	;

AssignOp:
	'='
		{}
	| INCREMENT
		{}
	| DECREMENT
		{}
	;
