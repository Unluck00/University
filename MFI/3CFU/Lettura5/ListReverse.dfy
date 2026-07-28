/**
    Reversing a list in linear time
 */

datatype List<T> = Nil | Cons(head: T, tail: List<T>)

function Length<T> (xs: List<T>): nat
{
    match xs
    case Nil => 0
    case Cons(_, ys) => 1 + Length(ys)
}

function Append<T>(xs: List<T>, ys: List<T>): List<T>
    ensures Length(Append(xs, ys)) == Length(xs) + Length(ys)
{
    match xs
    case Nil => ys
    case Cons(z, zs) => Cons(z, Append(zs, ys))
}

/** For all T the set of data of type List<T> is a monoid w.r.t the binary operation 
    Append and the unit Nil:

    Append(Nil, xs) == xs == Append(xs, Nil)
    Append(Append(xs, ys), zs) == Append(xs, Append(ys, zs))

*/

lemma AppendNil<T>(xs: List<T>)
    ensures xs == Append(xs, Nil)
{}

lemma AppendAssoc<T>(xs: List<T>, ys: List<T>, zs: List<T>)
    ensures Append(Append(xs, ys), zs) == Append(xs, Append(ys, zs))
{}

// Snoc(xs, y) adds y at the end of xs in time O(n)
function Snoc<T>(xs: List<T>, y: T): List<T>
{
    match xs
    case Nil => Cons(y, Nil)
    case Cons(z, zs) => Cons(z, Snoc(zs, y))
}

lemma SnocAppend<T>(xs: List<T>, y: T)
    ensures Snoc(xs, y) == Append(xs, Cons(y, Nil))
{}

// SlowReverse(xs) computes the inverse of xs in time O(n^2)
function SlowReverse<T>(xs: List<T>): List<T>
{
    match xs
    case Nil => Nil
    case Cons(y, ys) => Snoc(SlowReverse(ys), y)
}

// Toward the definition of a linear time algorithm, define the auxiliary function
// ReverseAux(xs, acc) which computese the concatenation of the inverse of xs with acc
function ReverseAux<T>(xs: List<T>, acc: List<T>): List<T>
{
    match xs
    case Nil => acc
    case Cons(y, ys) => ReverseAux(ys, Cons(y, acc))
}

function Reverse<T>(xs: List<T>): List<T>
{
    ReverseAux(xs, Nil)
}

// goal: Reverse(xs) == SlowReverse(xs)
// To prove the above equation we establish the following lemma

lemma ReverseAuxSlowCorrect<T>(xs: List<T>, acc: List<T>)
    ensures ReverseAux(xs, acc) == Append(SlowReverse(xs), acc)
{
    match xs
    case Nil =>
    case Cons(y, ys) => // xs == Cons(y, ys)
        /* calc {
            ReverseAux(Cons(y, ys), acc);
        ==  ReverseAux(ys, Cons(y, acc));   // by def. of ReverseAux
        ==  {ReverseAuxSlowCorrect(ys, Cons(y, acc));}
            Append(SlowReverse(ys), Cons(y, acc));      // by ind. hyp.
        } */
        calc {
            Append(SlowReverse(Cons(y, ys)), acc);
        ==  Append(Snoc(SlowReverse(ys), y), acc);  // by def. of SlowReverse
        ==  {SnocAppend(SlowReverse(ys), y);}
            Append(Append(SlowReverse(ys), Cons(y, Nil)), acc); // by SnocAppend 
        == {AppendAssoc(SlowReverse(ys), Cons(y, Nil), acc);}
            Append(SlowReverse(ys), Append(Cons(y, Nil), acc)); // associativity of Append
        ==  Append(SlowReverse(ys), Cons(y, Append(Nil, acc))); // by def. of Append
        ==  Append(SlowReverse(ys), Cons(y, acc));              // by def. of Append
        }
}

lemma ReverseCorrect<T>(xs: List<T>)
    ensures Reverse(xs) == SlowReverse(xs)
{
    calc {
        Reverse(xs);
    ==  ReverseAux(xs, Nil);
    ==  {ReverseAuxSlowCorrect(xs, Nil);}
        Append(SlowReverse(xs), Nil);
    ==  {AppendNil(SlowReverse(xs));}
        SlowReverse(xs);
    }
}