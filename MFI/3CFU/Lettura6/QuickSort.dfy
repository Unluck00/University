/** 
    Quick sort
*/

predicate Ordered(a: array<int>)
    reads a
 {
    forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]
 }

 predicate SplitPoint(a: array<int>, n: int)
    requires 0 <= n <= a.Length
    reads a
{
    forall i, j :: 0 <= i < n <= j < a.Length ==> a[i] <= a[j]
}

twostate predicate SwapFrame(a: array<int>, lo: int, hi: int)
    requires 0 <= lo <= hi <= a.Length
    reads a
{
    (forall i :: (0 <= i < lo || hi <= i < a.Length) ==> a[i] == old(a[i])) 
    // a[..lo] == old(a[..lo]) and a[hi..] == old(a[hi..])
    && multiset(a[..]) == multiset(old(a[..]))
}

 method QuickSort(a: array<int>)
    ensures Ordered(a)
    ensures multiset(a[..]) == multiset(old(a[..])) 
    modifies a
{
    QuickSortAux(a, 0, a.Length);
}

method QuickSortAux(a: array<int>, lo: int, hi: int)
    requires 0 <= lo <= hi <= a.Length
    requires SplitPoint(a, lo) && SplitPoint(a, hi)
    ensures  forall i, j :: lo <= i < j < hi ==> a[i] <= a[j]
    ensures  SplitPoint(a, lo) && SplitPoint(a, hi)
    ensures  SwapFrame(a, lo, hi)
    modifies a
    decreases hi - lo
{
    if lo < hi {
        var p := Partition(a, lo, hi);
        QuickSortAux(a, lo, p);
        QuickSortAux(a, p + 1, hi);
    }
}

method Partition(a: array<int>, lo: int, hi: int) returns (p: int)
    requires 0 <= lo < hi <= a.Length
    requires SplitPoint(a, lo) && SplitPoint(a, hi)
    ensures  lo <= p < hi
    ensures  forall i :: lo <= i < p ==> a[i] <= a[p]
    ensures  forall j :: p <= j < hi ==> a[p] <= a[j]
    ensures  SplitPoint(a, lo) && SplitPoint(a, hi)
    ensures  SwapFrame(a, lo, hi)
    modifies a
{
    var pivot := a[lo];
    var m, n := lo + 1, hi;
    while m < n
        invariant lo + 1 <= m <= n <= hi
        invariant a[lo] == pivot
        invariant forall i :: lo <= i < m ==> a[i] <= pivot
        invariant forall i :: n <= i < hi ==> pivot < a[i]
        invariant SplitPoint(a, lo) && SplitPoint(a, hi)
        invariant SwapFrame(a, lo, hi)
    {
        if a[m] <= pivot { 
            m := m + 1;
        } else if a[n - 1] > pivot {
            n := n - 1;
        } else {
            assert a[m] > pivot && a[n - 1] <= pivot;
            a[m], a[n - 1] := a[n - 1], a[m];
            m, n := m + 1, n - 1;
        }
    }
    a[lo], a[m - 1] := a[m - 1], a[lo];
    p := m - 1;
}