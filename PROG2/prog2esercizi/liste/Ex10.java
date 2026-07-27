/*
Salvate tutto in un solo file Ex4.java.
Completate il metodo List l() nella classe List come segue. 
l() conta gli elementi >10 e quelli <=10. 
Supponiamo che gli elementi >10 siano in numero minore di quelli <=10. 
Allora l() restituisce la lista aggiungendo all'inizio un numero di 100 (cento) pari alla differenza (in modo che le due categorie diventino in numero uguale). 
Altrimenti l() restituisce la lista immutata. Non modificate altro che il metodo.
Eseguite il main della classe Ex4 come test per la risposta. Non definite il vostro metodo per casi sui test.
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

public class Ex4 {
    public static void main(String[] args) {
        test();
    }

    public static Random r = new Random();

    public static int r() {
        return r.nextInt(40);
    }

    public static int greater10() {
        int i = r();
        return (i > 10) ? i : greater10();
    }

    public static int lessEqual10() {
        int i = r();
        return (i <= 10) ? i : lessEqual10();
    }

    public static int coin() {
        if (r.nextInt(2) == 0) return greater10();
        else return lessEqual10();
    }

    public static void test() {
        for (int i = 0; i < 10; ++i) test(i);
    }

    public static void test(int n) {
        List l = new List(), ris = new List();
        int a = 0, greater = 0, lessEqual = 0;
        for (int i = 0; i < n; ++i) {
            a = coin();
            l.insertFirst(a);
            ris.insertFirst(a);
            if (a > 10) greater++;
            else lessEqual++;
        }
        ris.l();
        check(l, ris, greater, lessEqual);
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

    public static void check(List l, List ris, int greater, int lessEqual) {
        int extra = Math.max(lessEqual - greater, 0);
        int len = extra + greater + lessEqual;
        String l_string = l.toString(), ris_string = ris.toString();
        System.out.println("lista     : " + l_string);
        System.out.println("risultato : " + ris_string);
        if (countElem(ris_string) != len) {
            System.out.println("*ERRORE*  : dovevi aggiungere " + extra + " elementi ad l");
        } else if (check(ris, extra, 100) == false) {
            System.out.println("*ERRORE*  : i primi " + extra + " elementi devono essere 100");
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
    int greater10 = 0;
    int lessEqual10 = 0;
    Node current = first;
    while (current != null) {
        int elem = current.getElem();
        if (elem > 10) greater10++;
        else lessEqual10++;
        current = current.getNext();
    }

    if (greater10 < lessEqual10) {
        int diff = lessEqual10 - greater10;
        for (int i = 0; i < diff; i++) {
            insertFirst(100);
        }
    }
    return this;
}
*/