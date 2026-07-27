/*
Salvate tutto in un solo file Ex14.java.

1) Completate il corpo del metodo Node duplicate(Node p) in modo che fornisca una lista con ogni elemento di p duplicato.

2) Non modificate altro che il corpo del metodo.

3) Eseguite il main della classe Ex14 come test per la risposta.
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

class Ex14 {
    public static Node duplicate(Node p) {
        // INSERIRE CODICE QUI
    }
         
    static Node p = new Node(1, new Node(2, new Node(3, null)));
    
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
        check("duplicate(p)=", toString(duplicate(p)), "1 1 2 2 3 3 ");
    }
}

// SOLUZIONE
/*
public static Node duplicate(Node p) {
    if (p == null)
        return null;
    else
        return new Node(p.getElem(), new Node(p.getElem(), duplicate(p.getNext())));
}
*/