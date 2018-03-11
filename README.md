# Digestive

Work-in-progress implementation of a highly interactive JIT compiler focused on incremental compilation. Currently supporting little of planned functionality.

### Build & Setup
Building and installing the binary:
```shell
git clone --recurse-submodules https://github.com/williamsjoblom/compiler-experiment.git
cd compiler-experiment

# Build
mkdir build
cd build
cmake ../
make

# Install
ln -s $(pwd)/dg /usr/bin/
cd ..
echo "export DGROOT=$(pwd)/dg/" > ~/.bashrc # Replace '.bashrc' if using shell different from bash.
```

The compiler can now be called using 

### Examples
```swift
fun fib(n : i32) -> i32 {
    if (n <= 1) return n;
    if (n == 2) return 1;
    return fib(n - 1) + fib(n - 2);
}

pln fib(10);
```
Example that prints the 10th fibonacci number.

```swift
fun printTriple(v : (i32, i32, i32)) {
    pln v.0; pln v.1; pln v.2;
}

var triple := (13, 14, 15);
printTriple(triple);
```
Example that prints the values of a tuple.

### Interaction
One key features is the ability to reload code at runtime. Such interaction with the compiler VM is done by writing commands to a named pipe called `interact` located in the current working directory.

#### Example
```shell
mkfifo interact # Create named pipe for talking to the compiler VM.
dg ~/test.dg    # Run your program (assuming 'dg' is the compiler executable).

nano ~/test.dg  # Make changes to the source of the running program...

echo reload ~/test.dg > interact # Tell the compiler VM to apply your source changes to the running program.

# The running program will now have the recent source changes applied!
```

One limitation is that only function additions and changes will be incorporated in the running program when reloading a source file. This means that top-level changes to the program will not be incorporated when reloading.


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
```swift
var x := 42;
```
The type of `x` will be infered from its assigned value.
##### Without type inference:
```swift
var x : i32 = 42;
```
`x` will be of type `i32`.
### Function Declarations:
Functions are declared using the following syntax:
```swift
'fun' <type> ['->' <type>]? '{' <stmt>* '}'
```
#### Examples
##### Without parameters and return values:
```swift
fun f() { }
```
##### Single return value:
```swift
fun f() -> i32 { return 42; }
```
Return values are specified using the `->` syntax. The `return` keyword is used to exit the function and in this case return the value '42'.

##### Single parameter:
```swift
fun f(x : i32) { pln x; }
```
Since this function only takes a single element tuple it can be called with (`f(42);`) or without (`f 42;`) parantheses. These two are equivalent.

##### Multiple parameters:
```swift
fun f(x : i32, y : i32) { pln x + y; }
```
Theoretically this function only takes a single argument (which in this case is a tuple of type `(i32, i32)`). Since the argument is a tuple with more than one element paranthesis can not be omitted when calling it!
