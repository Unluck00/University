/****************************************** 
Lecture 1
Exercises: MaxSum and ReconstructMaxSum
           after Leino's Program Proofs
           exercises 1.7
******************************************/

// Exercise 1.7

// (a) sepecify MaxSum(x, y) such that it returns s == x + y and m == max(x, y)
//     without the body
method MaxSum(x: int, y: int) returns (s: int, m: int)
    ensures s == x + y
    ensures (m == x || m == y) && x <= m && y <= m
// note: without {...} this is just a declaration

// (b) write a method with an assertion about the values of MaxSum(1928, 1)
method MaxSumCaller() {
    var s, m := MaxSum(1928, 1);
    assert s == 1929 && m == 1928;
}

// (c) implement MaxSum
method MaxSum'(x: int, y: int) returns (s: int, m: int)
    ensures s == x + y
    ensures (m == x || m == y) && x <= m && y <= m
{
    // to be completed
    if x >= y {
        m := x;
        s := x + y;
    } else {
        m := y;
        s := x + y;
    }
    
}

// Exercise 1.8

// (a) consider the specification of a method to reconstruct x and y from MaxSum(x, y):

/*
method ReconstructMaxSum(s: int, m: int) returns (x: int, y: int)
    ensures s == x + y
    ensures (m == x || m == y) && x <= m && y <= m

which cannot be implemented; constrain the specification such that it becomes implementable */

method ReconstructMaxSum(s: int, m: int) returns (x: int, y: int)
    requires s - m <= m
    ensures  s == x + y
    ensures (m == x || m == y) && x <= m && y <= m
{
    // to be completed
    x := m;
    y := s - m;
}

/*  Suggestion. One may reason as follows: it is always the case that
                  s = s - m + m
    which implies that
                  s - m <= m
    therefore the precondition allways holds; however it is of help for the prover
    to establish the postconditions.
*/

// (b) write TestMaxSum() to verify the method

method TestMaxSum(x: int, y: int) {
    // to be completed

    var s, m := MaxSum(x, y);
    var x1, y1 := ReconstructMaxSum(s, m);
    assert x == x1 && y == y1;
}