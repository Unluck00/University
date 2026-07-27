public class ex_1x4 {
    public static void main(String[] args) {
        System.out.println(scan4("123") == true);
        System.out.println(scan4("123.5") == true);
        System.out.println(scan4(".567") == true);
        System.out.println(scan4("+7.5") == true);
        System.out.println(scan4("-.7") == true);
        System.out.println(scan4("67e10") == true);
        System.out.println(scan4("1e-2") == true);
        System.out.println(scan4("-.7e2") == true);
        System.out.println(scan4("1e2.3") == true);
        System.out.println(scan4(".") == false);
        System.out.println(scan4("e3") == false);
        System.out.println(scan4("123.") == false);
        System.out.println(scan4("+e6") == false);
        System.out.println(scan4("1.2.3") == false);
        System.out.println(scan4("4e5e6") == false);
        System.out.println(scan4("++3") == false);
    }

    public static boolean scan4(String s) 
    {
        int state = 0;
        int i = 0;
        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0:
                    if (ch == '+' || ch == '-') // inizia con il simbolo di + o -
                        state = 1;
                    else if (ch == '.') // inizia con la parte decimale
                        state = 2;
                    else if (Character.isDigit(ch)) // stato in cui è una cifra numerica (non parte decimale)
                        state = 3;
                    else 
                        state = -1;
                    break;

                case 1: // stato in cui la stringa è iniziato con un + o -
                    if (Character.isDigit(ch))
                        state = 3;
                    else if (ch == '.') // passa allo stato con la parte decimale
                        state = 2;
                    else 
                        state = -1;
                    break;

                case 2: // stato con la parte decimale dopo il .
                    if (Character.isDigit(ch))
                        state = 4;
                    else
                        state = -1;
                    break;

                case 3: // stato in cui si è nella cifra numerica
                    if(Character.isDigit(ch))
                        state = 3;
                    else if (ch == 'e') // si entra nello stato sulla parte dello esponenziale
                        state = 5;
                    else if (ch == '.')
                        state = 2;
                    else 
                        state = -1;
                    break;
                
                case 4: // stato in cui si è dentro la parte decimale 
                    if(Character.isDigit(ch))
                        state = 4;
                    else if(ch == 'e')
                        state = 5;
                    else 
                        state = -1;
                    break;
                
                case 5: // stato in cui si è nella parte dello esponenziale
                    if(Character.isDigit(ch)) // si passa ad un nuovo stato che non conterrà più la parte esponenziale dopo
                        state = 8;
                    else if (ch == '.') // si passa ad un nuovo stato (decimale) che non conterrà più la parte esponenziale dopo
                        state = 6;
                    else if (ch == '+' || ch == '-') // si passa ad un nuovo stato (con simbolo + o -) che non conterrà più la parte esponenziale dopo
                        state = 7;
                    else
                        state = -1;
                    break;
                
                case 6: // stato con la parte decimale dopo il . (dopo la parte esponenziale)
                    if (Character.isDigit(ch))
                        state = 8;
                    else
                        state = -1;
                    break;

                case 7: // stato in cui la stringa è iniziato con un + o - (dopo la parte esponenziale)
                    if (Character.isDigit(ch))
                        state = 8;
                    else if (ch == '.') // passa allo stato con la parte decimale
                        state = 6;
                    else 
                        state = -1;
                    break;
                
                case 8: // stato in cui si è nella cifra numerica
                    if(Character.isDigit(ch))
                        state = 8;
                    else if (ch == '.')
                        state = 6;
                    else 
                        state = -1;
                    break;
            }   
        }
        return state == 3 || state == 4 || state == 8;
    }
}