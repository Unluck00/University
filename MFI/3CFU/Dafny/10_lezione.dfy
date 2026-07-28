/**
    Sequenze mutabili: gli array (allocati sugli heap)
    accesso agli elementi tramite indici

    Gli array sono oggetti di tipo array<T> che sono allocati nello heap, come in C++ e Java
*/

// esempio di Array, in cui gli elementi sono di tipo int, e la lunghezza massima è 10
// infatti con i dati mutabili si ha che nell'esempio di TestArray, se j == k, allora a[j] == 65, altrimenti a[j] == 60, e non è possibile che a[j] sia 60 e 65 allo stesso tempo
method TestArray(j: nat, k: nat)
    requires j < 10 && k < 10
{
    var a := new int[10]; // a: array<int>
    assert a.Length == 10;
    a[j] := 60;     // se mettessi a[j+1] := 60; sarebbe un errore di indice fuori dai limiti, essendo che j+1 potrebbe essere 10
    a[k] := 65;
    if j == k {
        assert a[j] == 65;
    } else {
        assert a[j] == 60;
    }
}

// esempio di aliasing, in cui a e b sono due variabili che fanno riferimento allo stesso array
// quindi se si modifica b, si modifica anche a, e viceversa
method ArrayReferences()
{
    var a := new string[20]; // a: array<string>
    a[7] := "hello";
    var b := a;         // b: array<string>, b è un alias di a
    assert b[7] == "hello";
    b[7] := "hi";
    assert a[7] == "hi"; // side-effect
}

/**
    Le sequenze sono oggetti immutabili simili agli array, e hanno tipo seq<T>
    I loro elementi sono accessibili tramite indici, e la lunghezza di una sequenza s è |s|
*/

// esempio di confronto tra array e sequenze, in cui si mostra che le sequenze sono dati immutabili, mentre gli array sono dati mutabili
// Dafny usa il tipo array<T> per rappresentare gli array, e il tipo seq<T> per rappresentare le sequenze; con generici T che rappresentano un tipo qualsiasi
method ArraysVsSequences()
{
    // dichiarazione
    var greetings: seq<string>;     // greetings: seq<string>, è una variabile di tipo sequenza di stringhe
    // assegnazione
    greetings := ["hey", "hola", "ciao"];
    assert |greetings| == 3;        // |greetings| è la lunghezza della sequenza greetings
    assert greetings[2] == "ciao";
    // greetings[2] := "salve"; illecito perché le sequenze sono dati immutabili
    // definizione
    var greetings' := ["hey", "hola", "ciao"];
    assert greetings == greetings';
    assert greetings + ["salut"] == ["hey", "hola", "ciao", "salut"]; // le sequenze supportano l'operazione di concatenazione, che restituisce una nuova sequenza che è la concatenazione delle due sequenze

    var a := new int[3];
    a[0], a[1], a[2] := 6, 28, 49;      // assegnazione multipla, equivalente a a[0] := 6; a[1] := 28; a[2] := 49;
    ghost var s := a[..];     // s: seq<int>; (Non si è usato new per indicare array, ma bensì una sequenza) 
                        // l'operatore .. è l'operatore di slicing, che restituisce una sequenza che contiene gli elementi dell'array a; in questo caso s è la sequenza [6, 28, 49]
    assert s == [6, 28, 49];
    var t := a[1..3];   // t: seq<int>; 1..2 è l'intervallo 1, 2 dell'array a
    assert t == [28, 49];
    var r := a[..3];    // r: seq<int>; 0..3 è l'intervallo 0, 1, 2 dell'array a
    assert r == [6, 28, 49];
}


method LinearSearchInt(a: array<int>, x: int) returns (n: int)
// si può anche scrivere il metodo con un tipo generico T, in modo da poter cercare un elemento di qualsiasi tipo in un array di qualsiasi tipo, e non solo in un array di int
// method LinearSearch<T>(a: array<T>, p: T->bool) returns (n: int)
    ensures 0 <= n <= a.Length
    ensures n < a.Length ==> a[n] == x      // se n < a.Length, allora a[n] == x, cioè se n è un indice valido dell'array a, allora a[n] è uguale a x, e quindi x è presente in a
    ensures n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != x       // se n == a.Length, allora x non è presente in a, e quindi per ogni i compreso tra 0 e a.Length - 1, a[i] != x
{
    n := 0;
    while n < a.Length && a[n] != x
        // potrei mettere invariant true, ma è meglio mettere degli invariant che aiutano a capire cosa succede durante l'esecuzione del ciclo, e che aiutano a dimostrare la correttezza del ciclo
        // questa invariante vale per la prima condizione del ciclo, cioè n < a.Length
        invariant 0 <= n <= a.Length    // è necessario per dimostrare che n è un indice valido dell'array a, e quindi che a[n] è definito, e che a[i] è definito per ogni i compreso tra 0 e n - 1
        // questa invariante vale per la seconda condizione del ciclo, cioè a[n] != x
        invariant forall i :: 0 <= i < n ==> a[i] != x      // è necessario per dimostrare che se n < a.Length, allora a[n] == x, e quindi che x è presente in a, e che se n == a.Length, allora x non è presente in a, e quindi per ogni i compreso tra 0 e a.Length - 1, a[i] != x
        decreases a.Length - n // not necessary perché il ciclo termina quando n == a.Length, ma è meglio mettere un decreases per dimostrare che il ciclo termina, e quindi che la funzione LinearSearch è ben fondata, cioè che non ci sono cicli infiniti nella definizione della funzione LinearSearch, e quindi che la dimostrazione per induzione è valida
    {
        n := n + 1; 
    }
    // tutte le affermazioni successive sono superflue, perché alla fine del ciclo vale la negazione della guardia
    assert n >= a.Length || a[n] == x;     
    assert n >= a.Length ==> n == a.Length ==> forall i :: 0 <= i < a.Length ==> a[i] != x;      // se n >= a.Length, allora n == a.Length e allora per ogni i compreso tra 0 e a.Length si ha che a[i] != x
    assert n < a.Length ==> a[n] == x;      //se n < a.Length, allora a[n] == x
}

// lascia stare la ricerca dicotomica negli array (guarda appunti)

// esempio di ricerca del massimo in un array, che è un algoritmo lineare, perché si deve scorrere tutto l'array per trovare il massimo
method MaxOfArray(a: array<int>) returns (max: int)
    requires a.Length > 0       // pre-condizione in cui l'array ha almeno un elemento
    ensures forall k :: 0 <= k < a.Length ==> a[k] <= max      // post-condizione in cui max è maggiore o uguale a ogni elemento dell'array (rappresentato da k)
    ensures exists k :: 0 <= k < a.Length && a[k] == max    // post-condizione in cui max è uguale a un elemento dell'array (rappresentato da k)
{
    max := a[0];
    var i := 1;
    while i < a.Length
        invariant 0 < i <= a.Length     // l'indice i sarà nel range dell'array a
        invariant forall k :: 0 <= k < i ==> a[k] <= max    // per ogni k compreso tra 0 e i - 1, a[k] <= max, cioè max è maggiore o uguale a ogni elemento dell'array a che è stato esaminato fino a quel momento 
        invariant exists k :: 0 <= k < i && a[k] == max   // per ogni k compreso tra 0 e i - 1, esiste un k tale che a[k] == max, cioè max è uguale a un elemento dell'array a che è stato esaminato fino a quel momento
    {
        if a[i] > max 
        { 
            max := a[i]; 
        }
        i := i + 1;
    }
}

ghost function SumSeq(s: seq<int>): int 
// somma degli elementi della sequenza s
// definita per induzione sulla dimensione di s |s|
{
    if |s| == 0 then 0      // |s| definisce la lunghezza della sequenza s
    else SumSeq(s[..|s| - 1]) + s[|s| - 1]
}

// Somma degli elementi di un array di interi
/** 
    Da capire questo method perché usa due assiomi di cui Dafny non ha una definizione esplicita, ma che sono impliciti nella definizione di sequenza e nell'operazione di slicing, e che sono necessari per dimostrare la correttezza del metodo SumArray

    usa due assiomi della riga 140 e 147 
    problema di come sono assiomatizzate le sequenze 
    usi un lemma che dimostri la sequenza dell'array
*/

method SumArray(a: array<int>) returns (sum: int)
    ensures sum == SumSeq(a[..])    // sum è la sommatoria di tutti gli elementi dell'array, Dafny non conosce la sommatoria degli elementi di un array
                                    // quindi lo si indica tramite una function ghost, che è una funzione che può essere usata solo per la dimostrazione, 
                                    // e non può essere usata nel programma vero e proprio, e che è definita in modo ricorsivo su s, 
                                    // e quindi la dimostrazione avviene per induzione sulla struttura della funzione SumSeq
                                    // gli passa come sequenza tutto l'array di a, convertendola in sequenza
{
    sum := 0;
    var i := 0;
    assert sum == SumSeq(a[..i]);       // in questo caso la sum sarà 0
    while i < a.Length
        invariant 0 <= i <= a.Length
        invariant sum == SumSeq(a[..i])     // qua sum continuerà a confrontare ad ogni passo, inoltre si hanno bisogno degli assert 
                                            // perché Dafny non conosce la sommatoria degli elementi di un array, e quindi non sa che sum + a[i] == SumSeq(a[..i + 1])
    {
        assert (a[..i + 1])[..i] == a[..i];     
        
        // aggiunto sto assert per dimostrare a Dafny la struttura della sequenza; 
        // dimostrare che SumSeq(a[..i + 1]) == SumSeq(a[..i]) + a[i] 
        // ma SumSeq è definita ricorsivamente sulla struttura della sequenza
        // quindi applicata a a[..i+1], Dafny espande: SumSeq(a[..i+1]) = SumSeq((a[..i+1])[..i]) + (a[..i+1])[i]
        // si ha che (a[..i+1])[..i] deve diventare a[..i]
        // (a[..i+1])[i] deve diventare a[i]
        // Il secondo passaggio Dafny lo capisce facilmente.
        // Il primo invece spesso non lo deduce automaticamente, quindi serve l’assert assert: (a[..i + 1])[..i] == a[..i];
            // la sequenza ottenuta togliendo l’ultimo elemento da a[..i+1] è esattamente a[..i]
        // [6,28] = [6] + 28

        // es. a = [6, 28, 49] i = 1 allora a[..i+1] = a[..2] = [6, 28]
        // ora si scrive (a[..i+1])[..i]
        // a[..i+1] → ottieni la sequenza [6, 28]
        // [..i] applicato a quella sequenza → prendi i primi i elementi della sequenza ottenuta
        // quindi ([6, 28])[..1] = [6] == a[..i] = a[..1] = [6]

        sum := sum + a[i];
        assert SumSeq(a[..i + 1]) == SumSeq(a[..i]) + a[i];
        i := i + 1;
    }
    assert a[..a.Length] == a[..];
}