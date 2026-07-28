module Support.CT0395NonApplicativeFunctors where

{-
# Functors that cannot be Applicative
-}

-- -----------------------------------------------------------
-- ## A Functor `Sized` with non-canonical Applicative interface
data Sized a where
  Sized:: (Int, a) -> Sized a

instance Functor Sized where
  fmap :: (a -> b) -> Sized a -> Sized b
  fmap f (Sized (n, x)) = Sized (n, f x)

instance Applicative Sized where
  pure :: a -> Sized a
  pure x = Sized (0, x)
  (<*>) :: Sized (a -> b) -> Sized a -> Sized b
  (<*>) (Sized (m, f::a->b)) (Sized (n, x)) = Sized (m+n, f x)

{-
No canonical choice for `pure`, or `(<*>)` exists.
We could very well choose another `pure` among:

~~~haskell
  pure x = Sized (-1, x)
  pure x = Sized ( 1, x)
~~~

Likewise `(<*>)` could be one among:

~~~haskell
  (<*>) :: Sized (a -> b) -> Sized a -> Sized b
  (<*>) (Sized (m, f::a->b)) (Sized (n, x)) =
    Sized (m, f x)

  (<*>) :: Sized (a -> b) -> Sized a -> Sized b
  (<*>) (Sized (m, f::a->b)) (Sized (n, x)) =
    Sized (n, f x)
~~~~

-}

-- -----------------------------------------------------------
-- ## A Functor `F` with non-canonical Applicative interface
data F a where
  F:: (Int, Int -> a) -> F a

instance Functor F where
  fmap:: (a -> b) -> F a -> F b
  fmap g (F (n, f)) = F (n, g . f)

instance Applicative F where
  pure:: a -> F a
  pure x = F (0, const x)
  (<*>):: F (a -> b) -> F a -> F b
  (<*>) (F (n::Int, g::(Int -> a -> b))) 
        (F (m::Int, f::(Int -> a))) 
        = F (m, const (g  n (f m)))

{-
No canonical choice for `pure`, or `(<*>)` exists.
We could very well choose another `pure` among:

~~~haskell
  pure x = F (0, const x)
  pure x = F (1, const x)
  pure x = F (2, const x)
  pure x = F (3, const x)
~~~

Likewise `(<*>)` could be one among:

~~~haskell
  (<*>) (F (n::Int, g::(Int -> a -> b))) 
        (F (m::Int, f::(Int -> a))) 
        = F (n, const (g n (f m)))

  (<*>) (F (n::Int, g::(Int -> a -> b))) 
        (F (m::Int, f::(Int -> a))) 
        = F (n, const (g m (f n)))
~~~~

-}

-- -----------------------------------------------------------
-- ## A Functor `W` with non-canonical Applicative interface
data W a b where
 W:: (a, [b]) -> W a b

instance Functor (W a) where
  fmap :: (b -> c) -> W a b -> W a c
  fmap f (W (x::a, l::[b])) = W (x, map f l)

{- 
`pure` does not even seem to exist: what first element to choose as first element of its output?
instance Applicative (W a) where
  pure :: b -> W a b
  pure x = W ( ?? , [x])
-}