
import java.util.*;

class Ex3 {

    /*
    Leggete attentamente la definizione del metodo
      public static double ratio(Node q, int x)
    Completate il corpo di
      public static boolean ok(Node q, int x)
    in modo che restituisca vero se e solo se chiamare
    ratio(q,x) non genera eccezioni né restituisce
    valori infinite o NaN.
    Non usate 'catch' ma un semplice controllo.
     */

    public static boolean ok(Node q, int x) {
        while (q != null) {
            if (q.getElem() > x) {
                return true;
            }
            q = q.getNext();
        }
        return false;
    }

    public static double ratio(Node q, int x) {
        int cntLe = 0, cntGt = 0;
        while (q != null) {
            if (q.getElem() <= x) {
                cntLe++; 
            }else {
                cntGt++;
            }
            q = q.getNext();
        }
        return (double) cntLe / cntGt;
    }

    //---- metodi di supporto per il testing ----
    public static String toString(Node p) {
        String s = "";
        while (p != null) {
            s += p.getElem() + " ";
            p = p.getNext();
        }
        return s;
    }

    public static Random r = new Random();

    public static int r(int a) {
        return a + r.nextInt(11) - 5;
    }

    public static int prop(int a, boolean b) {
        int i = r(a);
        if (((i > a) == false) == b) {
            return i; 
        }else {
            return prop(a, b);
        }
    }

    public static Node list(int a, int n, boolean ris) {
        if (n == 0) {
            return null;
        }
        Node p = null;
        int j = ris ? r.nextInt(n) : n;
        for (int i = 0; i < n; ++i) {
            if (i == j) {
                p = new Node(prop(a, false), p); 
            }else {
                p = new Node(prop(a, true), p);
            }
        }
        return p;
    }

    public static void test(int a, int n, boolean ris) {
        Node p = list(a, n, ris);
        check("p = " + toString(p) + " ok(p," + a + ")=",
                ok(p, a), ris);
    }

    public static void test() {
        int a = 0;
        System.out.print("lista di lunghezza 0:\n");
        test(a, 0, false);
        for (int i = 1; i < 10; i++) {
            System.out.print("liste di lunghezza " + i + ":\n");
            a = r.nextInt(11) - 5;
            test(a, i, true);
            test(a, i, false);
        }
    }

    public static void check(String s, boolean a, boolean b) {
        if (a == b) {
            System.out.println(s + a + " OK"); 
        }else {
            System.out.println(s + a + " *EXPECTED* " + b);
        }
    }

    public static void main(String[] args) {
        test();
    }
}

class Node {

    private int elem;
    private Node next;

    public Node(int elem, Node next) {
        this.elem = elem;
        this.next = next;
    }

    public int getElem() {
        return elem;
    }

    public Node getNext() {
        return next;
    }

    public void setElem(int e) {
        elem = e;
    }

    public void setNext(Node n) {
        next = n;
    }
}

// soluzione professore:
/*
public static boolean ok(Node q, int x) {
    while (q != null) {
        if (q.getElem() > x) return true;
        q = q.getNext();
    }
    return false;
}
 */
