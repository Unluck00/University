/****************************************** 
Lecture 2 - Weakest preconditions

******************************************/

/*  We assume that the programming language we are considering is "deterministic", that is
    for all program statement S and state s if (S, s)=> t for some state t then it is unique.

    Let P and Q be state predicates and S a program statement; we say that P is a 
    (total) pre-condition of Q w.r.t S if

        forall s :: P s ==> exists t :: (S, s)=> t && Q t
    
    Define the predicate wp(S, Q) as the weakest (total) precondition namely

    i)  wp(S, Q) is a total pre-condtion of Q w.r.t. S and
    ii) for all total pre-condition P of Q w.r.t. S it holds P ==>' wp(S, Q)

    where P ==>' Q if forall s :: P s ==> Q s.
    
    Then an explicit definition of wp(S, Q) is:

        wp(S, Q) s <==>  exists t :: (S, s)=> t && Q t

    Total pre-conditions are partial pre-condtions, but not vice versa. The difference emerges
    when considering programs that may not terminate; however if we discard loops and recursive 
    procedures (which we shall treate later) then all programs terminate in any state. 

    Prop. (total versus partial pre-conditions)
    i)  if P is a total pre-condition of Q w.r.t. S then the triple {P} S {T} is valid
    ii) if {P} S {T} is valid and S terminates from any s s.t. P s, then P is a total 
        pre-condition of Q w.r.t. S.

    To any program statement S the mapping wp(S, _) (where wp(S, _) Q = wp(S, Q)) is a function of 
    predicates to predicates, called a "predicate transformer". The interest of wp(S, _) is that,
    at least when S does not include loops nor procedure calls, it can be given an explicit definition:

    i)    wp(x := a, Q) = Q[a/x]
    ii)   wp(S;T, Q) = wp(S, wp(T, Q))
    iv)   wp(if b then S else T, Q) = (b' &&' wp(S, Q)) ||' (!b' &&' wp(T, Q)) 

    where b' s = bval b s (bval evaluates the boolean expression b in the state s),
    !b' s = !(bval b s), (P &&' Q) s = P s && Q s, (P ||' Q) s = P s || Q s.

    Theorem (wp-Lemma)
    If S does not contain loops nor precedure calls, then for all Q:
    i)    {wp(S, Q)} S {Q} is derivable in Hoare logic, hence valid
    ii)   {P} S {Q} is derivable if and only if P ==>' wp(S, Q).

    As a consequence of the wp-Lemma, we can compute the weakest precondition
    of a program S when reasoning backward starting with a post-condition Q
    so that to verify S against the specification with pre-condition P it suffices
    to prove P ==>' wp(S, Q) using just mathematical logic and arithmetic.
     
*/

method MyMethod1(x: int) returns (y: int)
    requires 10 <= x
    ensures  25 <= y
{
    // 1
    var a: int, b: int;
    // 2
    a := x + 3;
    // 3
    if x < 20 {
        // 4
        b := 32 - x;
        // 5
    } else {
        // (6)
        b := 16;
        // (7)
    }
    // (8)
    y := a + b;
    // (9)
}

/*  Exercise: assuming that the requires clause (the pre-condition) and the 
    ensure clause (the post-condition) have been copied to pos1tions (1) and (9)
    respectively, compute assertions from (9) to (1) by means of wp starting with

    (8) := wp(y := a + b, (9))
    ...
    (1) := wp(a := x + 3, (2))

*/

//  Solution:

method MyMethod2(x: int) returns (y: int)
    requires 10 <= x
    ensures  25 <= y
{
    assert 10 <= x; // (1)
    var a: int, b: int;
    assert (x < 20 && 25 <= x + 3 + 32 - x) || 
           (!(x < 20) && 25 <= x + 3 + 16); // (2)
    a := x + 3;
    assert (x < 20 && 25 <= a + 32 - x) || 
           (!(x < 20) && 25 <= a + 16);  // (3)
    if x < 20 {
        assert 25 <= a + 32 - x; // (4)
        b := 32 - x;
        assert 25 <= a + b; // (5)
    } else {
        assert 25 <= a + 16; // (6)
        b := 16;
        assert 25 <= a + b; // (7)
    }
    assert 25 <= a + b; // (8)
    y := a + b;
    assert 25 <= y; // (9)
} 