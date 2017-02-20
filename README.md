Work-in-progress implementation of a highly interactive JIT compiler focused on incremental compilation and self modifying code.

### Example
```c
func fib(n) {
    if (n <= 2) return 1;
    return fib(n - 1) + fib(n - 2);
}

pln fib(10);
```
Example that prints the 10th fibonacci number.