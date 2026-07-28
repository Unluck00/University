// Loop invariants

method Div_Iter(x: nat, y: nat) returns (q: nat, r: nat)
    requires y > 0
    ensures  x == y * q + r && r < y
{
    r, q := x, 0;
    // assert x == y * q + r;
    while y <= r 
        invariant x == y * q + r
        // decreases r
    {
        // assume x == y * q + r;
        // assert x == y * (q + 1) + r - y;
        r := r - y;
        // assert x == y * (q + 1) + r;
        q := q + 1;
        // assert x == y * q + r;
    }
}

method Div_rec(x: nat, y: nat) returns (q: nat, r: nat)
    requires y > 0
    ensures  x == y * q + r && r < y
{
    if x < y {
        q, r := 0, x;
        // assert x == y * q + r && r < y;
    } 
    else {
        // assert !(x < y);
        // assert x - y  < x && 0 <= x - y && y > 0;
        q, r := Div_rec(x - y, y);
        // assert x - y == y * q + r && r < y;
        q := q + 1;
        // assert x == y * q + r && r < y;
    }
}