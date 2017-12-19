# TAC Operations (TACC)

Documentation for the three address code used as an intermediate representation. This intermediate representation allow easier implementation of backends for different CPU architectures and fancy optimizing transformations yielding blazing fast machine code.

## Arithmetics

#### Add `add <dest> <op1> <op2>`:
`dest := op1 + op2`

#### Subtract `sub <dest> <op1> <op2>`:
`dest := op1 - op2`

#### Multiply `mul <dest> <op1> <op2>`:
`dest := op1 * op2`

#### Divide `div <dest> <op1> <op2>`:
`dest := op1 / op2`

## Comparisons

#### Compare Equal `cmpEQ <dest> <op1> <op2>`
Set `dest` to 1 if `op1 == op2` otherwise 0.

#### Compare Not Equal `cmpNE <dest> <op1> <op2>`
Set `dest` to 1 if `op1 != op2` otherwise 0.

#### Compare Greater `cmpG <dest> <op1> <op2>`
Set `dest` to 1 if `op1 > op2` otherwise 0.

#### Compare Less `cmpL <dest> <op1> <op2>`
Set `dest` to 1 if `op1 < op2` otherwise 0.

#### Compare Greater Equal `cmpGE <dest> <op1> <op2>`
Set `dest` to 1 if `op1 >= op2` otherwise 0.

#### Compare Less Equal `cmpLE <dest> <op1> <op2>`
Set `dest` to 1 if `op1 <= op2` otherwise 0.

## Branching

#### Set Argument `setArg <dest> <op1> _`:
Set argument indexed by `dest` to `op1`.

#### Call `call _ <op1> _`:
Jump to subroutine specified by `op1` (with args set by `setArg`).

#### Jump `jmp _ <op1> _`:
Jump to address specified by `op1`.

### Conditional Branching

#### Jump Zero `jmpZ _ <op1> <op2>`:
Jump to address specified by `op1` if `op2` is zero.

#### Jump Not Zero `jmpZ _ <op1> <op2>`:
Jump to address specified by `op1` if `op2` is not zero.

#### Jump Negative `jmpZ _ <op1> <op2>`:
Jump to address specified by `op1` if `op2` is negative.

## Memory

#### Move `move <dest> <op1> _`:
`dest := op1`
