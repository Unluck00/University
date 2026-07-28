module Numbers.Positive.Structure where
{-
# Positive Binary Numbers: Structure

## Data-type `Positive`
-}
data Positive where
  XI :: Positive -> Positive  -- from n to 2n+1
  XO :: Positive -> Positive  -- from n to 2n+0
  XH :: Positive              -- MSB
  deriving (Show)

{-
### Reason to introduce binary natural numbers without zero:

- once extended with a single representative for zero, the set we obtain becomes a representation of natural numbers in binary notation;
- the obtained representation is an effective alternative to using lists for representing numbers with a unique representative of the zero;

`Positive` was inspired by [Coq.Numbers.BinNums](https://coq.inria.fr/doc/V8.18.0/stdlib/Coq.Numbers.BinNums.html).

## Equivalence Between Two `Positive`

As soon as a new structure is defined, it is natural to ask for how identifying equivalence classes, i.e. how to define a notion of equality.

Making `Positive` instance of `Eq` tells us when two terms of `Positive` belong to the same equivalence class.

### Example

Is it true that `add XH XH == XO XH`?

It depends on the following definition:
-}
instance Eq Positive where
  (==) :: Positive -> Positive -> Bool
  (==)  XH     XH     = True
  (==) (XO b) (XO b') = (==) b b'
  (==) (XI b) (XI b') = (==) b b'
  (==)  _      _      = False

  (/=) :: Positive -> Positive -> Bool
  (/=) x y = not (x == y)
{-
as required by what we get when issuing `ghci> :i Eq`:

~~~haskell
type Eq :: * -> Constraint
class Eq a where
  (==) :: a -> a -> Bool
  (/=) :: a -> a -> Bool
  {-# MINIMAL (==) | (/=) #-}
~~~

### Observation

Haskell does not provide any tool for probing that `(==) :: Positive -> Positive -> Bool` is an equivalence relation on `Positive × Positive`, by showing that:

- `(==)` is reflexive : for every `b       :: Positive`, `b == b = True`;
- `(==)` is symmetric : for every `a, b    :: Positive`, if `a == b = True`, then `b == a = True`;
- `(==)` is transitive: for every `a, b, c :: Positive`, if `a == b = True` and `b == c = True`, then `a == c = True`.


## Order between two `Positive`

To define an instance of `Ord`, we follow what we obtain by `ghci> :i Ord`:

~~~haskell
type Ord :: * -> Constraint
class Eq a => Ord a where
  compare :: a -> a -> Ordering
  (<) :: a -> a -> Bool
  (<=) :: a -> a -> Bool
  (>) :: a -> a -> Bool
  (>=) :: a -> a -> Bool
  max :: a -> a -> a
  min :: a -> a -> a
  {-# MINIMAL compare | (<=) #-}
~~~

`Positive` is already an instance of `Eq`, as defined via `(==) :: Positive -> Positive -> Bool` above. By defining `(<=) :: Positive -> Positive -> Bool`, we can complete the `Ord` instance.

#### Comparison Operation for `Positive`

The auxiliary type `Comp` supports the definition of the `comp` function, allowing us to resolve comparisons correctly.
-}
data Comp where
  Lt :: Comp
  Eq :: Comp
  Gt :: Comp

comp :: Positive -> Positive -> Comp
comp  XH     XH     = Eq
comp  XH    (XO _)  = Lt
comp  XH    (XI _)  = Lt
comp (XO _)  XH     = Gt
comp (XI _)  XH     = Gt
comp (XO p) (XO p') = comp p p'
comp (XI p) (XI p') = comp p p'
comp (XO p) (XI p') = case comp p p' of
                      Lt -> Lt
                      Eq -> Lt
                      Gt -> Gt
comp (XI p) (XO p') = case comp p p' of
                      Lt -> Lt
                      Eq -> Gt
                      Gt -> Gt

{-
We can now define the ordering relation `(<) :: Positive -> Positive -> Bool`:
-}
instance Ord Positive where
  (<) :: Positive -> Positive -> Bool
  (<) p p' = case comp p p' of
             Lt -> True
             _  -> False

  (<=) :: Positive -> Positive -> Bool
  (<=) b b' = (<) b b' || (==) b b'
