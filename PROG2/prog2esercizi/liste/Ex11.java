/*
Salvate tutto in un solo file Ex11.java.

1) Completate il corpo del metodo Node concat(Node p, Node q) in modo che fornisca la lista p seguita dalla lista q.

2) Non modificate altro che il corpo del metodo.

3) Eseguite il main della classe Ex11 come test per la risposta.
*/

class Node { 
    private int elem; 
    private Node next;
    
    public Node(int elem, Node next) {
        this.elem = elem;
        this.next = next;
    }
    
    public int getElem() {
        return elem;
    }
    
    public Node getNext() {
        return next;
    }
    
    public void setElem(int elem) {
        this.elem = elem;
    }
    
    public void setNext(Node next) {
        this.next = next;
    }
}

class Ex11 {
    public static Node concat(Node p, Node q) {
        // INSERIRE CODICE QUI
    }
         
    // p = {1, 2, 3}
    static Node p = new Node(1, new Node(2, new Node(3, null)));
    // q = {10, 20, 30}
    static Node q = new Node(10, new Node(20, new Node(30, null)));
    
    public static String toString(Node p) {
        if (p == null) 
            return ""; 
        else 
            return p.getElem() + " " + toString(p.getNext());
    }
    
    public static void check(String msg, String a, String b) {
        if (a.equals(b))
            System.out.println(msg + a + " OK");
        else
            System.out.println(msg + a + " INSTEAD OF " + b);
    }
    
    public static void main(String[] args) {
        check("concat(p,q)=", toString(concat(p, q)), "1 2 3 10 20 30 ");
    }
}

// SOLUZIONE
/*
public static Node concat(Node p, Node q) {
    if (p == null)
        return q;
    else
        return new Node(p.getElem(), concat(p.getNext(), q));
}
*/