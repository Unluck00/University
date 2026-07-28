/**
    Ricerca Dicotomica (binary search)
*/

// con forall si sta limitando la ricerca del quantificatore universale

method BinSearch(a: array<int>, key: int) returns (n: nat)
    // richiede che l'array sia ordinato come precondizione
    requires forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]  // significa che l'array a è ordinato
    ensures 0 <= n <= a.Length // indica che n sarà un indice dell'array o della sua lunghezza
    // si identifica la condizione in cui a sx di n si hanno valori < di key
    ensures forall i :: 0 <= i < n ==> a[i] < key
    // viceversa a dx
    ensures forall i :: n <= i < a.Length ==> key <= a[i]
{
    var lo, hi := 0, a.Length;  // è l'intervallo lo..hi == 0..a.Length
    while lo < hi
        invariant 0 <= lo <= hi <= a.Length     // si ha che se lo == hi allora lo..hi è vuoto (estremo superiore)
        // si approssima il risultato (delle due postcondizioni con forall)
        invariant forall i :: 0 <= i < lo ==> a[i] < key
        invariant forall i :: hi <= i < a.Length ==> key <= a[i]
    {
        var mid := (lo + hi) / 2; // divisione intera
        if a[mid] < key {
            lo := mid + 1;
        } else {
            assert key <= a[mid];
            hi := mid;
        }
    }
    n := lo;
    // si può dimostrare che 
    assert n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != key;     // non c'è la key nell'array
    // assert n < a.Length ==> key == a[n];
}

method Contents(a: array<int>, key: int) returns (present: bool)
    requires forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]  // significa che l'array a è ordinato
    ensures present == exists i :: 0 <= i < a.Length && key == a[i]
{
    var n := BinSearch(a, key);
    present := n < a.Length && a[n] == key;
}


/**
    Dynamic frameworks sono asserzioni che specificano quali parti di una struttura di dati 
    allocata dinamicamente possono essere accedute in lettura o scrittura
    
    Algoritmi che modificano l'array 
    come quelli di ordinamento come quicksort
*/

predicate Ordered(a: array<int>)
    reads a // il predicate può leggere da a[..]
{
    forall i, j :: 0 <= i < j < a.Length ==> a[i] <= a[j]
}

// il complementare di ordered e modifies

method SetEndPoints(a: array<int>, left: int, right: int)
    requires 0 < a.Length
    modifies a // permette di specificare che l'array a si può modificare, altrimenti non si potrebbe andare a lavorare sui suoi elemtni
{
    a[0] := left;
    a[a.Length-1] := right;
}

method Aliases(a: array<int>, b: array<int>)
    requires 100 <= a.Length
    modifies a
{
    a[0] := 10;
    var c := a; // si fa l'aliasing di a
    c[20] := a[14] + 2;  // quindi c è modificabile 
    if a == b { // se b è un alias di a
        b[10] := 3; // perchè b non è modifies
    }
}

// uso di old per indicare il vecchio valore di un elemento che può essere modificato 
method UpdateElements(a: array<int>)
    requires a.Length == 10
    modifies a 
    ensures a[4] > old(a[4]) // per dire che a[4] è stato incrementato
    ensures a[6] == old(a[6]) // per dire che a[6] non è stato cambiato
    ensures a[8] == old(a[8]) // per dire che a[8] è stato invariato alla fine (prima incremento e poi decremento)
{
    a[4], a[8] := a[4] + 3, a[8] + 1;
    a[7], a[8] := 54, a[8] - 1;
}


/**
    Select sort 
*/

method SelectSort(a: array<int>)
    // le seguenti 3 righe finiranno nei algoritmi di sorting
    ensures Ordered(a)
    ensures multiset(a[..]) == multiset(old(a[..])) // questo implica che la a nel post-state è una permutazione della stessa a nel pre.state
    modifies a