public class ex_1x2 {
    public static void main(String[] args) {
        System.out.println(scan2("x") ? "OK" : "NOPE");
        System.out.println(scan2("flag1") ? "OK" : "NOPE");
        System.out.println(scan2("x2y2") ? "OK" : "NOPE");
        System.out.println(scan2("x_1") ? "OK" : "NOPE");
        System.out.println(scan2("lft_lab") ? "OK" : "NOPE");
        System.out.println(scan2("_temp") ? "OK" : "NOPE");
        System.out.println(scan2("x_1_y_2") ? "OK" : "NOPE");
        System.out.println(scan2("x___") ? "OK" : "NOPE");
        System.out.println(scan2("__5") ? "OK" : "NOPE");
        System.out.println(scan2("5") ? "OK" : "NOPE");
        System.out.println(scan2("221B") ? "OK" : "NOPE");
        System.out.println(scan2("123") ? "OK" : "NOPE");
        System.out.println(scan2("9_to_5") ? "OK" : "NOPE");
        System.out.println(scan2("___") ? "OK" : "NOPE");
    }

    public static boolean scan2(String s) 
    {
        int state = 0;
        int i = 0;
        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0:
                    if (ch == '_')
                        state = 1;
                    else if (Character.isLetter(ch)) // funzione che permette di determinare se il carattere è una lettera
                        state = 2;
                    else    
                        state = -1;  // comincia con un numero la stringa, quindi errore
                    break;

                case 1:
                    if (ch == '_')
                        state = 1;
                    else if (Character.isLetterOrDigit(ch)) // // funzione che permette di determinare se il carattere è una lettera o un numero
                        state = 2;
                    else
                        state = -1;
                    break;

                case 2:
                    if (Character.isLetterOrDigit(ch) || ch == '_')
                        state = 2;
                    else
                        state = -1;
                    break;
            }
        }
        return state == 2; // permette di controllare che si è rimasto nello stato 2 (stringa corretta)
    }
}
