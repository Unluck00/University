import java.util.*;

class Ex6 {
    /*
    Completate il corpo di
      public static boolean ok(Node q, int x)
    in modo che resti vero se e solo se
    chiamare
      public static int indexOf(Node q, int x)
    non solleverebbe eccezioni.
    */

    public static boolean ok(Node q, int x) {
        // da completare
    }

    public static int indexOf(Node q, int x) {
        int idx = 0;
        while (q.getElem() != x) {
            q = q.getNext();
            idx++;
        }
        return idx;
    }

    //---- supporto testing ----

    public static Random r = new Random();

    public static int randNotEq(int x) {
        int y;
        do { y = r.nextInt(11) - 5; }
        while (y == x);
        return y;
    }

    public static int propVal(int x, boolean eq) {
        return eq ? x : randNotEq(x);
    }

    public static Node list(int x, int n, boolean ris) {
        if (n == 0) return null;
        Node p = null;
        int j = ris ? r.nextInt(n) : n;
        for (int i = 0; i < n; i++) {
            if (i == j) p = new Node(propVal(x, true), p);
            else        p = new Node(propVal(x, false), p);
        }
        return p;
    }

    public static void test(int x, int n, boolean ris) {
        Node p = list(x, n, ris);
        check("x=" + x + " p=" + toString(p) + " ok=", ok(p, x), ris);
    }

    public static String toString(Node p) {
        String s = "";
        while (p != null) {
            s += p.getElem() + " ";
            p = p.getNext();
        }
        return s;
    }

    public static void test() {
        System.out.print("indexOf\nlista di lunghezza 0:\n");
        test(0, 0, false);
        for (int i = 1; i < 10; i++) {
            System.out.print("liste di lunghezza " + i + ":\n");
            int x = r.nextInt(11) - 5;
            test(x, i, true);
            test(x, i, false);
        }
    }

    public static void check(String s, boolean a, boolean b) {
        if (a == b) System.out.println(s + a + " OK");
        else         System.out.println(s + a + " *EXPECTED* " + b);
    }

    public static void main(String[] args) {
        test();
    }
}

class Node {
    private int elem;
    private Node next;
    public Node(int elem, Node next) { this.elem = elem; this.next = next; }
    public int getElem() { return elem; }
    public Node getNext() { return next; }
    public void setElem(int e) { elem = e; }
    public void setNext(Node n) { next = n; }
}

// soluzione professore:
/*
public static boolean ok(Node q, int x) {
    while (q != null) {
        if (q.getElem() == x) return true;
        q = q.getNext();
    }
    return false;
}
*/
