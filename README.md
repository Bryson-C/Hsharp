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


Int a = 10;
Int b = 10,20,30,40;
Int c = 20;
Int d = b;
Int e = a + c + a;
// does not work
// TODO: Append variable to array
Int arrayPlusInt = b + c;


// Tests Of String Variables And Generation
String str = "Hello World";
String hello = "Hello";
String world = "World";
// TODO: Create Larger String From 2 Strings
String greeting = hello + world;
String combine = "Hello","World";


// TODO: Indexing
// TODO: Functions
 
```
### Output
```C++
long long int a = 10 ;
long long int b[4] = {10 ,20 ,30 ,40 };
long long int c = 20 ;
long long int d[4] = {10 , 20 , 30 , 40 };
long long int e = 10 + 20 + 10 ;
long long int arrayPlusInt[4] = {10 , 20 , 30 , 40 }+ 20 ;
const char* str = "Hello World" ;
const char* hello = "Hello" ;
const char* world = "World" ;
const char* greeting = "World  ";
const char* combine[2] = {"Hello" ,"World" };

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

