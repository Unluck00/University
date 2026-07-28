/****************************************** 
Lecture 1
Basics: methods, assertions, functions,
        pre and post-conditions
******************************************/

/* In Dafny a method is a procedure; it may contain several
   expressions and statements; in particular it may contain
   commands like assignments:

   <varname> := <expression>;

   Variables and expressions are typed; they are declared in the
   signature of the method and within the block of the method body.

   Other commands are conditionals or case selections, iterations
   (just while) and method calls.

   Commands are state transformers, assertions are predicates
   of the state. A predicate is a boolean valued expression 
   that may contain logical connectives and quantifiers.

   The assert statement:

   assert <predicate>;

   has not the effect of blocking the execution and rising an excpetion
   in case of failure (as for similar statements e.g. in the assert 
   library of C). It is a proof obbligation: Dafny is asked to 
   prove that it must be true whenever the control reaches that 
   assert statement.

*/

method SquareDiff(x: real, y: real) returns (z: real)
{
    var u : real := (x + y) * (x - y); // local variable
    // the type real for u can be omitted as it is deduced 
    // from the type of (x + y) * (x - y)
    z := u;
    // redundant writing: it suffices
    // z := (x + y) * (x - y);
    // or equivalently
    // return (x + y) * (x - y);
    assert z == x * x - y * y; 
    // this has to be proved to hold for any possible execution 
    // of the method when it reaches this point
}

// More on assertions and execution paths

/* A sequence of program points that can be reached in some exection
   is called an execution path, or just a path. Each command in the path
   may affect the state, which is a mapping associating values to (visible)
   variables.
   An assertion is checked to hold of any state obtained by an initial one,
   we shall call the pre-state of the method, after the modifications induced
   by the previous commands in the path that leads to it.
*/

method Triple(x: int) returns (r: int) {
    var y := 2 * x;
    // x := 0; error: input var. x is immutable
    r := x + y;
}


method Triple1(x: int) returns (r: int) {
    var y := 2 * x;
    r := x + y;
    assert r == 3 * x; // proof obligation
}

/*
method Triple2(x: int) returns (r: int) {
    var y := 2 * x;
    assert false;
    r := x + y;
    assert r == 3 * x + 1; // it holds since this point is unreachable
}
*/

/*
method Triple3(x: int) returns (r: int) {
    var y := 2 * x;
    r := x + y;
    assert r == 10 * x; // error: does not hold in general, but only if x == 0
    assert r < 5; // it holds becouse this point is reached when x == 0
    assert false; // error: cannot hold ever
}
*/

// Pre and post-conditions: contracts

/* Pre and post-conditions are assertions associated to a method
   using the keywords requires and ensures respectively.

   The pre-condition is an hypothesis on the pre-state, namely
   on the values of the method parameters. If the pre-condition
   fails of the pre-state then the method verifies trivially;
   otherwise the method is expected to terminate with a post-state
   that satisfies the post-condition.

   The pair of pre and post-conditions is called a contract.
   The metaphor is that the pre-condition formalizes the rights
   of the method, while the post-condition its duties.

*/

method Triple4(x: int) returns (r: int) 
    requires x % 2 == 0 
    ensures  r == 3 * x
{
    var y := x / 2; // integer division is rounded
    r := 6 * y;
}

// if the pre-condition is empty, or equivalently it is true,
// it can be omitted
method SquareDiff'(x: real, y: real) returns (z: real)
    ensures z == x * x - y * y
{
    z := (x + y) * (x - y);
}

// Functions

/* Functions consist of just one expression and are "pure" namely
   cannot affect the state. They can be used both to calculate
   values and to specify methods behaviour in assertions.
*/

function Abs(x: int): int
    ensures Abs(x) >= 0
    ensures Abs(x) == x || Abs(x) == - x
    // multiple ensures clauses are interpreted as the 
    // conjunction of the respective predicates 
    // (similarly for multiple requires)
{
    if x < 0 then - x else x
}

method AbsMet(x: int) returns (y: int)
    ensures y == Abs(x)
{
    if x < 0 {y := - x; }
    else { y := x; }
    // differently than in case of expressions,
    // the command if-then-else 
    // requires using blocks {...}
}

// Using assertions methods can be tested without actually running.
method TestAbs()
{
    var r := - 123;
    ghost var p := r;
    r := AbsMet(r);
    assert r == - p;
}

// A more substantial example (to do)