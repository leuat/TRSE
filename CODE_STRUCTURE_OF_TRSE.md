TRSE is split into several substructures
- main.cpp: main() function, sets up applicaton as gui or cli
- mainwindow: the main window of TRSE. Loads projects, manages signals and messages from subwindows to the main screen bottom info window
- Subforms that can reside in the mainwindow tab:
  - formraseditor: the main TRSE code editor form. used for .ras files. performs compilation of .ras files.
  - formimageeditor: image editor form with its toolboxes and buttons
  - formhexedit: hex editor
  - formpaw: paw editor

## Compiler overview
Breakdown of the TRSE compier: <br>
- First, we employ a lexer to convert text code to tokens
- Tokens are then parsed tokens into an abstract syntax tree (AST)
- When the AST has been generated, the program start node is visited by a AbstractCodeGen visitor that will walk through the AST and generate assembly text code output for a given CPU architecture.
- When the .asm source code has finished generating, the assembler will post-optimize away superfluous instructions.
- After the assembly text code has been optimized, OrgAsm will produce the binary machine code and save the file to disk
- Finally, the post-optimization step might perform washing/cleaning/augmenting of the produced binary code. examples:
  - C64 programs get a 2-byte header added to the start of the machine code
  - Computers with tape drives will get a tape header added
  - For computers with disk drives, TRSE will typically generate a disk with the program file + various loadable data (defined in a .paw disk file)

## Syntax
The "heart" of data in TRSE is defined in syntax.h/cpp and the corresponding data file <b>syntax.txt</b>. The syntax files contain all the keywords, constants, built-in methods and other essentials, and are loaded and parsed before compilation begins. 

 Computer systems
Another important set of helper classes are the System classes - these define the properties of each computer, or class of computers. Example: Every system must inherit from "AbstractSystem". Below Abstractsystem you have "Systemmos6502", which is the parent class for all computers using a 6502 CPU. Similarly, you have SystemZ80 etc. 6502 computers like SystemC64 and systembbc inherits from systemmos6502.

System classes contain information specific to that computer, such as program start address, memory map layouts, emulator parameters, post-processing steps (tap/disk creation) etc

## Compiler specifics

### Tokens
A token is a single unit of information in a compiler. in the statement "if a>b then"m "if" is a token of type "IF", reserved keyword, "a" is a token if type ID, while ">" is a token of type GREATER THAN.  

### Lexer
The lexer (lexer.h/cpp) breaks down a stream of raw text (code) into a stream of tokens

### parser
The parser (parser.h/cpp) creates an AST from the tokens using the Node classe. Example:

 if a>5 then 
    c:=1;
 else
    printstuff(c);

 will be converted into something like the following node structure

     [ NodeConditional ] 
              [Node(a), NodeVar(b), Token(GREATER)]
       [ True ] 
           [ AssignStatement ] 
                [NodeVar(c), NodeNumber(1)]
       [ Else ]
           [ NodeProcedure(printstuff), NodeVar(c) ]


The parser also handles all preprocessor directives, such as @ifdef @if @elseif and things like @export and other compile-time options. 

### Code generation
The Codegen visits each node in term, and outputs assembly code. All code generators must inherit from AbstractCodeGenerator. Examples of subsystems are codegen_6502 and codegen_z80

### Compiler.h/cpp
