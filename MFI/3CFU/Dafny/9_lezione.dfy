/**
    Le liste reversal in un tempo lineare
*/

datatype List<T> = Nil | Cons(head: T, tail: List<T>)

function Length<T>(xs: List<T>): nat
{
    match xs
    case Nil => 0
    case Cons(_, tail) => 1 + Length(tail)
}

function Append<T>(xs: List<T>, ys: List<T>): List<T>
    ensures Length(Append(xs, ys)) == Length(xs) + Length(ys)
{
    match xs
    case Nil => ys
    case Cons(x, tail) => Cons(x, Append(tail, ys))
}

/** 
    Insieme delle liste di ogni tipo, append ha una operazione di concatenazione monoidale (binaria, associativa, con elemento neutro Nil): 
    Per tutti i tipi T, l'insieme dei dati di tipo List<T> è un monoide rispetto all'operazione binaria Append e all'elemento neutro Nil:

    Append(Nil, xs) == xs == Append(xs, Nil)
    Append(Append(xs, ys), zs) == Append(xs, Append(ys, zs))

*/

lemma AppendNil<T>(xs: List<T>)
    ensures xs == Append(xs, Nil)
{}

lemma AppendAssoc<T>(xs: List<T>, ys: List<T>, zs: List<T>)
    ensures Append(Append(xs, ys), zs) == Append(xs, Append(ys, zs))
{}

// Snoc(xs, y) aggiunge y alla fine di xs in tempo O(n)
function Snoc<T>(xs: List<T>, y: T): List<T>
{
    match xs
    case Nil => Cons(y, Nil)
    case Cons(x, tail) => Cons(x, Snoc(tail, y))
}

lemma SnocAppend<T>(xs: List<T>, y: T)
    ensures Snoc(xs, y) == Append(xs, Cons(y, Nil))
{}

// SlowReverse(xs) calcola l'inverso di xs in tempo O(n^2)
function SlowReverse<T>(xs: List<T>): List<T>
{
    match xs
    case Nil => Nil
    case Cons(x, tail) => Snoc(SlowReverse(tail), x)
}

// Verso la definizione di un algoritmo lineare, definiamo la funzione ausiliaria 
// ReverseAux(xs, acc) che calcola la concatenazione dell'inverso di xs con acc
function ReverseAux<T>(xs: List<T>, acc: List<T>): List<T>
{
    match xs
    case Nil => acc
    case Cons(x, tail) => ReverseAux(tail, Cons(x, acc))

    // esempio ReverseAux(Cons(1, Cons(2, Cons(3, Nil))), Nil) 
    // == ReverseAux(Cons(2, Cons(3, Nil)), Cons(1, Nil)) 
    // == ReverseAux(Cons(3, Nil), Cons(2, Cons(1, Nil))) 
    // == ReverseAux(Nil, Cons(3, Cons(2, Cons(1, Nil)))) 
    // == Cons(3, Cons(2, Cons(1, Nil)))
}

function Reverse<T>(xs: List<T>): List<T>
{
    ReverseAux(xs, Nil)
}

// l'obiettivo è quello di dimostrare che Reverse(xs) == SlowReverse(xs)
// per dimostrare l'equazione qui sopra, si dimostra in termini di ReverseAux con il seguente lemma
lemma ReverseAuxSlowCorrect<T>(xs: List<T>, acc: List<T>)
    ensures ReverseAux(xs, acc) == Append(SlowReverse(xs), acc)
{
    match xs
    case Nil =>
    case Cons(x, tail) => // xs == Cons(x, tail)
        calc {
            ReverseAux(Cons(x, tail), acc);
        ==  ReverseAux(tail, Cons(x, acc));  // by def. of ReverseAux
        == {ReverseAuxSlowCorrect(tail, Cons(x, acc));} // by lemma ReverseAuxSlowCorrect
            Append(SlowReverse(tail), Cons(x, acc)); // by hyp. induttiva
        }
        // adesso si prende la definizione di SlowReverse per riscrivere Append(SlowReverse(Cons(x, tail)), acc) in Append(Snoc(SlowReverse(tail), x), acc) 
        // uso di un secondo calc per dimostrare che Append(SlowReverse(Cons(x, tail)), acc) == Append(SlowReverse(tail), Cons(x, acc))
        // quindi si va la stessa cosa ma dall'altra parte (cioè da Append(SlowReverse(Cons(x, tail), acc)) 
        calc {
            Append(SlowReverse(Cons(x, tail)), acc); 
        ==  Append(Snoc(SlowReverse(tail), x), acc); // by def. of SlowReverse
        ==  {SnocAppend(SlowReverse(tail), x);} // by lemma SnocAppend
            Append(Append(SlowReverse(tail), Cons(x, Nil)), acc); // by SnocAppend
        == {AppendAssoc(SlowReverse(tail), Cons(x, Nil), acc);} // by lemma AppendAssoc
            Append(SlowReverse(tail), Append(Cons(x, Nil), acc)); // associativity of Append
        ==  Append(SlowReverse(tail), Cons(x, Append(Nil, acc))); // by def. of Append
        ==  Append(SlowReverse(tail), Cons(x, acc));  
        }
}

lemma ReverseCorrect<T>(xs: List<T>)
    ensures Reverse(xs) == SlowReverse(xs)
{
    calc {
        Reverse(xs);
    == ReverseAux(xs, Nil); // by def. of Reverse
    == {ReverseAuxSlowCorrect(xs, Nil);} // by lemma ReverseAuxSlowCorrect
        Append(SlowReverse(xs), Nil); // by def. of Append
    == {AppendNil(SlowReverse(xs));} // by lemma AppendNil  // facendo {} si specifica che la riga precedente è giustificata da un lemma
        SlowReverse(xs); // by def. of Append
    }
}