Work-in-progress implementation of a highly interactive JIT compiler focused on incremental compilation. Currently supporting little of planned functionality.

### Example
```c
func fib(n : i32) -> i32 {
    if (n <= 1) return n;
    if (n == 2) return 1;
    return fib(n - 1) + fib(n - 2);
}

pln fib(10);
```
Example that prints the 10th fibonacci number.

## Language

### Types
All types are tuple-types, even primtives. Primitive expressions are treated as tuples with a single element. 

#### Primitives
| Name          | Description               |
| ------------- |---------------------------|
| `i16`         | 16 bit signed integer     |
| `i32`         | 32 bit signed integer     |
| `i64`         | 64 bit signed integer     |
| `n8`, `byte`  | 8 bit unsigned integer    |
| `n16`         | 16 bit unsigned integer   |
| `n32`         | 32 bit unsigned integer   |
| `n64`         | 64 bit unsigned integer   |

Numeric literals are treated as 32 bit signed integers unless another type is explicitly specified (Numeric literals will get implicitly casted when assigned to a variable if needed).

#### Tuples
##### Type syntax:
```c
<type>
'('<type>[',' <type>]*')'
```
##### Literal syntax:
```c
<expr>
'('<expr>[',' <expr>]*')'
```

### Variable Declarations
##### Syntax:
```c
'var' <identifier> ':' <type>
'var' <identifier> ':' <type>? '=' <expr>
```
#### Examples
##### With type inference:
```c
var x := 42;
```
The type of `x` will be infered from its assigned value.
##### Without type inference:
```c
var x : i32 = 42;
```
`x` will be of type `i32`.
### Function Declarations:
Functions are declared using the following syntax:
```c
'func' <type> ['->' <type>]? '{' <stmt>* '}'
```
#### Examples
##### Without parameters and return values:
```c
func f() { }
```
##### Single return value:
```c
func f() -> i32 { return 42; }
```
Return values are specified using the `->` syntax. The `return` keyword is used to exit the function and in this case return the value '42'.

##### Single parameter:
```c
func f(x : i32) { pln x; }
```
Since this function only takes a single element tuple it can be called with (`f(42)`) or without (`f 42`) parantheses. These two are equivalent.

##### Multiple parameters:
```c
func f(x : i32, y : i32) { pln x + y; }
```
Theoretically this function only takes a single argument (which in this case is a tuple of type `(i32, i32)`). Since the argument is a tuple with more than one element paranthesis can not be omitted when calling it!







