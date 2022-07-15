# LongBow
Transpiled Language

### No In Depth Testing Has Occured.
### Language Does Not Check Correctness Of Written Code

### TODO:
- Make A Function To Check The Format Of The Last Few Statements Added To The Statement Stack
  - This Would Help Check For Correctness
  
- Make Code Easier To Read
- Document More Code

### Input
```C

// Variable With Type `Int` And Value Of `10`
Int a = 10;
Int b = 10,20,30,40;
Int c;

String Str = "hello world";

Void add [ Int a | Int b ] {}
Void sub [ Int a | Int b ] {}

```
### Output
```C++
#include <stdint.h>
 int64_t a = 10 ;
 int64_t b [ 4 ]  = { 10, 20, 30, 40 };
 int64_t c ;
 const char* Str = "hello world" ;
 void add ( int64_t a , int64_t b ) {}
 void sub ( int64_t a , int64_t b ) {}
 
```
### Block Visualization
```
Block:
  | VAR_TYPE: 'Int'
  | CUSTOM_STRING: 'a'
  | INITIALIZER: '='
  Child:
    | DIGIT: '10'

Block:
  | VAR_TYPE: 'Int'
  | CUSTOM_STRING: 'b'
  | INITIALIZER: '='
  Child:
    | DIGIT: '10'
    | COMMA: ','
    | DIGIT: '20'
    | COMMA: ','
    | DIGIT: '30'
    | COMMA: ','
    | DIGIT: '40'

Block:
  | VAR_TYPE: 'Int'
  | CUSTOM_STRING: 'c'
  | END_STATEMENT: ';'

Block:
  | VAR_TYPE: 'String'
  | CUSTOM_STRING: 'Str'
  | INITIALIZER: '='
  Child:
    | CUSTOM_STRING: '"hello world"'

Block:
  | VAR_TYPE: 'Void'
  | CUSTOM_STRING: 'add'
  | OPEN_ARRAY: '['
  | CLOSE_ARRAY: ']'
  Child:
    | VAR_TYPE: 'Int'
    | CUSTOM_STRING: 'a'
    | STRAIGHT_BRACKET: '|'
    | VAR_TYPE: 'Int'
    | CUSTOM_STRING: 'b'

Block:
  | VAR_TYPE: 'Void'
  | CUSTOM_STRING: 'sub'
  | OPEN_ARRAY: '['
  | CLOSE_ARRAY: ']'
  Child:
    | VAR_TYPE: 'Int'
    | CUSTOM_STRING: 'a'
    | STRAIGHT_BRACKET: '|'
    | VAR_TYPE: 'Int'
    | CUSTOM_STRING: 'b'

```

