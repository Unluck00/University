import java.util.*;

class Ex5 {
    /*
    Completate il corpo di
      public static boolean ok(Node q)
    in modo che resti vero se e solo se
    calcolare
      public static int max(Node q)
    non solleverebbe eccezioni.
    */

    public static boolean ok(Node q) {
        // da completare
    }

    public static int max(Node q) {
        int m = q.getElem();
        while (q != null) {
            m = Math.max(m, q.getElem());
            q = q.getNext();
        }
        return m;
    }

    //---- supporto testing ----

    public static Random r = new Random();

    public static int randVal() {
        return r.nextInt(101) - 50;
    }

    public static Node list(int n) {
        Node p = null;
        for (int i = 0; i < n; i++) {
            p = new Node(randVal(), p);
        }
        return p;
    }

    public static void test(int n, boolean ris) {
        Node p = list(n);
        check("len="+n+" ok=", ok(p), ris);
    }

    public static void test() {
        System.out.println("max elemento");
        for (int n = 0; n < 10; n++) {
            boolean ris = n > 0;
            System.out.print("liste di lunghezza " + n + ":\n");
            test(n, ris);
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
    return q != null;
}
*/
