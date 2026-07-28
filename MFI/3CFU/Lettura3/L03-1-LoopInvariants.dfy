/****************************************** 
Lecture 3-1 - Loop invariants

******************************************/

/*  Loop invariants

    Control structures for loops, like while commands in the case of IMP, are
    necessary in actual programming and unavoidable in any Turing complete
    programming language, which is the case of IMP.
    However, reasoning backword with wp-predicate transformers is 
    not effective any more when considering loops because of two issues.
    First, the truth of at least one state of the predicate wp(while b do S, Q) 
    implies termination in that state, which cannot be garanteed in general, 
    and it is undecidable in principle.
    Second, given that we know that the execution of while b do S out of a 
    given state will end after k consecutive executions of S, and that all
    of these executions will properly terminate, the formula which we obtain
    from equation concerning sequential composition (item (ii) in the wp properties)
    would be:

        wp(S, wp(S, ... wp(S, Q)))  with k times nesting of wp's 

    This will represent a sort of symbolic trace of the executions that
    is hardly suggestive of meaningful properties of the loop itself,
    and of the idea behind the definition of the specific while command.

    As a first step to get out of the difficulty we shall consider the weaker property 
    expressed by the notion of "weakest liberal precondition" and the respective predicate 
    transformers, which is

        wlp(S, Q) s <==> forall t. (S, s)=> t ==> Q s

    If the execution of S in s does not terminate, then the implication above
    is vacuousy true. On the other hand it is easy to see that wp(S, true) s is true
    if and only if the execution of S from s properly terminates. Hence

        wp(S, Q) s <==> wlp(S, Q) s && wp(S, true) s

    so that by means of wlp we can factor away the issue of termination, which
    is treated in most of the proof assistants like Dafny by means of decreasing clauses
    w.r.t. some well founded ordering (see te next lecture).

    With wlp we can prove the following:

    Theorem (soundness and - relative - completeness of Hoare Logic)
    For all predicates P, Q and statement S of IMP

        {P} S {Q} is derivable in Hoare logic if and only if P ==> wlp(S, Q).

    In particular the rule for the while command in the logic is

                   {P && b} S {P}
        Loop --------------------------
             {P} while b do S {P && !b}
     
     The predicate P is called "loop invariant": in the premise {P && b} S {P} we
     are supposing that the truth of P will be preserved by any execution of S
     in a state in which the "guard" b is true. Consequently, if P is strue of
     the initial state in the execution of while b do S then if and when this
     terminates P will still hold of the final state, no matter how many times
     the "loop body" S has been executed, togather with !b which is the exit 
     clause of the loop.

    The same rule in (pseudo) Dafny has the form:

    {{ P }}
     while B 
        invariant J
    {
        Body
    }
    {{ P && !B }}
    
    To prove this triple Dafny will try:

    {{ P && B }}
    Body
    {{ P }}

    Note. The double curly braces {{...}} are not Dafny syntax; also, this rule omits
    the termination clause; for a full formulation see lecture 4. 
    Such a clause is automatically inferred in some easy cases, where it can be
    omitted.
*/

/*  The method LoopSpecifications() includes three loops wihout body; nontheless
    Dafny checks whether or not the assertion of a post-condion follows from
    the respective invariants.
*/

method LoopSpecifications()
{
    var x := 0;
    while x < 300
        invariant x % 2 == 0
        // here the body is omitted
    assert x % 2 == 0 && 300 <= x;

    var y := 2;
    while y < 50
        invariant y % 2 == 0
    // assert .... (write here the post-condition)

    var z := 0;
    while z % 2 == 0
        invariant 0 <= z <= 20
    // assert z == 19; (not provable: why?)
}

/*  The next examples illustrate the usage of invariants as specifications of loops
    whose body consists of just an increment of a decrement of a variable
*/

method UpWhileLess(N: int) returns (i: int)
    requires 0 <= N
    ensures  i == N
{
    i := 0;
    while i < N
        invariant i <= N
    {
        i := i + 1;
    }
}

method UpWhileNotEqual(N: int) returns (i: int)
    requires 0 <= N
    ensures  i == N
{
    i := 0;
    while i != N
        invariant i <= N
    {
        i := i + 1;
    }
}

method DownWhieNotEqual(N: int) returns (i: int)
    requires 0 <= N
    ensures  i == 0
{
    i := N;
    while i != 0
        invariant 0 <= i
    {
        i := i - 1;
    }
}

method DownWhileGreater(N: int) returns (i: int)
    requires 0 <= N
    ensures  i == 0
{
    i := N;
    while 0 < i
        invariant 0 <= i
    {
        i := i - 1;
    }
}

/*  The final example verifies an iterative implementation of 
    the Gauss formula:

        1 + 2 + ... + n = n * (n + 1) / 2
*/

method sum(n: int) returns (res: int)
  requires 0 <= n
  ensures  res == n * (n + 1) / 2
{
  res := 0;
  var i := 0;
  while i <= n
    invariant i <= (n + 1)
    invariant res == (i - 1) * i / 2
  {
    res := res + i;
    i := i + 1;
  }
}

// Below find a fully commented version of sum(), duely named after Gauss:

method Gauss(n: nat) returns (res: nat)
    ensures res == (n * (n + 1)) / 2
    // fully braced version of the Gauss formula
{
    res := 0;
    var i := 1;
    while i <= n 
        invariant i <= n + 1
        invariant res == ((i - 1) * i) / 2
    {
        calc {
            ((i - 1) * i) / 2;
        ==  (i * i - i) / 2;
        ==  (i * i + i - 2 * i) / 2;
        ==  (i * (i + 1) - 2 * i) / 2;
        ==  (i * (i + 1)) / 2 - (2 * i) / 2;
        ==  (i * (i + 1)) / 2 - i;
        }
        assert res + i == i * (i + 1) / 2;
        res := res + i;
        assert res == i * (i + 1) / 2;
        assert res == (i + 1 - 1) * (i + 1) / 2;
        i := i + 1;
        assert res == (i - 1) * i / 2;
    }
}