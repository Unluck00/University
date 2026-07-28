module Numbers.Binary.Applicative.IsNumericalSystem where
{-
## Numeric System on `BN`

With the above operations, plus the missing ones, `BN` as an instance of `BinNotation` forms a numeric system:
-}
-- import GHC.Base ( Applicative ( .. ) )
import Prelude hiding (succ, pred, add )

import Numbers.Positive.Structure ( Positive ( .. ) )
import Numbers.Positive.VsInteger ( int2Pos )
import qualified Numbers.Positive.VsInteger as P
import Numbers.Binary.Functor.Structure ( BinNotation ( .. ), BN )
import Numbers.Binary.Functor.OperationsUnary ( succ  )
import Numbers.Binary.Applicative.OperationsBinary ( add, sub, mul )
 
instance Num BN where
  (+) :: BN -> BN -> BN
  (+) = add

  (-) :: BN -> BN -> BN
  (-) = sub

  (*) :: BN -> BN -> BN
  (*) = mul

  negate :: BN -> BN
  negate x = x

  abs :: BN -> BN
  abs x = x

  signum :: BN -> BN
  signum N0 = N0
  signum _  = Npos XH
  
  fromInteger :: Integer -> BN
  fromInteger n | n <= 0 = N0
                | otherwise = Npos (P.int2Pos n)

