/*
Completate il metodo:
public static <T extends Comparable<T>> boolean allEqual(T v[][], T x)

in modo che restituisca true se TUTTI gli elementi in v sono uguali a x,
e false altrimenti. Usare x.compareTo(y) (x non null). 
Non modificate altro che il corpo del metodo.
*/

class Ex1 {
    // METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean allEqual(T v[][], T x) {
    
    }

    // TEST METHODS
      private static void runTest(String testName, boolean expected, boolean actual) {
        String status = (expected == actual) ? "✓ PASS" : "✗ FAIL";
        String color = (expected == actual) ? "\u001B[32m" : "\u001B[31m";
        
        System.out.printf("%s%-50s %s\u001B[0m%n", color, testName, status);
        
        if (expected != actual) {
            System.out.printf("    Expected: %-5b  Actual: %b%n", expected, actual);
        }
    }

    public static void main(String[] args) {
        System.out.println("==================== TEST ALLEQUAL ====================");
        
        // Test con Integer
        Integer[][] v1 = {{1}, {1,1}};
        runTest("Test 1.1: Tutti 1 in {{1},{1,1}}", true, allEqual(v1, 1));
        
        Integer[][] v2 = {{1}, {1,2}};
        runTest("Test 1.2: Elemento 2 in {{1},{1,2}}", false, allEqual(v2, 1));
        
        Integer[][] v3 = {{2,2}, {2}, {2,2,2}};
        runTest("Test 1.3: Tutti 2 in {{2,2},{2},{2,2,2}}", true, allEqual(v3, 2));
        
        Integer[][] v4 = {{}};
        runTest("Test 1.4: Array con riga vuota {{}}", true, allEqual(v4, 1));
        
        // Test con String
        String[][] v5 = {{"a"}, {"a", "a"}};
        runTest("Test 2.1: Tutti 'a' in {{\"a\"},{\"a\",\"a\"}}", true, allEqual(v5, "a"));
        
        String[][] v6 = {{"a"}, {"a", "b"}};
        runTest("Test 2.2: Elemento 'b' in {{\"a\"},{\"a\",\"b\"}}", false, allEqual(v6, "a"));
        
        // Test con Double
        Double[][] v7 = {{1.5}, {1.5, 1.5}};
        runTest("Test 3.1: Tutti 1.5 in {{1.5},{1.5,1.5}}", true, allEqual(v7, 1.5));
        
        Double[][] v8 = {{1.5}, {1.5, 2.0}};
        runTest("Test 3.2: Elemento 2.0 in {{1.5},{1.5,2.0}}", false, allEqual(v8, 1.5));
        
        // Test con Boolean
        Boolean[][] v9 = {{true}, {true, true}};
        runTest("Test 4.1: Tutti true in {{true},{true,true}}", true, allEqual(v9, true));
        
        Boolean[][] v10 = {{true}, {true, false}};
        runTest("Test 4.2: Elemento false in {{true},{true,false}}", false, allEqual(v10, true));
        
        // Test con array multi-dimensionali
        Integer[][] v11 = {{1,1,1}, {1,1}, {1}};
        runTest("Test 5.1: Array irregolari con tutti 1", true, allEqual(v11, 1));
        
        Integer[][] v12 = {{1,1,2}, {1,1}, {1}};
        runTest("Test 5.2: Array irregolari con elemento 2", false, allEqual(v12, 1));
        
        System.out.println("=====================================================");
        System.out.println("NOTA: Tutti i test assumono che v non sia null, come specificato");
        System.out.println("      Se x è null, viene lanciata NullPointerException");
    }
}
// Soluzione:
/*
public static <T extends Comparable<T>> boolean allEqual(T v[][], T x) {
    for (T[] row : v) {
        for (T element : row) {
            if (x.compareTo(element) != 0) return false;
        }
    }
    return true;
}
*/