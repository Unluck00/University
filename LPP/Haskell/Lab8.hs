-- Caratteri e stringhe
-- sequenze di escape riconosciute
{-
    '\n' -- ritorno a capo
    '\t' -- tabulazione
    '\\' -- backslash
    '\'' -- apice singolo
    '\"' -- doppi apici
-}

-- tipo dei Caratteri i chiama Char ed è distinto dal (e incompatibile con il) tipo dei numeri interi.  :type '0'

-- Il tipo delle liste si chiama String e in Haskell tale tipo è solo un alias per il tipo [Char]. In altre parole, le stringhe sono rappresentate come liste di caratteri.


{-
    Definire una funzione showHex :: Int -> String che, applicata a un numero intero, ne restituisca la rappresentazione in base 16. 
    Fare attenzione alla possibilità che il numero sia negativo. 
    Suggerimento: definire una funzione ausiliaria per convertire un numero intero compreso tra 0 e 15 (estremi inclusi) nel carattere corrispondente in base 16.
-}

import Data.Char (ord, chr)

showHex :: Int -> String
showHex n | n < 0     = '-' : showHex (negate n)
          | n < 16    = [aux n]
          | otherwise = showHex (n `div` 16) ++ [aux (n `mod` 16)]
  where
    aux n | n < 10    = chr (ord '0' + n)
          | otherwise = chr (ord 'A' + n - 10)


-- Input/output
-- operazioni di I/O sono racchiuse nel tipo speciale IO a causa della natura non puramente funzionale delle operazioni di I/O.
-- La funzione principale di un programma Haskell è sempre di tipo IO ().
main :: IO ()
main = putStrLn "Hello, world!"

-- length [print 2, print True, print "ciao"]
-- La funzione length calcola la lunghezza della lista senza eseguire le azioni di I/O contenute nella lista stessa.

-- Il tipo monade è un concetto astratto che generalizza il concetto di computazione sequenziale.
-- In Haskell, le monadi sono utilizzate per gestire effetti collaterali, come l'I/O, in un modo che mantiene la purezza funzionale del linguaggio.
-- Le monadi forniscono un modo per incapsulare valori e operazioni in un contesto, permettendo di combinare computazioni che producono effetti collaterali in modo controllato.

{-
    a causa della valutazione lazy di Haskell, le funzioni con effetti collaterali (impure) non possono essere gestite direttamente, come dimostrato dall'esempio di length [print 2, print True, print "ciao"] che non stampa nulla perché print non viene mai eseguito. Viene quindi introdotta la monade IO come struttura per descrivere azioni che, se eseguite da un esecutore, possono avere effetti collaterali.
    Informazioni essenziali:
    Haskell è un linguaggio con valutazione lazy, rendendo difficile prevedere l'ordine di valutazione delle espressioni.
    Non è possibile utilizzare funzioni "impure" con effetti collaterali direttamente in Haskell a causa della valutazione lazy.
    La monade IO è una struttura utilizzata in Haskell per gestire computazioni/azioni che possono avere effetti collaterali quando eseguite.
    Un esempio mostra che le funzioni print in una lista non vengono eseguite se la funzione length non ne utilizza gli elementi.

    In linguaggi a valutazione lazy come Haskell, l'ordine di esecuzione è imprevedibile, rendendo difficile gestire le funzioni impure in modo coerente. 
    Per questo motivo, Haskell utilizza costrutti specifici come le monadi (es. IO) per gestire tali azioni in modo controllato.
-}

-- In linguaggi a valutazione lazy come Haskell, l'ordine di esecuzione è imprevedibile, rendendo difficile gestire le funzioni impure in modo coerente. 
-- Per questo motivo, Haskell utilizza costrutti specifici come le monadi (es. IO) per gestire tali azioni in modo controllato.

-- Per ogni monade è inoltre disponibile l’operatore >>, tradizionalmente pronunciato and then, che consiste in una variante di >>= in cui il valore prodotto dalla prima azione è ignorato. In altri, termini, >> è definito così:
    -- (>>) :: Monad m => m a -> m b -> m b
    -- (>>) m₁ m₂ = m₁ >>= const m₂


-- :type putChar; si ha che putChar permette in output un carattere e restituisce un'azione di tipo IO (). 


-- Le azioni getChar e getLine possono essere usate per leggere dal terminale un singolo carattere e un’intera riga di testo.
-- In particolare, getChar è un’azione che, se eseguita, legge un carattere dal terminale e produce quel carattere come risultato.
-- Analogamente, getLine è un’azione che, se eseguita, legge una riga di testo dal terminale e produce quella riga come risultato.