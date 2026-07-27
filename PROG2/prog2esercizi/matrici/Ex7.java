/*
public > boolean f(T v[][], T x)
1) Restituisce true se esiste almeno una riga in v dove tutti gli elementi sono uguali a x, e false altrimenti.
2) Confrontate gli elementi di T usando x.compareTo(y) (supponete che x non sia null).
3) Non modificate altro che il corpo del primo metodo.
 */
class Ex7 {

    //METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean f(T v[][], T x) {
        //
    }

    //TEST DEFINITION
    public static void test1() {
        Integer v[][] = {{1, 1}, {2, 2}, {3, 3}};
        for (int i = 1; i <= 3; ++i) {
            v[0][0] = v[0][1] = i;
            String msg = "v={ {" + i + "," + i + "},{2,2},{3,3} }, f(v," + i + ")=";
            comp(msg, f(v, i), i == i); // Always true, but test logic is in the solution
        }
    }

    public static void main(String[] args) {
        test1();
    }

    //COMPARING WITH THE CORRECT ANSWER
    public static void comp(String msg, boolean a, boolean b) {
        if (a == b) {
            System.out.println(msg + a + " OK");
        } else {
            System.out.println(msg + a + " *EXPECTED* " + b);
        }
    }
}

//SOLUTION:
/*
public static <T extends Comparable<T>> boolean f(T v[][], T x) {
    for (T[] row : v) {
        boolean allEqual = true;
        for (T elem : row) {
            if (elem.compareTo(x) != 0) {
                allEqual = false;
                break;
            }
        }
        if (allEqual) return true;
    }
    return false;
}
 */
