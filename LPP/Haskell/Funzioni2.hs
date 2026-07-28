-- funzioni anonime
successore :: Int -> Int
successore = \x -> x+1

-- sezione
s1 = (1 +)
s2 = (`mod` 2)
s3 = (2 `mod`) -- diverso dal s2

-- esercizi 
-- f.g: (f.g(x) = f(g(x)))
anonim1 = ((+ 1).(* 2))
anonim2 = ((== 0) . (`mod` 2)) -- Controlla se è pari il numero


-- funzioni a più argomenti (currying) e applicazione parziale
somma :: Int -> Int -> Int
somma x y = x + y
{-
    prima si riduce a somma x = \y -> x + y
    poi si riduce di nuovo a somma = \x -> \y -> x + y (currying)
    interpretato il tipo come Int -> (Int -> Int)
    mentre l'applicazione è interpretato come (somma x) y
-}

-- esercizi 
potenza :: Int -> Int -> Int 
potenza m 0 = 1
potenza m n = m * potenza m (n-1)

-- Fare potenza di 2 come applicazione parziale di potenza (es. precedente)
pow2 :: Int -> Int
pow2 = potenza 2

---------------------------------------------------------------------------------------------------------------

-- Iterazione e Ricorsione
{-
    fibonacci in java iterattivo
    public static int fibonacci(int k) {
        assert k >= 0; 
        int m = 0;  // contiene il valore prec-prec
        int n = 1;  // contiene il valore prec
        while (k > 0) {
            n = n + m;
            m = n - m;
            k = k - 1;
        }
        return m;
    }

    fibonacci in java ricorsiva
    public static int fibonacci_ric_wrapper(int k) { // il wrapper inizializza lo “stato nascosto” e poi delega tutto al vero algoritmo.
        return fibonacci_ric(k, 0, 1);
    }

    public static int fibonacci_ric(int k, int m, int n) {
        if(k <= 0) return m;
        else {
            return fibonacci_ric(k-1, n, m+n)
        }
    }
-}

-- Versione ricorsiva lineare di fibonacci (con i pattern matching e applicazione parziale)
fibonacciAux :: Int -> Int -> Int -> Int
fibonacciAux m n 0 = m
fibonacciAux m n k = fibonacciAux n (m+n) (k-1)

fibonacciRicWrap :: Int -> Int
fibonacciRicWrap = fibonacciAux 0 1 

-- Versione ricorsiva lineare di fibonacci (con le guardie e applicazione parziale)
fibonacciAux1 :: Int -> Int -> Int -> Int
fibonacciAux1 m n k | k <= 0 = m
                    | otherwise = fibonacciAux1 n (m+n) (k-1)

fibonacciRicWrap1 :: Int -> Int
fibonacciRicWrap1 k = fibonacciAux1 0 1 k -- ARGOMENTO ESPLICITATO


-- senza la where si legge dal alto verso il basso e quindi fibonacciRicWrap2 deve stare sotto fibonacciAux2 ma con where invece si crea una funziona locale alla funzione che chiama la where
fibonacciRicWrap2 :: Int -> Int 
fibonacciRicWrap2 = fibonacciAux2 0 1 --ARGOMENTO IMPLICITO
    where 
    fibonacciAux2 m n 0 = m
    fibonacciAux2 m n k = fibonacciAux1 n (m+n) (k-1)

{- UGUALE MA CON ARGOMENTO ESPLICITATO NELLA FUNZIONA PRINCIPALE ED OMESSO NELLA FUNZIONE CHIAMATA e con le guardie
fibonacciRicWrap2 :: Int -> Int 
fibonacciRicWrap2 k = fibonacciAux2 0 1 
    where 
    fibonacciAux2 m n | k <= 0 = m
                      | otherwise = fibonacciAux1 n (m+n) (k-1)
-}

---------------------------------------------------------------------------------------------------------------------------

{-
    primo in java iterattivo 
    public static boolean primo(int n) {
        assert n >= 0;
        int k = 2;
        while (k < n && n % k != 0) k++;
        return k == n;
    }

    primo in java ricorsivo
    public static int primo_ric_wrapper(int n) { // il wrapper inizializza lo “stato nascosto” e poi delega tutto al vero algoritmo.
        return primo_ric(n, 2);
    }

    public static int primo_ric(int n, int k) {
        if(n <= k) return k == n;
        else if (n % k == 0) return false;
        else return primo_ric(n, k++);
    }
-}

-- Versione ricorsiva lineare di primo (con le guardie e applicazione parziale)
primoAux1 :: Int -> Int -> Bool
primoAux1 k n | n <= k = k == n
              | n `mod` k == 0 = False
              | otherwise = primoAux1 (k+1) n

primoRicWrap1 :: Int -> Bool
primoRicWrap1 = primoAux1 2

-- Versione ricorsiva lineare di primo con where (con le guardie e applicazione parziale)
primoRicWrap2 :: Int -> Bool 
primoRicWrap2 = primoAux2 2 
    where 
    primoAux2 k n | k >= n = k == n
                  | n `mod` k == 0 = False
                  | otherwise = primoAux1 (k+1) n

-- Versione ricorsiva lineare di primo (con i pattern matching non si riesce a fare in maniera pure perché ci sono diverse condizioni numeriche, meglio usare le guardie)
-- USARE LE GUARDIE QUANDO NEL SISTEMA RICORSIVO NON SI DEVE DECREMENTARE FINO AD UN LIMITE (COME 0)
--primoAux :: Int -> Int -> Bool
--primoAux k n = n > k
--primoAux k n = n `mod` k /= 0 && primoAux (k+1) n

--primoRicWrap :: Int -> Bool
--primoRicWrap = primoAux 2 