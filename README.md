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
long long int arrayPlusInt[4] = {10 , 20 , 30 , 40 }+ 20 ; // <- Does Not Function Correctly (Needs To Append 20 To Array)
const char* str = "Hello World" ;
const char* hello = "Hello" ;
const char* world = "World" ;
const char* greeting = "World  "; // <- Does Not Function Correctly
const char* combine[2] = {"Hello" ,"World" };

```

## Macros


### Macro
```C++
#Int [x = v] => int $ = $;
```
### Input
```C++
Int a = 10;
Int b = 20;
```
### Output
```C++
int a = 10 ;
int b = 20 ;
```

