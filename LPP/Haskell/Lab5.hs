-- Programma in Haskell che permette di trovare i primi n numeri primi dato un n arbitrario 

primi :: Int -> [Integer]
primi = aux 2       -- funzione ausiliaria che inizializza a 2 un parametro che ha lo stesso ruolo della variabile locale k (va a generare tutti i numeri canditdati as essere numeri primi) 
  where
    aux _ 0 = []
    aux k n | primo k   = k : aux (k + 1) (n - 1)       -- il : serve per concatenare il numero primo trovato alla lista dei numeri primi trovati
            | otherwise = aux (k + 1) n
    primo x = x > 1 && all (\d -> x `mod` d /= 0) [2 .. x-1]        -- un numero è primo se è maggiore di 1 e non è divisibile per nessun numero tra 2 e x-1


-- enumFrom è una funzione predefinita in Haskell che genera una lista infinita di valori enumerati a partire da un valore dato.
--enumFrom :: Enum a => a -> [a]
--enumFrom n = n : enumFrom (succ n)


-- fibonacci infinito usando zipWith
-- zipWith prende una funzione e due liste e applica la funzione agli elementi corrispondenti delle due liste, creando una nuova lista.
xs :: [Integer]
xs = 0 : 1 : zipWith (+) xs (tail xs)


-- la funzione primoMaggioreDi prende un intero n e restituisce il primo numero primo maggiore di n
primoMaggioreDi2 :: Integer -> Integer
primoMaggioreDi2 n = head (filter primo (enumFrom (max 2 (n+1))))
  where
    primo x = x > 1 && all (\d -> x `mod` d /= 0) [2 .. x-1]



{- Definire una funzione ricorsiva che, applicata a una lista non vuota di numeri, 
  restituisca True se l’ultimo elemento è uguale alla somma di quelli che lo precedono (ovvero se) 
  e False altrimenti. Non è consentito l’uso di funzioni della libreria standard di Haskell.
-}