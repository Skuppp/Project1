/*scanner.c*/

//
// Scanner for SimpleSQL programming language. The scanner reads the input
// stream and turns the characters into language Tokens, such as identifiers,
// keywords, and punctuation.
//
// YOUR NAME
// Northwestern University
// CS 211, Winter 2023
//
// Contributing author: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdbool.h>  // true, false
#include <ctype.h>    // isspace, isdigit, isalpha
#include <string.h>   // stricmp

#include "util.h"
#include "scanner.h"


//
// SimpleSQL keywords, in alphabetical order. Note that "static" means 
// the array / variable is not accessible outside this file, which is
// intentional and good design.
//
static char* keywords[] = { "asc", "avg", "by", "count", "delete", "desc", "from", "inner",
  "insert", "intersect", "into", "join", "like", "limit", "max", "min", "on", "order", 
  "select", "set", "sum", "union", "update", "values", "where" };

static int numKeywords = sizeof(keywords) / sizeof(keywords[0]);


//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of the next input sequence. 
//
void scanner_init(int* lineNumber, int* colNumber, char* value)
{
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_init)");

  *lineNumber = 0;
  *colNumber  = 0;
  value[0]    = '\0';  // empty string ""
}


//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based 
// value is returned via the "value" parameter. For example, if the 
// token returned is an integer literal, then the value returned is
// the actual literal in string form, e.g. "123". For an identifer,
// the value is the identifer itself, e.g. "ID" or "title". For a 
// string literal, the value is the contents of the string literal
// without the quotes.
//
struct Token scanner_nextToken(FILE* input, int* lineNumber, int* colNumber, char* value)
{
  if (input == NULL)
    panic("input stream is NULL (scanner_nextToken)");
  if (lineNumber == NULL || colNumber == NULL || value == NULL)
    panic("one or more parameters are NULL (scanner_nextToken)");

  struct Token T;

  //
  // repeatedly input characters one by one until a token is found:
  //
  while (true)
  {
    int c = fgetc(input);

    if (c == EOF)  // no more input, return EOS:
    {
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = '$';
      value[1] = '\0';

      return T;
    }
    else if (c == '$')  // this is also EOF / EOS
    {
      T.id = SQL_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }
    else if (c == ';')
    {
      T.id = SQL_SEMI_COLON;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }
    else if (c == '>')  // could be > or >=
    {
      //
      // peek ahead to the next char:
      //
      c = fgetc(input);

      if (c == '=')
      {
        T.id = SQL_GTE;
        T.line = *lineNumber;
        T.col = *colNumber;

        value[0] = '>';
        value[1] = '=';
        value[2] = '\0';

        return T;
      }

      //
      // if we get here, then next char did not form a token, so 
      // we need to put char back to be processed on next call:
      //
      ungetc(c, input);

      T.id = SQL_GT;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = '>';
      value[1] = '\0';

      return T;
    }
    else
    {
      //
      // if we get here, then char denotes an UNKNOWN token:
      //
      T.id = SQL_UNKNOWN;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    }

  }//while

  //
  // execution should never get here, return occurs
  // from within loop
  //
}
