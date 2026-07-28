-- Tipo polimorfi e ricorsivi 

-- Haskell predefinisce un tipo polimorfo per rappresentare valori opzionali
-- Rispetto al costruttore Proprio Int visto prima, Maybe è polimorfo perché può essere usato con qualsiasi tipo di dato
data Maybe a = Nothing | Just a     -- variabile di tipo a che rappresenta il tipo dell’argomento del costruttore Just

{-
Da usare nel Prelude di GHCi:
    :t Just 1 :: Maybe Int     -- specifica che il tipo di dato contenuto in Just è Int; applicando a Maybe al tipo Int si ottiene un tipo isomorfo a ForseInt; quindi Maybe può essere applicato a qualsiasi altro tipo di dato
    :t Just True :: Maybe Bool
    :t Just [0.5] :: Maybe [Float]
-}


-- Un tipo può avere un numero arbitrario di parametri di tipo. Ad esempio, il (costruttore di) tipo Either è definito nella libreria standard di Haskell in questo modo
data Either a b = Left a | Right b
{-
    consente di rappresentare l’unione disgiunta di due tipi a e b. In altre parole, i valori di tipo Either T S hanno una di due forme possibili:
        valori della forma Left x dove x è di tipo T, oppure
        valori della forma Right y dove y è di tipo S.
-}

-- Il costruttore di un tipo di dato T può avere argomenti il cui tipo è a sua volta (costruito con) T
data List a = Nil | Cons a (List a)
-- viene scritto Nil per rappresentare la lista vuota e Cons per rappresentare una lista non vuota, il cui primo argomento è la testa della lista (di tipo a) e il secondo argomento è la coda della lista (di tipo List a)
-- Non si usano [] e (:) perché sono già definiti per le liste predefinite di Haskell, quindi si usano nomi diversi per evitare ambiguità



-- Alberi di ricerca binari


--  possiamo rappresentare alberi binari con un tipo polimorfo e ricorsivo, parametrico rispetto al tipo degli elementi contenuti nell’albero.
data Tree a = Leaf | Branch a (Tree a) (Tree a)
  deriving Show
-- Un albero binario è o una foglia (Leaf) oppure un nodo (Branch) che contiene un valore di tipo a e due sottoalberi (di tipo Tree a)

-- la funzione elements  raccoglie in una lista tutti gli elementi di un albero facendo una visita in ordine infisso
elements :: Tree a -> [a]
elements Leaf             = []
elements (Branch x t₁ t₂) = elements t₁ ++ [x] ++ elements t₂

-- Esempio elements Leaf oppure elements (Branch 1 Leaf (Branch 2 Leaf Leaf)) in GHCi


-- Riscrivere la funzione elements usando il tipo List definito sopra invece delle liste predefinite di Haskell; data List a = Nil | Cons a (List a)
elements1 :: Tree a -> List a
elements1 Leaf = Nil
elements1 (Branch x t1 t2) = append ( append (elements1 t1) (Cons x Nil) ) (elements1 t2)

elements2 :: Tree a -> List a
elements2 Leaf = Nil
elements2 (Branch x t1 t2) = append (elements2 t1) (Cons x (elements2 t2))

append :: List a -> List a -> List a
append Nil ys = ys
append (Cons x xs) ys = Cons x (append xs ys)

{- Alternativa con le liste predefinite di Haskell
    append :: [a] -> [a] -> a
    append [] ys = ys
    append (x : xs) ys = x : (append xs ys)
-}



-- Operazioni in più 
-- funzione tmax determina l’elemento più grande in un albero binario di ricerca, che può essere individuato seguendo tutte le diramazioni destre dell’albero
tmax :: Tree a -> a
tmax (Branch x _ Leaf) = x
tmax (Branch _ _ t)    = tmax t
-- è una funzione parziale, non definita se applicata all’albero vuoto
-- nella prima equazione di tmax usiamo il pattern matching “profondo” per fermare la ricorsione al livello immediatamente precedente al raggiungimento di una foglia. In secondo luogo, la funzione tmax restituisce l’elemento più grande di un albero binario di ricerca, ma non di un albero binario qualsiasi. In altri termini, tmax fa un’assunzione implicita

-- la funzione insert inserisce un elemento in un albero binario di ricerca, restituendo come risultato un nuovo albero binario di ricerca in cui l’inserimento è stato effettuato
insert :: Ord a => a -> Tree a -> Tree a
insert x Leaf = Branch x Leaf Leaf
insert x t@(Branch y t₁ t₂) | x == y    = t
                            | x < y     = Branch y (insert x t₁) t₂
                            | otherwise = Branch y t₁ (insert x t₂)

{-
Inserendo x nell’albero vuoto si ottiene un albero che contiene solo x. Per inserire x in una diramazione che contiene y occorre distinguere tre casi:

Se x ed y sono uguali, allora l’albero è invariato. Potremmo esprimere questo comportamento di insert scrivendo, a destra del simbolo =, l’espressione Branch y t₁ t₂ che ricostruisce esattamente lo stesso albero a cui insert è stata applicata. È più comodo (e lievemente più efficiente) restituire direttamente l’albero inziale. A tal fine usiamo un’ascrizione t@(Branch y t₁ t₂) che dà nome t all’albero in cui avviene l’inserimento e allo stesso tempo controlla che tale albero abbia la forma di una diramazione.
Se x è più piccolo di y, allora il risultato è una diramazione che continua ad avere y nella radice e t₂ come sotto-albero destro, ma in cui il sotto-albero sinistro è il risultato dell’inserimento di x in t₁.
Il caso in cui x è più grande di y è simmetrico del precedente.
-}

-- La funzione delete rimuove un elemento x da un albero binario di ricerca. Se l’albero non contiene x, l’operazione non ha alcun effetto.
delete :: Ord a => a -> Tree a -> Tree a
delete _ Leaf = Leaf
delete x (Branch y t₁ t₂) | x < y = Branch y (delete x t₁) t₂
                          | x > y = Branch y t₁ (delete x t₂)
delete x (Branch _ t Leaf) = t
delete x (Branch _ Leaf t) = t
delete x (Branch _ t₁ t₂)  = Branch y (delete y t₁) t₂
  where
    y = tmax t₁