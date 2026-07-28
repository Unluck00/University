// Basics

// SuqareDiff(x, y) returns x * x - y * y
method SquareDiff(x: real, y: real) returns (z: real)
    ensures z == x * x - y * y
{
    var u := (x + y) * (x - y);
    // u := (x + y) * (x - y);
    assert u == x * x - y * y;
    z := u;
    assert z == x * x - y * y;
}

method Triple(x: int) returns (r: int)
    ensures r == 3 * x
{
    var y := 2 * x;
    r := y + x;
    assert r == 3 * x;
}

ghost function Abs(x: int): int
    ensures Abs(x) >= 0
    ensures Abs(x) == x || Abs(x) == -x
{
    if x < 0 then -x else x
}

method AbsMet(x: int) returns (y: int)
    ensures y == Abs(x)
{
    if x < 0 { y := - x; }
    else { y := x; }
}

method TestAbs()
{
    var r := - 123;
    ghost var p := r;
    r := AbsMet(r);
    assert r == Abs(p);
}