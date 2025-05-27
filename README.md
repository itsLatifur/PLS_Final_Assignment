
# Recursive Descent Parser – CSC 461 Assignment

**Course:** CSC 461 – Programming Languages and Structures  
**Semester:** Spring 2025  
**Student Name:** Md. Latifur Rahman  
**Student ID:** 22103280  
**Instructor:** Md Nazir Ahmed, Lecturer, Dept. of CSE, IUBAT  

---

## Project Title
Recursive Descent Parser for Control Structures and Nested Expressions

## Objective
This C program implements a Recursive Descent Parser that validates control structures (`if`, `else`, `while`) and arithmetic expressions with nested brackets. It replaces the special token `LTD` with the last three digits of the student’s ID – `280`.

---

## Features
- Tokenizes keywords, symbols, operators, identifiers, numbers, and `LTD`
- Parses nested control blocks recursively
- Validates correct syntax and reports detailed errors
- Replaces `LTD` with `280` at runtime
- Error handling for missing semicolons, brackets, invalid identifiers, etc.

---

## Compilation Instructions
To compile the program using GCC:
```bash
gcc parser.c -o parser
```

## Execution Instructions
To run the program:
```bash
./parser
```
Note: When you run the file, after typing or pasting the input you must add a new line that contains only `~` and then click `Enter`

---

## Example Valid Input

```c
{
    if (a == LTD) {
        while (b < 100) {
            x = (a + b) * LTD;
        }
    } else {
        z = x - 1;
    }
}
```

### Output:
```
Program parsed successfully.
```

## Example Invalid Input
```c
{
    x = 5
}
```
### Output:
```
Syntax Error: Expected token ;, got }
```

---

## Test Cases
Refer to the included test case file for at least 5 valid and 5 invalid cases.

---

## 📌 Note
- LTD is dynamically replaced by `280` during expression evaluation.
- Make sure to input correct syntax to avoid parse errors.
