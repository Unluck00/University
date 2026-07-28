module Numbers.Positive.Exercises.PList where

import Prelude hiding ( succ, pred, toInteger )

-- # Positive Binary Numbers using `List` 
-- 
-- The goal is to design a data-type `PList`, using `[ Bool ]`, that
-- can be proven to be isomorphic to `Numbers.Positive.Structure.Positive`
--
-- #### Examples of instances of `PList`
-- 
-- ~~~haskell
--    s1 :: PList         -- == [False] represents 2
--    s1 = succ (PList []) 
--    ss1 :: PList        -- == [True ] represents 3
--    ss1 = succ s1
--    sss1 :: PList       -- == [False,False] represents 4
--    sss1 = succ ss1
--    ssss1 :: PList      -- == [True ,False] represents 5
--    ssss1 = succ sss1
--    sssss1 :: PList     -- == [False,True ] represents 6
--    sssss1 = succ ssss1
--    ssssss1 :: PList    -- == [True ,True ] represents 7
--    ssssss1 = succ sssss1-- 
-- ~~~
-- 
-- #### Intuition about how to define what's needed
-- Take:
-- 
-- ~~~haskell
--    sssss1 = [False,True]
-- ~~~
-- 
-- To realize that it represents 6 we have to think of it as 
-- 
-- ~~~haskell
--    False:True:[] 
-- ~~~
-- 
-- in which the empty list [] is the MSB set always to `1`, while `False` stands for `0` 
-- and `True` for `1`. Therefore `sssss1` yields:
-- 
-- ~~~haskell
--    2^0 * False + 2^1 * True + 2^2 * [] == 2^0 * 0 + 2^1 * 1 + 2^2 * 1
-- ~~~
-- 
-- which is `011` with its MSB to the right.
-- 
-- ## To do
-- 
-- Once given `PList`, define:
-- 
-- 1. successor `succ` and predecessor `pred`
-- 2. translations from `PList` to `Integers` and back
-- 3. the isomorphism with `Numbers.Positive.Structure.Positive`

--module Numbers.Positive.Exercises.PList where
import Numbers.Positive.Structure ( Positive (XH, XO, XI) )
import Numbers.Positive.OperationsUnary ()

-- ## How to design `PList`
-- 
-- **Hints** 
-- Use `[ Bool ]` to represent:
-- 
--    - the MSB 1 using `[]`
--    - both numbers `False:p::PList` and `True:p::PList`, if `p::PList`
--      adding a LSB to the left of the list
-- 

-- ### The type `PList`
newtype PList = PList [ Bool ] 
   deriving (Show, Eq)

-- ### Successor for `PList`
succ :: PList -> PList
succ (PList[]) = Plist [False]
succ (PList (False:p)) = Plist (True:p) -- (2p+0) +1 == 2p+1; dove viene tolto il False dalla lista e al suo posto viene messo True
succ (PList (True:p)) = 
   let (PList p') = succ (PList p) 
   in PList (False : p')  -- (2p+1) +1 == 2p+2 == 2(p +1); dove viene tolto il True dalla lista e al suo posto viene messo False, e si incrementa il resto della lista 

-- #### Testing `succ`
-- s1 :: PList     -- == 2
-- s1 = succ (PList [])
-- ss1 :: PList    -- == 3
-- ss1 = succ s1
-- sss1 :: PList   -- == 4
-- sss1 = succ ss1
-- ssss1 :: PList  -- == 5
-- ssss1 = succ sss1
-- sssss1 :: PList -- == 6
-- sssss1 = succ ssss1
-- ssssss1 :: PList -- == 7
-- ssssss1 = succ sssss1

-- ### Successor for `PList`
pred :: PList -> PList
pred (PList[]) = Plist []  -- 1 - 1 == 1 <== this cannot bi "zero"
pred (PList [False]) = Plist [] -- (2p+0) +1 == 2p+1; dove viene tolto il False dalla lista e al suo posto viene messo True
pred (PList (True:p)) = PList (False:p)  -- (2b+1) - 1 == 2b; dove viene tolto il True dalla lista e al suo posto viene messo False
pred (PList (False:p)) =
   let (PList p') = pred (PList p) 
   in PList (True : p')

-- ### Testing `pred`
-- p1 :: PList       -- == 1
-- p1 = pred (PList [])
-- ps1 :: PList      -- == 1
-- ps1 = pred s1
-- pss1 :: PList     -- == 2
-- pss1 = pred ss1
-- psss1 :: PList    -- == 3
-- psss1 = pred sss1
-- pssss1 :: PList   -- == 4
-- pssss1 = pred ssss1
-- psssss1 :: PList  -- == 5
-- psssss1 = pred sssss1

-- ### Map from `Numbers.Positive.Structure.Positive` to `Integer`
-- toInteger :: PList -> Integer

-- #### Testing `toInteger`
-- tI00 :: Integer
-- tI00 = toInteger s1
-- tI01 :: Integer
-- tI01 = toInteger ss1
-- tI02 :: Integer
-- tI02 = toInteger sss1
-- tI03 :: Integer
-- tI03 = toInteger ssss1

-- ### Maps between `Numbers.Positive.Structure.Positive` and `PList`
-- toPositive :: PList -> Positive
-- fromPositive :: Positive -> PList

-- #### Testing `toPositive`
-- tP00 :: Positive
-- tP00 = toPositive s1
-- tP01 :: Positive
-- tP01 = toPositive ss1
-- tP02 :: Positive
-- tP02 = toPositive sss1
-- tP03 :: Positive
-- tP03 = toPositive ssss1

-- #### Testing `fromPositive`
-- fP00 :: PList
-- fP00 = fromPositive XH
-- fP01 :: PList
-- fP01 = fromPositive (XO XH)
-- fP02 :: PList
-- fP02 = fromPositive (XI XH)
-- fP03 :: PList
-- fP03 = fromPositive (XI (XO XH))
