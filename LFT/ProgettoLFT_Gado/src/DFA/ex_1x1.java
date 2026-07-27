public class ex_1x1 {
    public static void main(String[] args) {
        System.out.println(scan("010101") == false);
        System.out.println(scan("1100011001") == true);
        System.out.println(scan("10214") == false);
        System.out.println(scan("000101") == true);

        System.out.println(scan1("010101") == true);
        System.out.println(scan1("1100011001") == false);
        System.out.println(scan1("10214") == false);
        System.out.println(scan1("000101") == false);
    }

    public static boolean scan1(String s) 
    {
        int state = 0;
        int i = 0;
        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0:
                    if (ch == '0')
                        state = 1;
                    else if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;

                case 1:
                    if (ch == '0')
                        state = 2;
                    else if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;

                case 2:
                    if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;
            }
        }
        return state != -1;
    }

    // DFA definito sull'alfabeto {0,1} che riconosce le stringhe con 3 zeri consecutivi
    public static boolean scan(String s) 
    {
        int state = 0;
        int i = 0;
        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0:
                    if (ch == '0')
                        state = 1;
                    else if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;

                case 1:
                    if (ch == '0')
                        state = 2;
                    else if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;

                case 2:
                    if (ch == '0')
                        state = 3;
                    else if (ch == '1')
                        state = 0;
                    else
                        state = -1;
                    break;
                    
                case 3:
                    if (ch == '0' || ch == '1')
                        state = 3;
                    else
                        state = -1;
                    break;
            }
        }
        return state == 3;
    }

}