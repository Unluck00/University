/*
Salvate tutto in un solo file Ex2.java.
Completate il metodo List l() nella classe List come segue. 
l() conta gli elementi pari e quelli dispari. 
Supponiamo che gli elementi pari siano in numero minore di quelli dispari. 
Allora l() restituisce la lista aggiungendo all'inizio un numero di 2 (due) pari alla differenza (in modo che pari e dispari diventino in numero uguale). 
Altrimenti l() restituisce la lista immutata. Non modificate altro che il metodo.
Eseguite il main della classe Ex2 come test per la risposta. Non definite il vostro metodo per casi sui test.
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

public class Ex6 {
    public static void main(String[] args) {
        test();
    }

    public static Random r = new Random();

    public static int r() {
        return r.nextInt(20) - 10;
    }

    public static int even() {
        int i = r();
        return (i % 2 == 0) ? i : even();
    }

    public static int odd() {
        int i = r();
        return (i % 2 != 0) ? i : odd();
    }

    public static int coin() {
        if (r.nextInt(2) == 0) return even();
        else return odd();
    }

    public static void test() {
        for (int i = 0; i < 10; ++i) test(i);
    }

    public static void test(int n) {
        List l = new List(), ris = new List();
        int a = 0, even = 0, odd = 0;
        for (int i = 0; i < n; ++i) {
            a = coin();
            l.insertFirst(a);
            ris.insertFirst(a);
            if (a % 2 == 0) even++;
            else odd++;
        }
        ris.l();
        check(l, ris, even, odd);
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

    public static void check(List l, List ris, int even, int odd) {
        int extra = Math.max(odd - even, 0);
        int len = extra + even + odd;
        String l_string = l.toString(), ris_string = ris.toString();
        System.out.println("lista     : " + l_string);
        System.out.println("risultato : " + ris_string);
        if (countElem(ris_string) != len) {
            System.out.println("*ERRORE*  : dovevi aggiungere " + extra + " elementi ad l");
        } else if (check(ris, extra, 2) == false) {
            System.out.println("*ERRORE*  : i primi " + extra + " elementi devono essere 2");
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
    int even = 0;
    int odd = 0;
    Node current = first;
    while (current != null) {
        int elem = current.getElem();
        if (elem % 2 == 0) even++;
        else odd++;
        current = current.getNext();
    }

    if (even < odd) {
        int diff = odd - even;
        for (int i = 0; i < diff; i++) {
            insertFirst(2);
        }
    }
    return this;
}
*/