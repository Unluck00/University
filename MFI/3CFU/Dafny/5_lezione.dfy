/** Recursion versus iteration */

// Example con divisione intera ricorsiva
method Div_Rec(x: nat, y: nat) returns (q: nat, r: nat) // q->quoziente, r->resto
    requires y > 0
    ensures x == y * q + r && 0 <= r < y // la postcondizione deve garantire che r sia un numero compreso tra 0 e y, altrimenti non sarebbe un resto valido
{
    if x < y { 
        q, r := 0, x; 
    } else {
        // assert x - y < x;
        q, r := Div_Rec(x-y, y); // ipotesi induttiva: assumiamo che la chiamata ricorsiva restituisca un quoziente e un resto corretti per x - y
                                 // assegna a q e r i valori restituiti da Div_Rec(x-y, y)
        assert x - y == y * q + r && 0 <= r < y; // sta facendo l'ipotesi induttiva
        // assert x == y * q + y + r && 0 <= r < y;
        // assert x == y * q + y + r == y * (q + 1) + r; // perché il +1??? Perché si mette y in comune
        q := q + 1; // aggiorna il quoziente aggiungendo 1, perché abbiamo sottratto y da x, quindi il quoziente deve aumentare di 1
        // l'incremento di q è giustificato dal fatto che x è maggiore o uguale a y, quindi sottraendo y da x stiamo effettivamente contando un'unità in più nel quoziente
    }
}
/**
    esempio x = 10, y = 3
    prima chiamata: Div_Rec(10, 3) -> q = 0, r = 10
    seconda chiamata: Div_Rec(7, 3) -> q = 0, r = 7
    terza chiamata: Div_Rec(4, 3) -> q = 0, r = 4
    quarta chiamata: Div_Rec(1, 3) -> q = 0, r = 1
    ritorno alla terza chiamata: q = 0 + 1 = 1, r = 1
    ritorno alla seconda chiamata: q = 1 + 1 = 2, r = 1
    ritorno alla prima chiamata: q = 2 + 1 = 3, r = 1
 */


// Example con divisione intera iterativa
method Div_Iter(x: nat, y: nat) returns (q: nat, r: nat) // q->quoziente, r->resto
    requires y > 0
    ensures x == y * q + r && 0 <= r < y
{
    q, r := 0, x; 
    while y <= r
        invariant  x == y * q + r && 0 <= r // l'invariante del ciclo deve garantire che r sia sempre un numero maggiore o uguale a 0, altrimenti non sarebbe un resto valido; non si aggiunge la condizione r < y all'invariante perché durante l'esecuzione del ciclo r può essere maggiore o uguale a y, ma alla fine del ciclo deve essere garantito che r sia minore di y
        // senza l'invariante del ciclo, non avremmo alcuna garanzia di r durante l'esecuzione del ciclo, il che potrebbe portare a risultati errati o a un ciclo infinito
    {
        // corpo dell'iterazione: sottrai y da r e incrementa q di 1 finché r è maggiore o uguale a y
        r := r - y;
        q := q + 1;
    }
}
// c'è una decrease clause implicita che garantisce la terminazione del ciclo, perché r diminuisce ad ogni iterazione e non può diventare negativo, quindi alla fine il ciclo terminerà quando r sarà minore di y
// non c'è bisogno di indicarlo esplicitamente, ma si potrebbe aggiungere una decrease clause per rendere più chiaro che il ciclo terminerà sicuramente


/** As y is a (non negative) integer we may distinguish the cases
    
    y is odd; y == 2 * k + 1, for some k

        x * y == x * (2 * k + 1) == (2 * x) * k + x
    
    y is even; y == 2 * k, for some k

        x * y == x * (2 * k) == (2 * x) * k

    
*/

// Example con moltiplicazione veloce ricorsiva
method FastMult_Rec(x: int, y: int) returns (m: int)
    requires 0 <= x && 0 <= y
    ensures m == x * y
    decreases y // per garantire la terminazione della ricorsione, è necessario che y diminuisca ad ogni chiamata ricorsiva, bisogna esplicitarlo perché Dafny non è in grado di dedurlo automaticamente, soprattutto nel caso in cui y sia dispari, perché in quel caso y / 2 potrebbe non essere strettamente minore di y, quindi è necessario specificare esplicitamente che y diminuisce ad ogni chiamata ricorsiva per garantire la terminazione del metodo
{
    if y == 0 { m := 0;}
    else if y % 2 == 1 {
        assert y == (y / 2) * 2 + 1;
        assert x * y == (2 * x) * (y / 2) + x;
        m := FastMult_Rec(2 * x, y / 2);
        // manca il + x, perché la chiamata ricorsiva restituisce solo (2 * x) * (y / 2), ma per ottenere x * y dobbiamo aggiungere anche x, quindi è necessario aggiungere x al risultato della chiamata ricorsiva per ottenere il risultato corretto
        // l'incremento di x è giustificato dal fatto che quando y è dispari, stiamo effettivamente contando un'unità in più nel prodotto, quindi dobbiamo aggiungere x al risultato della chiamata ricorsiva per ottenere il risultato corretto
        m := m + x;
    }
    else {
        assert y == (y / 2) * 2;
        assert x* y == (2 * x) * (y / 2);
        m := FastMult_Rec(2 * x, y / 2);
    }
}
/*
    esempio x = 3, y = 5
    prima chiamata: FastMult_Rec(3, 5) -> m = FastMult_Rec(6, 2) + 3
    seconda chiamata: FastMult_Rec(6, 2) -> m = FastMult_Rec(12, 1)
    terza chiamata: FastMult_Rec(12, 1) -> m = FastMult_Rec(24, 0) + 12
    quarta chiamata: FastMult_Rec(24, 0) -> m = 0
    ritorno alla terza chiamata: m = 0 + 12 = 12
*/

// Example con moltiplicazione veloce iterativa
method FastMult_Iter(x: int, y: int) returns (m: int)
    requires 0 <= x && 0 <= y
    ensures  m == x * y
    // la decreases non è una clausola che riferiamo alla post condizione, ma è una clausola che riferiamo al corpo del metodo, in particolare al ciclo, perché è necessario garantire la terminazione del ciclo, quindi è necessario specificare una decreases clause che garantisca che il ciclo terminerà sicuramente, in questo caso possiamo specificare che y diminuisce ad ogni iterazione del ciclo, perché ad ogni iterazione stiamo dividendo y per 2, quindi y diminuisce ad ogni iterazione e alla fine il ciclo terminerà quando y sarà uguale a 0
    // rispetto alla decrease della ricosività che li avviene ad ogni chiamata ricorsiva, in questo caso la decrease avviene ad ogni iterazione del ciclo, quindi è necessario specificare una decreases clause che garantisca che il ciclo terminerà sicuramente
{
    var a, b, z := x, y, 0; // a rappresenta il valore di x che viene raddoppiato ad ogni iterazione, b rappresenta il valore di y che viene diviso per 2 ad ogni iterazione, z rappresenta il risultato parziale del prodotto che viene aggiornato ad ogni iterazione
    // aggiunto le var perché a, b e z sono variabili che vengono aggiornate ad ogni iterazione del ciclo, quindi è necessario dichiararle come variabili locali all'interno del metodo
    while b > 0
        invariant x * y == a * b + z && 0 <= b // z è l'accumulatore
        decreases b
    {
        if b % 2 == 1 {
            assert b == 2 * (b / 2) + 1; // questa asserzione è necessaria per giustificare l'aggiornamento di z, perché quando b è dispari, stiamo effettivamente contando un'unità in più nel prodotto, quindi dobbiamo aggiungere a al risultato parziale z per ottenere il risultato corretto, quindi è necessario giustificare l'aggiornamento di z con l'asserzione che b è dispari e può essere scritto come 2 * (b / 2) + 1
            assert x * y == a * (2 * (b / 2) + 1) + z
                         == 2 * a * (b / 2) + a + z;
            z := z + a;
            assert x * y == 2 * a * (b / 2) + z;
        }
        // due asserzioni per giustificare l'aggiornamento di a e b, perché quando b è pari, stiamo effettivamente contando solo le potenze di 2 nel prodotto, quindi dobbiamo raddoppiare a e dividere b per 2 per ottenere il risultato corretto, quindi è necessario giustificare l'aggiornamento di a e b con l'asserzione che b è pari e può essere scritto come 2 * (b / 2)
        assert b % 2 == 0 ==> x * y == 2 * a * (b / 2) + z; // questa asserzione è necessaria per giustificare l'aggiornamento di a e b, perché quando b è pari, stiamo effettivamente contando solo le potenze di 2 nel prodotto, quindi dobbiamo raddoppiare a e dividere b per 2 per ottenere il risultato corretto, quindi è necessario giustificare l'aggiornamento di a e b con l'asserzione che b è pari e può essere scritto come 2 * (b / 2)
        assert x * y == 2 * a * (b / 2) + z; 
        a := 2 * a;
        b := b / 2;
        assert x * y == a * b + z;
    }
    assert x * y == a * b + z && b == 0; // b non è strettamente maggiore di 0, ma è uguale a 0, perché il ciclo termina quando b è uguale a 0, quindi è necessario specificare che b è uguale a 0 per garantire che il ciclo terminerà sicuramente
    assert x * y == z; // quando b è uguale a 0, il prodotto x * y è uguale a z, perché a * b è uguale a 0, quindi x * y è uguale a z, quindi è necessario specificare che x * y è uguale a z per garantire che il risultato restituito dal metodo sia corretto
    return z;
}