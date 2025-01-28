# **KMSL 1.0.0**
## Overview
### What is KMSL?
KMSL (_Keyboard and Mouse Simulation Language_) is a simple, **win-platform**, dynamic, high-level interpreted programming language. Its syntax is inspired primarily by Python, with some elements borrowed from C++. KMSL was created by Pawelgit1234 in 2024–2025.
### Why use KMSL?
KMSL is designed for writing concise scripts that help automate tasks such as solving mathematical expressions, simulating mouse movements, and controlling the keyboard. It stands out for its simplicity and lightweight nature, making it an excellent choice for quickly writing automation scripts. By using KMSL, you can save significant time and effort, whether you're handling repetitive tasks or creating small, efficient programs.
## Quickstart
1. **Download the Latest Release:** Get the latest version of KMSL from the official GitHub repository: [KMSL Releases](https://github.com/Pawelgit1234/KMSL/releases/tag/Release).
2. **Open the Command Prompt:** Navigate to the directory where the KMSL binary is located.
3. **Create a File:** Create a new file named `main.kmsl` in the same directory.
4. **Write Your Code:** Open the file and add the following line:
    
    ```
    PRINT "Hello World"
    ```
    
5. **Run Your Code:** In the command prompt, execute the following command:
    
    ```
    kmsl main.kmsl
    ```
    
6. **See the Output:** You should see the following output in the command prompt:
    
    ```
    Hello World
    ```
## Syntax
### Keywords
All keywords in KMSL can be written in **lowercase and uppercase**. However, it is **recommended to use uppercase** for better readability.

```plaintext
##### Constructions #####
IF, ELSE, WHILE, FOR, BREAK, CONTINUE

##### Times #####
YEAR, MONTH, WEEK, DAY, HOUR, MINUTE, SECOND, MILLI

##### Math #####
SIN, COS, TAN, ACOS, ASIN, ATAN, ABS, RCEIL, RFLOOR, PI, E, PHI

##### Filesystem #####
READFILE, WRITEFILE, APPENDFILE, CREATEFILE, CREATEDIR, REMOVE, COPY, RENAME, EXISTS

##### Functions #####
WAIT, !!, RANDOM, OS, DO, PRINT, INPUT

##### Mouse & Keyboard #####
MOVE, DMOVE, SCROLL, TYPE, PRESS, HOLD, RELEASE, GETX, GETY, STATE,
```
### Variables
Variables in KMSL are intuitive and follow a structure similar to Python. The language supports four primary data types:
- **INT**
- **FLOAT**
- **BOOL**
- **STRING** : in single or double quotes (e.g., `'Hello World'` or `"Hello World"`)

KMSL is a dynamically typed language, meaning you do not need to explicitly declare the variable's type—it is inferred automatically.
#### Examples

```plaintext
int = 10
float = 10.0
bool = TRUE
string = 'Hello World'
```
### Operators
#### Arithmetic Operators
KMSL supports standard arithmetic operators for performing basic mathematical operations.

```plaintext
+ # Addition
- # Subtraction
* # Multiplication
/ # Division
% # Modulo
// # Floor division
** # Power
%% # Root
^^ # Logarithm

=== Assign Version ===

+= # Addition
-= # Subtraction
*= # Multiplication
/= # Division
%= # Modulo
//= # Floor division
**= # Power
%%= # Root
^^= # Logarithm
```
#### Bitwise Operators
KMSL supports operators for performing bitwise operations.

```plaintext
& # Bitwise AND
| # Bitwise OR
^ # Bitwise XOR 
~ # Bitwise NOT 
<< # Left shift
>> # Right shift

=== Assign Version ===

&= # Bitwise AND
|= # Bitwise OR
^= # Bitwise XOR 
~= # Bitwise NOT 
<<= # Left shift
>>= # Right shift
```
#### Comparison Operators
KMSL provides operators for comparing values.

```plaintext
== # Equals
!= # Not equals
< # Less than
> # Greater than
<= # Less than or equal to
>= # Greater than or equal to
```
#### Logical Operators
KMSL includes logical operators for combining conditions.

```plaintext
&& # Logical AND
|| # Logical OR
! # Logical NOT
```
#### Increment and Decrement Operators
KMSL supports operators to increment or decrement values.

```plaintext
++ # Increment by 1
-- # Decrement by 1
```

#### String Multiplication
KMSL supports string multiplication, similar to Python.

```plaintext
"Hello" * 3 # "HelloHelloHello"
```

#### Implicit Conversion Between Strings, Integers, and Floats
KMSL does not include dedicated functions for converting integers or floats to strings. Instead, it follows JavaScript's approach of implicit type conversion.

```plaintext
"3" + 3 # 33
```
### Basic Functions
#### PRINT
The `PRINT` function outputs any type of data to the console. It does not automatically add a line break after the output. 

```plaintext
PRINT 42
PRINT 3.14
PRINT TRUE
PRINT "Hello World"
```
#### INPUT
The `INPUT` function prompts the user for input and automatically converts the input to the appropriate type (int/float/string).

```plaintext
PRINT "Type a random number: "
INPUT a
```
#### RANDOM
`RANDOM` is a special variable in KMSL that returns a random number every time it is accessed.

```plaintext
PRINT RANDOM # Example output: 23214
```
#### WAIT
The `WAIT` function takes an integer or float as an argument and pauses the program for the specified duration (in seconds).

```plaintext
WAIT 3 # Waits 3 seconds  
WAIT 1.5 # Waits 1.5 seconds
```
#### !!
`!!` acts as a program termination command, similar to Python's `exit()`.

```plaintext
PRINT "Hello"  
!! # Program terminates here  
PRINT "World" # "World" will not be printed
```
#### OS
The `OS` function executes a given string as a command in the system's command prompt. It does not return any value.

```plaintext
OS "echo %cd%" # Example output: C:\...
```
#### DO
The `DO` function is inspired by Python's `eval()` but is more powerful. It can execute any code provided as a string.

```plaintext
DO "x = 3"  
PRINT x # 3
```
### Constructions
#### IF Statement
The **IF** statement in KMSL allows conditional branching.
##### Syntax

``` plaintext
IF (condition)
{
    # code block
}
ELSE # Not required
{
    # alternative code block
}
```

KMSL does not have an equivalent of Python's `elif`. To create multiple branches, nest **IF** statements inside **ELSE** blocks:

```plaintext
IF (condition)
{
    # code block
}
ELSE
{
    IF (condition2)
	{
	    # code block
	}
}
```
##### Example

```plaintext
PRINT "How old are you?"
INPUT a
IF (a < 18)
{
	PRINT "You're not old enough."
}
ELSE
{
	IF (a == 18)
	{
		PRINT "That's just enough."
	}
	ELSE
	{
		PRINT "You're more than old enough."
	} 
}
```
#### FOR Loop
The **FOR** loop in KMSL is used for iterating with a counter.
##### Syntax

```plaintext
FOR (initialization, condition, iteration)
{
    # code block
}
```

The **FOR** loop in KMSL is reminiscent of C++ syntax but **uses commas** as separators instead of semicolons. This design choice simplifies the structure and enhances readability.
##### Example

```plaintext
FOR (i = 0, i < 10, i++)
{
	PRINT i
}

# 0123456789
```
#### WHILE Loop
The **WHILE** loop executes a block of code as long as the condition is true.
##### Syntax

```plaintext
WHILE (condition)
{
    # code block
}
```
##### Example

```plaintext
WHILE (TRUE)
{
    PRINT 'Running...\n'
}
```
#### BREAK and CONTINUE
In KMSL, **BREAK** and **CONTINUE** are control flow statements that affect the execution of loops.
##### BREAK
The **BREAK** statement is used to immediately exit the loop, regardless of the condition. It stops the current iteration and exits the loop entirely.

```plaintext
WHILE (TRUE)
{
	n = 0
	PRINT n
	IF (n == 10)
	{
		BREAK
	}
	n++
}

# 0123456789
```
##### CONTINUE
The **CONTINUE** statement is used to skip the current iteration and continue with the next iteration of the loop. When **CONTINUE** is encountered, the loop does not execute the remaining code in the current iteration and jumps to the next cycle.

```plaintext
FOR (i = 0, i < 10, i++)
{
	IF (i % 2 == 0)
	{
		CONTINUE
	}
	PRINT i
}

# 13579
```

#### Positioning of Braces and Parentheses
In KMSL, the positioning of braces and parentheses is flexible, allowing developers to choose a style that best suits their preferences or improves code readability. Below are the main styles supported in KMSL, along with examples.
##### Multi-line Parentheses
When a condition in an **IF** statement or a loop is long and spans multiple lines, parentheses can be broken across lines for better readability. Each condition part should align properly for clarity.

```plaintext
IF (condition1 &&
	condition2 &&
	condition3)
	{
		# code block
	}
```
##### Flexible Brace Placement
Braces `{}` can be placed in various styles to suit your coding habits. Here are a few examples:

- **Separate lines:** Braces can be placed on their own lines for clarity (recommended).

  ```plaintext
  IF (condition)
  {
      # code block
  }
  ELSE
  {
      # alternative code block
  }
  ```

- **Inline style:** Braces can also follow the control statement directly for a compact look.

  ```plaintext
  IF (condition) {
      # code block
  } ELSE {
      # alternative code block
  }
  ```

- **Break condition alignment:** For complex conditions, parentheses and braces can be broken into multiple lines for better organization.

  ```plaintext
  IF (condition1 &&
      condition2 &&
      condition3)
  {
      # code block
  }
  ```

The choice of style is up to the developer or the team. Consistency is key, so pick a style and use it throughout your project.
### Math
#### Functions
KMSL includes several mathematical functions. Trigonometric functions work with values in radians. Below are examples with calculations:

```plaintext
PRINT SIN 1.57 # Sine of 1.57 radians (~90 degrees): 1.0
PRINT COS 1.57 # Cosine of 1.57 radians (~90 degrees): 0.0
PRINT TAN 0.785 # Tangent of 0.785 radians (~45 degrees): 1.0
PRINT ASIN 1 # Arcsine of 1 (result in radians): 1.57
PRINT ACOS 0 # Arccosine of 0 (result in radians): 1.57
PRINT ATAN 1 # Arctangent of 1 (result in radians): 0.785

PRINT ABS -7 # Absolute value of -7: 7

PRINT RCEIL 2.3 # Rounds up 2.3 to the nearest integer: 3
PRINT RFLOOR 2.9 # Rounds down 2.9 to the nearest integer: 2
```
#### Constants
KMSL provides three key mathematical constants. You can directly use these constants in your calculations or print them for reference:

```plaintext
PRINT PI #
PRINT PHI #
PRINT E # 
```
### Times
KMSL includes several predefined variables to access the current time. These variables return values based on the system's clock:

```plaintext
YEAR    # Returns the current year (e.g., 2025)
MONTH   # Returns the current month (1-12)
WEEK    # Returns the current week of the year (1-52)
DAY     # Returns the current day of the month (1-31)
HOUR    # Returns the current hour (0-23)
MINUTE  # Returns the current minute (0-59)
SECOND  # Returns the current second (0-59)
MILLI   # Returns the current millisecond (0-999)
```
### Filesystem
#### READFILE
The `READFILE` operator allows you to specify a function and a filename. It retrieves the content of the file and returns it as a string.

```plaintext
READFILE "<filename>"
```

Being a unary operator, `READFILE` can seamlessly integrate into expressions:

```plaintext
# Assuming "example.txt" contains "Hello"
PRINT READFILE "example.txt" + " World" # Outputs: "Hello World"
```

This functionality proves especially useful for organizing code across multiple files when combined with the `DO` keyword:

```plaintext
DO READFILE "example.txt"
```
#### WRITEFILE
The `WRITEFILE` operator enables you to write content to files. The first argument specifies the filename, and the second argument contains the text to write. **Note:** Using this operator overwrites any existing content in the file.

```plaintext
WRITEFILE "example.txt", "Hello World"
```
#### APPENDFILE
The `APPENDFILE` operator functions similarly to `WRITEFILE`, but instead of overwriting the file, it appends the new content to the existing data.

```plaintext
APPENDFILE "example.txt", "Hello World"
```
#### CREATEFILE
The `CREATEFILE` operator creates an empty file with the specified name.

```plaintext
CREATEFILE "example.txt"
```
#### CREATEDIR
The `CREATEDIR` operator creates a new directory with the specified name.

```plaintext
CREATEDIR "example"
```
#### REMOVE
The `REMOVE` operator deletes files or directories.

```plaintext
REMOVE "example.txt"
```
#### COPY
The `COPY` operator copies files or directories to a new location.

```plaintext
COPY "example.txt", "example2.txt"
```
#### RENAME
The `RENAME` operator renames or moves files and directories.

```plaintext
RENAME "example.txt", "example.md"
```
#### EXISTS
The `EXISTS` operator checks whether a file or directory exists. It returns a boolean value.

```plaintext
a = EXISTS "example.txt" # a is FALSE or TRUE
```
### Keyboard & Mouse
#### MOVE
The `MOVE` operator moves the cursor to a specified position.

```plaintext
MOVE 100, 100 # Instantly moves the cursor to (100, 100)
```

You can also specify a duration for the movement:

```plaintext
MOVE 100, 100, 1   # Moves to (100, 100) in 1 second
MOVE 100, 100, 1.5 # Moves to (100, 100) in 1.5 seconds
```
#### DMOVE
The `DMOVE` operator moves the cursor relative to its current position.

```plaintext
DMOVE 100, 100 # Moves to (current_x + 100, current_y + 100)
```

It also supports specifying a duration:

```plaintext
DMOVE 100, 100, 1 # Moves in 1 second
```
#### SCROLL
The `SCROLL` operator simulates scrolling with the mouse wheel.

```plaintext
SCROLL 200 # Scrolls up by 200 units
```

You can also specify a duration:

```
SCROLL -200, 1 # Scrolls down by 200 units in 1 second
```
#### TYPE
The `TYPE` operator simulates typing text.

```plaintext
TYPE "Hello World"
```

You can set a delay between typing each character:

```plaintext
TYPE "Hello World", 1 # Types with a 1-second delay between each character
```
#### PRESS
The `PRESS` operator simulates pressing one or more keys.

```plaintext
PRESS 'w'          # Presses the 'w' key
PRESS 'SHIFT', 'w' # Presses 'SHIFT' and 'w' simultaneously
```

You can specify how long the keys should remain pressed:

```plaintext
PRESS ..., ..., ..., 3 # Presses all specified keys for 3 seconds
```
#### HOLD
The `HOLD` operator simulates holding one or more keys until they are released.

```plaintext
HOLD "SHIFT", ... # Holds the specified keys
```
#### RELEASE
The `RELEASE` operator simulates releasing previously held keys.

```
RELEASE "SHIFT", ... # Releases the specified keys
```
#### GETX, GETY
`GETX` and `GETY` are variables that return the current cursor coordinates as integers.

```plaintext
x = GETX
y = GETY
```
#### STATE
The `STATE` operator checks the state of a key. It returns `TRUE` if the key is pressed, and `FALSE` otherwise.

```plaintext
IF (PRESSED "w")
{
	PRINT "Hello World"
}
```

### Comments
In KMSL, comments are written similarly to Python. Use a `#` to indicate a comment.

```plaintext
a = 3 # <comment>
```
### Errors
KMSL can generate errors when there are mistakes in your code. Below is an example of how an error is displayed in the console:

```plaintext
> a =

1 error/-s was found...

[SYNTAX_ERROR][1] Expectet another value
a =
~~^~~
```

Key components of error messages include:

1. **Error count:** `1 error/-s was found...` indicates the number of errors.
2. **Error type:** `[SYNTAX_ERROR]` specifies the type of error. KMSL recognizes three types: `[SYNTAX_ERROR]`, `[SEMANTIC_ERROR]`, and `[RUNTIME_ERROR]`. While useful for debugging, the type is often less relevant in practice.
3. **Line number:** `[1]` shows the line where the error occurred.
4. **Error description:** `Expected another value` provides details about the issue.
5. **Error position:** `^` highlights the location of the error in the code.

**Note:** Error messages might sometimes be unclear, so careful code review is recommended. ;)
## Commands
**Note:**  Ensure that `kmsl.exe` is in your directory 
### Run file
To execute a file using KMSL use the following command:

```bash
kmsl <filename>
```
### Run console
To open the KMSL interactive console, simply run:

```plaintext
kmsl
```
### Current KMSL Version
To check the current version of KMSL, use one of the following commands:

```plaintext
kmsl -v
```

or

```plaintext
kmsl --version
```
### Logger mode
KMSL includes a logger mode that can be enabled with the following commands:

```plaintext
kmsl -l
kmsl <filename> -l
```

or

```plaintext
kmsl --log
kmsl <filename> --log
```
### Help
To display a list of available commands and options, use one of the following:

```plaintext
kmsl -h
```

or

```plaintext
kmsl --h
```
## Code example
Let the cursor draw circles originating from its current position.

```plaintext
circles_count = 10
FOR (i = 0, i < 360 * circles_count, i++)
{
	angle = (i * PI / 180)
	x = RFLOOR(-COS angle * 100 + 500)
	y = RFLOOR(-SIN angle * 100 + 500)
	MOVE x, y
}
```
## Contribution
Want to improve KMSL? Here’s how you can help:

1. **Fork the Repository:** Duplicate the KMSL repository to your GitHub account.
2. **Create a Branch:** Make a new branch for your changes.

	```plaintext
	git checkout -b feature-name
	```

3. **Make and Test Changes:** Add your feature or fix, and test it to ensure it works.
4. **Commit Your Work:** Save your changes with a clear message.

    ```
    git commit -m "Add feature X or Fix bug Y"
    ```

5. **Push and Submit:** Push your branch and open a pull request with a brief description of your changes.

    ```
    git push origin feature-name
    ```

Thank you for contributing and helping to make KMSL better!
