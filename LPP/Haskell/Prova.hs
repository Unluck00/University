pari :: Int -> Bool
pari n = n `mod` 2 == 0

pari1 :: Integral a => a -> Bool
pari1 n = if n `mod` 2 == 0
        then True
        else False

dispari n = n `mod` 2 == 1

dispari1 :: Int -> Bool
dispari1 = not.pari
-- Si potrebbe anche scriverlo con argomento esplicito: 
dispari2 n = not (pari n) 
-- Ma non è necessario, perché in Haskell sarebbe un tipico caso di Beta-riduzione: 
dispari3 = \n -> not (pari n)
-- Senza operatore di composizione funzionale
dispari4 :: Int -> Bool
dispari4 n = n `mod` 2 /= 0

-- Funzione somma
somma x y = x + y
-- Versione con lambda
somma1 x = \y -> (x + y)
-- Versione con lambda completa
somma2 = \x -> \y -> (x + y)