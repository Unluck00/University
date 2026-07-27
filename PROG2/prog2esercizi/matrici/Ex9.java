/*
public > boolean f(T v[][], T x)
1) Restituisce true se x è presente in almeno una riga di v, e false altrimenti.
2) Confrontate gli elementi di T usando x.compareTo(y) (supponete che x non sia null).
3) Non modificate altro che il corpo del primo metodo.
 */
class Ex9 {

    //METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean f(T v[][], T x) {
     //
    }

    //TEST DEFINITION
    public static void test1() {
        Integer v[][] = {{1, 2}, {3, 4}, {5, 6}};
        for (int i = 1; i <= 6; ++i) {
            String msg = "v={ {1,2},{3,4},{5,6} }, f(v," + i + ")=";
            boolean expected = (i >= 1 && i <= 6);
            comp(msg, f(v, i), expected);
        }
    }

    public static void main(String[] args) {
        test1();
    }

    //COMPARING WITH THE CORRECT ANSWER
    public static void comp(String msg, boolean a, boolean b) {
        if (a == b) {
            System.out.println(msg + a + " OK"); 
        }else {
            System.out.println(msg + a + " *EXPECTED* " + b);
        }
    }
}

//SOLUTION:
/*
public static <T extends Comparable<T>> boolean f(T v[][], T x) {
    for (T[] row : v) {
        for (T elem : row) {
            if (elem.compareTo(x) == 0) {
                return true;
            }
        }
    }
    return false;
}
*/
