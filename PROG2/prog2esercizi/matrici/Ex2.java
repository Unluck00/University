
/*
Completate il metodo:
public static <T extends Comparable<T>> int countGreater(T v[][], T x)

in modo che restituisca il numero di elementi > x in v. 
Usare compareTo (x non null). 
*/

class Ex2 {
    // METHOD TO FILL IN
    public static <T extends Comparable<T>> int countGreater(T v[][], T x) {
       
    }

    // Helper method for running tests
    private static void runTest(String description, int expected, int actual) {
        System.out.printf("%s | Expected: %d, Actual: %d | %s\n",
            description, expected, actual, (expected == actual ? "PASS" : "FAIL"));
    }

    // TEST METHODS
     public static void main(String[] args) {
        System.out.println("==================== TEST COUNTGREATER ====================");
        
        // Test con Integer
        Integer[][] v1 = {{1}, {2, 3}};
        runTest("Test 1.1: Count >1 in {{1},{2,3}}", 2, countGreater(v1, 1));
        
        Integer[][] v2 = {{5}, {3, 4}};
        runTest("Test 1.2: Count >4 in {{5},{3,4}}", 1, countGreater(v2, 4));
        
        Integer[][] v3 = {{1, 1}, {1, 1}};
        runTest("Test 1.3: Count >2 in {{1,1},{1,1}}", 0, countGreater(v3, 2));
        
        Integer[][] v4 = {{}};
        runTest("Test 1.4: Array vuoto {{}}", 0, countGreater(v4, 1));
        
        // Test con String
        String[][] v5 = {{"a"}, {"b", "c"}};
        runTest("Test 2.1: Count >\"a\" in {{\"a\"},{\"b\",\"c\"}}", 2, countGreater(v5, "a"));
        
        String[][] v6 = {{"apple"}, {"banana", "cherry"}};
        runTest("Test 2.2: Count >\"banana\" in {{\"apple\"},{\"banana\",\"cherry\"}}", 1, countGreater(v6, "banana"));
        
        // Test con Double
        Double[][] v7 = {{1.5}, {2.5, 3.5}};
        runTest("Test 3.1: Count >1.0 in {{1.5},{2.5,3.5}}", 3, countGreater(v7, 1.0));
        
        Double[][] v8 = {{0.5}, {0.1, 0.2}};
        runTest("Test 3.2: Count >0.5 in {{0.5},{0.1,0.2}}", 0, countGreater(v8, 0.5));
        
        // Test con elementi nulli
        Integer[][] v9 = {{null}, {1, 2}};
        runTest("Test 4.1: Count >1 con elementi null in {{null},{1,2}}", 1, countGreater(v9, 1));
        
        // Test con array multi-dimensionali
        Integer[][] v10 = {{1, 5, 3}, {2}, {4, 6}};
        runTest("Test 5.1: Count >3 in {{1,5,3},{2},{4,6}}", 3, countGreater(v10, 3));
        
        System.out.println("=========================================================");
        System.out.println("NOTA: Tutti i test assumono che v non sia null, come specificato");
        System.out.println("      Se x è null, viene lanciata NullPointerException");
    }
}

// Soluzione:
/*
public static <T extends Comparable<T>> int countGreater(T v[][], T x) {
    int count = 0;
    for (T[] row : v) {
        for (T element : row) {
            if (element.compareTo(x) > 0) count++;
        }
    }
    return count;
}
*/