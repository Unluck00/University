/*
Completate il metodo:
public static <T extends Comparable<T>> boolean inCol(T v[][], T x, int col)

in modo che restituisca true se x è presente nella colonna specificata,
false altrimenti (col valido). Usare compareTo().
*/

class Ex5 {
    // METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean inCol(T v[][], T x, int col) {
        for (int i = 0; i < v.length; i++) {
            if (col < v[i].length && v[i][col].compareTo(x) == 0)
                return true;
        }
        return false;
    }

    // TEST METHODS
    public static void comp(String msg, boolean a, boolean b) {
        if (a == b) System.out.println(msg + a + " OK");
        else System.out.println(msg + a + " *EXPECTED* " + b);
    }

    public static void test1() {
        Integer[][] v = {{1,2}, {3,4}};
        comp("test1: ", inCol(v, 2, 1), true);
    }

    public static void test2() {
        String[][] v = {{"a","b"}, {"c","d"}};
        comp("test2: ", inCol(v, "c", 0), false);
    }

    public static void main(String[] args) {
        test1();
        test2();
    }
}

// Soluzione:
/*
public static <T extends Comparable<T>> boolean inCol(T v[][], T x, int col) {
    for (int i = 0; i < v.length; i++) {
        if (col < v[i].length && v[i][col].compareTo(x) == 0) 
            return true;
    }
    return false;
}
*/