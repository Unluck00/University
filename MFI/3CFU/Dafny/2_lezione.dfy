/*
    Richiamo di alcune nozioni della teoria di IMP

    s: State = Var --> Val

    Asserzioni
    
        P, Q: State --> Bool

        (C, s) ==> t  se l'esecuzione di C a partire dallo stato s termina normalmente con t

        {{P}} C {{Q}} è (weakly valid) valido se per ogni s, t : State. P(s) && (C, s) ==> t ==> Q(t)

        quindi in parole semplici, {{P}} C {{Q}} è valido se per ogni stato s e t, se P è vera in s e l'esecuzione di C a partire da s termina normalmente con t, allora Q è vera in t
*/

// validità di {x == 0} y := x + 3 {y < 100} 
// che riguarda la definizione di validità di un triplo di Hoare, e che è valida perché per ogni stato s e t, se x == 0 è vera in s e l'esecuzione di y := x + 3 a partire da s termina normalmente con t, allora y < 100 è vera in t, perché y sarà uguale a 3 in t, e quindi y < 100 è vera in t
method HoareTriple1(x: int) 
{
    assume x == 0; // si chiede a Dafny di assumere che questa condizione sia vera indipendentemente dal valore di x
    // assert x + 3 < 100; // non è assume perché questa condizione è vera in ogni stato, indipendentemente dal valore di x, quindi è una verità logica, e quindi è un'asserzione che deve essere verificata in ogni stato, e non è una precondizione che deve essere assunta per verificare il metodo
    var y := x + 3; // si dichiara una variabile y e si assegna il valore di x + 3 a y
    assert y < 100;
} 

/** 
    La regola dell'assegnazione è un assioma fondamentale della logica di Hoare, che permette di calcolare la precondizione più debole (weakest precondition) di un'assegnazione, e che afferma che:

        {P[a/x]} x := a {P} è valido per ogni asserzione P, variabile x e espressione a, dove P[a/x] è l'asserzione ottenuta sostituendo tutte le occorrenze di x in P con a.
    
    La regola dell'assegnazione è un caso particolare della regola di composizione, che afferma che:

        P ==> P' {P'} C {Q} è valido per ogni asserzione P, P', Q e programma C, dove P ==> P' è una validità logica che afferma che P implica P'.

    quindi:

            assn -----------------
                 P[a/x] x := a {P}

        and 
                P ==> P'   {P'} C {Q}
            str ---------------------
                    {P} C {Q}
*/

// validità di {x == 0} y := x + 3 {0 <= x && y == 3} 
method HoareTriple2(x: int)
{
    assume x == 0;
    var y := x + 3;
    assert 0 <= x && y == 3;
} 

// validità di {x == 0} y := x + 3 {true} 
method HoareTriple3(x: int)
{
    assume x == 0;
    var y := x + 3;
    assert true;
}



// swapping and the use of ghost variables

// le variabili ghost sono variabili che non fanno parte dello stato del programma, e che vengono utilizzate solo per la verifica, e che non vengono eseguite durante l'esecuzione del programma
// e che possono essere utilizzate per specificare proprietà che devono essere vere in ogni stato del programma, e che possono essere utilizzate anche nelle pre e post condizioni dei metodi
method swap1()
{
    var x, y := 57, 12; // si dichiarano due variabili x e y e si assegnano i valori 57 e 12 rispettivamente a x e y
    ghost var X, Y := x, y; // si dichiarano due variabili ghost X e Y e si assegnano i valori di x e y rispettivamente a X e Y, in modo che X e Y rappresentino i valori originali di x e y prima dello swapping
    /*
        non si fa cosi:
        var temp := x;
        x := y;
        y := temp;
        assert x == 12 && y == 57; // questa asserzione non è valida, perché dopo lo swapping x sarà uguale a 12 e y sarà uguale a 57, quindi x == 12 && y == 57 è vero, ma x == 57 && y == 12 è falso, quindi l'asserzione non è valida
    */ 
    assert x == X && y == Y; // questa asserzione è valida, perché x == X && y == Y è vero, e rappresenta la precondizione del metodo, cioè che x e y sono uguali a X e Y rispettivamente, prima dello swapping
    var temp := x;
    x := y;
    y := temp;
    assert x == Y && y == X; // questa asserzione è valida, perché x == Y && y == X è vero, e rappresenta la postcondizione del metodo, cioè che x e y sono uguali a Y e X rispettivamente, dopo lo swapping 
}

// funzione swap2 che implementa lo swapping di x e y, e che è verificata rispetto alla specifica che afferma che x' è uguale a y e y' è uguale a x, dove x' e y' sono i valori di x e y dopo lo swapping, e x e y sono i valori di x e y prima dello swapping
method swap2(x: int, y: int) returns (x': int, y': int) 
    ensures x' == y && y' == x 
{
    x', y' := y, x; // si dichiarano due variabili x' e y' e si assegnano i valori di x e y rispettivamente a x' e y', in modo che x' e y' rappresentino i valori originali di y e x prima dello swapping    
}   

/** 
    La regola della composizione è una regola fondamentale della logica di Hoare, che permette di calcolare la precondizione più debole (weakest precondition) di un programma composto da due programmi, e che afferma che:

        P ==> P' {P'} C {Q} è valido per ogni asserzione P, P', Q e programma C, dove P ==> P' è una validità logica che afferma che P implica P'.
    
    quindi:
            {P} C {R}    {R} C' {Q}
       comp -----------------------
                {P} C;C' {Q}
*/

method swap3() 
{
    var x, y := 57, 12;
    ghost var x', y' := x, y;
    
    assert x == x' && y == y'; // P
    assert y == x + y - x == y' && x == y - (y - x) == x'; // P ==> P'
    x := y - x;
    assert x + y - x == y' && y - x == x'; // P'
    y := y - x;
    assert x + y == y' && y == x'; // P'
    x := x + y;
    assert x == y' && y == x'; // Q
}


// conditional statement
/**
    La regola della condizionale è una regola fondamentale della logica di Hoare, che permette di calcolare la precondizione più debole (weakest precondition) di un programma composto da un'istruzione condizionale, e che afferma che:

           {P && b} C {Q}       {P && !b} C' {Q}
      cond -------------------------------------
               {P} if b then C else C' {Q}
*/

method Max(x: int, y: int) returns (m: int) 
    ensures m >= x && m >= y // si specifica che m è maggiore o uguale a x e y
    ensures m == x || m == y // e che m è uguale a x o y, in modo che m rappresenti il massimo tra x e y
{
    if x < y {
        assert y == y >= x;
        m := y;
        assert m == y >= x;
        assert m == y >= x || m == x >= y; // questa asserzione è vera, perché m == y >= x è vero, e quindi m == y >= x || m == x >= y è vero, e rappresenta la postcondizione del ramo then, cioè che m è maggiore o uguale a x e y, e che m è uguale a y o x
    }
    else {
        assert !(x < y);
        assert x == x >= y;
        m := x;
        assert m == x >= y;
        assert m == y >= x || m == x >= y;
    }
    assert m == y >= x || m == x >= y; 
}


/** 
    P && b ==> U   {U} C {Q}   P && !b ==> V  {V} C' {Q'} 
    ----------------------------------------------------- 
            {P} if b then C else C' {Q || Q'}

    dimostra che è derivabile: 
    per dimostrare la derivabilità bisogna dimostrare che la regola della condizionale può essere derivata utilizzando le regole della logica di Hoare, in particolare la regola dell'assegnazione e la regola della composizione, e che quindi la regola della condizionale è una conseguenza delle altre regole della logica di Hoare, e quindi è derivabile.
*/


/** 
    Weakest Precondition

    A strong precondition P to C w.r.t. Q if

    forall s :: P s ==> exists t :: (C, s) ==> t && Q t

    The weakest precondition wp(C, Q) is an assertion (predicate) s.t.
    i) wp(C, Q) is a strong precondition to C w.r.t. Q
    ii) if P is a strong precondition to C w.r.t. Q then 
                    P ==> wp(C, Q)

    Prop. For all C, Q and state s 
        wp(C, Q) s <==> exists t :: (C, s) ==> t && Q t

    Remark. The definition of the (weakest) liberal precondition is 
        forall s :: (P s && (C, s) => t ==> Q t)        garantisce...
    and
        wlp(C, Q) s <==> forall t :: (P s && (C, s) => t ==> Q t)

    Prop. For all C, Q and state s 
        wp(C, Q) s <==> wlp(C, Q) s && wp(C, true) s

    Dim. Observe that wp(C, true) s if and only if exists t :: (C, s) => t
    and use the fact that the prog. language Dafny is deterministic

    wp(x := a, Q) = Q[a/x]
    wp(C;C', Q) = wp[C, wp(C', Q)      sono dei fruitori della regola di composizione, che afferma che la pre-condizione più debole di C;C' rispetto a Q è proprio la pre-condizione più debole di C rispetto alla pre-condizione più debole di C' rispetto a Q
    wp(if b then C else C', Q) = (b && wp(C, Q)) || (!b && wp(C', Q))

    (while b do C, s) --> (C; while b do C, s).   if bval b s = true
                      --> (C; C; while b do C, s) if bval b ([C] s) = true

    wp(while b do C) <==>  wp(C, wp(C, .... wp(C, Q) ....)) k times for some k          non è formativa

              {P && b} C {P}
    loop ---------------------------
          {P} while b do C {P && !b}



    La weakest precondition (wp) di un programma S rispetto ad una post-condizione Q è la pre-condizione più debole che garantisce che Q sia vera alla fine dell'esecuzione di S, e può essere calcolata utilizzando le regole della logica di Hoare, come la regola dell'assegnazione e la regola della composizione.

    In particolare, il wp di un programma S rispetto ad una post-condizione Q può essere calcolato utilizzando le seguenti regole:

    - wp(skip, Q) = Q 
        skip è un'istruzione che non fa nulla, quindi la pre-condizione più debole che garantisce che Q sia vera alla fine dell'esecuzione di skip è proprio Q
    - wp(x := a, Q) = Q[a/x] 
        la pre-condizione più debole che garantisce che Q sia vera alla fine dell'esecuzione di x := a è proprio Q[a/x], cioè l'asserzione ottenuta sostituendo tutte le occorrenze di x in Q con a
    - wp(S1; S2, Q) = wp(S1, wp(S2, Q))
        la pre-condizione più debole che garantisce che Q sia vera alla fine dell'esecuzione di S1; S2 è proprio wp(S1, wp(S2, Q)), cioè la pre-condizione più debole che garantisce che wp(S2, Q) sia vera alla fine dell'esecuzione di S1, e che Q sia vera alla fine dell'esecuzione di S2
    - wp(if b then S1 else S2, Q) = (b && wp(S1, Q)) || (!b && wp(S2, Q))
        la pre-condizione più debole che garantisce che Q sia vera alla fine dell'esecuzione di if b then S1 else S2 è proprio (b && wp(S1, Q)) || (!b && wp(S2, Q)), cioè la pre-condizione più debole che garantisce che wp(S1, Q) sia vera alla fine dell'esecuzione di S1 se b è vera, e che wp(S2, Q) sia vera alla fine dell'esecuzione di S2 se b è falsa
*/