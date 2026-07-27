/* 
Completate il metodo void f() in List, in modo che f() elimini dalla lista tutti gli elementi divisibili per 3.
Se non ce ne sono f() lascia la lista invariata. Non modificate altro che il metodo.
 */

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
        } else {
            --i;
            Node p = first;
            while (i > 0) {
                p = p.getNext();
                --i;
            }
            p.setNext(new Node(elem, p.getNext()));
        }
    }

    @Override
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
public class Ex3 {
    public static void main(String[] args) {
        // Test 1: Lista vuota
        List l1 = new List();
        l1.f();
        System.out.println("Test 1 (vuota):");
        System.out.println("  Ottenuto: \"\"");
        System.out.println("  input: \"" + l1.toString() + "\"\n");

        // Test 2: Nessun elemento divisibile per 3
        List l2 = new List();
        l2.insertFirst(2);
        l2.insertFirst(5);
        l2.insertFirst(7);
        l2.f();
        System.out.println("Test 2 (nessun divisibile per 3):");
        System.out.println("  Ottenuto: \"7, 5, 2\"");
        System.out.println("  input: \"" + l2.toString() + "\"\n");

        // Test 3: Tutti elementi divisibili per 3
        List l3 = new List();
        l3.insertFirst(9);
        l3.insertFirst(6);
        l3.insertFirst(3);
        l3.f();
        System.out.println("Test 3 (tutti divisibili per 3):");
        System.out.println("  Ottenuto: \"\"");
        System.out.println("  input: \"" + l3.toString() + "\"\n");

        // Test 4: Alcuni elementi divisibili per 3 (in mezzo)
        List l4 = new List();
        l4.insertFirst(8);
        l4.insertFirst(6);
        l4.insertFirst(5);
        l4.insertFirst(3);
        l4.insertFirst(2);
        l4.f();
        System.out.println("Test 4 (alcuni divisibili per 3):");
        System.out.println("  Ottenuto: \"2, 5, 8\"");
        System.out.println("  input: \"" + l4.toString() + "\"\n");

        // Test 5: Primo elemento divisibile per 3
        List l5 = new List();
        l5.insertFirst(4);
        l5.insertFirst(3);
        l5.insertFirst(1);
        l5.f();
        System.out.println("Test 5 (primo divisibile per 3):");
        System.out.println("  Ottenuto: \"1, 4\"");
        System.out.println("  input: \"" + l5.toString() + "\"\n");

        // Test 6: Ultimo elemento divisibile per 3
        List l6 = new List();
        l6.insertFirst(9);
        l6.insertFirst(7);
        l6.insertFirst(5);
        l6.f();
        System.out.println("Test 6 (ultimo divisibile per 3):");
        System.out.println("  Ottenuto: \"5, 7\"");
        System.out.println("  input: \"" + l6.toString() + "\"\n");

        // Test 7: Elementi alternati divisibili per 3
        List l7 = new List();
        l7.insertFirst(12);
        l7.insertFirst(11);
        l7.insertFirst(9);
        l7.insertFirst(8);
        l7.insertFirst(6);
        l7.insertFirst(5);
        l7.f();
        System.out.println("Test 7 (alternati):");
        System.out.println("  Ottenuto: \"5, 8, 11\"");
        System.out.println("  input: \"" + l7.toString() + "\"\n");
    }
}



//soluzione
/*
  public void f(){
    while (first != null && first.getElem() % 3 == 0) {
      first = first.getNext();
    }
    if (first == null) return;
    Node prev = first;
    Node curr = first.getNext();
    while (curr != null) {
      if (curr.getElem() % 3 == 0) {
        prev.setNext(curr.getNext());
        curr = prev.getNext();
      } else {
        prev = curr;
        curr = curr.getNext();
      }
    }
  }
 */
