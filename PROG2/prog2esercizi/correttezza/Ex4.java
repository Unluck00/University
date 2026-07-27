import java.util.*;

class Ex4 {
    /*
    Completate il corpo di
      public static boolean ok(Node q, int k)
    in modo che resti vero se e solo se il metodo
      public static Node get(Node q, int k)
    non solleverebbe eccezioni null-pointer.
    */

    public static boolean ok(Node q, int k) {
        int i = 0;
        while (i < k && q != null) {
            q = q.getNext();
            i++;
        }
        return q != null;
    }

    public static Node get(Node q, int k) {
        int i = 0;
        while (i < k) {
            q = q.getNext();
            i++;
        }
        return q;
    }

    //---- supporto testing ----

    public static String toString(Node p) {
        String s = "";
        while (p != null) {
            s += p.getElem() + " ";
            p = p.getNext();
        }
        return s;
    }

    public static Random r = new Random();

    public static int randVal() {
        return r.nextInt(21) - 10;
    }

    public static Node list(int k, int n, boolean ris) {
        if (n < 0) n = 0;
        Node p = null;
        for (int i = 0; i < n; i++) {
            p = new Node(randVal(), p);
        }
        return p;
    }

    public static void test(int k, int n, boolean ris) {
        // k scelto coerentemente con ris
        if (ris && n > 0) k = r.nextInt(n);
        else if (!ris)   k = r.nextInt(11) + n;
        Node p = list(k, n, ris);
        check("len=" + n + " k=" + k + " ok=", ok(p, k), ris);
    }

    public static void test() {
        System.out.println("get by index");
        for (int n = 0; n < 10; n++) {
            System.out.print("liste di lunghezza " + n + ":\n");
            if (n == 0) test(0, 0, false);
            else {
                test(0, n, true);
                test(0, n, false);
            }
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
public static boolean ok(Node q, int k) {
    int i = 0;
    while (i < k && q != null) {
        q = q.getNext();
        i++;
    }
    return q != null;
}
*/
