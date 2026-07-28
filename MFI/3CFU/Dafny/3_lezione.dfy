/*

(while b do C, s) --> (C; while b do C, s).   if bval b s = true
                      --> (C; C; while b do C, s) if bval b ([C] s) = true

    wp(while b do C) <==>  wp(C, wp(C, .... wp(C, Q) ....)) k times for some k          non è formativa

              {P && b} C {P}
    loop ---------------------------
          {P} while b do C {P && !b}



    With Dafny we have:
    assume P
    while b 
        invariant P 
        decreasing ... (clausola)
    {
        C; (corpo dell'iterazione)
    }
    assert !b && P
 
    which is proved by trying 
    assume P && b
    C;
    assert P
*/

method LoopSpec() 
{
    var x := 0;
    while x < 300   // dichiara anche senza il body
        invariant x % 2 == 0  // fa la verifica della precondizione
    /*
    {
        x := 2 * x; // va in overflow e non è in grado di dedurre che finisce
    }
    */
    assert x % 2 == 0 && 300 <= x; // da assunzione a conclusione, vedi riga 202
}

// senza invariante manca l'informazione in mezzo e risulterebbe sbagliato
method UpWhile(n: int) returns (i: int) 
    requires 0 <= n;
    ensures i == n;
{
    i := 0;
    assert i <= n;
    while i < n 
        invariant i <= n // i < n non è un invariante perché non è vero sempre la guardia sarà falso per uscire
    {
        assert i < n && i + 1 <= n;
        i := i + 1;
        assert i <= n;
    }
}

// esempio di Gauss
method Gauss(n: nat) returns (res: nat)
    ensures res == n * (n + 1) / 2  // è la somma dei primi n interi naturali
{
    res := 0;
    var i := 0;
    while i <= n 
        invariant i <= n + 1  // bisgona contemplare il caso di uscita
        invariant res == (i - 1) * i / 2 // matematicamente è vero che res + i == ((i + 1 - 1) * (i + 1)) / 2; è uguale a res == (i - 1) * i / 2
    {
        calc {
            ((i - 1) * i) / 2;
            == 
            ((i + 1 - 1) * (i + 1)) / 2 - i;
        }
        assert res == ((i + 1 - 1) * (i + 1)) / 2 - i;
        assert res + i == ((i + 1 - 1) * (i + 1)) / 2;
        res := res + i;
        assert res == ((i + 1 - 1) * (i + 1)) / 2;
        i := i + 1;
        assert res == ((i - 1) * i) / 2;
    }
}