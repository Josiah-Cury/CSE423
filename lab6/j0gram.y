%{
	#include <stdio.h>
	#include "tree.h"
	#include "defs.h"
	#include "symt.h"

	extern int yylex();
	extern int yyerror(const char*s);
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

%token <treeptr> CHAR_ERROR INT_ERROR UNKNOWN STRING_ERROR OPEN_COMMENT

%left <treeptr> THEN
%left ELSE

%left LOGIC_OR
%left LOGIC_AND
%left EQUAL NOT_EQUAL
%left '<' '>' GREATER_EQUAL LESS_EQUAL
%left '+' '-'
%left '*' '/' '%'
%left '!'

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

%token <treeptr> '=' '+' '-' '*' '/' '%'
%token <treeptr> '(' ')' ',' ';' ':' '[' '<' '.' '!' ']'
%token <treeptr> '>'

%start ClassDecl

%glr-parser
%%

ClassDecl:
	PUBLIC CLASS IDENTIFIER ClassBody
		{ tree_root = link_tree(PR_CLASS_DECL, "ClassDecl", 4, $1, $2, $3, $4); }
	;

ClassBody:
	'{' ClassBodyDecls '}'
		{ $$ = $2; }
	| '{' '}'
		{}
	;

ClassBodyDecls:
	ClassBodyDecl
		{}
	| ClassBodyDecls ClassBodyDecl
		{ $$ = link_tree(PR_CLASS_BODY_DECLS, "ClassBodyDecls", 2, $1, $2); }
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
		{ $$ = link_tree(PR_FIELD_DECL, "FieldDecl", 2, $1, $2); }
	| Type VarDeclarator '=' Literal ';'
		{ $$ = link_tree(PR_FIELD_DECL, "FieldDecl", 3, $1, $2, $4); }
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
	| CHAR
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
		{ $$ = link_tree(PR_QUALIFIED_NAME, "QualifiedName", 2, $1, $3); }
	;

VarDecls:
	VarDeclarator
		{ $$ =  link_tree(PR_VAR_DECLS, "VarDecls", 1, $1); }
	| VarDecls ',' VarDeclarator
		{ $$ = link_tree(PR_VAR_DECLS_LIST, "VarDeclsList", 2, $1, $3); }
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
		{ $$ = link_tree(PR_METHOD_DECL, "MethodDecl", 2, $1, $2); }
	;

MethodHeader:
	PUBLIC STATIC MethodReturnVal MethodDeclarator
		{ $$ = link_tree(PR_METHOD_HEADER, "MethodHeader", 4, $1, $2, $3, $4); }
	| PUBLIC MethodReturnVal MethodDeclarator
		{ $$ = link_tree(PR_METHOD_HEADER, "MethodHeader", 3, $1, $2, $3); }
	| STATIC MethodReturnVal MethodDeclarator
		{ $$ = link_tree(PR_METHOD_HEADER, "MethodHeader", 3, $1, $2, $3); }
	| MethodReturnVal MethodDeclarator
		{ $$ = link_tree(PR_METHOD_HEADER, "MethodHeader", 2, $1, $2); }
	;

MethodDeclarator:
	IDENTIFIER '(' FormalParmListOpt ')'
		{ $$ = link_tree(PR_METHOD_DECLARATOR, "MethodDeclarator", 2, $1, $3); }
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
		{ $$ = link_tree(PR_FORMAL_PARM_LIST, "FormalParmList", 2, $1, $3); }
	;

FormalParm:
	Type VarDeclarator
		{ $$ = link_tree(PR_FORMAL_PARM, "FormalParm", 2, $1, $2); }
	;

ConstructorDecl:
	ConstructorDeclarator Block
		{ $$ = link_tree(PR_CONSTRUCTOR_DECL, "ConstructorDecl", 2, $1, $2); }
	;

ConstructorDeclarator:
	IDENTIFIER '(' FormalParmListOpt ')'
		{ $$ = link_tree(PR_CONSTRUCTOR_DECLARATOR, "ConstructorDeclarator", 2, $1, $3); }
	;

ArgListOpt:
	ArgList
		{}
	| %empty
		{ $$ = NULL; }
	;

Block:
	'{' BlockStmtsOpt '}'
		{ $$ = $2; }
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
		{ $$ = link_tree(PR_BLOCK_STMTS, "BlockStmts", 2, $1, $2); }
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
		{ $$ = link_tree(PR_LOCAL_VAR_DECL, "LocalVarDecl", 2, $1, $2); }
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
	/*| %empty
		{ $$ = NULL; }*/
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
		{ $$ = link_tree(PR_IF_THEN_STMT, "IfThenStmt", 2, $3, $5); }
	;

IfThenElseStmt:
	IF '(' Expr ')' Block ELSE Block
		{ $$ = link_tree(PR_IF_THEN_ELSE_STMT, "IfThenElseStmt", 3, $3, $5, $7); }
	;

IfThenElseIfStmt:
	IF '(' Expr ')' Block ElseIfSequence
		{ $$ = link_tree(PR_IF_ELSEIF_STMT, "IfElseIfStmt", 3, $3, $5, $6); }
	|  IF '(' Expr ')' Block ElseIfSequence ELSE Block
		{ $$ = link_tree(PR_IF_ELSEIF_ELSE_STMT, "IfElseIfElseStmt", 4, $3, $5, $6, $8); }
	;

ElseIfSequence:
	ElseIfStmt
		{}
	| ElseIfSequence ElseIfStmt
		{ $$ = link_tree(PR_ELSE_IF_SEQUENCE, "ElseIfSequence", 2, $1, $2); }
	;

ElseIfStmt:
	ELSE IfThenStmt
		{ $$ = $2; }
	;

WhileStmt:
	WHILE '(' Expr ')' Stmt
		{ $$ = link_tree(PR_WHILE_STMT, "WhileStmt", 2, $3, $5); }
	;

ForStmt:
	FOR '(' ForInit ';' ExprOpt ';' ForUpdate ')' Block
		{ $$ = link_tree(PR_FOR_STMT, "ForStmt", 4, $3, $5, $7, $9); }
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
		{ $$ = link_tree(PR_STMT_EXPR_LIST, "StmtExprList", 2, $1, $3); }
	;

BreakStmt:
	BREAK ';'
		{}
	| BREAK IDENTIFIER ';'
		{ $$ = $2; }
	;

ReturnStmt:
	RETURN ExprOpt ';'
		{ $$ = $2; }
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
	| CHARACTER_LITERAL
		{}
	;

InstantiationExpr:
	NEW Name '(' ArgListOpt ')'
		{ $$ = link_tree(PR_INSTANTIATION_EXPR, "InstantiationExpr", 2, $2, $4); }
	;

ArgList:
	Expr
		{}
	| ArgList ',' Expr
		{ $$ = link_tree(PR_ARG_LIST, "ArgList", 2, $1, $3); }
	;

FieldAccess:
	Primary '.' IDENTIFIER
			{ $$ = link_tree(PR_FIELD_ACCESS, "FieldAccess", 2, $1, $3); }
	;

MethodCall:
	Name '(' ArgListOpt ')'
		{ $$ = link_tree(PR_METHODCALL_P, "MethodCall_P", 2, $1, $3); }
	| Name '{' ArgListOpt '}'
		{ $$ = link_tree(PR_METHODCALL_B, "MethodCall_B", 2, $1, $3); }
	| Primary '.' IDENTIFIER '(' ArgListOpt ')'
		{ $$ = link_tree(PR_METHODCALL_PP, "MethodCall_PP", 3, $1, $3, $5); }
	| Primary '.' IDENTIFIER '{' ArgListOpt '}'
		{ $$ = link_tree(PR_METHODCALL_BB, "MethodCall_BB", 3, $1, $3, $5); }
	;

PostFixExpr:
	Primary
		{}
	| Name
		{}
	;

UnaryExpr:
	 '-' UnaryExpr
		{ $$ = link_tree(PR_UNARY_EXPR_NEG, "UnaryExpr_Neg", 2, $1, $2); }
	| '!' UnaryExpr
		{ $$ = link_tree(PR_UNARY_EXPR_NOT, "UnaryExpr_Not", 2, $1, $2); }
	| PostFixExpr
		{}
	;

MulExpr:
	UnaryExpr
		{}
	| MulExpr '*' UnaryExpr
		{ $$ = link_tree(PR_MUL_EXPR_MULT, "MulExpr_Mult", 2, $1, $3); }

	| MulExpr '/' UnaryExpr
		{ $$ = link_tree(PR_MUL_EXPR_DIV, "MulExpr_Div", 2, $1, $3); }
	| MulExpr '%' UnaryExpr
		{ $$ = link_tree(PR_MUL_EXPR_MOD, "MulExpr_Mod", 2, $1, $3); }
	;

AddExpr:
	MulExpr
		{}
	| AddExpr '+' MulExpr
		{ $$ = link_tree(PR_ADD_EXPR_ADD, "AddExpr_Add", 2, $1, $3); }
	| AddExpr '-' MulExpr
		{ $$ = link_tree(PR_ADD_EXPR_SUB, "AddExpr_Sub", 2, $1, $3); }
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
		{ $$ = link_tree(PR_REL_EXPR, "RelExpr", 3, $1, $2, $3); }
	;

EqExpr:
	RelExpr
		{}
	| EqExpr EQUAL RelExpr
		{ $$ = link_tree(PR_EQ_EXPR, "EqExpr", 2, $1, $3); }
	| EqExpr NOT_EQUAL RelExpr
		{ $$ = link_tree(PR_EQ_EXPR_NOT, "EqExpr_Not", 2, $1, $3); }
	;

CondAndExpr:
	EqExpr
		{}
	| CondAndExpr LOGIC_AND EqExpr
		{ $$ = link_tree(PR_COND_AND_EXPR, "CondAndExpr", 2, $1, $3); }
	;

CondOrExpr:
	CondAndExpr
		{}
	| CondOrExpr LOGIC_OR CondAndExpr
		{ $$ = link_tree(PR_COND_OR_EXPR, "CondOrExpr", 2, $1, $3); }
	;

Expr:
	CondOrExpr
		{}
	| Assignment
		{}
	;

Assignment:
	LeftHandSide AssignOp Expr
		{ $$ = link_tree(PR_ASSIGNMENT, "Assignment", 3, $1, $2, $3); }
	| LeftHandSide AssignOp
		{ $$ = link_tree(PR_ASSIGNMENT_UNARY, "AssignmentUnary", 2, $1, $2); }
	| Type VarDeclarator AssignOp Expr
		{ $$ = link_tree(PR_ASSIGNMENT_DECL, "AssignmentDeclaration", 4, $1, $2, $3, $4); }
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
