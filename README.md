# LongBow
#### Transpiled Language

### No In Depth Testing Has Occured.
### Language Does Not Check Correctness Of Written Code

### TODO:
- Make A Function To Check The Format Of The Last Few Statements Added To The Statement Stack
  - This Would Help Check For Correctness
  
- Make Code Easier To Read
- Document More Code
- Finish New Parser Then Delete FileReader Directory
- Code Generation
- Compile Time Checks
- Abstract Value Evaluater
  - Ex:
```C
(1,2,3,(1..10)..20+1)
```

### Currently
- New Parsing System
- DataStage
  - The Data Stage Is Basically A Expression Parser (I Must Have Felt Quirky When I Named It)
  - It Activates Before The Tokenizer For Some Reason?
  - Kind Of Hackable (Via Function Pointers)
  - TODO: Make Tokenization Occur Before
