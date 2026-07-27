/* 
Completate il metodo void f() in List, in modo che f() elimini dalla lista tutti gli elementi uguali a zero.
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

public class Ex5 {

    public static void main(String[] args) {
        test(5);
    }

    public static Random r = new Random();

    public static int val() {
        return r.nextInt(11) - 5;
    }

    public static int nonZero() {
        int v = val();
        if (v != 0) {
            return v;
        } else {
            return nonZero();
        }
    }

    public static int zero() {
        return 0;
    }

    public static void listNonZero(int length, List a, List b) {
        int n;
        while (length > 0) {
            n = nonZero();
            a.insertFirst(n);
            b.insertFirst(n);
            --length;
        }
    }

    public static void testNonZero(int length) {
        List a = new List(), b = new List();
        listNonZero(length, a, b);
        check(a, b.toString());
    }

    public static void testZero(int length) {
        List a = new List(), b = new List();
        listNonZero(length, a, b);
        int i = r.nextInt(length + 1);
        a.insert(i, zero());
        i = r.nextInt(length + 2);
        a.insert(i, zero());
        i = r.nextInt(length + 3);
        a.insert(i, zero());
        check(a, b.toString());
    }

    public static void test(int n) {
        for (int i = 0; i < n; ++i) {
            testNonZero(i);
            testZero(i);
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
    while (first != null && first.getElem() == 0) {
      first = first.getNext();
    }
    if (first == null) return;
    Node prev = first;
    Node curr = first.getNext();
    while (curr != null) {
      if (curr.getElem() == 0) {
        prev.setNext(curr.getNext());
        curr = prev.getNext();
      } else {
        prev = curr;
        curr = curr.getNext();
      }
    }
  }
*/
