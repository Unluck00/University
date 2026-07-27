/* 
Completate il metodo void f() in List, in modo che f() elimini dalla lista tutti gli elementi pari.
Se non ce ne sono f() lascia la lista invariata. Non modificate altro che il metodo.
 */

class List {

    private Node first;

    public List() {
        first = null;
    }

    public void f() {
  
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

public class Ex2{
  public static void main(String[] args) {
    testEmptyList();
    testAllEven();
    testAllOdd();
    testMixed();
    testSingleEven();
    testSingleOdd();
    testConsecutiveEvens();
    testConsecutiveOdds();
  }

  static void testEmptyList() {
    List l = new List();
    l.f();
    assert l.toString().equals("") : "Failed on empty list";
    System.out.println("testEmptyList passed");
  }

  static void testAllEven() {
    List l = new List();
    l.insertFirst(4);
    l.insertFirst(2);
    l.insertFirst(0);
    l.f();
    assert l.toString().equals("") : "Failed on all even";
    System.out.println("testAllEven passed");
  }

  static void testAllOdd() {
    List l = new List();
    l.insertFirst(5);
    l.insertFirst(3);
    l.insertFirst(1);
    l.f();
    assert l.toString().equals("1, 3, 5") : "Failed on all odd";
    System.out.println("testAllOdd passed");
  }

  static void testMixed() {
    List l = new List();
    l.insertFirst(6);
    l.insertFirst(5);
    l.insertFirst(4);
    l.insertFirst(3);
    l.insertFirst(2);
    l.insertFirst(1);
    l.f();
    assert l.toString().equals("1, 3, 5") : "Failed on mixed";
    System.out.println("testMixed passed");
  }

  static void testSingleEven() {
    List l = new List();
    l.insertFirst(2);
    l.f();
    assert l.toString().equals("") : "Failed on single even";
    System.out.println("testSingleEven passed");
  }

  static void testSingleOdd() {
    List l = new List();
    l.insertFirst(3);
    l.f();
    assert l.toString().equals("3") : "Failed on single odd";
    System.out.println("testSingleOdd passed");
  }

  static void testConsecutiveEvens() {
    List l = new List();
    l.insertFirst(8);
    l.insertFirst(6);
    l.insertFirst(4);
    l.insertFirst(3);
    l.insertFirst(2);
    l.insertFirst(1);
    l.f();
    assert l.toString().equals("1, 3") : "Failed on consecutive evens";
    System.out.println("testConsecutiveEvens passed");
  }

  static void testConsecutiveOdds() {
    List l = new List();
    l.insertFirst(9);
    l.insertFirst(7);
    l.insertFirst(5);
    l.insertFirst(4);
    l.insertFirst(2);
    l.insertFirst(1);
    l.f();
    assert l.toString().equals("1, 5, 7, 9") : "Failed on consecutive odds";
    System.out.println("testConsecutiveOdds passed");
  }
}

//soluzione
/*
  public void f(){
    while (first != null && first.getElem() < 0) {
      first = first.getNext();
    }
    if (first == null) return;
    Node prev = first;
    Node curr = first.getNext();
    while (curr != null) {
      if (curr.getElem() < 0) {
        prev.setNext(curr.getNext());
        curr = prev.getNext();
      } else {
        prev = curr;
        curr = curr.getNext();
      }
    }
  }
*/
