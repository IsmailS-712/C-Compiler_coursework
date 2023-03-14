%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"


// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


%%

[-]?([0-9]+|[0-9]+\.[0-9]*|[0-9]+\/[0-9]+) { fprintf(stderr, "Number : %s\n", yytext);
                                             std::string yytext2 = std::string(yytext);
                                             if (yytext2.find("/") == std::string::npos) {
                                                yylval.numberValue = std::stod(yytext); 
                                             }
                                             else {
                                                size_t position = yytext2.find("/");
                                                std::string numerator = yytext2.substr(0, position);
                                                std::string denominator = yytext2.substr(position+1);
                                                double fraction = std::stod(numerator)/std::stod(denominator); 
                                                yylval.numberValue = fraction;
                                             }
                                             return Number; 
                                          }

[a-zA-Z]+|\[[\000-\134\136-\377]*\]   { fprintf(stderr, "Word : %s\n", yytext); 
                                       std::string yytext3 = std::string(yytext);
                                       if (yytext3[0] == '[') {
                                          std::string value1 = yytext3.substr(1, yytext3.length()-2);
                                          yylval.wordValue = new std::string(value1);
                                       }
                                       else {
                                          yylval.wordValue = new std::string(yytext);
                                       } 
                                       return Word; }

\n              { fprintf(stderr, "Newline\n"); }

[ \t\r]         { fprintf(stderr, "Whitespace\n"); }

.               { fprintf(stderr, "Stray char\n"); }

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
