{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Redundant bracket" #-}

module Testing.CT300FunctorsTest where

-- List is a functor of kind * -> *
data List a where
    MkNil :: List a
    MkCons :: a -> List a -> List a
instance Functor List where
  fmap :: (a -> b) -> List a -> List b
  fmap _ MkNil = MkNil
  fmap f (MkCons h t) = MkCons (f h) (fmap f t)

-- Coproduct type constructor of kind * -> * -> *
data Coproduct a b where
  Mkinl :: a -> Coproduct a b
  Mkinr :: b -> Coproduct a b

-- Half Coproduct is a functor of kind * -> *
instance Functor (Coproduct u) where
  fmap :: (a -> b) -> (Coproduct u a -> Coproduct u b)
  fmap _ (Mkinl x) = Mkinl x
  fmap f (Mkinr y) = Mkinr (f y)

-- Reader is a functor of kind * -> * -> *
newtype Reader r a = Reader { runReader :: r -> a}
instance Functor (Reader r) where
  fmap :: (a -> b) -> Reader r a -> Reader r b
  fmap f (Reader g) = Reader (f . g)
-- alternative fmap using 
fmap' :: (a -> b) -> Reader r a -> Reader r b
fmap' f g = Reader (f . (runReader g))

-- Diagonal is a functor of kind * -> *
newtype Diagonal a = Diagonal {runDiagonal :: (,) a a}
instance Functor Diagonal where
  fmap :: (a -> b) -> Diagonal a -> Diagonal b
  fmap f d = let (x,y) = runDiagonal d in Diagonal ((,) (f x) (f y))

-- BTree is a functor of kind * -> *
data BTree n l where
    MkLeaf :: l -> BTree n l
    MkNode :: n -> BTree n l        -- nodes store elements of n
                    -> BTree n l    -- from two trees get new tree
                        -> BTree n l

instance Functor (BTree n) where
    fmap :: (a -> b) -> BTree n a -> BTree n b
    fmap f (MkLeaf l) = MkLeaf (f l)
    fmap f (MkNode v l r) = MkNode v (fmap f l) (fmap f r)