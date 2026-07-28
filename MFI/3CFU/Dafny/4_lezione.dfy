/**
    Ricorsione e terminazione 
*/

// esempio di funzioni ricorsive non definite
/**
    method BadDouble(x: int) returns (d: int)
        ensures d == 2 * x
    {
        var y := BadDouble(x - 1); // unbounded recursion (numero di chiamate ricorsive illimitato, non termina mai)
        d := y + 2;    
    }

    function Dubious(): nat
    {
        1 + Dubious() // unbounded recursion (numero di chiamate ricorsive illimitato, non termina mai)
    }
*/

// esempio di funzioni ricorsive definite
function Fib(n: nat): nat
    decreases n // unnecessary as it is automatic in this case; clausola per definire la corretta terminazione
{
    if n < 2 then n else Fib(n - 2) + Fib(n - 1)
}

/*
la clausola decreases n genera il proof obbligation

    call                    proof obbligation
    ------------------------------------------
    Fib(n) calls Fib(n-2)       n > n-2
    Fib(n) calls Fib(n-1)       n > n-1
*/


function Fib'(n: nat): nat
    decreases 3 * n + 2 // clausola con diversi tipi di relazione d'ordine (infinite possibilità)
{
    if n < 2 then n else Fib(n - 2) + Fib(n - 1)
}


// seq<int> p tipo un array di interi finito, ma immutabile (si può solo leggere)
// sommare interi tra l'intervallo lo (low) e hi (high)
function SeqSum(s: seq<int>, lo: int, hi: int): int
    requires 0 <= lo <= hi < |s| // dimensione corretta dell'intervallo
    decreases hi - lo
{
    if lo == hi then s[lo] else s[lo] + SeqSum(s, lo + 1, hi)
}

/*
la clausola decreases hi - lo genera il proof obbligation

    call                                                proof obbligation
    ------------------------------------------------------------------------------------
    SeqSum(s,lo,hi) calls SeqSum(s,lo+1,hi)       hi - lo >  hi - (lo + 1) 
                                                          == hi - lo - 1
*/





/*
    ordine degli interi standard, ma dei booleani si ha:
    true == 1 > 0 == false

    l'ordine lessicografico è un modo per confrontare più parametri (finiti) è un confronto sui singoli parametri

    m_1, ... m_h > n_1, ... n_k se 
    
    i)   h = 0 namely m_1, ... m_h = <> (è vuota) > n_1, ... n_k
    ii)  or m_1 > n_1
    iii) or m_1 = n_1 and m_2, ... m_h > n_2, ... n_k
    
    Examples:
    2, 5 > 1, 7 as 2 > 1
    2, 5 > 2, 4 as 2 = 2 but 5 > 4
    2 > 2, 5 as 2 = 2 and <> > 5 (2 is shorter than 2, 5)
*/

function N(x: int, y: int, b: bool): int
    decreases x, b // guarda l'ordine lessicografico (da sx a dx)
{
    if x <= 0 || y <= 0 then x + y 
    else if b then N(x, y + 3, !b)
    else N(x - 1, y, true)
}

/*
la clausola decreases x, b genera il proof obbligation

    call                                              proof obbligation
    --------------------------------------------------------------------
    N(x, y, true) calls N(x, y+3, false)           x, true > x, false    && x > 0
    N(x, y, false) calls N(x-1, y, true)          x, false > x - 1, true && x > 0
*/