import java.util.Random;

class Ex {
    // METHOD TO FILL IN
    public static int h(int[][] z) {
        int diversi = 0;

        for (int i = 0; i < z.length; i++) {
            int m = z[i].length;
            int mod = z[i][m - 1] % 16;
            int sum = 0;

            for (int j = 0; j < z[i].length - 1; j++) {
                sum += z[i][j];
            }

            if (m == 2)
                sum = z[i][0];

            sum = sum % 16;
            if (sum != mod)
                diversi++;
        }

        return diversi;
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
