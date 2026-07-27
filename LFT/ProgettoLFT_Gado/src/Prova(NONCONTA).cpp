import java.io.*;

public class Translator5x1 {
    private Lexer2x3 lex;
    private BufferedReader pbr;
    private Token look;

    SymbolTable st = new SymbolTable();
    CodeGenerator code = new CodeGenerator();
    int count=0;

    public Translator5x1(Lexer2x3 l, BufferedReader br) {
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
                int lnext_prog = code.newLabel();
                statlist(lnext_prog);
                code.emitLabel(lnext_prog);
                match(Tag.EOF);
                try {
                    code.toJasmin();
                }
                catch(java.io.IOException e) {
                    System.out.println("IO error\n");
                };
            }
            default -> error("Error in prog");
        }
    }

    // GUIDA(<statlist> --> <stat><statlistp>) = FIRST(<stat>) = {assign, print, read, for, if, { }
    private void statlist(int lnext) {
        switch (look.tag) {
            case Tag.ASSIGN, Tag.PRINT, Tag.READ, Tag.FOR, Tag.IF, '{' -> {
                int lnext_stat = code.newLabel();
                stat(lnext);
                code.emitLabel(lnext_stat);
                statlistp(lnext_stat);
            }
            default -> error("Error in statlist");
        }
    }

    private void statlistp(int next) {
        switch (look.tag) {
            // GUIDA(<statlistp> --> ;<stat><statlistp>) = FIRST(;) = {;}
            case ';':
                int lnext_stat = code.newLabel();
                match(59); // 59 è ; in ASCII
                stat(lnext_stat);
                code.emitLabel(lnext_stat);
                statlistp(lnext_stat);
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

    private void stat(int next) {
        switch (look.tag) {
            // GUIDA(<stat> --> assign<assignlist>) = FIRST(assign) = {assign}
            case Tag.ASSIGN -> {
                match(Tag.ASSIGN);
                assignlist(next);
            }

            // GUIDA(<stat> --> print(<exprlist>) = FIRST(print) = {print}
            case Tag.PRINT -> {
                match(Tag.PRINT);
                match(40); // 40 è ( in ASCII
                exprlist(1); // passa 1 per identificare la print
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> read(<idlist>) = FIRST(read) = {read}
            case Tag.READ -> {
                int value = 0;
                match(Tag.READ);
                match(40); // 40 è ( in ASCII
                code.emit(OpCode.invokestatic, 0);
                idlist(1, value, next); // passa 1 per identificare la read (0 per identificare la assignlist)
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> for(ID:=<expr>;<bexpr>)do<stat> = FIRST(for) = {for}
            // GUIDA(<stat> --> for(<bexpr>)do<stat> = FIRST(for) = {for}
            case Tag.FOR -> {
                int for_true = code.newLabel();
                int for_false = next;
                int stat_next = code.newLabel();

                match(Tag.FOR);
                match(40); // 40 è ( in ASCII
                exprfor(for_true, for_false, stat_next);
                match(41); // 40 è ) in ASCII
                match(Tag.DO);

                code.emitLabel(for_true); // label caratterizzata da for_true che permette di fare JMP qui dopo che la condizione è vera 

                stat(next);

                code.emit(OpCode.GOto, stat_next); // label che fa il salto all'inizio del ciclo
            }

            // GUIDA(<stat> --> if(<bexpr>)<stat>else<stat>end) = FIRST(if) = {if}
            // GUIDA(<stat> --> if(<bexpr>)<stat>end) = FIRST(if) = {if}
            case Tag.IF -> {
                int if_true = code.newLabel();
                int if_false = code.newLabel();

                match(Tag.IF);
                match(40); // 40 è ( in ASCII

                bexpr(if_true, if_false);
                match(41); // 41 è ) in ASCII

                code.emitLabel(if_true);

                stat(next);
                code.emit(OpCode.GOto, next);
                code.emitLabel(if_false);
                exprif(next);            }

            // GUIDA(<stat> --> {<statlist>}) = FIRST({) = {{}
            case '{' -> {
                match(123); // 123 è { in ASCII
                statlist(next);
                match(125); // 125 è } in ASCII
            }

            default -> error("Error in term");
        }
    }

    private void exprfor(int for_true, int for_false, int stat_next) {
        switch (look.tag) {
            case Tag.ID:
                String temp = "";
                int temp_id_addr = 0;
                int id_addr = st.lookupAddress(((Word) look).lexeme);
                if (id_addr == -1) {
                    temp_id_addr = count;
                    temp =((Word) look).lexeme;
                }

                match(Tag.ID);
                match(Tag.INIT);
                expr();

                if (id_addr == -1) {
                    id_addr = temp_id_addr;
                    st.insert(temp, count++);
                    code.emit(OpCode.istore, id_addr);
                }

                match(';');
                code.emitLabel(stat_next);
                bexpr(for_true, for_false);
                break;
            
            case Tag.RELOP:
                code.emitLabel(stat_next);
                bexpr(for_true, for_false);
                break;
            
            default:
                error("Expected either ID := expr; or RELOP in for loop");
        }
    }

    private void exprif(int next) {
        switch (look.tag) {
            case Tag.ELSE:
                match(Tag.ELSE);
                stat(next);
                match(Tag.END);
                break;
            case Tag.END:
                match(Tag.END);
                code.emit(OpCode.GOto, next);
                break;
            default:
                error("Error in ifexpr : Unexpected " + ((char) look.tag));
            }
            /*
            // GUIDA(<statp> --> else<stat>end) = FIRST(else) = {else}
            case Tag.ELSE -> {
                code.emit(OpCode.GOto, if_end); // label che fa il salto alla fine del if (non c'è l'else)
                code.emitLabel(if_false); // label caratterizzata da if_false che permette di fare JMP qui dopo che l'if ha l'else

                match(Tag.ELSE);
                stat(next);
                match(Tag.END);
            }

            // GUIDA(<statp> --> end) = FIRST(end) = {end}
            case Tag.END -> {
                code.emit(OpCode.GOto, if_end);
                code.emitLabel(if_false); // label caratterizzata da if_false che permette di fare JMP qui dopo che l'if ha l'else

                match(Tag.END);
                code.emit(OpCode.GOto, next);
            }

            default -> error("Error in statp: Unexpected");
            */
        }
    
    private void assignlist(int next) {
        switch (look.tag) {
            // GUIDA(<assignlist> --> [<expr>to<idlist>]<assignlistp>) = FIRST([) = {[}
            case '[':
                int value = 0;
                match(91); // 91 è [ in ASCII
                value = expr();
                match(Tag.TO);
                idlist(0, value, next);
                match(93); // 93 è ] in ASCII
                assignlistp(next);
                break;

            default:
                error("Error in assignlist");
                break;
        }
    }

    private void assignlistp(int next) {
        switch (look.tag) {
            // GUIDA(<assignlistp> --> [<expr>to<idlist>]<assignlistp>) = FIRST([) = {[}
            case '[':
                int value = 0;
                match(91); // 91 è [ in ASCII
                value = expr();
                match(Tag.TO);
                idlist(0, value, next);
                match(93); // 93 è ] in ASCII
                assignlistp(next);
                break;

            // GUIDA(<assignlistp> --> ε) = FOLLOW(assignlistp) = {;}EOF}
            case ';':
                // no match in epsilon
                break;

            case '}':
                // no match in epsilon
                break;

            case Tag.ELSE:
                break;
            
            case Tag.END:
                break;

            case -1: // perché secondo la grammatica statlistp->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in assignlistp");
                break;
        }
    }

    // GUIDA(<idlist> --> ID<idlistp>) = FIRST(ID) = {ID}
    private void idlist(int read_assign, int value, int next) {
        if (look.tag == Tag.ID) {
            // controlliamo con lookupAddress se l'ID è già presente in memoria, altrimenti con id_addr-1 e ne inserisce uno nuovo
            int id_addr = st.lookupAddress(((Word)look).lexeme);
            if (id_addr == -1) {
                id_addr = count;
                st.insert(((Word)look).lexeme, count++);
            }

            match(Tag.ID);

            if (read_assign == 1) 
                code.emit(OpCode.invokestatic, 0); // dato da read(); quindi lo carico da Output.read() in cui si aspetta di leggere un valore da input
            code.emit(OpCode.istore, id_addr); // memorizza idlist nella variabile locale con indirizzo id_addr

            idlistp(read_assign, value, next);
        } else {
            error("Error in idlist");
        }
    }

    private void idlistp(int read_assign, int value, int next) {
        switch (look.tag) {
            // GUIDA(<idlistp> --> ,ID<idlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII

                int id_addr = st.lookupAddress(((Word)look).lexeme);
                if (id_addr == -1) {
                    id_addr = count;
                    st.insert(((Word)look).lexeme, count++);
                }

                match(Tag.ID);

                if (read_assign == 0) {
                    code.emit(OpCode.ldc, value);
                    code.emit(OpCode.istore, id_addr);
                } else {
                    code.emit(OpCode.invokestatic, 0);
                    code.emit(OpCode.istore, id_addr);
                }

                idlistp(read_assign, value, next);
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

    private void bexpr(int label_true, int label_false) {
        // GUIDA(<bexpr> --> RELOP<expr><expr>) = FIRST(RELOP) = {RELOP}
        if (look.tag == Tag.RELOP) {
            String relop = ((Word)look).lexeme; // permette di salvare il valore di RELOP in una variabile locale per il JMP

            match(Tag.RELOP);
            expr();
            expr();

            switch(relop) {
                case "<" -> code.emit(OpCode.if_icmplt, label_true);
                case ">" -> code.emit(OpCode.if_icmpgt, label_true);
                case "==" -> code.emit(OpCode.if_icmpeq, label_true);
                case "<=" -> code.emit(OpCode.if_icmple, label_true);
                case "<>" -> code.emit(OpCode.if_icmpne, label_true);
                case ">=" -> code.emit(OpCode.if_icmpge, label_true);
                default -> error("Error in Word.java RELOP definition");
            }
            //code.emit(OpCode.GOto, label_false);

        } else {
            error("Error in bexpr");
        }
        code.emit(OpCode.GOto, label_false);
    }

    private int expr() {
        int value = 0;
        switch (look.tag) {
            // GUIDA(<expr> --> +(<exprlist>)) = FIRST(+) = {+}
            case '+' -> {
                match(43); // 43 è + in ASCII
                match(40); // 40 è ( in ASCII
                exprlist(0); // passa a 0 per identificare la somma
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> -<expr><expr>) = FIRST(-) = {-}
            case '-' -> {
                match(45); // 45 è - in ASCII
                expr();
                expr();
                code.emit(OpCode.isub); // genera il bytecode isub
            }

            // GUIDA(<expr> --> *(<exprlist>)) = FIRST(*) = {*}
            case '*' -> {
                match(42); // 42 è * in ASCII
                match(40); // 40 è ( in ASCII
                exprlist(2); // passa a 2 per identificare la moltiplicazione
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> /<expr><expr>) = FIRST(/) = {/}
            case '/' -> {
                match(47); // 47 è / in ASCII
                expr();
                expr();
                code.emit(OpCode.idiv); // genera il bytecode idiv
            }

            // GUIDA(<expr> --> NUM) = FIRST(NUM) = {NUM}
            case Tag.NUM -> {
                code.emit(OpCode.ldc, ((NumberTok)look).value); // fa il caricamento di un valore costante intero (o float) sullo stack.
                value = (((NumberTok)look).value);
                match(Tag.NUM);
            }

            // GUIDA(<expr> --> ID) = FIRST(ID) = {ID}
            case Tag.ID -> {
                int id_addr = st.lookupAddress(((Word)look).lexeme);
                if (id_addr == -1) {
                    error("Error in expr() : identifier not defined");
                }

                match(Tag.ID);
                code.emit(OpCode.iload, id_addr);
            }

            default -> error("Error in expr");
        }
        return value;
    }

    private void exprlist(int print_somma_moltiplicazione) {
        switch (look.tag) {
            // GUIDA(<exprlist> --> <expr><exprlistp>) = FIRST(<expr>) = {+,-,*,/,NUM,ID}
            case '+', '-', '*', '/', Tag.NUM, Tag.ID -> {
                expr();
                if (print_somma_moltiplicazione == 1) {
                    code.emit(OpCode.invokestatic, 1); // dato da print(); quindi lo carico da Output.print() in cui stampa il valore
                }
                exprlistp(print_somma_moltiplicazione);
            }

            default -> error("Error in exprlist");
        }
    }

    public void exprlistp(int print_somma_moltiplicazione) {
        switch (look.tag) {
            // GUIDA(<exprlistp> --> ,<expr><exprlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII
                expr();

                switch (print_somma_moltiplicazione) {
                    case 0 -> code.emit(OpCode.iadd);
                    case 1 -> code.emit(OpCode.invokestatic, 1);
                    case 2 -> code.emit(OpCode.imul);
                }

                exprlistp(print_somma_moltiplicazione);
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
        String path = "files/translator5x1.lft"; // il percorso del file da leggere
        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            Translator5x1 translator = new Translator5x1(lex, br);
            translator.prog();
            System.out.println("Input OK");
            br.close();
        } catch (IOException e) {e.printStackTrace();}
    }
}


























import java.io.*;

public class Translator5x1 {
    private Lexer2x3 lex;
    private BufferedReader pbr;
    private Token look;

    SymbolTable st = new SymbolTable();
    CodeGenerator code = new CodeGenerator();
    int count=0;

    public Translator5x1(Lexer2x3 l, BufferedReader br) {
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
        int lnext_prog = code.newLabel();

        switch (look.tag) {
            case Tag.ASSIGN, Tag.PRINT, Tag.READ, Tag.FOR, Tag.IF, '{' -> {
                statlist();
                code.emitLabel(lnext_prog);
                match(Tag.EOF);
                try {
                    code.toJasmin();
                }
                catch(java.io.IOException e) {
                    System.out.println("IO error\n");
                };
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
                exprlist(1); // passa 0 per identificare la print
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> read(<idlist>) = FIRST(read) = {read}
            case Tag.READ -> {
                int value = 0;
                match(Tag.READ);
                match(40); // 40 è ( in ASCII
                idlist(1, value); // passa 1 per identificare la read (0 per identificare la assignlist)
                match(41); // 40 è ) in ASCII
            }

            // GUIDA(<stat> --> for(ID:=<expr>;<bexpr>)do<stat> = FIRST(for) = {for}
            // GUIDA(<stat> --> for(<bexpr>)do<stat> = FIRST(for) = {for}
            case Tag.FOR -> {
                int for_true = code.newLabel();
                int for_false = code.newLabel();
                int for_start = code.newLabel();

                code.emitLabel(for_start); // label caratterizzata da for_start che permette di fare JMP qui dopo che il for viene eseguito

                match(Tag.FOR);
                match(40); // 40 è ( in ASCII
                
                statid();
                bexpr(for_true, for_false); 
                match(41); // 40 è ) in ASCII
                match(Tag.DO);

                code.emitLabel(for_true); // label caratterizzata da for_true che permette di fare JMP qui dopo che la condizione è vera 

                stat();

                code.emit(OpCode.GOto, for_start); // label che fa il salto all'inizio del ciclo
                code.emitLabel(for_false); // label caratterizzata da for_true che permette di fare JMP qui dopo che la condizione è falsa
            }

            // GUIDA(<stat> --> if(<bexpr>)<stat>else<stat>end) = FIRST(if) = {if}
            // GUIDA(<stat> --> if(<bexpr>)<stat>end) = FIRST(if) = {if}
            case Tag.IF -> {
                int if_true = code.newLabel();
                int if_false = code.newLabel();
                int if_end = code.newLabel();

                match(Tag.IF);
                match(40); // 40 è ( in ASCII

                bexpr(if_true, if_false);
                match(41); // 41 è ) in ASCII

                code.emitLabel(if_true);

                stat();
                statp(if_false, if_end); // bisogna considerare i due casi di if (con o senza else)
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

    private void statp(int if_false, int if_end) {
        switch (look.tag) {
            // GUIDA(<statp> --> else<stat>end) = FIRST(else) = {else}
            case Tag.ELSE -> {
                code.emit(OpCode.GOto, if_end); // label che fa il salto alla fine del if (non c'è l'else)
                code.emitLabel(if_false); // label caratterizzata da if_false che permette di fare JMP qui dopo che l'if ha l'else

                match(Tag.ELSE);
                stat();
                match(Tag.END);

                code.emitLabel(if_end);
            }

            // GUIDA(<statp> --> end) = FIRST(end) = {end}
            case Tag.END -> {
                code.emit(OpCode.GOto, if_end);

                match(Tag.END);

                code.emitLabel(if_false);
                code.emitLabel(if_end);
            }
        }
    }
    
    private void assignlist() {
        switch (look.tag) {
            // GUIDA(<assignlist> --> [<expr>to<idlist>]<assignlistp>) = FIRST([) = {[}
            case '[':
                int value = 0;
                match(91); // 91 è [ in ASCII
                value = expr();
                match(Tag.TO);
                idlist(0, value);
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
                int value = 0;
                match(91); // 91 è [ in ASCII
                value = expr();
                match(Tag.TO);
                idlist(0, value);
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

            case Tag.ELSE:
                break;
            
            case Tag.END:
                break;

            case -1: // perché secondo la grammatica statlistp->ε, quindi può accedere se il prossimo token è EOF(-1)
                break;

            default:
                error("Error in assignlistp");
                break;
        }
    }

    // GUIDA(<idlist> --> ID<idlistp>) = FIRST(ID) = {ID}
    private void idlist(int read_assign, int value) {
        if (look.tag == Tag.ID) {
            // controlliamo con lookupAddress se l'ID è già presente in memoria, altrimenti con id_addr-1 e ne inserisce uno nuovo
            int id_addr = st.lookupAddress(((Word)look).lexeme);
            if (id_addr == -1) {
                id_addr = count;
                st.insert(((Word)look).lexeme, count++);
            }

            match(Tag.ID);

            if (read_assign == 1) {
                code.emit(OpCode.invokestatic, 0); // dato da read(); quindi lo carico da Output.read() in cui si aspetta di leggere un valore da input
            }
            code.emit(OpCode.istore, id_addr); // memorizza idlist nella variabile locale con indirizzo id_addr

            idlistp(read_assign, value);
        } else {
            error("Error in idlist");
        }
    }

    private void idlistp(int read_assign, int value) {
        switch (look.tag) {
            // GUIDA(<idlistp> --> ,ID<idlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII

                int id_addr = st.lookupAddress(((Word)look).lexeme);
                if (id_addr == -1) {
                    id_addr = count;
                    st.insert(((Word)look).lexeme, count++);
                }

                match(Tag.ID);

                if (read_assign == 0) {
                    code.emit(OpCode.ldc, value);
                    code.emit(OpCode.istore, id_addr);
                } else {
                    code.emit(OpCode.invokestatic, 0);
                    code.emit(OpCode.istore, id_addr);
                }

                idlistp(read_assign, value);
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

    private void bexpr(int label_true, int label_false) {
        // GUIDA(<bexpr> --> RELOP<expr><expr>) = FIRST(RELOP) = {RELOP}
        if (look.tag == Tag.RELOP) {
            String relop = ((Word)look).lexeme; // permette di salvare il valore di RELOP in una variabile locale per il JMP

            match(Tag.RELOP);
            expr();
            expr();

            switch(relop) {
                case "<" -> code.emit(OpCode.if_icmplt, label_true);
                case ">" -> code.emit(OpCode.if_icmpgt, label_true);
                case "==" -> code.emit(OpCode.if_icmpeq, label_true);
                case "<=" -> code.emit(OpCode.if_icmple, label_true);
                case "<>" -> code.emit(OpCode.if_icmpne, label_true);
                case ">=" -> code.emit(OpCode.if_icmpge, label_true);
                default -> error("Error in Word.java RELOP definition");
            }

        } else {
            error("Error in bexpr");
        }
        code.emit(OpCode.GOto, label_false);
    }

    private int expr() {
        int value = 0;
        switch (look.tag) {
            // GUIDA(<expr> --> +(<exprlist>)) = FIRST(+) = {+}
            case '+' -> {
                match(43); // 43 è + in ASCII
                match(40); // 40 è ( in ASCII
                exprlist(0); // passa a 0 per identificare la somma
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> -<expr><expr>) = FIRST(-) = {-}
            case '-' -> {
                match(45); // 45 è - in ASCII
                expr();
                expr();
                code.emit(OpCode.isub); // genera il bytecode isub
            }

            // GUIDA(<expr> --> *(<exprlist>)) = FIRST(*) = {*}
            case '*' -> {
                match(42); // 42 è * in ASCII
                match(40); // 40 è ( in ASCII
                exprlist(2); // passa a 2 per identificare la moltiplicazione
                match(41); // 41 è ) in ASCII
            }

            // GUIDA(<expr> --> /<expr><expr>) = FIRST(/) = {/}
            case '/' -> {
                match(47); // 47 è / in ASCII
                expr();
                expr();
                code.emit(OpCode.idiv); // genera il bytecode idiv
            }

            // GUIDA(<expr> --> NUM) = FIRST(NUM) = {NUM}
            case Tag.NUM -> {
                code.emit(OpCode.ldc, ((NumberTok)look).value); // fa il caricamento di un valore costante intero (o float) sullo stack.
                value = (((NumberTok)look).value);
                match(Tag.NUM);
            }

            // GUIDA(<expr> --> ID) = FIRST(ID) = {ID}
            case Tag.ID -> {
                int id_addr = st.lookupAddress(((Word)look).lexeme);
                if (id_addr == -1) {
                    error("Error in expr() : identifier not defined");
                }
                match(Tag.ID);
                code.emit(OpCode.iload, id_addr);
            }

            default -> error("Error in expr");
        }
        return value;
    }

    private void exprlist(int print_somma_moltiplicazione) {
        switch (look.tag) {
            // GUIDA(<exprlist> --> <expr><exprlistp>) = FIRST(<expr>) = {+,-,*,/,NUM,ID}
            case '+', '-', '*', '/', Tag.NUM, Tag.ID -> {
                expr();
                if (print_somma_moltiplicazione == 1) {
                    code.emit(OpCode.invokestatic, 1); // dato da print(); quindi lo carico da Output.print() in cui stampa il valore
                }
                exprlistp(print_somma_moltiplicazione);
            }

            default -> error("Error in exprlist");
        }
    }

    public void exprlistp(int print_somma_moltiplicazione) {
        switch (look.tag) {
            // GUIDA(<exprlistp> --> ,<expr><exprlistp>) = FIRST(,) = {,}
            case ',':
                match(44); // 44 è , in ASCII
                expr();

                switch (print_somma_moltiplicazione) {
                    case 0 -> code.emit(OpCode.iadd);
                    case 1 -> code.emit(OpCode.invokestatic, 1);
                    case 2 -> code.emit(OpCode.imul);
                }

                exprlistp(print_somma_moltiplicazione);
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
        String path = "files/translator5x1.lft"; // il percorso del file da leggere
        try {
            BufferedReader br = new BufferedReader(new FileReader(path));
            Translator5x1 translator = new Translator5x1(lex, br);
            translator.prog();
            System.out.println("Input OK");
            br.close();
        } catch (IOException e) {e.printStackTrace();}
    }
}