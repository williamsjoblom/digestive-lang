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
Tuple types are declared using the following syntax:
```c
<type>
'('<type>[',' <type>]*')'
```
The same syntax applies to tuple literals:
```c
<expr>
'('<expr>[',' <expr>]*')'
```

### Variable Declarations
Variables are declared using the following syntax:
```c
'var' <identifier> ':' <type>
'var' <identifier> ':' <type>? '=' <expr>
```
#### Examples
With type inference (the type of `x` will be infered from its assigned value):
```c
var x := 42;
```
Without type inference (`x` will be of type `i32`)
```c
var x : i32 = 42;
```

### Function Declarations
Functions are declared using the following syntax:
```c
'func' <type> ['->' <type>]? '{' <stmt>* '}'
```
#### Examples
Function without parameters and return values:
```c
func f() { }
```
Function with a single parameter:
```c
func f(x : i32) { pln x; }
```
Since this function only takes a single element tuple it can be called with (`f(42)`) or without (`f 42`) parantheses. These two are equivalent.

##### Multiple parameters
```c
func f(x : i32, y : i32) { pln x + y; }
```
Theoretically this function only takes a single argument (which in this case is a tuple of type `(i32, i32)`). Since the argument is a tuple with more than one element paranthesis can not be omitted when calling it!







