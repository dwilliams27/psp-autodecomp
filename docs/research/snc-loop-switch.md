# SNC Loop and Switch Codegen

Reverse-engineered from SNC compilation experiments (2026-04-14, 69 tool uses).

## Loops

### No Unrolling
SNC does NOT unroll loops at any optimization level (-O2 through -O5). Tested with:
- Simple counter loops
- Array traversal
- Linked list traversal
All produce single-iteration loop bodies.

### -O2 through -O5 Are Identical
For all tested loop patterns, -O2/-O3/-O4/-O5 produce byte-identical output.

### Loop Structure
- `while(cond)`: pre-test at top, branch to exit, body, branch back to test
- `do-while(cond)`: body first, test at bottom, branch back to body
- `for(;;)`: equivalent to `while` with init before loop

### while vs do-while
These produce DIFFERENT code structures:
```cpp
while(p) { f(p); p = p->next; }     // test at top
do { f(p); p = p->next; } while(p); // test at bottom
```
The binary tells you which was used — check if the loop test is before or after the body.

## Switch Statements

### Strategy by Case Count
- **2-5 cases**: Binary tree of if/else comparisons
- **5-7 cases**: Linear scan (sequential comparisons)
- **8+ cases**: Jump table (indexed branch)

### Jump Table Pattern
```asm
sltiu $v0, $a0, N      // bounds check
beqz  $v0, default      // if out of range, go to default
sll   $v0, $a0, 2       // index * 4
lui   $at, %hi(table)
addu  $at, $at, $v0
lw    $v0, %lo(table)($at)
jr    $v0               // jump to case
```

### Dense vs Sparse
- Dense cases (0,1,2,3,4): jump table
- Sparse cases (0,5,100,200): binary tree comparisons
- SNC decides based on the "density" of case values

### Default Case Position
The `default:` case position in source doesn't affect codegen — SNC always puts the default handler after all explicit cases.

## Linked List Traversal

The idiomatic game engine pattern:
```cpp
Node *node = list_head;
while (node != NULL) {
    process(node);
    node = node->next;
}
```
Produces `bnezl` (branch-likely) with next-field preload in delay slot. See `snc-branch-likely.md` pattern 4.

## Implications for Matching

1. Never expect loop unrolling — write loops that iterate once per element
2. Check the binary for pre-test (while) vs post-test (do-while) structure
3. For switch statements, count the cases to predict binary tree vs jump table
4. Linked list traversals should use `while(node)` with early member access
