/** 
    Lists
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

lemma {:induction false} AppendLength<T>(xs: List<T>, ys: List<T>)
    ensures Length(Append(xs, ys)) == Length(xs) + Length(ys)
{
    match xs // by induction over xs
    case Nil => 
        calc {
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
        ==  {AppendLength(zs, ys);}
            1 + Length(zs) + Length(ys);      // by induction
        ==  Length(Cons(z, zs)) + Length(ys); // by def. of Length
        }
}

// Insert sort of lists of integers

// If xs is ordered then Insert(y, xs) is the oredered
// list whose elements are y and those of xs
function Insert(y: int, xs: List<int>): List<int>
{
    match xs
    case Nil => Cons(y, Nil)
    case Cons(z, zs) =>           // xs == Cons(z, zs)
        if y < z then Cons(y, xs)
        else Cons(z, Insert(y, zs))
}

// InsertSort(xs) is the ordere list of the elements of xs
function InsertSort(xs: List<int>): List<int>
{
    match xs
    case Nil => Nil
    case Cons(y, Nil) => xs
    case Cons(y, ys) => Insert(y, InsertSort(ys))
}

predicate Ordered(xs: List<int>)
{
    match xs
    case Nil => true
    case Cons(y, Nil) => true 
    case Cons(y, Cons(z, zs)) => y <= z && Ordered(Cons(z, zs))
}

lemma InsertOrdered(y: int, xs: List<int>)
    requires Ordered(xs)
    ensures  Ordered(Insert(y, xs))
{}

lemma InsertSortOrdered(xs: List<int>)
    ensures Ordered(InsertSort(xs))
{
    match xs
    case Nil => 
    case Cons(z, zs) => // xs == Cons(z, zs)
        calc {
            true;
        ==> {InsertSortOrdered(zs);}            
            Ordered(InsertSort(zs));            // by Lemma InsertSortOrdered
        ==> {InsertOrdered(z, InsertSort(zs));}
            Ordered(Insert(z, InsertSort(zs))); // by induction
        ==> Ordered(InsertSort(Cons(z, zs)));   // by def. of InsertSort
        }
}