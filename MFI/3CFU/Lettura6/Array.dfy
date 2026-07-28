/** 
    Mutable sequences: the arrays

    Arrays are objects of type array<T> which are allocated in the heap,
    like with C++ and Java
*/

method TestArray(j: nat, k: nat)
    requires j < 10 && k < 10
{
    var a := new int[10]; // a: array<int>
    assert a.Length == 10;
    a[j] := 60;
    a[k] := 65;
    if j == k {
        assert a[j] == 65;
    } else {
        assert a[j] == 60;
    }
}

method ArrayReferences()
{
    var a := new string[20]; // a: array<string>
    a[7] := "hello";
    var b := a;         // b: array<string>, b is an alias of a
    b[7] := "hi";
    assert a[7] == "hi"; // side-effect
}

/** 
    Sequences are immutable objects similar to the arays, and have type seq<T>
    Their elements are accessed via indexes, and the length of a sequence s is |s|
*/

method ArraysVsSequences()
{
    var greetings: seq<string>;
    greetings := ["hey", "hola", "ciao"];
    assert |greetings| == 3;   // |greetings| is the length of the sequence greetings
    assert greetings[2] == "ciao";
    // greetings[2] := "salve"; illegal because sequences are immutable data
    var greetings' := ["hey", "hola", "ciao"];
    assert greetings == greetings';
    assert greetings + ["salut"] == ["hey", "hola", "ciao", "salut"];

    var a := new int[3];
    a[0], a[1], a[2] := 6, 28, 49;
    ghost var s := a[..]; // s: seq<int>
    assert s == [6, 28, 49];
    assert s[0] == 6;
    var t := a[1..3]; // 1..2 is the interval 1
    assert t == [28, 49];
    var r := a[..3]; // 0..3 is the interval 0, 1, 2
}

method LinearSearch(a: array<int>, x: int) returns (n: int)
    ensures 0 <= n <= a.Length
    ensures n < a.Length ==> a[n] == x
    ensures n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != x
{
    n := 0;
    while n < a.Length && a[n] != x
        invariant 0 <= n <= a.Length
        invariant forall i :: 0 <= i < n ==> a[i] != x
        decreases a.Length - n // not necessary
    {
        n := n + 1; 
    }
    // all the subsequent assertions are unnecessary
    assert a.Length <= n || a[n] == x; // at the end of the loop it holds the negation of guard
    assert a.Length <= n ==> n == a.Length;
    assert n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != x;
    assert n < a.Length ==> a[n] == x;
}

// Search of the maximum in an array
method MaxOfArray(a: array<int>) returns (max: int)
    requires a.Length > 0
    ensures  forall k :: 0 <= k < a.Length ==> a[k] <= max
    ensures  exists k :: 0 <= k < a.Length && a[k] == max
{
    max := a[0];
    var i := 1;
    while i < a.Length
        invariant 0 < i <= a.Length
        invariant forall k :: 0 <= k < i ==> a[k] <= max
        invariant exists k :: 0 <= k < i && a[k] == max
    {
        if a[i] > max { max := a[i]; }
        i := i + 1;
    }
}

ghost function SumSeq(s: seq<int>): int
// sum of the elements of the sequence s
// def. by induction over |s|
{
    if |s| == 0 then 0
    else SumSeq(s[..|s| - 1]) + s[|s| - 1]
}

// Sum of the elemens of an array of integers
method SumArray(a: array<int>) returns (sum: int)
    ensures sum == SumSeq(a[..])
{
    sum := 0;
    var i := 0;
    assert sum == SumSeq(a[..i]);
    while i < a.Length
        invariant 0 <= i <= a.Length
        invariant sum == SumSeq(a[..i])
    {
        assert (a[..i + 1])[..i] == a[..i];
        sum := sum + a[i];
        assert SumSeq(a[..i + 1]) == SumSeq(a[..i]) + a[i];
        i := i + 1;
    }
    assert a[..a.Length] == a[..];
}