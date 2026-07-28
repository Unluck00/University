/**
    Inductive data types
 */

datatype Color = Blue | Yellow | Green | Red

datatype Nat = Zero | Suc (Pred: Nat)

// Zero, Suc(Zero), Suc(Suc(Zero)), ...

function Nat2nat (x: Nat): nat
{
    match x
    case Zero => 0
    case Suc(y) => 1 + Nat2nat(y)
}

function nat2Nat (n: nat): Nat
{
    if n == 0 then Zero
    else Suc (nat2Nat(n - 1))
}

lemma Nat2natCorrespondence(x: Nat, n: nat)
    ensures nat2Nat(Nat2nat(x)) == x
    ensures Nat2nat(nat2Nat(n)) == n
{}

/*

Add(x, y) is defined by primitive recursion over y

Add(x, Zero)   = x
Add(x, Suc(y)) = Suc(Add(x, y))

*/

function Add(x: Nat, y: Nat): Nat
{
    match y
    case Zero   => x
    case Suc(z) => Suc(Add(x, z))
}

/**
                Add
    Nat x Nat --------> Nat
        |                |
      f |                | g
        V        +       V
    nat x nat --------> nat

    f = Nat2nat x Nat2nat,  g = Nat2nat

    g o Add = + o f -- the diagram commutes
*/

lemma {:induction false} AddCorrect(x: Nat, y: Nat)
    ensures Nat2nat(Add(x, y)) == Nat2nat(x) + Nat2nat(y)
{
    // proof by induction over y
    match y
    case Zero => 
    calc {
        Nat2nat(Add(x, y));
    ==  Nat2nat(x);             // by definition of Add
    ==  Nat2nat(x) + 0;
    ==  Nat2nat(x) + Nat2nat(y); // since y == Zero
    }
    case Suc(z) =>
    calc {
        Nat2nat(Add(x, Suc(z))); // since y == Suc(z)
    ==  Nat2nat(Suc(Add(x, z))); // by definition of Add
    ==  1 + Nat2nat(Add(x, z));  // by definition of Nat2nat
    ==  { AddCorrect(x, z);
          assert Nat2nat(Add(x, z)) == Nat2nat(x) + Nat2nat(z); }  // by induction
        1 + Nat2nat(x) + Nat2nat(z);
    }
}

// Exercise: prove that Add is commutative

lemma AddCommutative (x: Nat, y: Nat)
    ensures Add(x, y) == Add(y, x)
{}