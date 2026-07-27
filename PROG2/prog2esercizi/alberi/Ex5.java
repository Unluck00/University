/*
Completate il metodo int q(T x, T z) in <T> e Branch<T> in modo che, in un albero generico con elementi di tipo T,
conti gli elementi elem tali che: (elem < x) e (elem ≠ z).
Usate a.compareTo(b) per confrontare a e b. Supponete che gli elementi dell’albero e x, z non siano null.
Usate new Leaf<T> e new Branch<T>. Se dimenticate la precisazione <T>, Moodle considera la definizione "unchecked or unsafe".
In tal caso, aggiungete <T>. Usate una definizione ricorsiva. Non modificate altro. Non usate "for" né "while".

Fate verificare la risposta a Moodle. Non definite il vostro metodo per casi sui test.
Se la risposta passa il controllo inviate tutto e terminate, altrimenti correggete gli errori.
Normalmente i test devono essere tutti giusti perché la soluzione abbia valore.
*/

import java.util.*;

abstract class Tree<T extends Comparable<T>> {
    public abstract int q(T x, T z);
    public abstract String toString();
}

class Leaf<T extends Comparable<T>> extends Tree<T> {
    public int q(T x, T z) {
        return 0;
    }
    public String toString() {
        return "{}";
    }
}

class Branch<T extends Comparable<T>> extends Tree<T> {
    private T elem;
    private Tree<T> left, right;

    public Branch(T elem, Tree<T> left, Tree<T> right) {
        this.elem = elem;
        this.left = left;
        this.right = right;
    }

    public int q(T x, T z) {
        // da completare
    }

    public String toString() {
        return "{" + elem.toString() + "," + left.toString() + "," + right.toString() + "}";
    }
}

class T implements Comparable<T> {
    public String s;
    public T(String s) { this.s = s; }
    public int compareTo(T y) { return s.compareTo(y.s); }
    public String toString() { return s; }
}

class Ex5 {
    static Random r = new Random();

    static T r() {
        int n = r.nextInt(5);
        return switch (n) {
            case 0 -> new T("a");
            case 1 -> new T("b");
            case 2 -> new T("c");
            case 3 -> new T("d");
            default -> new T("e");
        };
    }

    static T x = new T("c");
    static T z = new T("a");

    static int c(T y) {
        return (y.compareTo(x) < 0 && y.compareTo(z) != 0) ? 1 : 0;
    }

    public static void test() {
        T[] v = {r(), r(), r(), r(), r(), r(), r(), r(), r()};
        Tree<T> l = new Leaf<>();
        Tree<T> t1 = new Branch<>(v[1], l, new Branch<>(v[2], new Branch<>(v[3], l, l), new Branch<>(v[4], l, l)));
        Tree<T> t2 = new Branch<>(v[5], new Branch<>(v[6], new Branch<>(v[7], l, l), new Branch<>(v[8], l, l)), l);
        Tree<T> t = new Branch<>(v[0], t1, t2);

        check("t = " + t.toString() + "\nt.q(x,z) = ",
              t.q(x, z),
              Arrays.stream(v).mapToInt(Ex5::c).sum());
    }

    static <T> void check(String s, T a, T b) {
        if (a.equals(b)) System.out.println(s + a + "  ===OK===");
        else System.out.println(s + a + "  ***EXPECTED*** " + b);
    }

    public static void msg() {
        System.out.println("Conta gli elementi minori di x ma diversi da z.");
    }

      public static void main(String[] args) {
        msg();
        for (int i = 0; i < 5; i++) test();
    }
}