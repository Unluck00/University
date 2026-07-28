-- Polimorfismo
{- 
    Quando possiamo dare più di un tipo a un espressione/funzione, infatti
    id x = x (la funzione identità restituisce il proprio argomento, non dipende da nessuna proprietà dell'argomento)
    :t id
    id :: p -> p (id è un tipo p -> p (detto variabile di tipo), dove p è qualsiasi tipo)
    
    si può fare:
    idapp = id id (id applicato a id, ma Haskell dà alla prima id il tipo (p -> p) -> p -> p)

    invece se applichiamo;
    id 6 (id avrà come tipo Num; id :: Num -> Num)

    il tipo di identità viene dato in base all'argomento, inoltre
    dare id i vari tipi diversi (Int -> Int, Bool -> Bool, ecc..) vale a dire che sono istanze del tipo p -> p


    Si possono esprimere vincoli tra domini e codomini di una funzione usando le variabili di tipo, es.

    Concatenazione di liste
        (++) :: [a] -> [a] -> [a]
        (++) []       ys = ys
        (++) (x : xs) ys = x : (++) xs ys
        specifica che le due liste da concatenare possono avere elementi di tipo arbitrario (qualsiasi argomento), purché il tipo sia lo stesso nelle due liste (il tipo a).

    Composizione funzionale
        (.) :: (b -> c) -> (a -> b) -> a -> c
        (.) f g x = f (g x) 
        indica che una composizione f.g è ben tipata purché il codominio di g coincida con il dominio di f
-}

-- Esercizi 
{-
    Determinare il tipo più generale delle seguenti funzioni della libreria standard.

    fst (x, _)       = x                -->     fst :: (a,b) -> a       (le tuple possono avere elementi con tipi diversi)
    snd (_, y)       = y                -->     snd :: (a,b) -> b 
    const x _        = x                -->     const :: a -> b -> a    (funzione constante definita con argomento da la funzione constante) 
    curry f x y      = f (x, y)         -->     curry :: ( (a,b) -> c ) -> a -> b -> c      (\f -> \x -> \y -> f (x,y))     (f = \x -> \y -> f (x,y))
        la funzione curry trasforma una funzione (che prende una coppia come argomento (cioè una funzione che lavora su una tupla)) in una funzione che prende due argomenti separati 
        (\f -> \x -> \y -> f (x,y)) come dire che viene preso f, x e y e viene restituito f (x,y) (cioè la tupla (x,y))
    uncurry f (x, y) = f x y            -->     uncurry :: ( a -> b -> c ) -> (a,b) -> c    (\f -> \(x,y) -> f (x,y))       (f -> \(x,y) -> f (x,y))


    ---------------------------------------------------------------------------------------------------------------------------------------------------------------


    La funzione seguente è apparentemente “magica”, nel senso che è in grado di produrre un risultato di qualsiasi tipo. Come si spiega?

    magia :: Int -> a
    magia n = magia (n - 1) 

    è una computazione infinita, qualsiasi valore viene messo non termina mai
    :t magia    ->      magia :: Num t1 => t1 -> t2

    // PERCHE' ALLORA VIENE MESSO DI TIPO Int come Dominio -> magia :: Int?
    perché il tipo Num è una classe di tipi, cioè un insieme di tipi che condividono alcune operazioni (come +, *, ecc..)
    e Int è un'istanza della classe di tipi Num, quindi il tipo più generale di magia è Num t1 => t1 -> t2
    e siccome non c'è nessun vincolo sul tipo t1, Haskell sceglie Int come tipo predefinito per i numeri


    ---------------------------------------------------------------------------------------------------------------------------------------------------------------


    Sapendo che le funzioni Haskell sono “pure” (non hanno effetti collaterali), 
    è possibile dedurre molte informazioni e a volte indovinare esattamente il comportamento di una funzione guardandone solo il tipo. 
    Speculare sul comportamento delle funzioni che hanno i tipi seguenti tenendo presente che, a parità di tipo, sono possibili comportamenti diversi. 
    Consultare la documentazione delle funzioni nelle risposte usando il motore di ricerca Hoogle, 
    che consente di ricercare funzioni nella libreria standard di Haskell anche per mezzo del loro tipo.

    [a] -> Int
    [a] -> Bool
    [a] -> a
    [a] -> [a]
    [[a]] -> [a]
    [a] -> Int -> a
    Int -> [a] -> [a]
    [a] -> [b] -> [(a, b)]
    [(a, b)] -> ([a], [b])
-}



-- Approfondimento sulle classi
{-
    si possono ottenere informazioni sulle classi con il comando :info (oppure :i) di GHCi
    es. :info Num

    La classe Bounded è usata per nominare il limite minimo e massimo di un qualsiasi tipo
    Fammi un esempio con minBound e maxBound
    :type minBound -- minBound :: Bounded a => a
    :type maxBound -- maxBound :: Bounded a => a

    Ma perché viene usata la sintassi minBound :: Int e non minBound :: Bounded a => a?
    Perché Bounded a => a è un tipo generico, cioè può essere qualsiasi tipo che è istanza della classe Bounded
    mentre Int è un tipo specifico, cioè un tipo concreto che è istanza della classe Bounded
    inoltre per ottenere il valore più piccolo di Int bisogna specificare che il tipo è Int, altrimenti Haskell non sa quale tipo usare
    infatti se si prova a fare minBound Int senza specificare il tipo si ottiene un errore
-}



-- Trasformazioni di liste e Quick Sort
{-
    Map

    Operazione frequente è quella di modificare ogni elemento di una lista secondo una certa regola e produrre una juova lista contenente gli elementi trasformati, 
    ad esempio arrotondare ogni numero di una lista di numeri reali.
    
    arrotonda :: (RealFrac a, Integral b) => [a] -> [b]
    arrotonda []       = []
    arrotonda (x : xs) = round x : arrotonda xs

    Si può generalizzare questa operazione definendo una funzione di ordine superiore che prende come argomento una funzione di trasformazione e la lista da trasformare.

    Ordine superiore: funzione che può accettare altre funzioni come argomenti e/o restituire una funzione come risultato del proprio calcolo

    Funzione di ordine superiore (primo argomento una funzione che andrà ad applicare alla prima lista di tipo a un'altra lista con tipo b), 
    map :: (a -> b) -> [a] -> [b]
    map _ []       = []
    map f (x : xs) = f x : map f xs

    Esempio:
    map negate [1,2,3] restiuisce [-1,-2,-3]
    map length [[1,2],[3,4,5]] restituisce [2,3]



    Filter

        Funzione che filtra gli elementi di una lista in base a un predicato (funzione che restituisce un Bool)

    Funzione di ordine superiore (simile alla Map) può restituire una lista che toglie alcuni parametri che non corrispondono al predicato
    filter :: (a -> Bool) -> [a] -> [a]
    filter _ [] = []
    filter p (x : xs) | p x       = x : filter p xs
                    | otherwise = filter p xs

    Esempio:
    filter (>= 0) [-1,0,2] restiuisce [0,2]

    Si scrive (a -> Bool) perché il predicato prende un argomento di tipo a e restituisce un Bool



    Fold

    Un'altra operazione frequente sulle liste è quella di combinare gli elementi di una lista per produrre un singolo valore,

    Funzione che può essere foldr o foldl 

    --- FOLDR
    foldr :: (a -> b -> b) -> b -> [a] -> b         // ( (a -> b -> b) -> b -> [a] ) -> b (b è il valore restituito di tipo b)
    foldr :: Foldable t => (a -> b -> b) -> b -> t a -> b       // Perché viene scritto cosi??? Perché foldr funziona anche con altre strutture dati (t a) oltre alle liste (es. Maybe, Tree, ecc..)
    foldr _ x []       = x
    foldr f x (y : ys) = f y (foldr f x ys)

    Esempio:
    foldr (+) 0 [1,2,3]

    foldr (+) 0 (1 : [2,3])
    (+) 1 (foldr (+) 0 [2,3])
    (+) 1 ((+) 2 (foldr (+) 0 [3]))
    (+) 1 ((+) 2 ((+) 3 (foldr (+) 0 [])))
    (+) 1 ((+) 2 ((+) 3 0))
    (+) 1 ((+) 2 3)
    (+) 1 5
    6

    --- FOLDL
    foldl :: (b -> a -> b) -> b -> [a] -> b
    foldl _ x []       = x
    foldl f x (y : ys) = foldl f (f x y) ys


    Esempio:
    foldl (+) 0 [1,2,3]

    foldl (+) ((+) 0 1 ) [2,3]
    foldl (+) ((+) ((+) 0 1) 2) [3]
    foldl (+) ((+) ((+) ((+) 0 1) 2) 3) []
    ((+) ((+) ((+) 0 1) 2) 3)
    ((+) ((+) 1 2) 3)
    ((+) 3 3)
    6    

-----------------------------------------------------------------------

    si ha che, foldl f (f x y) ys; quindi la funzione foldl (\xs x -> x : xs)
    foldl (\xs x -> x : xs) [] [1,2,3] // permette di iterare la lista xs e restituisce una nuova lista inversa
    foldl (\xs x -> x : xs) ((\xs x -> x : xs) [] 1) [2,3]
    foldl (\xs x -> x : xs) ((\x -> x : []) 1) [2,3]
    foldl (\xs x -> x : xs) ((1 : [])) [2,3]
    foldl (\xs x -> x : xs) [1] [2,3]
    foldl (\xs x -> x : xs) ((\xs x -> x : xs) [1] 2) [3]  è cosi via....
    ATTENZIONE (\xs x si intende con \xs.\x)

    invece con foldr si ha la funzione 
    foldr (\xs x -> x : xs) [] [1,2,3] 
    che però da un errore di tipo, perchè foldr f x (y : ys) = f y (foldr f x ys) ---- si aspetta una lista ma invece si va a staccare un elemento
    quindi f deve prendere come primo argomento un elemento della lista (cioè di tipo a) e come secondo argomento il risultato di foldr f x ys (cioè di tipo b)
    La soluzione è foldr (\x xs -> x : xs) [] [1,2,3] che restituisce la stessa lista
-} 

-- Esercizi
{-
    (length . filter (>= 0)) [-1,1,2,3] 
    si usa prima la composizione e poi l'applicazione (sottoforma di f(g(x)) dove f = length e g = filter (>= 0) e l'argomento x è la lista [-1,1,2,3])
    se scrivessi  length . filter (>=0) [-1,1,2,3] senza le parentesi verrebbe interpretato come length . (filter (>=0) [-1,1,2,3]) e quindi 
    restituirebbe un errore perché filter (>=0) [-1,1,2,3] restituisce una lista e length . lista non ha senso perché l'operatore (.) si aspetta come secondo argomento una funzione e non una lista, inoltre non viene preso nessun argomento esplicito,
    infatti :t (.) restituisce (.) :: (b -> c) -> (a -> b) -> a -> c 
    quindi (.) prende come primo argomento una funzione (b -> c) e come secondo argomento una funzione (a -> b) e restituisce una funzione (a -> c)

    anche questa funzione: length (filter (>= 0) [-1,1,2,3]) funziona con lo stesso principio di applicazione



    foldr (&&) True . map (>= 0) $ [-1,1,2,3]
    L’operatore($) serve solo per evitare parentesi (bassa precedenza -> “applica tutto quello che c’è prima” al valore dopo)
    permette di collegare una funzione a un suo argomento in modo più chiaro
    $ forza la composizione prima dell’applicazione ⇒ corretto.
    ($) :: (a -> b) -> a -> b

    si usa prima l'applicazione e poi la composizione (sottoforma di f(g(x)) dove f = foldr (&&) True e g = map (>= 0) e l'argomento x è la lista [-1,1,2,3])
    se scrivessi foldr (&&) True . map (>= 0) [-1,1,2,3] senza il $ verrebbe interpretato come foldr (&&) True . (map (>= 0) [-1,1,2,3]) e quindi
    restituirebbe un errore perché map (>= 0) [-1,1,2,3] restituisce una lista e foldr (&&) True . lista non ha senso perché l'operatore (.) si aspetta come secondo argomento una funzione e non una lista, inoltre non viene preso nessun argomento esplicito

    anche questa funzione: (foldr (&&) True . map (>= 0)) [0,1,3] funziona con lo stesso principio di applicazione
    la funzione con le parentesi complete con la composizione sarebbe (foldr ())
-}


-- Esercizio: Definire le seguenti funzioni della libreria standard senza usare esplicitamente la ricorsione:
-- concat :: [[a]] -> [a] per concatenare tutti gli elementi in una lista di liste
concat0 :: [[a]] -> [a]  -- [[1,2],[3,4]] = [1,2,3,4]
concat0 xs = foldr (++) [] xs

concat1 :: [[a]] -> [a]  -- [[1,2],[3,4]] = [1,2,3,4]
-- concat1 [] = []
concat1 b = foldr (\xs ys -> xs ++ ys) [] b

concat2 :: [[a]] -> [a]  -- [[1,2],[3,4]] = [1,2,3,4]
-- concat1 [] = []
concat2 b = foldl (\xs ys -> xs ++ ys) [] b     -- Funziona anche con foldl perché la funzione ++ è assocciativa xs ++ (ys ++ zs) == (xs ++ ys) ++ zs)
-- Vanno anche con argomento esplicito

-- any :: (a -> Bool) -> [a] -> Bool per determinare se esiste un elemento di una lista (il secondo argomento della funzione) che soddisfa un certo predicato (il primo argomento della funzione)
any0 :: (a -> Bool) -> [a] -> Bool  -- Provare ad usare il metodo con la composizione o anche senza?
any0 p = foldr (||) False . map p

any1 :: (a -> Bool) -> [a] -> Bool
any1 f xs = foldr (||) False (map f xs)

all1 :: (a -> Bool) -> [a] -> Bool
all1 f xs = foldl (&&) True (map f xs)

-- gli operatori logici sono associativi


-- Esercizio: Definire le seguenti funzioni senza usare esplicitamente la ricorsione:
-- massimo :: Ord a => [a] -> a per calcolare il valore massimo di una lista non vuota
massimo :: Ord a => [a] -> a
massimo (x : xs) = foldr (max) x xs

massimo2 :: Ord a => [a] -> a -- Problema C'E LA CHIAMATA RICORSIVA (risolverla senza usarla); vanno bene le funzioni ausiliarie
massimo2 (b : bs) | any (\x -> x > b) bs == True = massimo bs
                  | otherwise = b

aux :: Ord a => a -> a -> a
aux a b | a > b     = a
        | otherwise = b
massimo3 :: Ord a => [a] -> a
massimo3 (x : xs) = foldr aux x xs


occorrenze x = (foldr (+) 0) . filter (==x) -- fa la somma ma senza il vincolo di tipo

occorrenze1 :: Eq a => a -> [a] -> Int
occorrenze1 x = (foldr (+) 0 ) . (map (const 1)) . filter (==x)


membro :: Eq a => a -> [a] -> Bool
membro x = any (== x)  -- es. membro 2 [1,2, 3 `div` 0]  --> restituisce 2



-- List comprehension
{-
    [ x ^ 2 | x <- [1..10] ] permette di dare alla lista gli elementi con potenza di 2
    quindi si genera una lista da un'altra lista con un generatore, in cui
    l'espressione a sinistra di | specifica come calcolare ogni elemento della lista, usando x definito dal generatore (x <- [1..10])
    la forma è equivalente a: map (^ 2) [1..10]

    si può definire un numero qualsiasi di generatori, ma così si considerano tutti gli assegnamenti possibili dei nomi definiti da tali generatori
    es. [ (x, y) | x <- [1..10], y <- [1..10] ] 
    mi permette di accoppiare tutti i numeri da 1 a 10 con tutti i numeri da 1 a 10
    [(1,1),(1,2),(1,3)...(10,10)] e cosi via, infatti la lunghezza della lista è 10 * 10 = 100

    Inoltre si possono specificare le guardie (filtri) per selezionare solo alcuni degli assegnamenti possibili
    es. [ (x, y) | x <- [1..10], y <- [1..10], x <= y ]
    che genera la lista [(1,1),(1,2),(1,3)...(10,10)] ma senza le coppie (2,1), (3,1), (3,2) ecc.. (cioè quelle che non rispettano la guardia x <= y)

    si definisce la map e filter con list comprehension
    map :: (a -> b) -> [a] -> [b]
    map f xs = [ f x | x <- xs ]

    filter :: (a -> Bool) -> [a] -> [a]
    filter p xs = [ x | x <- xs, p x ]
-}

primo :: Integral a => a -> Bool
primo n = length [ x | x <- [1..n], n `mod` x == 0 ] == 2


-- Integrazione numerica
{-
    CONTROLLARE, CREDO USI LA zip, che cose la zip?
    la funzione zip viene usata per combinare due liste in una singola lista di coppie, dove la coppia alla posizione i è composta dall'elemento i-esimo della prima lista e dall'elemento i-esimo della seconda
    Non viene applicata la tail alla lista vuota perché non esiste, e la zip quando confronta con una lista vuota allora ritorna quello che ha

    esempio: zip [1,2,3] [4,5,6] restituisce [(1,4),(2,5),(3,6)]
    esempio: zip [1,2,3] [] restituisce []
    esempio: zip [] [4,5,6] restituisce []
    esempio: zip [1,2,3] [4,5] restituisce [(1,4),(2,5)]
    esempio: zip [1,2] [4,5,6] restituisce [(1,4),(2,5)]
    esempio: zip [1,2,3] [4] restituisce [(1,4)]
    esempio: zip [1] [4,5,6] restituisce [(1,4)]
    
-}

match :: Eq a => [a] -> [a] -> Bool
match xs ys = any (uncurry (==)) (zip xs ys)

adiacenti :: Eq a => [a] -> Bool
adiacenti xs = any (uncurry (==)) (zip (take ((length xs) -1) xs) (drop 1 xs))

adiacenti1 :: Eq a => [a] -> Bool
adiacenti1 (x:xs) = match (x:xs) (xs) -- confronta la lista intera con la sua tail

adiacenti2 :: Eq a => [a]  -> Bool
adiacenti2 (x:xs) = any (uncurry (==)) (zip (x:xs) xs)

--polinomio :: [Float] -> Float -> Float
--polinomio xs x = any (uncurry ())