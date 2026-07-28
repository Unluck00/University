module Numbers.Binary.Functor.Structure where
{-
# A functor towards natural numbers in binary notation
-}

import Prelude hiding (succ, pred, div)
import Numbers.Positive.Structure ( Positive ( .. ) )
import Numbers.Positive.OperationsUnary ( succ, pred )
import qualified Numbers.Positive.OperationsUnary as P

{-
We define a type constructor `BinNotation :: * -> *` that:

  - adds a constructor `N0` (ideally representing "zero") to any set `a` of elements,
  - embeds into `BinNotation` every element of the set `a`, using the constructor `Npos`.
-}
data BinNotation a where
  N0   :: BinNotation a
  Npos :: a -> BinNotation a
  deriving (Show)

{-
## `BinNotation` is a Functor
-}
instance Functor BinNotation where
  fmap :: (a -> b) -> BinNotation a -> BinNotation b
  fmap _ N0 = N0
  fmap f (Npos x) = Npos (f x)

{-
#### Notes

- `N0` and `Npos` distinguish `BinNotation` from any other type `a`, 
- the first argument `f` of `fmap` cannot be an element of `BinNotation`.
  
  So, the definition of `fmap` must be such that:
  
  - `fmap` returns `N0` when the second argument is `N0` . 
  - if the second argument of `fmap` is wrapped with `Npos`, then `f` can be applied and its result wrapped.

## Natural Numbers in Binary notation via `BinNotation`

Applying the type constructor `BinNotation :: * -> *` to `Positive` gives us a representation of natural numbers in binary notation with a unique representation for "zero":
-}
type BN = BinNotation Positive

{-
### `BN` can be instance of `Eq`

From `ghci :i Eq`:

~~~haskell
type Eq :: * -> Constraint
class Eq a where
  (==) :: a -> a -> Bool
  (/=) :: a -> a -> Bool
~~~

Only one of `(==)` or `(/=)` is required, but we define both:
-}
instance Eq BN where
  (==) :: BN -> BN -> Bool
  (==) N0 N0 = True
  (==) (Npos b) (Npos b') = b == b'
  (==) _ _ = False

  (/=) :: BN -> BN -> Bool
  (/=) x y = not (x == y)

{-
### `BN` can be instance of `Ord`
-}
instance Ord BN where
  (<) :: BN -> BN -> Bool
  (<) N0 (Npos _) = True
  (<) (Npos p) (Npos p') =
          ((<):: Positive -> Positive -> Bool) p p'
  (<) _ _ = False

  (<=) :: BN -> BN -> Bool
  (<=) b b' = b < b' || b == b'
