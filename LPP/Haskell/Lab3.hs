-- Tipi e classi
{-
Uso comando :type in GHCi per mostrare il tipo della costante es. :type True -> True :: Bool
Forzare a vedere un espressione del tipo che si vuole come per un tipo sconosciuto Num a e Fractional a es. 1 :: Float (solo questo 1 verrà visto come float)

Tipo delle funzioni :type not è una funzione e verrà definita come tale es. not :: Bool -> Bool
grazie a :type si riesce ad avere tante informazioni es. :type negate; in cui Num a => a -> a definisce che negate è una funzione che accetta argomenti di qualunque tipo a sia istanza di Num e produce risultati dello stesso tipo dell’argomento
es. :type negate (1 :: Float) che accetta un float (istanza di Num e fornisce un tipo float all'uscita)

Attenzioni alle conversioni che non possono essere fatte da un tipo all'altro (es. 1.5 :: Int), ma si può fare applicando una apposita funzione, tra le seguenti:
fromIntegral :: (Integral a, Num b)      => a -> b
truncate     :: (RealFrac a, Integral b) => a -> b          (tronca un numero frazionario)
round        :: (RealFrac a, Integral b) => a -> b          (arrotonda i numeri)

truncate 1.5 `div` 2 /= 1.5 `div` truncate 2 
truncate 1.5 `div` 2 == 2 `div` truncate 1.5

fromIntegral(2 :: Int) + (3 :: Integer) // Risolve il problema se non ci fosse stato fromIntegral in cui il 2 che era un tipo Int si trasforma in un Num
-}


-- Coppie e Tuple
{-
La tupla è sequenza ordinata e finita di elementi anche di tipi diversi, definita tramite parentesi tonde e separate da virgola es. (1, True)
:type (False, (2, True)) verrà definito come Num a => (Bool, (a, Bool)) dove Num a (o anche Fractional) definisce il valore numerico inserito nella tupla
:type (1, (2, 3)) definito come (Num a1, Num a2, Num b) => (a1, (a2, b))
Si possono confrontare tuple solo se sono dello stesso tipo, ad esempio questo confronto non vale (False, 2, True) /= (False, (2, True))

addizione1 x y = x + y
addizione2 :: (Int, Int) -> Int
addizione2 (x, y) = x + y // come argomento vuole una coppia (considerato un solo argomento)
-}

scambia :: (Int, Int) -> (Int, Int)
scambia (x, y) = (y, x) 

ordina :: (Int, Int, Int) -> (Int, Int, Int)
ordina (x, y, z) | x > y = ordina(y, x, z)
                 | y > z = ordina(x, z, y)
                 | otherwise = (x, y, z)


-- Liste
{-
La lista è una sequenza omogenea di elementi di tipi solo uguali, definita tramite parentesi quadre e separate da virgola 
    []                 -- lista vuota
    [1]                -- lista che contiene il solo numero 1
    [1, 2, 3]          -- lista con i numeri 1, 2 e 3
    [0.5, 3]           -- lista con i numeri floating-point 0.5 e 3; :t [0.5, 3] :: Fractional a => [a]
    [True, False]      -- lista con i valori True e False
    [[], [1, 2], [3]]  -- lista di liste di numeri

    [1..10]     -- lista con i numeri (interi) da 1 a 10
    [1..]       -- lista infinita da 1 in poi

    Costruttori canonici (per costruire una lista o aggiungere un elemento alla lista)
    1 : 2 : 3 : [] 
    dove rappresenta X : L  primo elemento X (testa) e continua con la lista L (coda)
    1 : 2 : 3 : [] == 1 : (2 : (3 : [])) // Associativo a destra

    tipi della lista ottenuta mettendo tra le parentesi quadrate il tipo dei suoi elementi, in cui L ha tipo [T] dove T è il tipo degli elementi presenti nella lista L
    es. [] ha tipo [a] dove a rappresenta un tipo sconosciuto (Haskell non è in grado di imparare nulla di più sul tipo della lista)
    può essere usato l'operatore :: per forzare la la conversione dell'espressione es. :type [1, 2, 3] :: [Int] sarà di tipo Int invece che Num

    length :: [a] -> Int 
    Prende una lista e ritorna un Int che sarà il numero degli elementi presenti nella lista
    funzioni per le liste:
    length, sum, product, [] ++ [] (append, concatena le liste)

    si possono fare le operazioni negli elementi della lista, ma attenzione alle espressioni tipate  
-}

media :: [Int] -> Float
media [] = -1 -- in caso di lista vuota restituisce -1 perché non si può fare la divisione per zero
media xs = fromIntegral (sum xs) / fromIntegral (length xs) -- verranno considerate Num dopo fromIntegral in modo che possano fare l'operazione con / (che vale solo per i numeri con la virgola)

fact :: Int -> Int -- fattoriale usando la lista con la funzione product
fact n = product [2..n]

-- Esercizio di intervallo con le guardie
intervallo :: Int -> Int -> [Int] -- Int -> (Int -> [Int]) visto come: due numeri in ingresso di tipo Int emmettono in uscita una lista di tipo Int
intervallo m n | m > n = []
               | m == n = [n] -- non serve questa riga essendo che viene creata una lista vuota appena m > n
               | otherwise = m : intervallo (m+1) n 

intervallo1 :: Int -> Int -> [Int] -- Int -> (Int -> [Int]) visto come: due numeri in ingresso di tipo Int emmettono in uscita una lista di tipo Int
intervallo1 m n | m > n = []
                | m == n = [n] -- non serve questa riga essendo che viene creata una lista vuota appena m > n
                | otherwise = m : intervallo1 (m+1) n 

{-
    Pattern matching di liste

    uso dei costruttori canonici (per costruire una lista o aggiungere un elemento alla lista)
    somma :: [Int] -> Int
    somma []       = 0
    somma (x : xs) = x + somma xs -- Viene controllato se la lista ha la forma (non vuota) x : xs (testa x e coda xs) e poi prende il primo elemento (testa) e lo somma alla somma della coda (xs)

    smontare la lista col pattern matching più profondo (prendendo ad esempio i primi 2 elementi)
    ordinata :: [Int] -> Bool
    ordinata []           = True
    ordinata [_]          = True
    ordinata (x : y : xs) = x <= y && ordinata (y : xs)
    ATTENZIONE: se al posto di ordinata (y : xs) si fosse messo ordinata xs si otterrebbe una funzione che confronta gli elementi della lista a coppie, ma non senza controllare che gli elementi di coppie diverse siano in relazione
    es. ordinata [1, 2, 0] == True invece di False, perché confronta 1 <= 2 e 2 <= 0 ma non 1 <= 0

    pattern matching su più argomenti
    stessaLunghezza :: [Int] -> [Int] -> Bool
    stessaLunghezza xs ys = length xs == length ys
    ha un costo computazionale proporzionale alla lunghezza di xs

    altro metodo migliore (se una è infinita e l'altra no allora si ferma prima grazie alla 2° e 3° clausola, altrimenti se sono entrambe fallisce la funzione):
    stessaLunghezza :: [Int] -> [Int] -> Bool
    stessaLunghezza []       []       = True
    stessaLunghezza []       _        = False
    stessaLunghezza _        []       = False
    stessaLunghezza (_ : xs) (_ : ys) = stessaLunghezza xs ys
    il pattern _ corrisponde a qualsiasi lista (anche infinita)
-}

prodotto :: [Int] -> Int
prodotto [] = 1
prodotto (x : xs) = x * prodotto xs


inverti :: [Int] -> [Int]
inverti [] = [] 
inverti (x : xs) = inverti (xs) ++ [x]


-- Insertion Sort
-- La insert che segue funziona sottoipotesi che la lista passata come secondo argomento sia ordinata
insert :: Int -> [Int] -> [Int]
insert x [] = [x]
insert x (y : ys) | x <= y    = x : y : ys
                  | otherwise = y : insert x ys

insertSort :: [Int] -> [Int]
insertSort []       = []
insertSort (x : xs) = insert x (insertSort xs)

-- Insertion Sort con l'uso della clausola where
insertSort2 :: [Int] -> [Int]
insertSort2 []       = []
insertSort2 (x : xs) = insert x (insertSort2 xs)
  where
    insert x [] = [x]
    insert x (y : ys) | x <= y    = x : y : ys
                      | otherwise = y : insert x ys


-- Merge Sort
-- Operazione di split (divisione in due parti di lunghezza comparabile della lista):
-- Uso del pattern matching sui risultati (non solo su argomento)
split :: [Int] -> ([Int], [Int])
split []           = ([], [])
split [x]          = ([x], []) -- Si è deciso di metterlo nella lista a sx, ma si può metterlo anche in quello a dx
split (x : y : xs) = (x : ys, y : zs)
  where
    (ys, zs) = split xs

-- Operazione di merge (date due liste ordinate crea una lista unica ordinata delle due)
merge :: [Int] -> [Int] -> [Int]
merge []       ys = ys
merge xs       [] = xs
merge (x : xs) (y : ys) | x <= y    = x : merge xs (y : ys) 
                        | otherwise = y : merge (x : xs) ys

-- Algoritmo del mergeSort
mergeSort :: [Int] -> [Int]
mergeSort []  = []
mergeSort [x] = [x]
mergeSort xs  = merge (mergeSort ys) (mergeSort zs)
  where
    (ys, zs) = split xs

-- ATTENZIONE, senza la riga "mergeSort [x] = [x]", si avrebbe un problema
-- Senza quell’equazione, tentando di ordinare una lista singoletto si otterrebbe una partizione in cui ys è di nuovo una lista singoletto, pertanto l’applicazione mergeSort ys causerebbe una ricorsione infinita.