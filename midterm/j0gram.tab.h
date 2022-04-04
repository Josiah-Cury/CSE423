/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_J0GRAM_TAB_H_INCLUDED
# define YY_YY_J0GRAM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BREAK = 258,
    DOUBLE = 259,
    ELSE = 260,
    FOR = 261,
    IF = 262,
    INT = 263,
    RETURN = 264,
    VOID = 265,
    WHILE = 266,
    CLASS = 267,
    STRING = 268,
    BOOLEAN = 269,
    CASE = 270,
    CHAR = 271,
    CONTINUE = 272,
    DEFAULT = 273,
    FLOAT = 274,
    INSTANCEOF = 275,
    LONG = 276,
    NEW = 277,
    SWITCH = 278,
    NOT_RESERVED = 279,
    PUBLIC = 280,
    STATIC = 281,
    IDENTIFIER = 282,
    INTEGER_LITERAL = 283,
    FLOAT_LITERAL = 284,
    STRING_LITERAL = 285,
    BOOLEAN_LITERAL = 286,
    NULLVAL = 287,
    CHARACTER_LITERAL = 288,
    INCREMENT = 289,
    DECREMENT = 290,
    TYPE_CAST = 291,
    LESS_EQUAL = 292,
    GREATER_EQUAL = 293,
    EQUAL = 294,
    NOT_EQUAL = 295,
    LOGIC_AND = 296,
    LOGIC_OR = 297,
    CHAR_ERROR = 298,
    INT_ERROR = 299,
    UNKNOWN = 300
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "j0gram.y"

	struct tree *treeptr;

#line 107 "j0gram.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_J0GRAM_TAB_H_INCLUDED  */
