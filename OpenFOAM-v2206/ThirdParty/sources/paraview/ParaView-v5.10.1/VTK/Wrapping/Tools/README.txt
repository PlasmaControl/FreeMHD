Notes on usage of LEX and YACC to generate VTK parse files
----------------------------------------------------------

The contents of this directory are used to generate a C library called
vtkWrappingTools, which provides utility functions for parsing C++ header
files.  The core of the wrapping tools is a parser that is built using
the classic compiler-generator tools lex and yacc.  These tools are
available on macOS and on most linux systems.


LEX:
----

The file vtkParse.l contains regular expressions for tokenizing a C++
header file.  It is used to generate the file lex.yy.c, which is directly
included (i.e. as a C file) by the main parser file, vtkParse.tab.c.

To generate lex.yy.c from vtkParse.l, use the following steps:

1. Get a copy of flex, version 2.6.4 or later
2. Run flex --nodefault --noline -olex.yy.c vtkParse.l
3. Edit the file lex.yy.c as follows, to eliminate compiler warnings and
   to make it pass the git commit hook tests:
a) Convert tabs to 2 spaces, e.g. :%s/\t/  /g
b) Remove extra whitespace from the ends of lines, e.g. :%s/  *$//
c) Remove blank lines at the beginning and end of the file
d) Remove the definition of "struct yy_trans_info", which is used nowhere
   and will cause a compiler warning.
e) Add the following code (around line 24) to avoid warnings about
   isatty() being used without a declaration:

#ifndef __cplusplus
extern int isatty(int);
#endif /* __cplusplus */


YACC:
-----

The file vtkParse.y contains the rules for parsing a C++ header file.
Many of the rules in this file have the same names as in description
of the grammar in the official ISO standard.  The file vtkParse.y is
used to generate the file vtkParse.tab.c, which contains the parser.

1. Get a copy of bison 3.2.3 or later, it has a yacc-compatible front end.
2. Run bison --no-lines -b vtkParse vtkParse.y, to generate vtkParse.tab.c
3. Edit the file vtkParse.tab.c as follows, to eliminate compiler warnings
   and to make it pass the git commit hook tests:
a) replace all instances of "static inline" with "static"
b) replace "#if ! defined lint || defined __GNUC__" with "#if 1"
c) remove YY_ATTRIBUTE_UNUSED from yyfillin, yyfill, and yynormal
d) remove the "break;" after "return yyreportAmbiguity"
e) replace "(1-yyrhslen)" with "(1-(int)yyrhslen)"
f) replace "sizeof yynewStates[0] with "sizeof (yyGLRState*)"
g) replace "sizeof yynewLookaheadNeeds[0] with "sizeof (yybool)"

When bison is run, it should not report any shift/reduce or reduce/reduce
warnings.  If modifications to the rules cause these warnings to occur,
you can run yacc with the --debug and --verbose options:
 bison --debug --verbose -b vtkParse vtkParse.y
This will cause bison to produce a file called "vtkParse.output" that
will show which rules conflict with other rules.

The rules in vtkParse.y avoid most of the ambiguities that are present
in the official C++ grammar by only parsing declarative statements.
Non-declarative statements, such the contents of function bodies, are
simply ignored using the rule "ignore_items".  Constant expressions,
which appear in declarative statements as default argument values or
enum values, are parsed by the rule "constant_expression" which simply
copies the expression into a string without attempting to evaluate it.
