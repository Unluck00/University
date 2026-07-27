/*
Salvate tutto in un solo file Ex3.java.
Completate il metodo List l() nella classe List come segue. 
l() conta gli elementi multipli di 3 e quelli non multipli di 3. 
Supponiamo che i multipli di 3 siano in numero minore dei non multipli. 
Allora l() restituisce la lista aggiungendo all'inizio un numero di 9 (nove) pari alla differenza (in modo che le due categorie diventino in numero uguale). 
Altrimenti l() restituisce la lista immutata. Non modificate altro che il metodo.
Eseguite il main della classe Ex3 come test per la risposta. Non definite il vostro metodo per casi sui test.
*/

import java.util.*;

class List {
    private Node first;

    public List() {
        first = null;
    }

    public void insertFirst(int elem) {
        first = new Node(elem, first);
    }

    public List l() {
        // DA COMPLETARE
    }

    public Node getFirst() {
        return first;
    }

    public String toString() {
        String s = "";
        for (Node p = first; p != null; p = p.getNext()) {
            if (p != first) s += ", ";
            s += p.getElem();
        }
        return s;
    }
}

class Node {
    private int elem;
    private Node next;

    Node(int elem, Node next) {
        this.elem = elem;
        this.next = next;
    }

    int getElem() {
        return elem;
    }

    void setElem(int elem) {
        this.elem = elem;
    }

    Node getNext() {
        return next;
    }

    void setNext(Node next) {
        this.next = next;
    }
}

public class Ex8 {
    public static void main(String[] args) {
        test();
    }

    public static Random r = new Random();

    public static int r() {
        return r.nextInt(30) - 15;
    }

    public static int multiple3() {
        int i = r();
        return (i % 3 == 0) ? i : multiple3();
    }

    public static int notMultiple3() {
        int i = r();
        return (i % 3 != 0) ? i : notMultiple3();
    }

    public static int coin() {
        if (r.nextInt(2) == 0) return multiple3();
        else return notMultiple3();
    }

    public static void test() {
        for (int i = 0; i < 10; ++i) test(i);
    }

    public static void test(int n) {
        List l = new List(), ris = new List();
        int a = 0, mult3 = 0, notMult3 = 0;
        for (int i = 0; i < n; ++i) {
            a = coin();
            l.insertFirst(a);
            ris.insertFirst(a);
            if (a % 3 == 0) mult3++;
            else notMult3++;
        }
        ris.l();
        check(l, ris, mult3, notMult3);
    }

    public static int countElem(String s) {
        if (s.equals("")) return 0;
        int countElem = 1, i = 0, len = s.length();
        while (i < len) {
            if (s.charAt(i) == ',') ++countElem;
            ++i;
        }
        return countElem;
    }

    public static void check(List l, List ris, int mult3, int notMult3) {
        int extra = Math.max(notMult3 - mult3, 0);
        int len = extra + mult3 + notMult3;
        String l_string = l.toString(), ris_string = ris.toString();
        System.out.println("lista     : " + l_string);
        System.out.println("risultato : " + ris_string);
        if (countElem(ris_string) != len) {
            System.out.println("*ERRORE*  : dovevi aggiungere " + extra + " elementi ad l");
        } else if (check(ris, extra, 9) == false) {
            System.out.println("*ERRORE*  : i primi " + extra + " elementi devono essere 9");
        } else if (!ris_string.endsWith(l_string)) {
            System.out.println("*ERRORE*  : risultato deve terminare con lista");
        } else {
            System.out.println("   ========OK========");
        }
    }

    public static boolean check(List ris, int extra, int expected) {
        Node p = ris.getFirst();
        while (extra > 0) {
            if (p.getElem() != expected) return false;
            p = p.getNext();
            --extra;
        }
        return true;
    }
}

// SOLUZIONE
/*
public List l() {
    int mult3 = 0;
    int notMult3 = 0;
    Node current = first;
    while (current != null) {
        int elem = current.getElem();
        if (elem % 3 == 0) mult3++;
        else notMult3++;
        current = current.getNext();
    }

    if (mult3 < notMult3) {
        int diff = notMult3 - mult3;
        for (int i = 0; i < diff; i++) {
            insertFirst(9);
        }
    }
    return this;
}
*/