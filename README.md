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
