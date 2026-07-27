public class ex_1x6 {
    public static void main(String[] args) {
        System.out.println(scan6("aaa/****/aa") == true);
        System.out.println(scan6("aa/*a*a*/") == true);
        System.out.println(scan6("aaaa") == true);
        System.out.println(scan6("/****/") == true);
        System.out.println(scan6("/*aa*/") == true);
        System.out.println(scan6("*/a") == true);
        System.out.println(scan6("a/**/***a") == true);
        System.out.println(scan6("a/**/***/a") == true);
        System.out.println(scan6("a/**/aa/***/a") == true);
        System.out.println(scan6("aaa/*/aa") == false);
        System.out.println(scan6("a/**//***a") == false);
        System.out.println(scan6("aa/*aa") == false);
    }

    public static boolean scan6(String s) 
    {
        int state = 0;
        int i = 0;

        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0: // se il carattere è con / allora è in procinto di fare un commento
                    if (ch == '/')
                        state = 1;
                    else if (ch == 'a' || ch == '*')
                        state = 0;
                    else
                        state = -1;
                    break;

                case 1: 
                    if (ch == '*') // nel caso in cui il carattere è * allora si è aperto un commento
                        state = 2;
                    else if (ch == 'a') // nel caso in cui il carattere è a allora è un falso allarme è ritorna allo stato precedente
                        state = 0;
                    else if (ch == '/') // nel caso in cui il carattere è / allora si è in procinto di fare un commento
                        state = 1;
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

                case 4: // si è chiuso il commento e quindi se il prossimo carattere è / allora si è nuovamente in procinto di aprire un altro commento 
                    if (ch == 'a' || ch == '*')
                        state = 4;
                    else if (ch == '/')
                        state = 5;
                    else
                        state = -1;
                    break;
                
                case 5:
                    if (ch == '*') // si è aperto un nuovo commento che deve essere chiuso
                        state = 2;
                    else if (ch == 'a') // non si aperto nessun nuovo commento
                        state = 4;
                    else if (ch == '/') // si è di nuovo in procinto di chiudere un commento
                        state = 5;
                    else 
                        state = -1;
            }
        }

        return state == 4 || state == 0 || state == 5;
    }
}