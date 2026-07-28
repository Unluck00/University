/** 
    Recursion and termination

*/

/* method BadDouble(x: int) returns (d: int)
    ensures d == 2 * x
    //decreases x
{
    var y := BadDouble(x - 1);
    d := y + 2;
}


function Dobious(): nat
{
    1 - Dobious();
} */

function Fib(n: nat): nat
    decreases n // unnecessary
{
    if n == 0 || n == 1 then n else Fib(n - 2) + Fib(n - 1)
}

/*
call                       proof obbligation
--------------------------------------------
Fib(n) calls Fib(n - 2)    n > n - 2
Fib(n) calls Fib(n - 1)    n > n - 1

*/

function Fib'(n: nat): nat
    decreases 3 * n + 1
{
    if n < 2 then n else Fib'(n - 2) + Fib'(n - 1)
}

function SeqSum(s: seq<int>, lo: int, hi: int): int
    requires 0 <= lo <= hi < |s|
    decreases hi - lo
{
    if lo == hi then s[lo] else s[lo] + SeqSum(s, lo + 1, hi)
}

/*
call                                       proof obbligation
------------------------------------------------------------------
SeqSum(s,lo,hi) calls SeqSum(s,lo+1,hi)    hi - lo >  hi - (lo + 1) 
                                                   == hi - lo - 1
*/

function N(x: int, y: int, b: bool): int
    decreases x, b
{
    if x <= 0 || y <= 0 then x + y
    else if b then N(x, y + 3, !b)
    else N(x - 1, y, true)
}

/*
call                                       proof obbligation
--------------------------------------------------------------------------
N(x, y, true) calls N(x, y + 3, false)     x, true > x, false     && x > 0
N(x, y, false) calls N(x - 1, y, true)     x, false > x - 1, true && x > 0
*/

// true == 1 > 0 == false

/* m_1, ... , m_h > n_1, ... , n_k if either

i)   h = 0 namely m_1, ... , m_h = <> > n_1, ... , n_k
ii)  m_1 > n_1 
iii) m_1 = n_1, k > 1 and m_2, ... , m_h > n_2, ... , n_k

Examples

2, 5 > 1, 7     as 2 > 1
2, 5 > 2, 4     as 5 > 4
2 > 2, 5        as <> > 5
*/

/** Recursion versus iteration */

method Div_Rec(x: nat, y: nat) returns (q: nat, r: nat)
    requires y > 0
    ensures  x == y * q + r && 0 <= r < y
{
    if x < y 
        { q, r := 0, x; }
    else {
        // assert x - y < x;
        q, r := Div_Rec(x - y, y);
        // assert x - y == y * q + r && 0 <= r < y; // ip. ind.
        // assert x == y * q + y + r && 0 <= r < y;
        // assert x == y * q + y + r == y * (q + 1) + r;
        q := q + 1;
    }
}

method Div_Iter(x: nat, y: nat) returns (q: nat, r: nat)
    requires y > 0
    ensures  x == y * q + r && 0 <= r < y
{
    q, r := 0, x;
    while y <= r 
        invariant x == y * q + r && 0 <= r
    {
        r := r - y;
        q := q + 1;
    }
}

/** As y is a (non negative) integer we may distinguish the cases

y is odd:  y == 2 * k + 1 for some k

    x * y == x * (2 * k + 1) == (2 * x) * k + x

y is even: y == 2 * k     for some k

    x * y == x * (2 * k) == (2 * x) * k

*/

method FastMult_Rec(x: int, y: int) returns (m: int)
    requires 0 <= x && 0 <= y
    ensures  m == x * y
    decreases y
{
    if y == 0 { m := 0; }
    else if y % 2 == 1 {
        assert y == (y / 2) * 2 + 1;
        assert x * y == (2 * x) * (y / 2) + x;
        m := FastMult_Rec(2 * x, y / 2);
        m := m + x;
    }
    else {
       assert y == (y / 2) * 2; 
       assert x * y == (2 * x) * (y / 2);
        m := FastMult_Rec(2 * x, y / 2);
    }
}

method FastMult_Iter(x: int, y: int) returns (m: int)
    requires 0 <= x && 0 <= y
    ensures  m == x * y
{
    var a, b, z := x, y, 0;
    while b > 0
        invariant x * y == a * b + z && 0 <= b
        decreases b
    {
        if b % 2 == 1 {
            assert b == 2 * (b / 2) + 1;
            assert x * y == a * (2 * (b / 2) + 1) + z
                         == 2 * a * (b / 2) + a + z;
            z := z + a;
            assert x * y == 2 * a * (b / 2) + z;
        }
        assert b % 2 == 0 ==> x * y == 2 * a * (b / 2) + z;
        assert x * y == 2 * a * (b / 2) + z;
        a := 2 * a;
        b := b / 2;
        assert x * y == a * b + z;
    }
    assert x * y == a * b + z && b == 0;
    assert x * y == z;
    return z;
}