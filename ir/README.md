# TAC Operations (TACC)

Documentation for the three address code used as an intermediate representation. This intermediate representation allow easier implementation of backends for different CPU architectures and fancy optimizing transformations yielding blazing fast machine code.

## Arithmetics

#### Add `add <s0> <s1> <d>`:
`d := s0 + s1`

#### Subtract `sub <s0> <s1> <d>`:
`d := s0 - s1`

#### Multiply `mul <s0> <s1> <d>`:
`d := s0 * s1`

#### Divide `div <s0> <s1> <d>`:
`d := s0 / s1`

## Comparisons

#### Compare Equal `cmpEQ <s0> <s1> <d>`
Set `d` to 1 if `s0 == s1` otherwise 0.

#### Compare Not Equal `cmpNE <s0> <s1> <d>`
Set `d` to 1 if `s0 != s1` otherwise 0.

#### Compare Greater `cmpG <s0> <s1> <d>`
Set `d` to 1 if `s0 > s1` otherwise 0.

#### Compare Less `cmpL <s0> <s1> <d>`
Set `d` to 1 if `s0 < s1` otherwise 0.

#### Compare Greater Equal `cmpGE <s0> <s1> <d>`
Set `d` to 1 if `s0 >= s1` otherwise 0.

#### Compare Less Equal `cmpLE <s0> <s1> <d>`
Set `d` to 1 if `s0 <= s1` otherwise 0.

## Branching

#### Set Argument `setArg <s0> _ <d>`:
Set argument indexed by `d` to `s0`.

#### Call `call <s0> _  _`:
Jump to subroutine specified by `s0` (with args set by `setArg`).

#### Jump `jmp <s0> _ _`:
Jump to address specified by `s0`.
 
### Conditional Branching

#### Jump Zero `jmpZ <s0> <s1> _`:
Jump to address specified by `s0` if `s1` is zero.

#### Jump Not Zero `jmpZ <s0> <s1> _`:
Jump to address specified by `s0` if `s1` is not zero.

#### Jump Negative `jmpZ <s0> <s1> _`:
Jump to address specified by `s0` if `s1` is negative.

## Memory

#### Move `move <s0> _ <d>`:
`d := s0`
