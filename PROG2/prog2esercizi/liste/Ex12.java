/*
Salvate tutto in un solo file Ex12.java.

1) Completate il corpo del metodo Node filterEven(Node p) in modo che fornisca una lista contenente solo gli elementi pari di p, nello stesso ordine.

2) Non modificate altro che il corpo del metodo.

3) Eseguite il main della classe Ex12 come test per la risposta.
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

class Ex12 {
    public static Node filterEven(Node p) {
        // INSERIRE CODICE QUI
    }
         
    static Node p = new Node(1, new Node(2, new Node(3, new Node(4, null))));
    
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
        check("filterEven(p)=", toString(filterEven(p)), "2 4 ");
    }
}

// SOLUZIONE
/*
public static Node filterEven(Node p) {
    if (p == null)
        return null;
    else if (p.getElem() % 2 == 0)
        return new Node(p.getElem(), filterEven(p.getNext()));
    else
        return filterEven(p.getNext());
}
*/