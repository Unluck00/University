import java.util.*;

class Ex7 {
    /*
    Completate il corpo di
      public static boolean ok(Node q)
    in modo che resti vero se e solo se
      public static double avgPos(Node q)
    non solleverebbe eccezioni né restituisse NaN.
    avgPos restituisce la media dei soli elementi > 0
    e fallisce se non ce ne sono.
    */

    public static boolean ok(Node q) {
        // da completare
    }

    public static double avgPos(Node q) {
        double sum = 0;
        int cnt = 0;
        while (q != null) {
            if (q.getElem() > 0) {
                sum += q.getElem();
                cnt++;
            }
            q = q.getNext();
        }
        return sum / cnt;
    }

    //---- supporto testing ----

    public static Random r = new Random();

    public static int randPos(boolean pos) {
        if (pos) return r.nextInt(20) + 1;
        else     return -r.nextInt(20);
    }

    public static Node list(int n, boolean ris) {
        if (n == 0) return null;
        Node p = null;
        int j = ris ? r.nextInt(n) : n;
        for (int i = 0; i < n; i++) {
            if (i == j) p = new Node(randPos(true), p);
            else        p = new Node(randPos(false), p);
        }
        return p;
    }

    public static void test(int n, boolean ris) {
        Node p = list(n, ris);
        check("len=" + n + " ok=", ok(p), ris);
    }

    public static void test() {
        System.out.println("avgPos");
        System.out.print("lista di lunghezza 0:\n");
        test(0, false);
        for (int i = 1; i < 10; i++) {
            System.out.print("liste di lunghezza " + i + ":\n");
            test(i, true);
            test(i, false);
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
public static boolean ok(Node q) {
    while (q != null) {
        if (q.getElem() > 0) return true;
        q = q.getNext();
    }
    return false;
}
*/
