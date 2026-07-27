import java.io.*;

public class Valutatore4x1 {
    private Lexer2x3 lex;
    private BufferedReader pbr;
    private Token look;

    public Valutatore4x1(Lexer2x3 l, BufferedReader br) {
        lex = l;
        pbr = br;
        move();
    }

    void move() {
        look = lex.lexical_scan(pbr);
        System.out.println("token = " + look);
    }

    void error(String s) {
        throw new Error("near line " + lex.line + ": " + s);
    }

    void match(int t) {
        if (look.tag == t) {
            if (look.tag != Tag.EOF)
                move();
        } else
            error("syntax error");
    }

    public void start() {
        int expr_val;

        switch (look.tag) {
            case '(', Tag.NUM -> {
                expr_val = expr();
                match(Tag.EOF);
                System.out.println(expr_val);
            }
            default -> error("Error in start");
        }
    }

    private int expr() {
        int term_val, exprp_val;

        switch (look.tag) {
            case '(', Tag.NUM -> {
                term_val = term();
                exprp_val = exprp(term_val);
                return exprp_val; // exprp_val = expr_val
            }

            default ->
                error("Error in expr");
        }
        return 0;
    }

    private int exprp(int exprp_i) {
        int term_val, exprp_val;

        switch (look.tag) {
            case '+':
                match(43); // 43 è + in ASCII
                term_val = term();
                exprp_val = exprp(exprp_i + term_val); // exprp(1)_i = exprp(1)_val
                return exprp_val; // exprp(1)_val = exprp_val

            case '-':
                match(45); // 45 è - in ASCII
                term_val = term();
                exprp_val = exprp(exprp_i - term_val); // exprp(1)_i = exprp(1)_val
                return exprp_val; // exprp(1)_val = exprp_val

            case ')':
                break;

            case -1:
                break;

            default:
                error("Error in exprp");
        }
        return exprp_i; // exprp_i = exprp_val
    }

    private int term() {
        int fact_val, termp_val;

        switch (look.tag) {
            case '(', Tag.NUM -> {
                fact_val = fact();
                termp_val = termp(fact_val);
                return termp_val; // termp_val = term_val
            }

            default ->
                error("Error in term");
        }
        return 0;
    }

    private int termp(int termp_i) {
        int fact_val, termp_val;

        switch (look.tag) {
            case '*':
                match(42); // 42 è * in ASCII
                fact_val = fact();
                termp_val = termp(termp_i * fact_val); // termp(1)_i = termp(1)_val
                return termp_val; // termp(1)_val = termp_val

            case '/':
                match(47); // 47 è / in ASCII
                fact_val = fact();
                termp_val = termp(termp_i / fact_val); // termp(1)_i = termp(1)_val
                return termp_val; // termp(1)_val = termp_val

            case ')':
                break;

            case '+':
                break;
            
            case '-':
                break;

            case -1:
                break;

            default:
                error("Error in termp");

        }
        return termp_i; // termp_i = termp_val
    }

    private int fact() {
        int expr_val, NUM_val;

        switch (look.tag) {
            case '(' -> {
                match(40); // 40 è ( in ASCII
                expr_val = expr();
                match(41); // 41 è ) in ASCII
                return expr_val; // expr_val = fact_val
            }

            case Tag.NUM -> {
                NumberTok num = (NumberTok) look;
                NUM_val = num.value;
                match(Tag.NUM);
                return NUM_val; // NUM_val = fact_val
            }

            default -> 
                error("Error in fact");
        }
        return 0;
    }

    public static void main(String[] args) {
        Lexer2x3 lex = new Lexer2x3();
        String path = "files/valutatore4x1.lft"; // il percorso del file da leggere
        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            Valutatore4x1 valutatore = new Valutatore4x1(lex, br);
            valutatore.start();
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
