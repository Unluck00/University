/*
Completate il metodo:
public static <T extends Comparable<T>> T findMax(T v[][])

in modo che restituisca l'elemento massimo in v (array non vuoto). 
Usare compareTo().
*/

class Ex3 {
    // METHOD TO FILL IN
    public static <T extends Comparable<T>> T findMax(T v[][]) {
      
    }

    // TEST METHODS
    private static <T> void runTest(String testName, T expected, T actual) {
        boolean success;
        if (expected == null) {
            success = actual == null;
        } else {
            success = expected.equals(actual);
        }

        String status = success ? "✓ PASS" : "✗ FAIL";
        String color = success ? "\u001B[32m" : "\u001B[31m";
        
        System.out.printf("%s%-60s %s\u001B[0m%n", color, testName, status);
        
        if (!success) {
            System.out.printf("    Expected: %-10s  Actual: %s%n", expected, actual);
        }
    }

    public static void main(String[] args) {
        System.out.println("==================== TEST FINDMAX ====================");
        
        // Test con Integer
        Integer[][] v1 = {{1}, {2, 3}};
        runTest("Test 1.1: Max in {{1},{2,3}}", 3, findMax(v1));
        
        Integer[][] v2 = {{5}, {3, 4}};
        runTest("Test 1.2: Max in {{5},{3,4}}", 5, findMax(v2));
        
        Integer[][] v3 = {{-1, -5}, {-3, -2}};
        runTest("Test 1.3: Max in {{-1,-5},{-3,-2}}", -1, findMax(v3));
        
        // Test con String
        String[][] v4 = {{"a"}, {"b", "c"}};
        runTest("Test 2.1: Max in {{\"a\"},{\"b\",\"c\"}}", "c", findMax(v4));
        
        String[][] v5 = {{"apple"}, {"banana", "cherry"}};
        runTest("Test 2.2: Max in {{\"apple\"},{\"banana\",\"cherry\"}}", "cherry", findMax(v5));
        
        // Test con Double
        Double[][] v6 = {{1.5}, {2.5, 3.5}};
        runTest("Test 3.1: Max in {{1.5},{2.5,3.5}}", 3.5, findMax(v6));
        
        Double[][] v7 = {{0.5}, {0.1, 0.2}};
        runTest("Test 3.2: Max in {{0.5},{0.1,0.2}}", 0.5, findMax(v7));
        
        // Test con elementi nulli
        Integer[][] v8 = {{null}, {1, 2}};
        runTest("Test 4.1: Max in {{null},{1,2}}", 2, findMax(v8));
        
        Integer[][] v9 = {{null}, {null, null}};
        try {
            findMax(v9);
            runTest("Test 4.2: Array con solo null", "Exception", "No Exception");
        } catch (IllegalArgumentException e) {
            runTest("Test 4.2: Array con solo null", "Exception", "Exception");
        }
        
        // Test con array multi-dimensionali
        Integer[][] v10 = {{1, 5, 3}, {2}, {4, 6}};
        runTest("Test 5.1: Max in {{1,5,3},{2},{4,6}}", 6, findMax(v10));
        
        // Test eccezioni
        try {
            Integer[][] v11 = {};
            findMax(v11);
            runTest("Test 6.1: Array vuoto {}", "Exception", "No Exception");
        } catch (IllegalArgumentException e) {
            runTest("Test 6.1: Array vuoto {}", "Exception", "Exception");
        }
        
        try {
            Integer[][] v12 = null;
            findMax(v12);
            runTest("Test 6.2: Array null", "Exception", "No Exception");
        } catch (NullPointerException e) {
            runTest("Test 6.2: Array null", "Exception", "Exception");
        }
        
        System.out.println("====================================================");
    }
}

// Soluzione:
/*
  T max = null;
        boolean found = false;
        for (int i = 0; i < v.length; i++) {
            for (int j = 0; j < v[i].length; j++) {
                T element = v[i][j];
                if (element != null) {
                    if (!found) {
                        max = element;
                        found = true;
                    } else if (element.compareTo(max) > 0) {
                        max = element;
                    }
                }
            }
        }
        if (!found) throw new IllegalArgumentException("Array contains only null elements"); questo controllo è per array vuoti o composti solo da null
        return max;
*/