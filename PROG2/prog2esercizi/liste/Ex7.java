/*
Salvate tutto in un solo file Ex5.java.
Completate il metodo List l() nella classe List come segue. 
l() conta gli elementi negativi e quelli non-negativi (>=0). 
Supponiamo che gli elementi negativi siano in numero minore di quelli non-negativi. 
Allora l() restituisce la lista aggiungendo all'inizio un numero di -1 (meno uno) pari alla differenza (in modo che le due categorie diventino in numero uguale). 
Altrimenti l() restituisce la lista immutata. Non modificate altro che il metodo.
Eseguite il main della classe Ex5 come test per la risposta. Non definite il vostro metodo per casi sui test.
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

public class Ex7{
    public static void main(String[] args) {
        test();
    }

    public static Random r = new Random();

    public static int r() {
        return r.nextInt(21) - 10;
    }

    public static int negative() {
        int i = r();
        return (i < 0) ? i : negative();
    }

    public static int nonNegative() {
        int i = r();
        return (i >= 0) ? i : nonNegative();
    }

    public static int coin() {
        if (r.nextInt(2) == 0) return negative();
        else return nonNegative();
    }

    public static void test() {
        for (int i = 0; i < 10; ++i) test(i);
    }

    public static void test(int n) {
        List l = new List(), ris = new List();
        int a = 0, negative = 0, nonNegative = 0;
        for (int i = 0; i < n; ++i) {
            a = coin();
            l.insertFirst(a);
            ris.insertFirst(a);
            if (a < 0) negative++;
            else nonNegative++;
        }
        ris.l();
        check(l, ris, negative, nonNegative);
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

    public static void check(List l, List ris, int negative, int nonNegative) {
        int extra = Math.max(nonNegative - negative, 0);
        int len = extra + negative + nonNegative;
        String l_string = l.toString(), ris_string = ris.toString();
        System.out.println("lista     : " + l_string);
        System.out.println("risultato : " + ris_string);
        if (countElem(ris_string) != len) {
            System.out.println("*ERRORE*  : dovevi aggiungere " + extra + " elementi ad l");
        } else if (check(ris, extra, -1) == false) {
            System.out.println("*ERRORE*  : i primi " + extra + " elementi devono essere -1");
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
    int negative = 0;
    int nonNegative = 0;
    Node current = first;
    while (current != null) {
        int elem = current.getElem();
        if (elem < 0) negative++;
        else nonNegative++;
        current = current.getNext();
    }

    if (negative < nonNegative) {
        int diff = nonNegative - negative;
        for (int i = 0; i < diff; i++) {
            insertFirst(-1);
        }
    }
    return this;
}
*/