// Floyd-Hoare logic and Weakest Preconditions

method HoareTriple1(x: int)
{
    assume x + 1 <= 43;
    var y := x + 1;
    assert y <= 43;
}

method Swap()
{
    var x, y := 57, 12;
    
    ghost var X, Y := x, y; // le variabili ghost sono variabili che non fanno parte dello stato del programma, e che vengono utilizzate solo per la verifica, e che non vengono eseguite durante l'esecuzione del programma

    var temp: int;
    assert x == X && y == Y; // P
    assert x == X && y == Y ==> y == Y && x == X; // P ==> P'
    assert y == Y && x == X; // P' := S[x/temp]
    temp := x;
    assert y == Y && temp == X; // S := R[y/x]
    x := y;
    assert x == Y && temp == X; // R := Q[temp/y]
    y := temp;
    assert x == Y && y == X; // Q' == Q
}