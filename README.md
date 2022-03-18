<img src="/logo-banner.png" width="500" />

# wx
A programming language written in C as a side project, heavily helped by the amazing tutorials from [Sebastian Karlsson](https://www.youtube.com/c/HelloWorldCode/videos).

##### Feature Progress
- [x] Lexer
    - [x] Tokenizer
- [x] Parser
    - [x] Abstract Syntax Tree creation
- [ ] Compiler (In Progress)
    - [x] Read from file or string
    - [ ] Recursive descent assembler

- [ ] Additional goals
    - [ ] Further syntax implementation (lexer, parser, compiler)
    - [ ] Transpile to C (maybe?)

#### Syntax
The goal of wx is to provide low level access to data while utilizing some of my personal favorite syntactic sugars.
- General
    - Dynamic typing, but control over size as needed.
    - Newlines used to terminate each line of code.
- Functions
    - Definition with keyword "function".
    - Code is run sequentially, and then based on function calls, meaning no main function is needed.
...and more to come as I implement it!
