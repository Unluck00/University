/**
    Binary search
 */

 predicate Ordered(a: array<int>)
    reads a
 {
    forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]
 }

 method BinSearch(a: array<int>, key: int) returns (n: int)
    requires Ordered(a)
    ensures  0 <= n <= a.Length
    ensures  forall i :: 0 <= i < n ==> a[i] < key
    ensures  forall i :: n <= i < a.Length ==> key <= a[i]
    ensures  exists i :: 0 <= i < a.Length && key == a[i] <==>
             n < a.Length && a[n] == key;
{
    var lo, hi := 0, a.Length;
    while lo < hi
        invariant 0 <= lo <= hi <= a.Length // remember that lo == hi then lo..hi is empty
        invariant forall i :: 0 <= i < lo ==> a[i] < key
        invariant forall i :: hi <= i < a.Length ==> key <= a[i]
    {
        var mid := (lo + hi) / 2;
        if a[mid] < key {
            lo := mid + 1;
        } else {
            hi := mid;
        }
    }
    assert lo == hi;
    n := lo;
    assert n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != key;
    assert n < a.Length ==> key <= a[n];
    assert exists i :: 0 <= i < a.Length && key == a[i] <==>
            n < a.Length && a[n] == key;
}