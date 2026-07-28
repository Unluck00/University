-- Enumerazioni

-- Si usa data per definire un nuovo tipo di dato con un insieme finito di costruttori.
data Mossa = Sasso | Carta | Forbici
-- E' nel Prelude se si fara :type Sasso verra fuori il suo tipo Mossa
-- Se si aggiunge 
  deriving Show
-- si puo stampare a video il valore di tipo Mossa

-- Possibile usare il pattern matching per definire funzioni che operano su Mossa
vince :: Mossa -> Mossa -> Int
vince Sasso   Carta   = 2 -- vince il giocatore 2
vince Sasso   Forbici = 1 -- vince il giocatore 1
vince Carta   Sasso   = 1
vince Carta   Forbici = 2
vince Forbici Sasso   = 2
vince Forbici Carta   = 1
vince _       _       = 0 -- in tutti gli altri casi, parità

-- Funzione che simula una partita di morra cinese tra due giocatori
morra :: [Mossa] -> [Mossa] -> Int
morra [] [] = 0
morra _  [] = 1
morra [] _  = 2
morra (x : xs) (y : ys) | vincitore /= 0 = vincitore
                        | otherwise      = morra xs ys
  where
    vincitore = vince x y


-- Il tipo unitario () è un tipo di dato con un solo costruttore, anch'esso chiamato ().
-- Viene usato per rappresentare l'assenza di un valore significativo, simile a "void" in altri linguaggi di programmazione.

data Giorno = Lun | Mar | Mer | Gio | Ven | Sab | Dom
  deriving Show

domani :: Giorno -> Giorno
domani Lun = Mar
domani Mar = Mer
domani Mer = Gio
domani Gio = Ven
domani Ven = Sab
domani Sab = Dom
domani Dom = Lun

fra :: Int -> Giorno -> Giorno
fra 0 g = g
fra n g = fra (n-1) (domani g)

{-
fra :: Int -> Giorno -> Giorno
fra 0 = id
fra n = domani . fra (n-1)
-}

-- Usare replicate per evitare la ricorsione esplicitata
fra_replicate :: Int -> Giorno -> Giorno
fra_replicate n = foldr (.) id (replicate n domani)




-- Costruttori con argomenti

-- Definizione di un tipo di dato che rappresenta un numero intero che può essere "niente" o "proprio" un intero
-- Usare questo tipo per definire una funzione che converte una stringa in un intero, restituendo Niente se la conversione fallisce
data ForseInt = Niente | Proprio Int
  deriving Show

-- Funzione che restituisce l'iesimo elemento di una lista come ForseInt
-- Se l'indice è fuori dai limiti della lista, restituisce Niente altrimenti restituisce Proprio elemento
elemento :: Int -> [Int] -> ForseInt
elemento 0 (x : _)  = Proprio x
elemento n (_ : xs) = elemento (n - 1) xs
elemento _ _        = Niente

{-
    Esempio: 
        elemento 0 [1, 2, 3]
        elemento 1 [1, 2, 3]
        elemento 3 [1, 2, 3]
-}

-- sul risultato di una funzione
-- definire una funzione che data una lista e un elemento, restituisce la posizione dell'elemento nella lista come ForseInt
posizione :: Int -> [Int] -> ForseInt
posizione _ []                = Niente
posizione x (y : _)  | x == y = Proprio 0
posizione x (_ : xs) = case posizione x xs of
                         Niente    -> Niente
                         Proprio n -> Proprio (n + 1)

{-
    The function takes two parameters:

    x: The integer we're looking for
    A list of integers to search through
    There are three cases:

    Base case: posizione _ [] returns Niente when the list is empty, meaning the element wasn't found.

    Match case: posizione x (y : _) | x == y returns Proprio 0 when the first element matches what we're looking for, indicating we found it at position 0.

    Recursive case: posizione x (_ : xs) skips the current element and searches the rest of the list. If the element is found later (getting Proprio n), it adds 1 to that position since we skipped one element. If nothing is found (Niente), it propagates Niente up.
-}

{-
    Esempio: 
        posizione 0 [1, 2, 3]
        posizione 1 [1, 2, 3]
        posizione 3 [1, 2, 3]
-}


-- Realizzare una funzione totale testa :: [Int] -> ForseInt che, applicata a una lista xs, restituisca la testa di xs se xs non è vuota, e Niente altrimenti.