{-
# `BinNotation` as an Applicative Functor
-}
module Numbers.Binary.Applicative.OperationsBinary where

import GHC.Base ( Applicative ( .. ) )
import Prelude hiding (succ, pred, div, add )

import Numbers.Positive.OperationsBinary ( add, Mask ( .. ), sub, mul )
import qualified Numbers.Positive.OperationsBinary as P
import Numbers.Binary.Functor.Structure ( BinNotation ( .. ), BN )

{-
## Applicative Functor

For convenience, we recall what we get from `ghci` with `:i Applicative`:

~~~haskell
type Applicative :: (* -> *) -> Constraint
class Functor f => Applicative f where
  pure :: a -> f a
  (<*>) :: f (a -> b) -> f a -> f b
  GHC.Base.liftA2 :: (a -> b -> c) -> f a -> f b -> f c
  ...
  {-# MINIMAL pure, ((<*>) | liftA2) #-}
~~~

To define an `Applicative`:

- The first argument must be a type constructor of kind `* -> *`;
- It must be an instance of `Functor`;
- You must define `pure :: a -> f a` and either `(<*>)` or `liftA2`.

### Notes

- `pure` can be seen as the base case of an inductive process.
- `(<*>)` acts like a lifted function application, applying a function wrapped in a functor to an argument also wrapped in a functor.
- It generalizes `($) :: (a -> b) -> a -> b` to functorial contexts. It is like saying that we should look at `($)` as its type was `ID (a -> b) -> ID a -> ID b` where `ID` is the functor "Identity".

### `BinNotation` as an Applicative Instance
-}
instance Applicative BinNotation where
  pure :: a -> BinNotation a
  pure = Npos

  (<*>) :: BinNotation (a -> b) -> BinNotation a -> BinNotation b
  (<*>) (Npos f) (Npos x) = Npos (f x)
  (<*>) _ _ = N0

  liftA2 :: (a -> b -> c) -> BinNotation a -> BinNotation b -> BinNotation c
  liftA2 f (Npos x) (Npos y) = Npos (f x y)
  liftA2 _ _ _ = N0

{-
### Notes

- `pure` embeds any value into the `BinNotation` structure using `Npos`.
- `(<*>)` extracts both function and argument from their `BinNotation` wrappers and applies them, rewrapping the result.
- `liftA2` applies a binary function to two wrapped values and rewraps the result.

### What does `<*>` enable?

With `<*>`, we can continue applying functions argument by argument, treating `BN` as an instance of `BinNotation`.

#### Example

We’ve already seen:

~~~haskell
(fmap sum) (Npos XH) == Npos (sum XH)
(fmap sum) (Npos (XO XH)) == Npos (sum (XO XH))
~~~

With `<*>`  available, we can apply the here above functions to a second argument:

~~~haskell
((fmap sum) (Npos XH)) <*> (Npos XH) 
== Npos (sum XH XH) 
== Npos (XO XH)

((fmap sum) (Npos (XO XH))) <*> (Npos XH) 
== Npos (sum (XO XH) XH)
== Npos (XI XH)
~~~

## Binary Operations on `BN`

### Two Additions

Using `<*>` and `liftA2`, we can define addition in two ways.

The first one is based on `<*>` and eta-reduction:
-}
add :: BN -> BN -> BN
add N0 x = x
add x N0 = x
add x y = (<*>) (fmap P.add x) y

{-
The second ina is based on `liftA2` directly:
-}
add' :: BN -> BN -> BN
add' N0 x = x
add' x N0 = x
add' x y  = liftA2 P.add x y
  
{-
### Multiplication

It is defined similarly to `add`:
-}
mul :: BN -> BN -> BN
mul N0 _ = N0
mul _ N0 = N0
mul x y = (<*>) (fmap P.mul x) y

{-
### Subtraction

Since `sub :: Positive -> Positive -> Mask` returns a tagged result, we must handle all the cases accordingly:-}
sub :: BN -> BN -> BN
sub (Npos b) (Npos b') = case P.sub b b' of
                         IsPos v -> Npos v
                         IsNeg   -> N0
                         IsNul   -> N0
sub N0 _ = N0
sub b N0 = b