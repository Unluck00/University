public class ex_1x3 {
    public static void main(String[] args) {
        System.out.println(scan3("123456Bianchi") == true);
        System.out.println(scan3("654321Rossi") == true);
        System.out.println(scan3("654321Bianchi") == false);
        System.out.println(scan3("123456Rossi") == false);
        System.out.println(scan3("2Bianchi") == true);
        System.out.println(scan3("122B") == true);
        System.out.println(scan3("654322") == false);
        System.out.println(scan3("Rossi") == false);
    }

    public static boolean scan3(String s) 
    {
        int state = 0;
        int i = 0;
        while (state >= 0 && i < s.length()) {
            final char ch = s.charAt(i++);

            switch (state) {
                case 0: // controlla se il primo carattere è un numero e va in stato 1 se è pari oppure in stato 0 se dispari
                    if (Character.isDigit(ch) && ch%2 == 0)
                        state = 1;
                    else if(Character.isDigit(ch) && ch%2 != 0) 
                        state = 2;
                    else
                        state = -1;
                    break;

                case 1: // stato in cui il numero è pari e se è un carattere maiuscolo (dopo il numero antecedente) allora controlla che sia del turno T2 (cognome tra A e K)
                    if (Character.isDigit(ch) && ch%2 == 0)
                        state = 1;
                    else if (Character.isDigit(ch) && ch%2 != 0)
                        state = 2;
                    else if (Character.isUpperCase(ch) && ch >= 'A' && ch <= 'K')     
                        state = 3;
                    else 
                        state = -1;
                    break;

                case 2: // stato in cui il numero è dispari e se è un carattere maiuscolo (dopo il numero antecedente) allora controlla che sia del turno T3 (cognome tra L e Z)
                    if (Character.isDigit(ch) && ch%2 == 0)
                        state = 1;
                    else if (Character.isDigit(ch) && ch%2 != 0)
                        state = 2;
                    else if (Character.isUpperCase(ch) && ch >= 'L' && ch <= 'Z')
                        state = 3;
                    else
                        state = -1;
                    break;

                case 3:
                    if(Character.isLetter(ch))
                        state = 3;
                    else 
                        state = -1;
                    break;
            }   
        }
        return state == 3;
    }
}