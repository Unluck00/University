/*
Salvate tutto in un solo file Ex1.java.
Completate il metodo List l() nella classe List come segue. 
l() conta gli elementi positivi (>=0) e quelli negativi (<0). 
Supponiamo che gli elementi positivi siano in numero minore di quelli negativi. 
Allora l() restituisce la lista aggiungendo all'inizio un numero di zeri (0) pari alla differenza (in modo che positivi e negativi diventino in numero uguale). 
Altrimenti l() restituisce la lista immutata. Non modificate altro che il metodo.
Eseguite il main della classe Ex1 come test per la risposta. Non definite il vostro metodo per casi sui test.
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

public class Ex9 {
    public static void main(String[] args) {
        test();
    }

    public static Random r = new Random();

    public static int r() {
        return r.nextInt(11) - 5;
    }

    public static int positive() {
        int i = r();
        return Math.max(i, 0);
    }

    public static int negative() {
        int i = r();
        return Math.min(i, -1);
    }

    public static int coin() {
        if (r.nextInt(2) == 0) return positive();
        else return negative();
    }

    public static void test() {
        for (int i = 0; i < 10; ++i) test(i);
    }

    public static void test(int n) {
        List l = new List(), ris = new List();
        int a = 0, positive = 0, negative = 0;
        for (int i = 0; i < n; ++i) {
            a = coin();
            l.insertFirst(a);
            ris.insertFirst(a);
            if (a >= 0) ++positive;
            else ++negative;
        }
        ris.l();
        check(l, ris, positive, negative);
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

    public static void check(List l, List ris, int positive, int negative) {
        int extra = Math.max(negative - positive, 0);
        int len = extra + positive + negative;
        String l_string = l.toString(), ris_string = ris.toString();
        System.out.println("lista     : " + l_string);
        System.out.println("risultato : " + ris_string);
        if (countElem(ris_string) != len) {
            System.out.println("*ERRORE*  : dovevi aggiungere " + extra + " elementi ad l");
        } else if (check(ris, extra, 0) == false) {
            System.out.println("*ERRORE*  : i primi " + extra + " elementi devono essere 0");
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
    int positive = 0;
    int negative = 0;
    Node current = first;
    while (current != null) {
        int elem = current.getElem();
        if (elem >= 0) positive++;
        else negative++;
        current = current.getNext();
    }

    if (positive < negative) {
        int diff = negative - positive;
        for (int i = 0; i < diff; i++) {
            insertFirst(0);
        }
    }
    return this;
}
*/