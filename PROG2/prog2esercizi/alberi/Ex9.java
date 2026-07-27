/* 
Salvate tutto in un solo file Ex9.java.
Completate il metodo Tree <T> m(T x) in Leaf <T> e Branch <T> come segue.

1) m(x) inverte i sottoalberi sinistro e destro di ogni nodo il cui elemento è minore o uguale a x.
2) Se un nodo non soddisfa la condizione, il metodo lascia il nodo invariato e procede ricorsivamente sui sottoalberi.
3) Terminate restituendo l'indirizzo "this" dell'albero (l'indirizzo non cambia mai).
4) Usate a.compareTo(b) per confrontare a,b in T e supponete che gli elementi dell'albero e x non siano null.
5) Usate una definizione ricorsiva. Non modificate altro. Non usate "for" nè "while".

Eseguite il main della classe Ex9 come test per la risposta. Non definite il vostro metodo per casi sui test.
*/

import java.util.*;

abstract class Tree<T extends Comparable<T>> {
    public abstract Tree<T> m(T x);
    public abstract String toString();
}

class Leaf<T extends Comparable<T>> extends Tree<T> {
    public Tree<T> m(T x) {
        // Codice per completare il metodo
    }
    public String toString() { return "{}"; }
}

class Branch<T extends Comparable<T>> extends Tree<T> {
    private T elem;
    private Tree<T> left, right;
    public Branch(T elem, Tree<T> left, Tree<T> right) {
        this.elem = elem;
        this.left = left;
        this.right = right;
    }

    public Tree<T> m(T x) {
        // Codice per completare il metodo
    }

    public String toString() {
        return "{" + elem.toString() + "," + left.toString() + "," + right.toString() + "}";
    }
}

class Ex9 {
    public static Random r = new Random();

    public static T r() {
        int n = r.nextInt(5);
        if (n == 0) return new T("a");
        else if (n == 1) return new T("b");
        else if (n == 2) return new T("c");
        else if (n == 3) return new T("d");
        else return new T("e");
    }

    static T x = new T("c");

    public static Tree<T> c(T elem, Tree<T> left, Tree<T> right) {
        return new Branch<T>(elem, left, right);
    }

    public static Tree<T> d(T elem, Tree<T> left, Tree<T> right) {
        if (elem.compareTo(x) <= 0) {
            return new Branch<T>(elem, right, left);
        } else {
            return new Branch<T>(elem, left, right);
        }
    }

    public static void test() {
        T[] v = {r(), r(), r(), r(), r(), r(), r(), r(), r()};
        Tree<T> l = new Leaf<>();

        Tree<T> t1 = c(v[1], l, c(v[2], c(v[3], l, l), c(v[4], l, l)));
        Tree<T> t2 = c(v[5], c(v[6], c(v[7], l, l), c(v[8], l, l)), l);

        Tree<T> ris1 = d(v[1], l, d(v[2], d(v[3], l, l), d(v[4], l, l)));
        Tree<T> ris2 = d(v[5], d(v[6], d(v[7], l, l), d(v[8], l, l)), l);

        check("t1 = " + t1.toString() + "\n t1.m(c)=", t1.m(x).toString(), ris1.toString());
        check("t2 = " + t2.toString() + "\n t2.m(c)=", t2.m(x).toString(), ris2.toString());
    }

    public static void check(String s, String a, String b) {
        if (a.equals(b)) System.out.println(s + a + " OK");
        else System.out.println(s + a + " *EXPECTED* " + b);
    }

    static void msg() {
        System.out.println("Alberi nella forma leaf={}, branch = {elem,left,right}");
        System.out.println("m inverte i sottoalberi dei nodi con elemento <= c");
    }

    public static void main(String[] args) {
        msg(); test(); test(); test();
    }
}

class T implements Comparable<T> {
    public String s;
    public T(String s) { this.s = s; }
    public int compareTo(T y) { return s.compareTo(y.s); }
    public String toString() { return s; }
}

/*
SOLUZIONE:
Per Leaf<T>:
public Tree<T> m(T x) {
    return this;
}

Per Branch<T>:
public Tree<T> m(T x) {
    if (elem.compareTo(x) <= 0) {
        Tree<T> temp = left;
        left = right;
        right = temp;
    }
    left = left.m(x);
    right = right.m(x);
    return this;
}
*/