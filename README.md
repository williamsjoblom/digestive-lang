Work-in-progress implementation of a highly interactive JIT compiler focused on incremental compilation and self modifying code. Currently supporting little of planned functionality.

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

### Variable Declarations
With type inference (the type of `x` will be infered from its assigned value):
```
var x := 42;
```
Without type inference
```
var x : i32 = 42;
```
### Types

#### Primitive Types
| Name          | Description               |
| ------------- |---------------------------|
| `i16`         | 16 bit signed integer     |
| `i32`         | 32 bit signed integer     |
| `i64`         | 64 bit signed integer     |
| `n8`, `byte`  | 8 bit unsigned integer    |
| `n16`         | 16 bit unsigned integer   |
| `n32`         | 32 bit unsigned integer   |
| `n64`         | 64 bit unsigned integer   |

#### Tuple Types
