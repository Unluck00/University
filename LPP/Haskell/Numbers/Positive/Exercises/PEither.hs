module Numbers.Positive.Exercises.PEither where

import Prelude hiding ( succ, pred )

-- # Positive Binary Numbers using nested `Either` 
-- 
-- The goal is to design a data-type `PEither`, nesting occurrences of `Either`, that can be proven to be isomorphic to `Numbers.Positive.Structure.Positive`
--
-- #### Examples of instances of `PEither` that we expect
-- 
-- ~~~haskell
--    eZO1   :: PEither -- represents 1
--    eZO1   = PEither (Left ())    
--    eZO01  :: PEither -- represents 2, MSB rightmost bit
--    eZO01  = PEither (Right (Right (PEither (Left ()))))   
--    eZO11  :: PEither -- represents 3
--    eZO11  = PEither (Right (Left  (PEither (Left ()))))
--    eZO001 :: PEither -- represents 4
--    eZO001 = PEither (Right (Right (PEither (Right (Right (PEither (Left ())))))))
--    eZO101 :: PEither -- represents 5
--    eZO101 = PEither (Right (Left  (PEither (Right (Right (PEither (Left ())))))))
--    eZO011 :: PEither -- represents 5
--    eZO011 = (PEither . Right . Right . PEither . Right . Left . PEither . Left) ()
-- ~~~
-- 
-- #### The intuition about how to define what's needed
-- Take:
-- 
-- ~~~haskell
--    eZO011 = (PEither . Right . Right . PEither . Right . Left . PEither . Left) ()
-- ~~~
-- 
-- To realize that it represents 6 we can forget the sequences `PEither . Right` which serve to keep the structure of the elements consistent, but are "useless" to encode the binary number. Once forgotten them, we have:
-- 
-- ~~~haskell
--    (Right . Left . Left) ()
-- ~~~
-- 
-- If we think of `Right` as representing `0`, `Left` representing `1`, and `, and `PEither . Left` representing the MSB `1`, we get:
-- 
-- ~~~haskell
--    0 1 1
-- ~~~
-- 
-- which is 6 in binary notation.
-- 
-- ## To do
-- 
-- Once given `PEither`, define:
-- 
-- 1. successor `succ` and predecessor `pred`
-- 2. translations from `PEither` to `Integers` and back
-- 3. the isomorphism with `Numbers.Positive.Structure.Positive`


-- ## How to design `PEither`
-- Hints. 
-- Use `Either` to represent:
-- 
--    - the MSB 1 using `Left ()`
--    - both numbers `0p::PEither` and `1p::PEither`, by adding a LSB (Least Significant Bit), using `Right` and `Left`, for every `p::PEither`
-- 

newtype PEither = PEither (Either () (Either PEither PEither))
   deriving (Show, Eq)

-- ### Successor for `PEither`
succ :: PEither -> PEither
succ (PEither (Left())) = PEither (Right (PEither (Left ()) ) ) 
succ (PEither (Right (Right p))) = PEither (Right (Left p))
succ (PEither (Right (Left p))) =
   let (PEither p') = succ (PEither p)
   in PEither (Right (Right p'))
-- Base case :      1 + 1
-- Inductive cases: 2p + 1 and (2p+1) + 1

-- ### Tests for `succ`
-- s1 :: PEither     -- == 2
-- ss1 :: PEither    -- == 3
-- sss1 :: PEither   -- == 4
-- ssss1 :: PEither  -- == 5
-- sssss1 :: PEither -- == 6

-- ### Predecessor for `PEither`
pred :: PEither -> PEither
-- "Fake" base case:  1 - 1
-- Real base case  :  2 - 1
-- Inductive cases :  (2p+1) - 1  and 2p - 1

-- ### Tests for `pred`
-- p1 :: PEither       -- == 1
-- ps1 :: PEither      -- == 1
-- pss1 :: PEither     -- == 2
-- psss1 :: PEither    -- == 3
-- pssss1 :: PEither   -- == 4
-- psssss1 :: PEither  -- == 5
