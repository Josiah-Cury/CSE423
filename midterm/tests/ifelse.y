%token IF ELSE EXPR OTHERSTMT
%%
Start: IfThenStmt | IfThenElseStmt | IfThenElseIfStmt ;

IfThenStmt: IF '(' Expr ')' Block ;
IfThenElseStmt: IF '(' Expr ')' Block ELSE Block ;

IfThenElseIfStmt: IF '(' Expr ')' Block ElseIfSequence ELSE Block ;

ElseIfSequence: ElseIfStmt | ElseIfSequence ElseIfStmt ;
ElseIfStmt: ELSE IfThenStmt ;

Block: '{' Statements '}' | Statement ;
Statements: | Statements Statement ;
Statement: IfThenStmt | IfThenElseStmt | OTHERSTMT ';' ;

Expr: EXPR;

%%
