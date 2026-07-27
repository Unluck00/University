// Il codice è stato anche formattato in modo leggibile
// Se la traccia esce fuori schermo usa il word wrapping (option + z su macOS)

/*
 * Sia z un vettore di vettori-riga, con tutte le righe di due o più elementi. Completate il metodo int h(int z[][]) nella classe Ex in modo che, 
 * per ogni vettore-riga z[i] di m elementi, h prima calcoli sum=z[i][0] + ... + z[i][m-2], poi controlli se sum è uguale modulo 16 a z[i][m-1]. 
 * Notate che m vale almeno 2, e che se m=2 allora sum=z[i][0]. Alla fine h(z) restituisce il numero dei vettore-riga in cui i due valori sono diversi. Non modificate altro. 
 */
import java.util.Random;

class Ex {
    // METHOD TO FILL IN
    public static int h(int[][] z) {
        // qui!
        int ris=0;
        for(int i=0; i<z.length-1; i++) {
            int sum=0;
            int m=z[i].length;
            for(int j=0; j<m-2; j++) {
                sum += z[i][j];
            }
            if(sum%16 != z[i][m-1]%16) {
                ris++;
            }
        }
        return ris;
    }

    // TEST DEFINITION
    public static Random r = new Random();

    public static int val() {
        return r.nextInt(21);
    }

    public static int[][] makeVett(int n, int errate)
    // generiamo un vettore di n righe
    // con un numero "errate" di righe che falliscono il checksum
    {
        int[][] v = new int[n][];
        int sum, m;
        for (int i = 0; i < n; i++) {
            sum = 0;
            m = r.nextInt(n) + 2;// m=2,...,n+1=lunghezza riga numero i
            v[i] = new int[m];
            for (int j = 0; j < m - 1; j++) {
                v[i][j] = val();
                sum = sum + v[i][j];
            }
            v[i][m - 1] = sum % 16;
            // aggiungo errori nel checksum finche' il numero di righe errate diventa zero
            if (r.nextInt(n - i) < errate) {
                v[i][m - 1] = v[i][m - 1] + 1;
                --errate;
            }
        }
        return v;
    }

    public static String toString(int[] v, int i) {
        if (v.length == 0)
            return "{}";
        else if (v.length == 1)
            return "{" + v[0] + "}";
        else if (i == 0)
            return "{" + v[0] + "," + toString(v, 1);
        else if (i == v.length - 1)
            return v[i] + "}";
        else
            return v[i] + "," + toString(v, i + 1);
    }

    public static String toString(int[][] v, int i) {
        if (v.length == 0)
            return "{}";
        else if (v.length == 1)
            return "{" + toString(v[0], 0) + "}";
        else if (i == 0)
            return "{" + toString(v[0], 0) + "," + toString(v, 1);
        else if (i == v.length - 1)
            return toString(v[i], 0) + "}";
        else
            return toString(v[i], 0) + "," + toString(v, i + 1);
    }

    public static void test(int n, int errate) {
        int[][] v = makeVett(n, errate);
        check("v=" + toString(v, 0)
                + "\n h(v)=",
                h(v),
                errate);
    }

    public static void test() {
        System.out.println("======================");
        for (int n = 0; n < 5; n++) {
            System.out.println(
                    "   numero righe = " + n + "\n======================");
            for (int errate = 0; errate <= n; ++errate)
                test(n, errate);
        }
    }

    // COMPARING WITH THE RIGHT ANSWER
    public static void check(String s, int a, int b) {
        if (a == b)
            System.out.println(s + a + " OK" + "\n======================");
        else
            System.out.println(s + a + " *EXPECTED* " + b + "\n======================");
    }

    // TEST EXECUTION
    public static void main(String[] args) {
        test();
    }

}
