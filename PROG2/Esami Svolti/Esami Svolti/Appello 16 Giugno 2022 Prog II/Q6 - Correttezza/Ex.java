
// Il codice è stato anche formattato in modo leggibile
// Se la traccia esce fuori schermo usa il word wrapping (option + z su macOS)
import java.util.*;

/*
 Leggete attentamente la definizione del metodo int g(int[]  w). 
 Completate il corpo del metodo boolean ok(int[]  w) in modo che restituisca vero se e solo se eseguire il metodo int g(int[] w) completerebbe senza sollevare eccezioni, 
 nè restituendo valori inf, NaN. Non usate 'catch', ma fornite un algoritmo che decida quando non ci sono eccezioni/inf/NaN. Non modificate altro che il corpo di ok. 
 */
class Ex {
    // METHOD TO FILL IN
    public static boolean ok(int[] w) {
        // qui!
        // Suggerimento: Invece di cercare di capire cosa faccia il metodo soltanto
        // guardandolo,
        // prova a copiarlo ed incollarlo qui, ed eseguire i test. In questo modo potrai
        // vedere gli errori che devi prevenire.
        // Chiaramente per capire diversi tipi di errori, man mano che li correggi,
        // aggiungi la loro prevenzione
        // sopra / all'inizio di questo metodo, cosi esegue quei test case in maniera
        // corretta e fa vedere i prossimi problemi
        
    }

    public static int g(int[] w) {
        int i = 0;
        while ((w[i] <= w[i + 1]) == true) {
            i = i + 1;
        }
        return i;
    }

    // TEST DEFINITION
    public static String toString(int[] w) {
        int i = 0, l = w.length;
        String s = "";
        while (i < l) {
            s = s + w[i] + " ";
            i = i + 1;
        }
        return s;
    }

    public static Random r = new Random();

    public static int r(int a) {
        return a + r.nextInt(11) - 5;
    }

    public static int prop(int a, boolean b) {
        int i = r(a);
        if (((a <= i) == true) == b)
            return i;
        else
            return prop(a, b);
    }

    public static int[] makeVett(int a, int n, boolean ris) {
        int[] vett = new int[n];
        if (n == 0)
            return vett;
        else if (n == 1) {
            vett[0] = a;
            return vett;
        } else // n>=2
        {
            vett[0] = a;
            int j = (ris == true) ? r.nextInt(n - 1) + 1 : n;
            // se ris=true allora j=1,...,n-1 altrimenti j=n
            for (int i = 1; i < n; ++i)
                if (i == j) {
                    vett[i] = prop(vett[i - 1], false);
                } else {
                    vett[i] = prop(vett[i - 1], true);
                }
            return vett;
        }
    }

    public static void test(int a, int n, boolean ris) {
        int[] w = makeVett(a, n, ris);
        check("w = " + toString(w) + " ok(w)=",
                ok(w),
                ris);
    }

    public static void test() {
        int a = r.nextInt(11) - 5;
        System.out.print("vettori di lunghezza <= 1: \n");
        test(a, 0, false);
        test(a, 1, false);
        for (int i = 2; i < 10; i++) {
            System.out.print("vettori di lunghezza " + i + ":\n");
            a = r.nextInt(11) - 5;
            test(a, i, true);
            test(a, i, false);
        }
    }

    // COMPARING WITH THE CORRECT ANSWER
    // USE "*" ONLY FOR FAILURE, NEVER USE IN msg
    public static void check(String s, boolean a, boolean b) {
        if (a == b)
            System.out.println(s + a + " OK");
        else
            System.out.println(s + a + " *EXPECTED* " + b);
    }

    // TEST EXECUTION
    public static void main(String[] args) {
        test();
    }
}
