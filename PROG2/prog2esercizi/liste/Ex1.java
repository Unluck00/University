/* 
Completate il metodo void f() in List, in modo che f() elimini dalla lista tutti gli elementi minori di 0.
Se non ce ne sono f() lascia la lista invariata. Non modificate altro che il metodo.
Ricordatevi: per "eliminare" tutti i primi nodi minori di 0 riassegnate ogni volta first all'indirizzo successivo.  
Per "eliminare" un nodo p diverso dal primo, usate setNext per far puntare il nodo precedente al nodo p al nodo successivo al nodo p.
Quando spostate un cursore, fate attenzione a non posizionarlo sopra un nodo eliminato.
Fate verificare la risposta a Moodle. Non definite il vostro metodo per casi sui test.
Se la risposta passa il controllo inviate tutto e terminate, altrimenti correggete gli errori.
Normalmente i test devono essere tutti giusti perché la soluzione abbia valore.
 */

class List {

    private Node first;

    public List() {
        first = null;
    }

    public void f() {
        //da completare
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

public class Ex1 {

    public static void main(String[] args) {
        testSoloNonNegativi();
        testSoloNegativi();
        testMisto();
        testNegativiInTesta();
        testNegativiInMezzo();
        testNegativiInFondo();
        testListaVuota();
    }

    // Test 1: solo numeri >= 0
    public static void testSoloNonNegativi() {
        System.out.println("== Test: Solo valori >= 0 ==");
        List a = new List();
        for (int i = 0; i < 5; i++) a.insertFirst(i + 1); // 1, 2, 3, 4, 5
        String atteso = a.toString();
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 2: solo numeri < 0
    public static void testSoloNegativi() {
        System.out.println("== Test: Solo valori < 0 ==");
        List a = new List();
        for (int i = 0; i < 5; i++) a.insertFirst(-i - 1); // -1, -2, -3, -4, -5
        String atteso = "";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 3: numeri misti
    public static void testMisto() {
        System.out.println("== Test: Valori misti ==");
        List a = new List();
        a.insertFirst(3);
        a.insertFirst(-2);
        a.insertFirst(1);
        a.insertFirst(-5);
        a.insertFirst(4);
        // Lista iniziale: 4, -5, 1, -2, 3
        String atteso = "4, 1, 3";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 4: negativi in testa
    public static void testNegativiInTesta() {
        System.out.println("== Test: Negativi all'inizio ==");
        List a = new List();
        a.insertFirst(4);
        a.insertFirst(3);
        a.insertFirst(-1);
        a.insertFirst(-2);
        // Lista: -2, -1, 3, 4
        String atteso = "3, 4";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 5: negativi in mezzo
    public static void testNegativiInMezzo() {
        System.out.println("== Test: Negativi al centro ==");
        List a = new List();
        a.insertFirst(5);
        a.insertFirst(-3);
        a.insertFirst(-1);
        a.insertFirst(2);
        a.insertFirst(1);
        // Lista: 1, 2, -1, -3, 5
        String atteso = "1, 2, 5";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 6: negativi alla fine
    public static void testNegativiInFondo() {
        System.out.println("== Test: Negativi alla fine ==");
        List a = new List();
        a.insertFirst(-4);
        a.insertFirst(-2);
        a.insertFirst(3);
        a.insertFirst(1);
        // Lista: 1, 3, -2, -4
        String atteso = "1, 3";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Test 7: lista vuota
    public static void testListaVuota() {
        System.out.println("== Test: Lista vuota ==");
        List a = new List();
        String atteso = "";
        a.f();
        stampaRisultato(a, atteso);
    }

    // Funzione comune per stampa e verifica
    public static void stampaRisultato(List l, String atteso) {
        System.out.println("Risultato ottenuto: " + l);
        System.out.println("Valore atteso     : " + atteso);
        System.out.println(l.toString().equals(atteso) ? ">>> OK\n" : ">>> ERRORE\n");
    }
}

// Soluzione:
// while (first != null && first.getElem() < 0) 
//     first = first.getNext();
// if (first == null) return;
// Node prev = first;
// Node curr = first.getNext();
// while (curr != null) {
//     if (curr.getElem() < 0) {
//         prev.setNext(curr.getNext());
//         curr = prev.getNext();
//     } else {
//         prev = curr;
//         curr = curr.getNext();
//     }
// }
