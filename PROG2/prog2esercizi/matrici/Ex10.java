/*
public > boolean f(T v[][], T x)
1) Restituisce true se tutte le righe in v hanno almeno un elemento maggiore di x, e false altrimenti.
2) Confrontate gli elementi di T usando x.compareTo(y) (supponete che x non sia null).
3) Non modificate altro che il corpo del primo metodo.
 */
class Ex10 {

    //METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean f(T v[][], T x) {
       //
    }

    //TEST DEFINITION
    public static void test1() {
        Integer v[][] = {{1, 5}, {2, 6}, {3, 7}};
        comp("v all rows have >4? ", f(v, 4), false);
        comp("v all rows have >0? ", f(v, 0), true);
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
        boolean hasGreater = false;
        for (T elem : row) {
            if (elem.compareTo(x) > 0) {
                hasGreater = true;
                break;
            }
        }
        if (!hasGreater) return false;
    }
    return true;
}
*/
