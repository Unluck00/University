bisestile :: Int -> Bool
bisestile n = (n `mod` 4 == 0 && n `mod` 100 /= 0) || (n `mod` 400 == 0)

somma :: Int -> Int
somma n = (n*(n+1)) `div` 2

area :: Float -> Float
area n = pi * n^2

successore :: Int -> Int
successore n = n + 1

assoluto :: Int -> Int
assoluto n | n >= 0 = n
           | n < 0  = negate n

assoluto1 :: Int -> Int
assoluto1 n | n >= 0 = n
            | otherwise = negate n

-- Soluzione con if then else
funz :: Int -> Int
funz n = if (n `mod` 2 == 0)
            then n+1
            else assoluto n

giorni :: Int -> Int
giorni n | bisestile n = 366
         | otherwise = 365

-- Soluzione con guardie
fattoriale :: Int -> Int
fattoriale n | n == 0 = 1
             | otherwise = n * fattoriale (n - 1)

-- Fattoriale in versione Haskell like (pattern matching)
fattoriale1 :: Int -> Int
fattoriale1 0 = 1
fattoriale1 n = n * fattoriale (n - 1)

-- pattern matching 
fibonacci :: Int -> Int
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = fibonacci (n - 1) + fibonacci (n - 2)

somma1 :: Int -> Int
somma1 0 = 0
somma1 n = n + somma1 (n-1)

-- Soluzione alternativa con guardie
bits :: Int -> Int
bits n | n == 0 = 0
       | n == 1 = 1
       | otherwise = n `mod` 2 + bits (n `div` 2)

-- Soluzione alternativa con pattern matching + guardie
bits1 :: Int -> Int
bits1 0 = 0
bits1 n | (n `mod` 2  == 0) = bits1 (n `div` 2)
        | otherwise = 1 + bits1 (n `div` 2)

-- potenzaDi2 con guardie
potenzaDi2 :: Int -> Bool
potenzaDi2 n | n == 0 = False
             | n == 1 = True
             | n `mod` 2 == 0 = potenzaDi2 (n `div` 2)
             | otherwise = False

-- potenzaDi2 con pattern matching
potenzaDi2parte2 :: Int -> Bool
potenzaDi2parte2 0 = False 
potenzaDi2parte2 1 = True 
potenzaDi2parte2 n = n `mod` 2 == 0 && potenzaDi2parte2(n `div` 2)

inversioni_lib :: Ord a => [a] -> Int
inversioni_lib xs = length ( filter (uncurry (>)) (zip xs (tail xs)) )
