public class ex_1x5 {
    public static void main(String[] args) {
        System.out.println(scan5("/****/") == true);
        System.out.println(scan5("/*a*a*/") == true);
        System.out.println(scan5("/*a/**/") == true);
        System.out.println(scan5("/**a///a/a**/") == true);
        System.out.println(scan5("/**/") == true);
        System.out.println(scan5("/*/*/") == true);
        System.out.println(scan5("/*/") == false);
        System.out.println(scan5("/**/***/") == false);
    }

    public static boolean scan5(String s) {
        int state = 0;
        int i = 0;

        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0:  // primo carattere dev'essere /
                    if (ch == '/')
                        state = 1;
                    else
                        state = -1;
                    break;

                case 1: // secondo carattere dev'essere *
                    if (ch == '*')
                        state = 2;
                    else
                        state = -1;
                    break;

                case 2: // in questo stato si mettono i caratteri che si vogliono, ma se si inserisce * allora è possibile che il prossimo carattere sia / (per concludere il commento)
                    if (ch == 'a' || ch == '/')
                        state = 2;
                    else if (ch == '*')
                        state = 3;
                    else
                        state = -1;
                    break;

                case 3:
                    if (ch == '/') // si chiude il commento
                        state = 4;
                    else if (ch == 'a') // si ritorna allo stato precedente
                        state = 2;
                    else if (ch == '*') // riporta di nuovo a questo stato essendo di nuovo in procinto di chiudere il commento
                        state = 3;
                    else
                        state = -1;
                    break;

                case 4:  // si è chiuso il commento
                    state = -1;
                    break;
            }
        }

        return state == 4;
    }
}
