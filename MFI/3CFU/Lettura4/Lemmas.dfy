/** 
    Proving assertions via lemmas

    lemma <lemma name>([<parameters with types>])
        [requires <hypothesis>]
        ensures <thesis>
    {
        [<proof body>]
    }

    whose meaning is

    lemma Lemma(x: T, y: T')
        requires P(x, y)
        ensrues  Q(x, y)

    corresponding to logical formula

    forall x: T, y: T' :: P(x, y) ==> Q(x, y)
*/

/** Example: prove that for all a, b, c: int 

    min(a - c, b - c) == min(a, b) - c

*/

ghost function min(a: int, b: int): int
{
    if a < b then a else b
}

lemma Min_lemma(a: int, b: int, c: int)
    ensures min(a - c, b - c) == min(a, b) - c
{
    if a < b {
        assert a - c < b - c;
        assert min(a - c, b - c) == a - c;
        assert a == min(a, b);
        assert min(a - c, b - c) == min(a, b) - c;
    }
    else {}
}