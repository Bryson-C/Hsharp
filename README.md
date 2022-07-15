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
Void sub [ Int a | Int b ];

```
### Output
```C++
#include <stdint.h>
 int64_t a = 10 ;
 int64_t b [] = { 10 , 20 , 30 , 40 };
 int64_t c ;
 const char* Str = "hello world" ;
 void add ( int64_t a , int64_t b ) {}
 void sub ( int64_t a , int64_t b ) ;
 
```

