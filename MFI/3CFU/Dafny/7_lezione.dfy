/**
    le variabili sono oggetti generici che possono essere usati per rappresentare qualsiasi valore di un certo tipo, e possono essere usati per dimostrare proprietà che sono vere per ogni valore di quel tipo, senza dover specificare un valore concreto.

    Inductive data types (tipi di dati induttivi) sono tipi di dati che possono essere definiti in termini di se stessi, e possono essere usati per rappresentare strutture dati ricorsive, come liste, alberi, ecc.

*/

datatype Color = Blue | Yellow | Green | Red 

// Blue, ... are nullary constructors (costruttori nullari), also called generators cioè costruttori che non prendono argomenti, e quindi rappresentano valori concreti di tipo Color, e possono essere usati per dimostrare proprietà che sono vere per ogni valore di tipo Color, senza dover specificare un valore concreto.

datatype Nat = Zero | Succ(n: Nat)

/** This corresponds to Peano's definition: 

    1. Zero in Nat
    2. n in Nat ==> Succ(n) in Nat
    3. nothing else is in Nat

    Zero, Succ(Zero), Succ(Succ(Zero)), ... sono i valori concreti di tipo Nat, e possono essere usati per dimostrare proprietà che sono vere per ogni valore di tipo Nat, senza dover specificare un valore concreto.
*/


// per Dafny predicate è una function di tipo Bool
predicate IsZero(n: Nat)
{
    match n // match è una costruzione che permette di fare un case analysis sui valori di tipo Nat, e quindi di dimostrare proprietà che sono vere per ogni valore di tipo Nat, senza dover specificare un valore concreto.
    case Zero => true
    case Succ(_) => false
}

function Pred(n: Nat): Nat
    requires !IsZero(n) // precondizione che richiede che n non sia Zero, perché altrimenti non è possibile definire il predecessore di Zero, e quindi la funzione Pred non è ben definita per n == Zero
    ensures Succ(Pred(n)) == n // postcondizione che garantisce che il predecessore di n è un valore di tipo Nat, e che Succ(Pred(n)) == n, cioè che il predecessore di n è un valore di tipo Nat che, se si applica Succ, si ottiene n
{
    match n
    case Succ(m) => m // se n è Succ(m), allora il predecessore di n è m, e quindi Pred(n) == m, e quindi Succ(Pred(n)) == Succ(m) == n, e quindi la postcondizione è soddisfatta
    // si ha che => Pred(Succ(m)) == m, e quindi Succ(Pred(Succ(m))) == Succ(m) == n, e quindi la postcondizione è soddisfatta
}

// lemma per dimostrare che Pred(Succ(n)) == n per ogni n in Nat, cioè che il predecessore di Succ(n) è n, e quindi che Succ(Pred(Succ(n))) == Succ(n) == n, e quindi la postcondizione è soddisfatta
lemma PredSucc(n: Nat) 
    ensures Pred(Succ(n)) == n == Succ(n).n // tesi da dimostrare
{}

function nat2Nat(n: nat): Nat
{
    if n == 0 then Zero
    else Succ(nat2Nat(n - 1))
}

function Nat2nat(n: Nat): nat
{
    match n
    case Zero => 0
    case Succ(n) => Nat2nat(n) + 1 // se n è Succ(n), allora il predecessore di n è n, e quindi Nat2nat(n) == Nat2nat(n) + 1, e quindi la postcondizione è soddisfatta
}

lemma Nat2NatCorrespondence(n: Nat, x: nat)
    ensures nat2Nat(Nat2nat(n)) == n
    ensures Nat2nat(nat2Nat(x)) == x
{}

/**
    nat2Nat o Nat2nat = Id_Nat
    Nat2nat o nat2Nat = Id_nat
*/