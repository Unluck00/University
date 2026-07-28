module Numbers.Positive.OperationsUnary where

{-
# Unary Operations on Positive Binary Numbers
-}
import Prelude hiding (succ, pred)
import Numbers.Positive.Structure ( Positive (XH, XO, XI) )

{-
## Incrementing a `Positive`

Standard inductive definition:
-}
succ :: Positive -> Positive
succ XH     = XO XH       -- 1 +1 == 2 
succ (XO b) = XI b        -- (2b+0) +1 == 2b+1 
succ (XI b) = XO (succ b) -- (2b+1) +1 == 2b+2 == 2(b +1) 

{-
## Decrementing a `Positive`

Since `XH` cannot be decremented, it is necessary to produce a number different from `0` if the argument is `1`.
-}
pred :: Positive -> Positive
pred XH      = XH           -- 1 - 1 == 1 <== this cannot bi "zero"
pred (XO XH) = XH           -- 2 - 1 == 1
pred (XI b)  = XO b         -- (2b+1) - 1 == 2b
pred (XO b)  = XI (pred b)  -- (2b+0) - 1 == ((2b+0)+1-1) -1
                            --            == (2b+0)-2+1
                            --            == (2b-2)+1
                            --            == 2(b -1)+1
