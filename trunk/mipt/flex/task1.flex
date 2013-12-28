%{

#include <iostream>
#include <algorithm>
#include <stack>
#include <stdexcept>
using namespace std;

stack<string> expressionStack;

%}

ALPHA [a-f|1][a-f|1]*
BLANK [ \t\n]
UNION "+"
CONCAT "."
ITERATE "*"

ILLEGAL [^{CONCAT} ^{BLANK} ^{UNION} ^{CONCAT} ^{ITERATE}] 

%%

{BLANK}+  {
          }

{ALPHA}+  {
            string str = yytext;

            str.erase( remove_if(str.begin(), str.end(), [] (char c) { return c == '1'; }), str.end() );

            expressionStack.push(str);
          }

{UNION}   {
            if (expressionStack.size() < 2)
            {
              throw std::logic_error("Can't apply union. There is only 1 arg on stack.");
            }
            string s1 = expressionStack.top();
            expressionStack.pop();
            string s2 = expressionStack.top();
            expressionStack.pop();
            if (s1 > s2)
              swap(s1, s2);
            expressionStack.push(s1);
          }

{CONCAT}  {
            if (expressionStack.size() < 2)
            {
              throw std::logic_error("Can't apply concatenation. There is only 1 arg on stack.");
            }
            string s1 = expressionStack.top();
            expressionStack.pop();
            string s2 = expressionStack.top();
            expressionStack.pop();
            expressionStack.push(s2 + s1);
          }

{ITERATE} {
            if (expressionStack.size() < 1)
            {
              throw std::logic_error("Can't apply iterate. There is only 0 arg on stack.");
            }
            string s1 = expressionStack.top();
            expressionStack.pop();
            expressionStack.push("");

            /* we want to  minimize it, so skip it*/
          }

{ILLEGAL}  {
              throw std::logic_error(string("Illegal characters: ") + string(yytext));
            }


%%
int main()
{
  yylex();
  if (expressionStack.size() != 1)
  {
    throw std::logic_error("Incorrect expression, got " + to_string(expressionStack.size()) + " params");
  }
  string s = expressionStack.top();
  expressionStack.pop();
  cout << s << endl;
  return 0;
}

