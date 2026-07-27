/*
Salvate tutto in un solo file Ex15.java.

1) Completate il corpo del metodo Node reverse(Node p) in modo che fornisca la lista p invertita.

2) Non modificate altro che il corpo del metodo.

3) Eseguite il main della classe Ex15 como test per la risposta.
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

class Ex15 {
    public static Node reverse(Node p) {
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
        check("reverse(p)=", toString(reverse(p)), "3 2 1 ");
    }
}

// SOLUZIONE
/*
public static Node reverse(Node p) {
    return reverseHelper(p, null);
}

private static Node reverseHelper(Node p, Node acc) {
    if (p == null)
        return acc;
    else
        return reverseHelper(p.getNext(), new Node(p.getElem(), acc));
}
*/