/*
public > boolean f(T v[][], T x)
1) Restituisce true se tutti gli elementi in v sono maggiori o uguali a x, e false altrimenti.
2) Confrontate gli elementi di T usando x.compareTo(y) (supponete che x non sia null).
3) Non modificate altro che il corpo del primo metodo.
 */
class Ex6 {

    //METHOD TO FILL IN
    public static <T extends Comparable<T>> boolean f(T v[][], T x) {
        //
    }

    //TEST DEFINITION
    public static void test1() {
        Integer v[][] = {{3}, {4, 5}};
        for (int i = 3; i <= 5; ++i) {
            for (int j = 3; j <= 5; ++j) {
                for (int k = 3; k <= 5; ++k) {
                    v[0][0] = i;
                    v[1][0] = j;
                    v[1][1] = k;
                    String msg = "v={ {" + i + "},{" + j + "," + k + "} }, f(v,3)=";
                    boolean conj = ((i >= 3) && (j >= 3) && (k >= 3));
                    comp(msg, f(v, 3), conj == true);
                }
            }
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
            if (elem.compareTo(x) < 0) {
                return false;
            }
        }
    }
    return true;
}
*/
