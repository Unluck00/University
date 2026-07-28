/**
    Le liste sono un tipo di dato parametrico, definendole facendo astrazione dal tipo dei dati della lista

    List<T> è un tipo di dato parametrico, che rappresenta una lista di elementi di tipo T, e può essere usato per rappresentare liste di qualsiasi tipo, come ad esempio liste di interi, liste di stringhe, ecc.

    Nil è la lista vuota e Cons(head, tail) è una lista non vuota, dove head è l'elemento in testa alla lista e tail è la coda della lista, che è a sua volta una lista di tipo List<T>.
    si inserisce head: e tail: per specificare i nomi dei campi del costruttore Cons

    // il comando Cons con Dafny permette di costruire una lista con un elemento in testa e una coda
*/

datatype List<T> = Nil | Cons(head: T, tail: List<T>)

// Esempi di funzioni sulle liste parametriche, quindi che prenda una lista di qualsiasi tipo T, e restituisca un valore di qualsiasi tipo U, e che quindi possono essere usate per dimostrare proprietà che sono vere per ogni lista di qualsiasi tipo T, senza dover specificare un tipo concreto.
// funzione generica segnata con una dipendenza accanto al nome: Length<T>

function Length<T>(xs: List<T>): nat
{
    match xs // si definiscono i casi
    case Nil => 0 // se xs è la lista vuota, allora la lunghezza della lista è 0, e quindi Length(Nil) == 0, e quindi la postcondizione è soddisfatta
    case Cons(_, ys) => 1 + Length(ys) // se xs è una lista non vuota, allora la lunghezza della lista è 1 più la lunghezza della coda della lista, e quindi Length(Cons(_, ys)) == 1 + Length(ys), e quindi la postcondizione è soddisfatta
    
    // esempio se xs è Cons(1, Cons(2, Nil)), allora Length(xs) == 1 + Length(Cons(2, Nil)) == 1 + (1 + Length(Nil)) == 1 + (1 + 0) == 2, e quindi la postcondizione è soddisfatta
}

// concatenazione di liste, con Append è una nuova lista che contiene gli elementi di xs seguiti dagli elementi di ys

function Append<T>(xs: List<T>, ys: List<T>): List<T>
    ensures Length(Append(xs, ys)) == Length(xs) + Length(ys) // postcondizione che garantisce che la lunghezza della lista risultante è la somma delle lunghezze delle liste di partenza, e quindi che la funzione Append è corretta
{
    match xs
    case Nil => ys
    case Cons(z, zs) => Cons(z, Append(zs, ys)) // senza _ altrimenti si butta via l'informazione di z

    // esempio se xs è Cons(1, Cons(2, Nil)) e ys è Cons(3, Cons(4, Nil)), 
    // allora Append(xs, ys) == Cons(1, Append(Cons(2, Nil), ys)) 
    // == Cons(1, Cons(2, Append(Nil, ys))) 
    // == Cons(1, Cons(2, ys)) 
    // == Cons(1, Cons(2, Cons(3, Cons(4, Nil)))), e quindi la postcondizione è soddisfatta
}

// uso di un lemma per dimostrare che la postcondizione di Append è soddisfatta
// {:induction false} è un attributo che indica che la dimostrazione del lemma non deve essere fatta per induzione, ma deve essere fatta con un altro metodo, come ad esempio il calcolo; disabilitazione euristica

lemma {:induction false} AppendLength<T>(xs: List<T>, ys: List<T>)
    ensures Length(Append(xs, ys)) == Length(xs) + Length(ys)
{
    match xs // by induction over xs
    case Nil =>
        calc { // si fa il calcolo per dimostrare che la postcondizione è soddisfatta
            Length(Append(xs, ys));
            ==  Length(Append(Nil, ys)); // by the hypothesis xs == Nil
            ==  Length(ys);              // by def. of Append
            ==  0 + Length(ys);
            ==  Length(xs) + Length(ys); // by def. of Length
        }
    case Cons(z, zs) =>
         calc {
            Length(Append(Cons(z, zs), ys));
            ==  Length(Cons(z, Append(zs, ys)));  // by def. of Append
            ==  1 + Length(Append(zs, ys));       // by def. of Length
            ==  {AppendLength(zs, ys);}         // le parentesi graffe indicano che si usa il lemma AppendLength per dimostrare che Length(Append(zs, ys)) == Length(zs) + Length(ys), e quindi che la postcondizione è soddisfatta
                1 + Length(zs) + Length(ys);      // by induction (o ipotesi induttiva)
            ==  Length(Cons(z, zs)) + Length(ys); // by def. of Length
        }
}

/**
    Inserimento ordinato di liste di interi (Insert sort)

    Se xs è ordinata allora Insert(y, xs) è la lista ordinata il cui elemento y e con quelli di xs
*/

function Insert(y: int, xs: List<int>): List<int>
{
    match xs 
    case Nil => Cons(y, Nil)
    case Cons(z, zs) =>           // xs == Cons(z, zs)
        if y < z then Cons(y, xs)
        else Cons(z, Insert(y, zs))
}

// InsertSort(xs) è la lista ordinata degli elementi di xs
function InsertSort(xs: List<int>): List<int>
{
    match xs
    case Nil => Nil
    case Cons(y, Nil) => xs
    case Cons(y, ys) => Insert(y, InsertSort(ys))

    // esempio se xs è Cons(3, Cons(1, Cons(2, Nil))), 
    // allora InsertSort(xs) == Insert(3, InsertSort(Cons(1, Cons(2, Nil)))) 
    // == Insert(3, Insert(1, InsertSort(Cons(2, Nil)))) 
    // == Insert(3, Insert(1, Insert(2, InsertSort(Nil)))) 
    // == Insert(3, Insert(1, Insert(2, Nil))) 
    // == Insert(3, Cons(1, Cons(2, Nil))) 
    // == Cons(1, Insert(3, Cons(2, Nil))) 
    // == Cons(1, Cons(2, Insert(3, Nil))) 
    // == Cons(1, Cons(2, Cons(3, Nil))), e quindi la postcondizione è soddisfatta
}

// si dimostra quello che abbiamo appena definito, quindi prima il predicato di essere ordinato

predicate Ordered(xs: List<int>)
{
    match xs
    case Nil => true // lista vuota è ordinata
    case Cons(y, Nil) => true // lista con un solo elemento è ordinata
    case Cons(y, Cons(z, zs)) => y <= z && Ordered(Cons(z, zs)) // se xs è una lista con almeno due elementi, allora è ordinata se il primo elemento è minore o uguale al secondo elemento, e la coda della lista è ordinata
}

// si spezza la verifica in due lemmi 
// uno per dimostrare che se xs è ordinata allora Insert(y, xs) è ordinata, e l'altro per dimostrare che se xs è ordinata allora InsertSort(xs) è ordinata

lemma InsertOrdered(y: int, xs: List<int>)
    requires Ordered(xs) // precondizione che richiede che xs sia ordinata, perché altrimenti non è possibile dimostrare che Insert(y, xs) è ordinata, e quindi la funzione Insert non è ben definita per xs non ordinata
    ensures  Ordered(Insert(y, xs)) // postcondizione che garantisce che se xs è ordinata allora Insert(y, xs) è ordinata, e quindi che la funzione Insert è corretta
{}

lemma InsertSortOrdered(xs: List<int>)
    ensures Ordered(InsertSort(xs)) // postcondizione che garantisce che se xs è ordinata allora InsertSort(xs) è ordinata, e quindi che la funzione InsertSort è corretta
{
    match xs
    case Nil => 
    case Cons(z, zs) => // xs == Cons(z, zs)
        calc {
            true; // si parte da true, perché la tesi da dimostrare è che InsertSort(xs) è ordinata, e quindi che la postcondizione è soddisfatta
            ==> {InsertSortOrdered(zs);}            
                Ordered(InsertSort(zs));            // by Lemma InsertSortOrdered
            ==> {InsertOrdered(z, InsertSort(zs));}
                Ordered(Insert(z, InsertSort(zs))); // by induction
            ==> Ordered(InsertSort(Cons(z, zs)));   // by def. of InsertSort
        }
}

// si deve verificare che la che quella lista sia una permutazione della lista data (non solo ordinata, ma anche con gli stessi elementi)
// si usa la dimostrazione di Leino basata su una funzione (Project(xs, p)) che conta il numero di occorrenze di un elemento (p) in una lista (xs), e si dimostra che questa funzione restituisce lo stesso valore per la lista data e per la lista ordinata, e quindi che le due liste sono permutazioni l'una dell'altra

ghost function Project(xs: List<int>, p: int): List<int>
{
    match xs
    case Nil => Nil
    case Cons(x, tail) =>
        if x == p then Cons(x, Project(tail, p)) // se x è uguale a p, allora si conta un'occorrenza di p nella lista, e si continua a contare le occorrenze di p nella coda della lista
        else Project(tail, p) // se x è diverso da p, allora non si conta un'occorrenza di p nella lista, e si continua a contare le occorrenze di p nella coda della lista 
}

// si definisce un lemma per dimostrare che se xs è una lista di interi, e p è un intero, allora Project(Cons(y, xs), p) == Project(Insert(y, xs), p), cioè che la funzione Insert non cambia il numero di occorrenze di p nella lista, e quindi che la funzione Insert è corretta
lemma InsertSameElements(y: int, xs: List<int>, p: int)
    // no precondizione, perché Insert(y, xs) è definita per ogni lista xs
    ensures Project(Cons(y, xs), p) == Project(Insert(y, xs), p) // postcondizione che garantisce che se xs è ordinata allora Insert(y, xs) ha lo stesso numero di occorrenze di p di Cons(y, xs), e quindi che la funzione Insert è corretta
{}

// si è in grado di dimostrare con InsertSort(xs)
lemma InsertSortSameElements(xs: List<int>, p: int)
    ensures Project(xs, p) == Project(InsertSort(xs), p) // postcondizione che garantisce che se xs è ordinata allora InsertSort(xs) ha lo stesso numero di occorrenze di p di xs, e quindi che la funzione InsertSort è corretta
{
    // si inserisce la dimostrazione per induzione su xs, e si usa il lemma InsertSameElements per dimostrare che se xs è una lista non vuota, allora InsertSort(xs) ha lo stesso numero di occorrenze di p di xs, e quindi che la postcondizione è soddisfatta
    // Dafny non è in grado di dedurre tramite l'induzione automatica
    match xs
    case Nil => // se xs è la lista vuota, allora Project(xs, p) == Project(InsertSort(xs), p) == Nil, e quindi la postcondizione è soddisfatta
    case Cons(x, tail) => // InsertSameElements(x, InsertSort(tail), p); // by Lemma InsertSameElements
        if x == p {
            calc {
                Project(Cons(x, tail), p);
            ==  Cons(x, Project(tail, p)); // by def. of Project
            ==  {InsertSortSameElements(tail, p);} 
                Cons(x, Project(InsertSort(tail), p)); // by ind. hyp.
            ==  Project(Cons(x, InsertSort(tail)), p); // by def. of Project
            ==  {InsertSameElements(x, InsertSort(tail), p);} 
                Project(InsertSort(Cons(x, tail)), p); // by lemma InsertSameElements
            }
        } else {
            assert x != p;
            calc {
                Project(Cons(x, tail), p);
            ==  Project(tail, p); // by def. of Project
            ==  {InsertSortSameElements(tail, p);} 
                Project(InsertSort(tail), p); // by ind. hyp.
            ==  Project(Cons(x, InsertSort(tail)), p); // by def. of Project
            == {InsertSameElements(x, InsertSort(tail), p);} 
                Project(Insert(x, InsertSort(tail)), p); // by lemma InsertSameElements
            ==  Project(InsertSort(Cons(x, tail)), p); // by def. of InsertSort
            }
        }
}

/**
    xs == Cons(x, tail)
    ind. hyp. InsertSortSameElements(xs, p): Project(xs, p) == Project(InsertSort(xs), p)
    
    InsertSort(Cons(x, tail)) == Insert(x, InsertSort(tail))
    tesi da dimostrare: Project(Cons(x, tail), p) == Project(Insert(x, InsertSort(tail)), p)
    si fa un case analysis sul fatto che x sia uguale a p o meno, e si usa il lemma InsertSameElements per dimostrare che se x è uguale a p, allora Project(Cons(x, tail), p) == Project(Insert(x, tail), p), e quindi che la postcondizione è soddisfatta, e se x è diverso da p, allora Project(Cons(x, tail), p) == Project(tail, p), e quindi che la postcondizione è soddisfatta 
    
    Cons(x, tail) => 
        if x == p {
            calc {
                Project(Cons(x, tail), p);
            ==  Cons(x, Project(tail, p)); // by def. of Project
            ==  {InsertSortSameElements(tail, p);} 
                Cons(x, Project(InsertSort(tail), p)); // by ind. hyp.
            ==  Project(Cons(x, InsertSort(tail)), p); // by def. of Project
            ==  {InsertSameElements(x, InsertSort(tail), p);} 
                Project(InsertSort(Cons(x, tail)), p); // by lemma InsertSameElements
            }
        } else {
            assert x != p;
            calc {
                Project(Cons(x, tail), p);
            ==  Project(tail, p); // by def. of Project
            ==  {InsertSortSameElements(tail, p);} 
                Project(InsertSort(tail), p); // by ind. hyp.
            ==  Project(Cons(x, InsertSort(tail)), p); // by def. of Project
            == {InsertSameElements(x, InsertSort(tail), p);} 
                Project(Insert(x, InsertSort(tail)), p); // by lemma InsertSameElements
            ==  Project(InsertSort(Cons(x, tail)), p); // by def. of InsertSort
            }
        }

*/