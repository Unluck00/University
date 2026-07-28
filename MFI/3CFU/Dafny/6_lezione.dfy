/**
    Proving assertions via lemmas

    I lemmi è uno strumento per usare il proof assistance (cioè il sistema di dimostrazione assistita) in modo modulare, cioè per suddividere una dimostrazione complessa in più parti più semplici.

    lemma <lemma name>([<parameters with types>]) 
        [requires <hypothesis>]
        ensures <thesis>
    {
        [<proof body>]
    }

    whosw meaning is (sintassi)

    lemma Lemma(x: T, y: T')
        requires P(x, y)
        ensures Q(x, y)
    
    corresponding to logical formula as  

    // definizione di forall
    forall x: T, y: T' :: P(x, y) ==> Q(x, y)
*/

/** Example: prove that for all a, b, c: int

    min(a - c, b - c) == min(a, b) - c

*/

// il metodo ghost è ignorato dal compilatore e serve solo per definire funzioni o procedure che servono solo per la dimostrazione, e non per il programma vero e proprio
ghost function min(a: int, b: int): int 
{
    if a < b then a else b
}

lemma Min_lemma(a: int, b: int, c: int)
    ensures min(a - c, b - c) == min(a, b) - c  // clausola da dimostrare attraverso il lemma (cioè la tesi) producendo un'asserzione che è la tesi stessa
{
    if a < b {
        assert a - c < b - c;
        assert min(a - c, b - c) == a - c;
        assert a == min(a, b);
        assert min(a - c, b - c) == min(a, b) - c;
    }
    else {
        // uguale ma simmetrico
        assert b <= a;
        assert b - c <= a - c;
        assert min(b - c, a - c) == b - c;
        assert b == min(b, a);
        assert min(b - c, a - c) == min(b, a) - c;
    }
}

function More(x: int): int
{
    if x <= 0 then 1 
    else More(x - 2) + 3
}

// adesso si definisce un lemma per dimostrare che More(x) > 0 per ogni x
lemma Increasing(x: int)
    // no hypothesis, perché vogliamo dimostrare che la tesi è vera per ogni x
    ensures x < More(x) // tesi da dimostrare
{} // se il corpo del lemma è vuoto, la dimostrazione avviene per induzione sulla struttura della funzione More, e non è necessario scrivere un corpo esplicito per il lemma

// aggiungere l'istruzione {:induction false} prima del lemma permette di disabilitare l'induzione automatica, e quindi di scrivere un corpo esplicito per il lemma, ma in questo caso non è necessario, perché la dimostrazione avviene per induzione sulla struttura della funzione More, ma comunque facciamo un esempio
lemma {:induction false} Increasing1(x: int)
    // no hypothesis, perché vogliamo dimostrare che la tesi è vera per ogni x
    ensures x < More(x) // tesi da dimostrare
    decreases x // non è necessario, ma serve per dimostrare che la funzione More è ben fondata, cioè che non ci sono cicli infiniti nella definizione della funzione More, e quindi che la dimostrazione per induzione è valida
{
    if x <= 0 {
        assert x <= 0 < 1 == More(x); // se x <= 0, allora More(x) == 1, e quindi x < More(x)
    } else {
        assert 0 < x; // se x > 0, allora More(x) > 1, e quindi x < More(x)
        Increasing1(x - 2); // se x > 0, allora More(x) == More(x - 2) + 3, e quindi per dimostrare che x < More(x), è sufficiente dimostrare che x - 2 < More(x - 2), e questo è vero per induzione sulla struttura della funzione More, perché x - 2 < x, e quindi la dimostrazione per induzione è valida
        // ipotesi induttiva: x - 2 < More(x - 2)
        assert x - 2 < More(x - 2);
        assert x + 1 < More(x - 2) + 3; // adding 3 to both sides; 
        assert x + 1 < More(x); // definizione di More(x) == More(x - 2) + 3
        assert x < More(x); // per aritmetica, in cui +1 è maggiore di 0, quindi x < x + 1 < More(x)
    }
}

// costruzione di un lemma con calc per dimostrare che x < More(x) per ogni x, senza usare l'induzione automatica, ma scrivendo un corpo esplicito per il lemma, e usando la costruzione calc per dimostrare la tesi
lemma {:induction false} Increasing2(x: int)
    // no hypothesis, perché vogliamo dimostrare che la tesi è vera per ogni x
    ensures x < More(x) // tesi da dimostrare
    decreases x // non è necessario, ma serve per dimostrare che la funzione More è ben fondata, cioè che non ci sono cicli infiniti nella definizione della funzione More, e quindi che la dimostrazione per induzione è valida
{
    if x <= 0 {
        assert x <= 0 < 1 == More(x); // se x <= 0, allora More(x) == 1, e quindi x < More(x)
    } else {
        assert 0 < x; // se x > 0, allora More(x) > 1, e quindi x < More(x)
        calc {
            0 < x; // se x > 0, allora More(x) > 1, e quindi x < More(x)
        ==> { Increasing2(x - 2); assert x - 2 < More(x - 2); } 
            x - 2 < More(x - 2);
        ==> x + 1 < More(x - 2) + 3; 
        ==> x < x + 1 < More(x);
        }
    }
}

/**

    Dafny usa l'induzione ben fondata, non l'induzione di Peano (o induzione al successore), quindi:

    P(0)  forall x. P(x) ==> P(x + 1)
    --------------------------------- (induzione di Peano)
              forall x. P(x)

    forall x. (forall y < x. P(y)) ==> P(x)
    --------------------------------------- (induzione ben fondata)
              forall x. P(x)

     L'induzione ben fondata è più generale dell'induzione di Peano, perché permette di dimostrare che una proprietà è vera per tutti gli elementi di un insieme ordinato, anche se non è un insieme di numeri naturali, e anche se non è un insieme di numeri interi, ma ad esempio un insieme di stringhe, o un insieme di liste, o un insieme di alberi, ecc.

     In questo caso, l'induzione ben fondata è più adatta per dimostrare che x < More(x) per ogni x, perché la funzione More è definita in modo ricorsivo su x, e quindi la dimostrazione per induzione sulla struttura della funzione More è più naturale e più semplice da scrivere rispetto alla dimostrazione per induzione di Peano.

*/

method ExampleUseOfLemma(a: int)
{
    var b := More(a); // infatti More non è ghost e quindi è una funzione che può essere usata nel programma vero e proprio, e non solo per la dimostrazione
    Increasing(a); assert a < More(a); // se usiamo il lemma Increasing, allora possiamo dimostrare che a < More(a) == b
    Increasing(b); assert b < More(b); // se usiamo il lemma Increasing, allora possiamo dimostrare che b < More(b) == c
    var c := More(b);
    assert 2 <= c - a; // non sa abbastanza della funzione More per dimostrare questa asserzione, ma se usiamo il lemma Increasing, allora possiamo dimostrare che c > b > a, e quindi c - a > 2
    // a < More(a) == b < More(b) == c, quindi c - a > 2
    // a < More(a) < More(More(a)) == c ==> a + 2 <= c
}