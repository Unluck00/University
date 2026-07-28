module Numbers.Positive.VsInteger where

{-
# Positive Binary Numbers: relation with Integers
-}
import Prelude hiding ( succ )
import Numbers.Positive.Structure ( Positive(XH, XO, XI) )
import Numbers.Positive.OperationsUnary ( succ )

{-
#### From Positive Binary to Integer

The structure of `pos2Int` is straightforward.

-}
pos2Int :: Positive -> Integer
pos2Int (XO p) = 0 + 2 * pos2Int p
pos2Int (XI p) = 1 + 2 * pos2Int p
pos2Int XH = 1

{-
#### From Integer to Positive Binary

- Integer values less than or equal to `0` are collapsed into `XH :: Positive`.
- For other values, the conversion proceeds inductively: by decrementing the argument of `int2Pos`, we obtain the corresponding `Positive` value to be incremented.

-}
int2Pos :: Integer -> Positive
int2Pos x =
   if x <= 1 then XH
   else succ (int2Pos (x - 1))
