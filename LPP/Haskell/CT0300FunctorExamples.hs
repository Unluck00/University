{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Functor law" #-}
{-# HLINT ignore "Use map once" #-}
{-# HLINT ignore "Redundant id" #-}
{-# HLINT ignore "Redundant bracket" #-}
module Support.CT0300FunctorExamples where

-- # Importing Void and absurd
import Data.Void (Void, absurd)
-- `Void` is the uninhabited type (initial object in category theory).z
-- `absurd :: Void -> a` allows us to eliminate impossible cases.

import Prelude hiding (Maybe (..), Either (..))

-- --------------------------------------------------------------
-- # Maybe Type
data Maybe a where
     Nothing :: Maybe a
     Just :: a -> Maybe a        -- type-constructor of kind * -> *
instance Functor Maybe where
  fmap :: (a -> b) -> Maybe a -> Maybe b
  fmap g Nothing  = Nothing
  fmap g (Just x) = Just (g x)

-- ## Maybe properties
-- ### identity law
idLawMaybeJust :: a -> ()
idLawMaybeJust x = 
  let _ = fmap id (Just x) 
      _ = Just (id x) 
      _ = Just x
  in ()
idLawMaybeNothing :: a -> ()
idLawMaybeNothing x = 
  let _ = fmap id Nothing 
      _ = Nothing 
  in ()
-- ### composition law
compLawMaybeJust :: (b -> c) -> (a -> b) -> a -> ()
compLawMaybeJust g f x =
  let _ = fmap (g . f) (Just x)      -- def fmap 
      _ = Just ((g . f) x)           -- def (.)
      _ = Just (g (f x))             -- def fmap
      _ = fmap g (Just (f x))        -- def fmap
      _ = fmap g (fmap f (Just x))   -- def fmap
      _ = (fmap g . fmap f) (Just x) -- def (.)
  in ()
compLawMaybeNothing :: (b -> c) -> (a -> b) -> ()
compLawMaybeNothing g f =
  let _ = fmap (g . f) Nothing       -- def. fmap 
      _ = Nothing                    -- def. fmap
      _ = fmap f Nothing             -- def. fmap
      _ = fmap g (fmap f Nothing)    -- def. fmap
      _ = (fmap g . fmap f) Nothing  -- def. (.)
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # List Type
-- A custom inductive list type with constructors for empty and non-empty lists.
data List a where
    Nil :: List a
    Cons :: a -> List a -> List a

-- ## Functor instance for List
-- `fmap` applies a function to each element of the list.
-- `<$` replaces all elements with a constant value (here simplified).
instance Functor List where
    fmap :: (a -> b) -> List a -> List b
    fmap _ Nil = Nil
    fmap f (Cons h t) = Cons (f h) (fmap f t)

-- ## List properties
-- ### identity law
idLawListCons :: a -> List a -> ()
idLawListCons h t =
  let _ = fmap id (Cons h t)        -- def. fmap
      _ = Cons (id h) (fmap id t)   -- def. id
      _ = Cons h (fmap id t)        -- inductive hypothesis
      _ = Cons h t
  in ()
idLawListNil :: ()
idLawListNil =
  let _ = fmap id Nil
      _ = Nil
  in ()
-- ### composition law
compLawListCons :: (b -> c) -> (a -> b) -> a -> List a -> ()
compLawListCons g f h t =
  let _ = fmap (g . f) (Cons h t)                -- def. fmap
      _ = Cons ((g . f) h) (fmap (g . f) t)      -- inductive hypothesis
      _ = Cons ((g . f) h) ((fmap g . fmap f) t) -- def. (.)
      _ = Cons (g (f h)) (fmap g (fmap f t))     -- def. fmap
      _ = fmap g (Cons (f h) (fmap f t))         -- def. fmap
      _ = fmap g (fmap f (Cons h t))             -- def. (.)
      _ = (fmap g . fmap f) (Cons h t)
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Coproduct Type
-- Represents a sum type (disjoint union).
data Either a b where
    Left :: a -> Either a b
    Right:: b -> Either a b

-- ## Functor instance for Coproduct
-- Maps over the second type parameter.
instance Functor (Either u) where
    fmap :: (a -> b) -> Either u a -> Either u b
    fmap _ (Left  (x::u)) = Left  x
    fmap f (Right (y::a)) = Right (f y)

-- ## Either properties
-- ### identity law
idLawEitherLeft :: a -> ()
idLawEitherLeft x =
  let _ = fmap id (Left x)         -- def. fmap
      _ = Left x 
  in ()
idLawEitherRight :: b -> ()
idLawEitherRight y =
  let _ = fmap id (Right y)         -- def. fmap
      _ = Right (id y)              -- def. id
      _ = Right y                     
  in ()
-- ### composition law
compositionLawEitherLeft :: (b -> c) -> (a -> b) -> u -> ()
compositionLawEitherLeft g f x = 
  let _ = fmap (g . f) (Left x)
      _ = Left x
      _ = fmap g (Left x)
      _ = fmap g (fmap f (Left x))
      _ = (fmap g . fmap f) (Left x)
  in ()
compositionLawEitherRight :: (b -> c) -> (a -> b) -> a -> ()
compositionLawEitherRight g f x = 
  let _ = fmap (g . f) (Right x)
      _ = Right ((g . f) x)
      _ = Right (g (f x))
      _ = fmap g (Right (f x))
      _ = fmap g (fmap f (Right x))
      _ = (fmap g . fmap f) (Right x)
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Binary Tree Type
-- A binary tree with labeled nodes and leaves.
data BTree n l where
  Leaf :: l -> BTree n l
  Node :: n -> BTree n l -> BTree n l -> BTree n l

-- ## Functor instance of Binary Tree
instance Functor (BTree n) where
    fmap :: (a -> b) -> BTree n a -> BTree n b
    fmap f (Leaf l) = Leaf (f l)
    fmap f (Node v l r) = Node v (fmap f l) (fmap f r)

-- ## Binary Tree properties
-- ### identity law
idLawBTreeLeaf :: a -> ()
idLawBTreeLeaf l =
  let _ = fmap id (Leaf l)
      _ = Leaf (id l)
      _ = Leaf l
  in ()
idLawBTreeNode :: n -> BTree n l -> BTree n l -> ()
idLawBTreeNode v tl tr =
  let _ = fmap id (Node v tl tr)           -- def. fmap
      _ = Node v (fmap id tl) (fmap id tr) -- inductive hypothesis
      _ = Node v tl tr
  in ()
-- -- ### composition law
compositionLawBTreeLeaf :: (b -> c) -> (a -> b) -> a -> ()
compositionLawBTreeLeaf g f l = 
  let _ = fmap (g . f) (Leaf l)      -- def fmap
      _ = Leaf ((g . f) l)           -- def (.)
      _ = Leaf (g (f l))             -- def fmap
      _ = fmap g (Leaf (f l))        -- def fmap
      _ = fmap g (fmap f (Leaf l))   -- def (.)
      _ = (fmap g . fmap f) (Leaf l) 
  in ()
compositionLawBTreeNode :: (b -> c) -> (a -> b) -> n -> BTree n a -> BTree n a -> ()
compositionLawBTreeNode g f v tl tr = 
  let _ = fmap (g . f) (Node v tl tr)                          -- def. fmap
      _ = Node v (fmap (g . f) tl) (fmap (g . f) tr)           -- inductive hypothesis
      _ = Node v ((fmap g . fmap f) tl) ((fmap g . fmap f) tr) -- def (.)
      _ = Node v (fmap g (fmap f tl)) (fmap g (fmap f tr))     -- def fmap
      _ = fmap g (Node v (fmap f tl) (fmap f tr))              -- def fmap
      _ = fmap g (fmap f (Node v tl tr))                       -- def (.)
      _ = (fmap g . fmap f) (Node v tl tr)                     
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Reader Type
-- Represents a computation that reads from a shared environment.
newtype Reader r a where
    Reader :: (r -> a) -> Reader r a  
-- The usual GADT syntax does not build automatically any "accessor field".

-- ## Functor instance for Reader
-- `fmap` composes the function with the reader.
instance Functor (Reader r) where
    fmap :: (a -> b) -> Reader r a -> Reader r b
    fmap f (Reader g) = Reader (f . g)

-- ## Reader properties
-- ### identity law
idLawReader :: (r -> a) -> ()
idLawReader g =
  let _ = fmap id (Reader g) 
      _ = Reader (id . g)
      _ = Reader (id . g)             
  in ()
-- -- ### composition law
compositionLawReader :: (b -> c) -> (a -> b) -> Reader r a -> ()
compositionLawReader h g (Reader fr) = 
  let _ = fmap (h . g) fr          -- def. fmap + extraction from r
      _ = Reader (h . g . fr)      -- def. fmap
      _ = fmap h (Reader (g . fr)) -- def. fmap + embedding into r
      _ = fmap h (fmap g fr)       -- def. (.)
      _ = (fmap h . fmap g) fr               
  in ()
-- --------------------------------------------------------------
-- --------------------------------------------------------------
-- # Another (equivalent) Reader Type
-- Represents a computation that reads from a shared environment.
newtype Reader' r a = Reader' { runReader' :: r -> a } 
-- The here above "record syntax" makes the "accessor field" `runReader` to access the function of type `r -> a` decorated/stored by means of the constructor `Reader`
-- The usual GADT syntax does not build automatically any "accessor field".

-- ## Functor instance for Reader
-- `fmap` composes the function with the reader.
instance Functor (Reader' r) where
    fmap :: (a -> b) -> Reader' r a -> Reader' r b
    fmap f g = Reader' (f . runReader' g)

-- ## Reader properties
-- ### identity law
idLawReader' :: (r -> a) -> ()
idLawReader' g =
  let _ = fmap id (Reader' g) 
      _ = Reader' (id . g)
      _ = Reader' (id . g)             
  in ()
-- -- ### composition law
compositionLawReader' :: (b -> c) -> (a -> b) -> Reader' r a -> ()
compositionLawReader' h g fr = 
  let _ = fmap (h . g) fr                    -- definition of fmap + extraction from r
      _ = Reader' (h . g . (runReader' fr))    -- definition of fmap
      _ = fmap h (Reader' (g . runReader' fr)) -- definition of fmap + embedding into r
      _ = fmap h (fmap g fr)                 -- definition of .
      _ = (fmap h . fmap g) fr               
  in ()
-- --------------------------------------------------------------