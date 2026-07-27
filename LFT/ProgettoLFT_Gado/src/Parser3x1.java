import java.io.*;

public class Parser3x1 {
    private final Lexer2x3 lex;
    private final BufferedReader pbr;
    private Token look;

    public Parser3x1(Lexer2x3 l, BufferedReader br) {
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

    // GUIDA(<start> --> <expr>EOF) = FIRST(<expr>) = FIRST(<term>) = FIRST(<fact>) = {(} U {NUM}
    public void start() {
        switch (look.tag) {
            case '(', Tag.NUM -> {
                expr();
                match(Tag.EOF);
            }
            default -> error("Error in start");
        }
    }

    // GUIDA(<expr> --> <term>) = FIRST(<term>) = FIRST(<fact>) = {(} U {NUM} 
    private void expr() {
        switch (look.tag) {
            case '(', Tag.NUM -> {
                term();
                exprp();
            }
            default -> error("Error in expr");
        }
    }

    private void exprp() {
        switch (look.tag) {
            // GUIDA(<exprp> --> +<term><exprp>) = FIRST(+) = {+}
            case '+':
                match(43); // 43 è + in ASCII
                term();
                exprp();
                break;

            // GUIDA(<exprp> --> -<term><exprp>) = FIRST(-) = {-}
            case '-':
                match(45); // 45 è - in ASCII
                term();
                exprp();
                break;

            // GUIDA(<exprp> --> ε) = FIRST(ε) U FOLLOW(<exprp>) = {)} U EOF
            case ')':
                // no match in epsilon
                break;
            
            case -1: // perché secondo la grammatica EXPRP->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in exprp");
                break;
        }
    }

    // GUIDA(<term> --> <fact><termp>) = FIRST(fact) = {(} U {NUM}
    private void term() {
        switch (look.tag) {
            case '(', Tag.NUM -> {
                fact();
                termp();
            }
            default -> error("Error in term");
        }

    }

    private void termp() {
        switch (look.tag) {
            // GUIDA(<termp> --> *<fact><exprp>) = FIRST(*) = {*}
            case '*':
                match(42); // 42 è * in ASCII
                fact();
                termp();
                break;

            // GUIDA(<termp> --> /<fact><exprp>) = FIRST(/) = {/}
            case '/':
                match(47); // 47 è / in ASCII
                fact();
                termp();
                break;

            // GUIDA(<termp> --> ε) = FIRST(ε) U FOLLOW(<termp>) = {)} U EOF U {+} U {-}
            case ')':
                // no match in epsilon
                break;

            case '+':
                // no match in epsilon
                break;

            case '-':
                // no match in epsilon
                break;
            
            case -1: // perché secondo la grammatica termp->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in termp");
                break;
        }
    }

    private void fact() {
        switch (look.tag) {
            // GUIDA(<fact> --> (<expr>) = FIRST(() = {(}
            case '(': 
                match(40); // 40 è ( in ASCII
                expr();
                match(41); // 41 è ) in ASCII
                break;

            // GUIDA(<fact> --> NUM) = FIRST(NUM) = {NUM}
            case Tag.NUM:
                match(Tag.NUM);
                break;
            
            default: 
                error("Error in fact");
                break;
        }
    }

    public static void main(String[] args) {
        Lexer2x3 lex = new Lexer2x3();
        String path = "files/parser3x1.lft"; // il percorso del file da leggere
        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            Parser3x1 parser = new Parser3x1(lex, br);
            parser.start();
            System.out.println("Input OK");
            br.close();
        } catch (IOException e) {e.printStackTrace();}
    }
}