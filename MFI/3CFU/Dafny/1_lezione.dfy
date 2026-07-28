function fib(n: nat): nat
{
if n == 0 then 0
else if n == 1 then 1
else fib(n-1) + fib(n-2)
}

// il linguaggio dafny consente di specificare pre e post condizioni
// per le funzioni, e invarianti per le classi

// il linguaggio dafny serve per scrivere programmi corretti, e permette di specificare le proprietà che devono essere vere per il programma, e di verificare che queste proprietà siano effettivamente vere
// il linaguaggio dafny non è un linguaggio di programmazione, ma un linguaggio di verifica funzionale, che permette di scrivere specifiche formali dei programmi, e di verificare che i programmi soddisfino queste specifiche
// quindi se si vuole scrivere ed eseguire un programma in dafny, è necessario scrivere una specifica formale del programma, e poi verificare che il programma soddisfi questa specifica, utilizzando il sistema di verifica di dafny (es. Dafny verifier)

// linguaggio object oriented e permette di definire metodi stand alone, cioè non associati ad una classe, e di definire classi con metodi associati (è deterministico, cioè se un programma è verificato rispetto alla specifica, allora il programma è corretto)
// linguaggi funzionali e permette di definire funzioni pure, cioè funzioni che non hanno effetti collaterali e che restituiscono sempre lo stesso risultato per gli stessi argomenti

method SquareDiff(x: real, y: real) // il method permette di definire un metodo che non restituisce un valore, ma che può modificare lo stato del programma, mentre x e y sono i parametri di input
  returns (z: real) // il returns permette di specificare il tipo di valore restituito dal metodo, in questo caso un numero reale
{
  z := (x + y) * (x - y); // il := è l'operatore di assegnazione, che permette di assegnare un valore a una variabile, in questo caso z
}

// la variabile real è un tipo di dato che rappresenta i numeri reali, mentre nat rappresenta i numeri naturali, cioè i numeri interi non negativi

// il comando ensure permette di specificare una post condizione, cioè una condizione che deve essere vera alla fine dell'esecuzione del metodo, in questo caso che z sia maggiore o uguale a zero
// il comando requires true per specificare che non ci sono pre condizioni, cioè che il metodo può essere chiamato con qualsiasi valore di x e y
method SquareDiff2(x: real, y: real) returns (z: real)
  requires true // pre condizione, in questo caso non ci sono pre condizioni, cioè il metodo può essere chiamato con qualsiasi valore di x e y
  ensures  z == x * x - y * y // per specificare che z deve essere uguale a (x + y) * (x - y); post condizione
{
  // z := (x + y) * (x - y);

  var w : real; // per dichiarare una variabile locale al metodo, in questo caso z
  w := (x + y) * (x - y); // per assegnare un valore alla variabile z, in questo caso (x + y) * (x - y)
  assert w == x * x - y * y; // per specificare che w deve essere uguale a (x + y) * (x - y)
  z := w; // per assegnare il valore di w a z, in questo caso z deve essere uguale a (x + y) * (x - y); si chiama anche stato del programma, cioè l'insieme delle variabili e dei loro valori in un punto specifico del programma
}

/* 
    true ==> wp(z := (x + y) * (x - y), z == x * x - y * y) 
    con wp che rappresenta la weakest precondition, cioè la pre condizione più debole che garantisce che la post condizione sia vera alla fine dell'esecuzione del metodo, in questo caso che z sia uguale a (x + y) * (x - y)
*/

// il comando assert permette di specificare una condizione che deve essere vera in un punto specifico del programma, in questo caso che z sia uguale a (x + y) * (x - y)


method Triple2(x: int) returns (r: int) {

  // non c'è bisogno di definire requires x >= 0, perché il metodo è verificato rispetto alla specifica, e quindi se x è negativo, il metodo non è verificato, e quindi non è possibile chiamare il metodo con un valore di x negativo
  // se si vuole specificare che x deve essere maggiore o uguale a zero, è necessario aggiungere la pre condizione requires x >= 0, e poi verificare che il metodo sia verificato rispetto alla specifica, cioè che restituisca sempre un valore maggiore o uguale a zero
  // es. method Triple2(x: int) returns (r: int)
  // requires x >= 0
  // ensures r == 3 * x
  // {
  // r := 3 * x;
  // }\

 var y := 2 * x;
 assert false; // l'assert false permette di specificare che il punto in cui si trova l'assert è irraggiungibile, cioè che non è possibile raggiungere quel punto durante l'esecuzione del programma, e quindi che tutte le affermazioni che seguono l'assert sono vere, perché non è possibile raggiungere quel punto se non si verifica una contraddizione
 // interrotto il cammino e quindi tutte le affermazioni che seguono l'assert sono vere, perché non è possibile raggiungere quel punto se non si verifica una contraddizione
 r := x + y;
 assert r == 3 * x + 1; // it holds since this point is unreachable
}

method Triple4(x: int) returns (r: int)
 requires x % 2 == 0
 ensures r == 3 * x
{
 var y := x / 2; // integer division is rounded
 r := 6 * y;
}


// le funzioni pure sono funzioni che non hanno effetti collaterali, cioè non modificano lo stato del programma, e che restituiscono sempre lo stesso risultato per gli stessi argomenti
// possono essere utilizzate sia per calcolare un valore che per specificare una proprietà che deve essere vera per il programma, e possono essere utilizzate anche nelle pre e post condizioni dei metodi
// quindi la differenza tra un metodo e una funzione è che un metodo può modificare lo stato del programma, mentre una funzione pure non può modificare lo stato del programma, e restituisce sempre lo stesso risultato per gli stessi argomenti

function Abs(x: int): int
 ensures Abs(x) >= 0
 ensures Abs(x) == x || Abs(x) == - x
 // multiple ensures clauses are interpreted as the
 // conjunction of the respective predicates
 // (similarly for multiple requires)
{
 if x < 0 then - x else x
}

// mostra il metodo AbsMet che implementa la funzione Abs, e che è verificato rispetto alla specifica della funzione Abs
method AbsMet(x: int) returns (y: int)
 ensures y == Abs(x)
{
 if x < 0 {y := - x; }
 else { y := x; }
 // differently than in case of expressions,
 // the command if-then-else
 // requires using blocks {...}
}

// il metodo TestAbs verifica che la funzione Abs sia corretta, cioè che restituisca sempre un valore maggiore o uguale a zero, e che restituisca x o - x a seconda del segno di x
method TestAbs()
{
 var r := - 123;
 ghost var p := r; // la parola chiave ghost permette di dichiarare una variabile che non fa parte dello stato del programma, e che viene utilizzata solo per la verifica, e che non viene eseguita durante l'esecuzione del programma; il compilatore trascura tutto quello che c'è con ghost
 r := AbsMet(r);
 assert r == - p;
}