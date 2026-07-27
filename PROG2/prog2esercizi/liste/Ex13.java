/*
Salvate tutto in un solo file Ex13.java.

1) Completate il corpo del metodo int sum(Node p) in modo che fornisca la somma degli elementi della lista p.

2) Non modificate altro che il corpo del metodo.

3) Eseguite il main della classe Ex13 come test per la risposta.
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

class Ex13 {
    public static int sum(Node p) {
        // INSERIRE CODICE QUI
    }
         
    static Node p = new Node(1, new Node(2, new Node(3, null)));
    
    public static void check(String msg, int a, int b) {
        if (a == b)
            System.out.println(msg + a + " OK");
        else
            System.out.println(msg + a + " INSTEAD OF " + b);
    }
    
    public static void main(String[] args) {
        check("sum(p)=", sum(p), 6);
    }
}

// SOLUZIONE
/*
public static int sum(Node p) {
    if (p == null)
        return 0;
    else
        return p.getElem() + sum(p.getNext());
}
*/