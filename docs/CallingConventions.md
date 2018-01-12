# Digestive Calling Conventions

### Argument passing
All functions expect a single argument. This is due to every expression in Digestive being a tuple. This means that when calling a function with "multiple arguments" it is actually a single tuple containing all the arguments being passed.


When passing a tuple the values inside will be passed as separate arguments according to the `cdecl` convention.

### Returns
If the return value is a tuple of length 1 (i.e. a single primitive) it will be returned using the `cdecl` convention. 

If the return value is a tuple of length greater than 1 a pointer to the tuples location in memory will be returned instead. Because this pointer may refer to stack memory above the stack pointer the tuple is copied to the callers stack frame by the caller when the callee has returned to avoid unwanted overwrites.
