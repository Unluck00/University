module Numbers.Binary.Functor.OperationsUnary where

{-
# Unary Operations on `BN`
-}
import Prelude hiding (succ, pred, div)

import Numbers.Positive.Structure ( Positive ( .. ) )
import Numbers.Positive.OperationsUnary ( succ, pred )
import qualified Numbers.Positive.OperationsUnary as P

import Numbers.Binary.Functor.Structure ( BinNotation ( .. ), BN )

{-
## Successor `succ :: BN -> BN`

Its definition uses the application of `fmap :: (a -> b) -> (BinNotation a) -> (BinNOtation b)` to `succ:: Positive -> Positive` only on elements of `BinNotation Positive = BN` that come from `Positive`. 
-}
succ :: BN -> BN
succ N0 = Npos XH
succ x  = fmap P.succ x
{-
#### Note. 
Were the definition of `succ` be simply:

~~~haskell
    succ :: BN -> BN
    succ = fmap Positive.succ
~~~
we would get:

~~~haskell
    succ N0 = fmap Positive.succ N0 = N0
~~~

in place of the expected `Npos XH`.

## Predecessor `pred :: BN -> BN`

Similar considerations apply to the predecessor.
Its definition must intercept the case where the argument is `XH` to avoid to pass it to `fmap Positive.pred`.
Otherwise, `fmap Positive.pred` would reduce to `Npos (Positive.pred XH)`, resulting in the identity.
-}
pred :: BN -> BN
pred N0 = N0
pred (Npos XH) = N0
pred x = fmap P.pred x
