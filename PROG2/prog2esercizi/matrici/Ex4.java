/*
Completate il metodo:
public static <T extends Comparable<T>> boolean inRow(T v[][], T x, int row)

in modo che restituisca true se x è presente nella riga specificata,
false altrimenti (row valido). Usare compareTo().
*/

class Ex4 {
    // METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean inRow(T v[][], T x, int row) {
        for (int i = 0; i < v[row].length; i++) {
            if (v[row][i] != null && v[row][i].compareTo(x) == 0) return true;
        }
        return false;
    }
    // TEST METHODS

    // Overload for boolean expected/actual
    public static void runTest(String description, boolean expected, boolean actual) {
        System.out.println(description + " | atteso: " + expected + " | ottenuto: " + actual + " | " + (expected == actual ? "OK" : "ERRORE"));
    }

    // Overload for String expected/actual (for exception tests)
    public static void runTest(String description, String expected, String actual) {
        System.out.println(description + " | atteso: " + expected + " | ottenuto: " + actual + " | " + (expected.equals(actual) ? "OK" : "ERRORE"));
    }
    
    public static void main(String[] args) {
        System.out.println("==================== TEST INROW ====================");
        
        // Test con Integer
        Integer[][] v1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        runTest("Test 1.1: 5 presente nella riga 1 {{1,2,3},{4,5,6},{7,8,9}}", true, inRow(v1, 5, 1));
        runTest("Test 1.2: 9 presente nella riga 2 {{1,2,3},{4,5,6},{7,8,9}}", true, inRow(v1, 9, 2));
        runTest("Test 1.3: 1 non presente nella riga 2 {{1,2,3},{4,5,6},{7,8,9}}", false, inRow(v1, 1, 2));
        
        // Test con String
        String[][] v2 = {{"a", "b"}, {"c", "d"}, {"e", "f"}};
        runTest("Test 2.1: \"d\" presente nella riga 1 {{\"a\",\"b\"},{\"c\",\"d\"},{\"e\",\"f\"}}", true, inRow(v2, "d", 1));
        runTest("Test 2.2: \"a\" non presente nella riga 2 {{\"a\",\"b\"},{\"c\",\"d\"},{\"e\",\"f\"}}", false, inRow(v2, "a", 2));
        
        
        // Test con array irregolari
        Integer[][] v4 = {{1}, {2, 3}, {4, 5, 6}};
        runTest("Test 4.1: 3 presente nella riga 1 {{1},{2,3},{4,5,6}}", true, inRow(v4, 3, 1));
        runTest("Test 4.2: 6 presente nella riga 2 {{1},{2,3},{4,5,6}}", true, inRow(v4, 6, 2));
        
       
        System.out.println("===================================================");
        System.out.println("NOTA: Tutti i test assumono che v e x non siano null (come specificato)");
        System.out.println("      e che l'indice row sia valido per l'array");
    }
}

// Soluzione:
/*
public static <T extends Comparable<T>> boolean inRow(T v[][], T x, int row) {
    for (T element : v[row]) {
        if (element.compareTo(x) == 0) return true;
    }
    return false;
}
*/