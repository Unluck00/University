/* 
Completate il metodo void f() in List, in modo che f() elimini dalla lista tutti gli elementi maggiori di 10.
Se non ce ne sono f() lascia la lista invariata. Non modificate altro che il metodo.
 */

import java.util.Random;

class List {

    private Node first;

    public List() {
        first = null;
    }

    public void f() {
        //

    }

    public void insertFirst(int elem) {
        first = new Node(elem, first);
    }

    public void insert(int i, int elem) {
        if (i == 0) {
            insertFirst(elem); 
        }else {
            --i;
            Node p = first;
            while (i > 0) {
                p = p.getNext();
                --i;
            }
            p.setNext(new Node(elem, p.getNext()));
        }
    }

    public String toString() {
        String s = "";
        for (Node p = first; p != null; p = p.getNext()) {
            if (p != first) {
                s += ", ";
            }
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
        test(5);
    }

    public static Random r = new Random();

    public static int val() {
        return r.nextInt(31) - 10; // Range: -10 to 20
    }

    public static int le10() {
        int v = val();
        if (v <= 10) {
            return v;
        } else {
            return le10();
        }
    }

    public static int gt10() {
        int v = val();
        if (v > 10) {
            return v;
        } else {
            return gt10();
        }
    }

    public static void listLe10(int length, List a, List b) {
        int n;
        while (length > 0) {
            n = le10();
            a.insertFirst(n);
            b.insertFirst(n);
            --length;
        }
    }

    public static void testLe10(int length) {
        List a = new List(), b = new List();
        listLe10(length, a, b);
        check(a, b.toString());
    }

    public static void testGt10(int length) {
        List a = new List(), b = new List();
        listLe10(length, a, b);
        int i = r.nextInt(length + 1);
        a.insert(i, gt10());
        i = r.nextInt(length + 2);
        a.insert(i, gt10());
        i = r.nextInt(length + 3);
        a.insert(i, gt10());
        check(a, b.toString());
    }

    public static void test(int n) {
        for (int i = 0; i < n; ++i) {
            testLe10(i);
            testGt10(i);
        }
    }

    public static void check(List l, String atteso) {
        System.out.println("lista    : " + l);
        l.f();
        System.out.println("ottenuto : " + l);
        System.out.println("atteso   : " + atteso);
        System.out.println(l.toString().equals(atteso) ? "=== OK" : "=== *ERRORE*");
        System.out.println();
    }
}

//soluzione
/*
  public void f(){
    while (first != null && first.getElem() > 10) {
      first = first.getNext();
    }
    if (first == null) return;
    Node prev = first;
    Node curr = first.getNext();
    while (curr != null) {
      if (curr.getElem() > 10) {
        prev.setNext(curr.getNext());
        curr = prev.getNext();
      } else {
        prev = curr;
        curr = curr.getNext();
      }
    }
  }
*/
