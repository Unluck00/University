import java.io.*;

public class Parser3x2 {
    private Lexer2x3 lex;
    private BufferedReader pbr;
    private Token look;

    public Parser3x2(Lexer2x3 l, BufferedReader br) {
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

    // GUIDA(<prog> --> <statlist>EOF) = FIRST(<stat>) = {assign, print, read, for, if, { }
    public void prog() {
        switch (look.tag) {
            case Tag.ASSIGN, Tag.PRINT, Tag.READ, Tag.FOR, Tag.IF, '{' -> {
                statlist();
                match(Tag.EOF);
            }
            default -> error("Error in prog");
        }
    }

    // GUIDA(<statlist> --> <stat><statlistp>) = FIRST(<stat>) = {assign, print, read, for, if, { }
    private void statlist() {
        switch (look.tag) {
            case Tag.ASSIGN, Tag.PRINT, Tag.READ, Tag.FOR, Tag.IF, '{' -> {
                stat();
                statlistp();
            }
            default -> error("Error in statlist");
        }
    }

    private void statlistp() {
        switch (look.tag) {
            // GUIDA(<statlistp> --> ;<stat><statlistp>) = FIRST(;) = {;}
            case ';':
                match(59); // 59 è ; in ASCII
                stat();
                statlistp();
                break;

            // GUIDA(<statlistp> --> ε) = FIRST(ε) U FOLLOW(<statlistp>) = {},EOF}
            case '}':
                // no match in epsilon
                break;
            
            case -1: // perché secondo la grammatica statlistp->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in statlistp");
                break;
        }
    }

    private void stat() {
        switch (look.tag) {
            // GUIDA(<stat> --> assign<assignlist>) = FIRST(assign) = {assign}
            case Tag.ASSIGN -> {
                match(Tag.ASSIGN);
                assignlist();
            }

            // GUIDA(<stat> --> print(<exprlist>) = FIRST(print) = {print}
            case Tag.PRINT -> {
                match(Tag.PRINT);
                match(40); // 40 è ( in ASCII
                exprlist();
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> read(<idlist>) = FIRST(read) = {read}
            case Tag.READ -> {
                match(Tag.READ);
                match(40); // 40 è ( in ASCII
                idlist();
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> for(ID:=<expr>;<bexpr>)do<stat> = FIRST(for) = {for}
            // GUIDA(<stat> --> for(<bexpr>)do<stat> = FIRST(for) = {for}
            case Tag.FOR -> {
                match(Tag.FOR);
                match(40); // 40 è ( in ASCII
                statid();
                bexpr();
                match(41); // 40 è ) in ASCII
                match(Tag.DO);
                stat();
            }

            // GUIDA(<stat> --> if(<bexpr>)<stat>else<stat>end) = FIRST(if) = {if}
            // GUIDA(<stat> --> if(<bexpr>)<stat>end) = FIRST(if) = {if}
            case Tag.IF -> {
                match(Tag.IF);
                match(40); // 40 è ( in ASCII
                bexpr();
                match(41); // 41 è ) in ASCII
                stat();
                statp();
            }

            // GUIDA(<stat> --> {<statlist>}) = FIRST({) = {{}
            case '{' -> {
                match(123); // 123 è { in ASCII
                statlist();
                match(125); // 125 è } in ASCII
            }

            default -> error("Error in term");
        }
    }

    private void statid() {
        switch (look.tag) {
            // GUIDA(<statid> --> id:=<expr>;) = FIRST(id) = {id}
            case Tag.ID -> {
                match(Tag.ID);
                match(Tag.INIT); 
                expr();
                match(59); // 59 è ; in ASCII
            }
        }
    }

    private void statp() {
        switch (look.tag) {
            // GUIDA(<statp> --> else<stat>end) = FIRST(else) = {else}
            case Tag.ELSE -> {
                match(Tag.ELSE);
                stat();
                match(Tag.END);
            }

            // GUIDA(<statp> --> end) = FIRST(end) = {end}
            case Tag.END -> match(Tag.END);
        }
    }
    
    private void assignlist() {
        switch (look.tag) {
            // GUIDA(<assignlist> --> [<expr>to<idlist>]<assignlistp>) = FIRST([) = {[}
            case '[':
                match(91); // 91 è [ in ASCII
                expr();
                match(Tag.TO);
                idlist();
                match(93); // 93 è ] in ASCII
                assignlistp();
                break;

            default:
                error("Error in assignlist");
                break;
        }
    }

    private void assignlistp() {
        switch (look.tag) {
            // GUIDA(<assignlistp> --> [<expr>to<idlist>]<assignlistp>) = FIRST([) = {[}
            case '[':
                match(91); // 91 è [ in ASCII
                expr();
                match(Tag.TO);
                idlist();
                match(93); // 93 è ] in ASCII
                assignlistp();
                break;

            // GUIDA(<assignlistp> --> ε) = FOLLOW(assignlistp) = {;}EOF}
            case ';':
                // no match in epsilon
                break;

            case '}':
                // no match in epsilon
                break;
            case -1: // perché secondo la grammatica statlistp->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in assignlistp");
                break;
        }
    }

    // GUIDA(<idlist> --> ID<idlistp>) = FIRST(ID) = {ID}
    private void idlist() {
        if (look.tag == Tag.ID) {
            match(Tag.ID);
            idlistp();
        } else {
            error("Error in idlist");
        }
    }

    private void idlistp() {
        switch (look.tag) {
            // GUIDA(<idlistp> --> ,ID<idlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII
                match(Tag.ID);
                idlistp();
                break;

            // GUIDA(<idlistp> --> ε) = FOLLOW(<idlistp>) = {)]}
            case ')':
                // no match in epsilon
                break;

            case ']':
                // no match in epsilon
                break;

            default:
                error("Error in idlistp");
        }
    }

    private void bexpr() {
        // GUIDA(<bexpr> --> RELOP<expr><expr>) = FIRST(RELOP) = {RELOP}
        if (look.tag == Tag.RELOP) {
            match(Tag.RELOP);
            expr();
            expr();
        } else {
            error("Error in bexpr");
        }
    }

    private void expr() {
        switch (look.tag) {
            // GUIDA(<expr> --> +(<exprlist>)) = FIRST(+) = {+}
            case '+' -> {
                match(43); // 43 è + in ASCII
                match(40); // 40 è ( in ASCII
                exprlist();
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> -<expr><expr>) = FIRST(-) = {-}
            case '-' -> {
                match(45); // 45 è - in ASCII
                expr();
                expr();
            }

            // GUIDA(<expr> --> *(<exprlist>)) = FIRST(*) = {*}
            case '*' -> {
                match(42); // 42 è * in ASCII
                match(40); // 40 è ( in ASCII
                exprlist();
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> /<expr><expr>) = FIRST(/) = {/}
            case '/' -> {
                match(47); // 47 è / in ASCII
                expr();
                expr();
            }

            // GUIDA(<expr> --> NUM) = FIRST(NUM) = {NUM}
            case Tag.NUM -> match(Tag.NUM);

            // GUIDA(<expr> --> ID) = FIRST(ID) = {ID}
            case Tag.ID -> match(Tag.ID);

            default -> error("Error in expr");
        }
    }

    private void exprlist() {
        switch (look.tag) {
            // GUIDA(<exprlist> --> <expr><exprlistp>) = FIRST(<expr>) = {+,-,*,/,NUM,ID}
            case '+', '-', '*', '/', Tag.NUM, Tag.ID -> {
                expr();
                exprlistp();
            }

            default -> error("Error in exprlist");
        }
    }

    public void exprlistp() {
        switch (look.tag) {
            // GUIDA(<exprlistp> --> ,<expr><exprlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII
                expr();
                exprlistp();
                break;

            // GUIDA(<exprlistp> --> ε) = FOLLOW[<exprlistp>] = {)}
            case ')':
                // no match in epsilon
                break;

            default:
                error("Error in exprlistp");
        }
    }

    public static void main(String[] args) {
        Lexer2x3 lex = new Lexer2x3();
        String path = "files/parser3x2.lft"; // il percorso del file da leggere
        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            Parser3x2 parser = new Parser3x2(lex, br);
            parser.prog();
            System.out.println("Input OK");
            br.close();
        } catch (IOException e) {e.printStackTrace();}
    }
}