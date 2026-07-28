{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Functor law" #-}
{-# HLINT ignore "Use map once" #-}
{-# HLINT ignore "Redundant id" #-}
{-# HLINT ignore "Redundant bracket" #-}
{-# HLINT ignore "Use section" #-}
{-# HLINT ignore "Evaluate" #-}
{-# HLINT ignore "Avoid lambda" #-}
{-# HLINT ignore "Redundant section" #-}
{-# HLINT ignore "Use bimap" #-}

module Support.CT0390ApplicativeInHaskell where

import Prelude hiding (Maybe (..), Either (..))

-- --------------------------------------------------------------
-- # Maybe Type
data Maybe a where                      -- kind * -> *
     Nothing :: Maybe a
     Just :: a -> Maybe a        

instance Functor Maybe where
  fmap :: (a -> b) -> Maybe a -> Maybe b
  fmap g Nothing  = Nothing
  fmap g (Just x) = Just (g x)

instance Applicative Maybe where
  pure :: a -> Maybe a
  pure = Just

  (<*>):: Maybe (a -> b) -> Maybe a -> Maybe b
  (<*>) _ Nothing = Nothing
  (<*>) (Just f) (Just x) = Just (f x)
   
-- ## Maybe property
-- ### applicative composition law
compLawMaybeJust :: (b -> c) -> (a -> b) -> a -> ()
compLawMaybeJust u v w = 
          -- left-hand side of the equation
  let _ = (<*>) ((<*>) ((<*>) (pure (.)) (Just u)) (Just v)) (Just w) -- def. pure
      _ = (<*>) ((<*>) ((<*>) (Just (.)) (Just u)) (Just v)) (Just w) -- def. (<*>)
      _ = (<*>) ((<*>) (Just ((.) u)) (Just v)) (Just w)              -- def. (<*>)
      _ = (<*>) (Just ((.) u v)) (Just w)                             -- def. (<*>)
      _ = Just (((.) u v) w)                                          -- def. (<*>)
      _ = Just (u (v w))                                              -- def. (.)           
      _ = (<*>) (Just u) (Just (v w))                                 -- def. (<*>)
      _ = (<*>) (Just u) ((<*>) (Just v) (Just w))                   
          -- right-hand side of the equation
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Coproduct Type
-- Represents a sum type (disjoint union)
data Either a b where
    Left :: a -> Either a b
    Right:: b -> Either a b

-- ## Functor instance for Coproduct
-- Maps over the second type parameter.
instance Functor (Either u) where
    fmap :: (a -> b) -> Either u a -> Either u b
    fmap _ (Left  (x::u)) = Left  x
    fmap f (Right (y::a)) = Right (f y)

instance Applicative (Either u) where
  pure :: a -> Either u a
  pure = Right

  (<*>) :: Either u (a -> b) -> Either u a -> Either u b
  (<*>) (Left (x::u)) _ = Left x
  (<*>) (Right _) (Left (x::u)) = Left x
  (<*>) (Right (f::a->b)) (Right (x::a)) = Right (f x)
  
-- ## Either properties
-- ### applicative composition law
compositionLawEitherRightRight:: (b -> c) -> (a -> b) -> a -> ()
compositionLawEitherRightRight u v w = 
  let _ = (<*>) ((<*>) ((<*>) (pure (.)) (Right u)) (Right v)) (Right w)  -- def. pure
      _ = (<*>) ((<*>) ((<*>) (Right (.)) (Right u)) (Right v)) (Right w) -- def. (<*>)
      _ = (<*>) ((<*>) (Right ((.) u)) (Right v)) (Right w)               -- def. (<*>)
      _ = (<*>) (Right (((.) u) v)) (Right w)                             -- def. (<*>)
      _ = Right ((((.) u) v) w)                                           -- def. (<*>)
      _ = Right (u (v w))                                                 -- def. (.)
      _ = (<*>) (Right u) (Right (v w))                                   -- def. (<*>)
      _ = (<*>) (Right u) ((<*>) (Right v) (Right w)) 
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

instance Applicative (Reader r) where
  pure :: a -> Reader r a
  pure x = Reader (const x)

  (<*>) :: Reader r (a -> b) -> Reader r a -> Reader r b
  (<*>) (Reader (g::r -> a -> b)) (Reader (f::r -> a)) =
    Reader (\r -> (g r) (f r))

-- ## Reader properties
-- ### applicative composition law
compositionLawReader :: 
  (r -> (b -> c)) -> (r -> (a -> b)) -> (r -> a) -> ()
compositionLawReader h g f = 
  let _ = (<*>) ((<*>) ((<*>) (pure (.)) 
                              (Reader h)) 
                       (Reader g)) 
                (Reader f)                                        -- def. pure
      _ = (<*>) ((<*>) ((<*>) (Reader (const (.))) 
                              (Reader h)) 
                       (Reader g)) 
                (Reader f)                                        -- def. <*>
      _ = (<*>) ((<*>) (Reader (\r -> (const (.) r) (h r))) 
                       (Reader g)) 
                (Reader f)                                        -- def. const
      _ = (<*>) ((<*>) (Reader (\r -> (.) (h r))) 
                       (Reader g)) 
                (Reader f)                                        -- def. (.)
      _ = (<*>) ((<*>) (Reader (\r -> ((h r).))) 
                       (Reader g)) 
                (Reader f)                                        -- def. <*>
      _ = (<*>) (Reader (\r -> ((\r -> ((h r).)) r) (g r))) 
                (Reader f)                                        -- beta riduzione
      _ = (<*>) (Reader (\r -> ((h r).) (g r))) 
                (Reader f)                                        -- def. .
      _ = (<*>) (Reader (\r -> (h r).(g r))) 
                (Reader f)                                        -- def. <*>
      _ = Reader (\r -> ((\r -> (h r).(g r)) r) (f r))            -- beta riduzione
      _ = Reader (\r -> ((h r).(g r)) (f r))                      -- def. (.)
      _ = Reader (\r -> (h r) ((g r) (f r)))
      _ = Reader (\r -> (h r) ((\r -> (g r) (f r)) r))
      _ = (<*>) (Reader h) (Reader (\r -> (g r) (f r)))
      _ = (<*>) (Reader h) ((<*>) (Reader g) (Reader f))
  in ()
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # List Type
-- A custom inductive list type with constructors for empty and non-empty lists
data List a where
    Nil :: List a
    Cons :: a -> List a -> List a

-- ## Functor instance for List
-- `fmap` applies a function to each element of the list
instance Functor List where
    fmap :: (a -> b) -> List a -> List b
    fmap _ Nil = Nil
    fmap f (Cons h t) = Cons (f h) (fmap f t)

append:: List a -> List a  -> List a 
append Nil l   = l
append l   Nil = l
append (Cons h t) l = Cons h (append t l)

instance Applicative List where
  pure :: a -> List a
  pure x = Cons x Nil
  (<*>) :: List (a -> b) -> List a -> List b
  (<*>) _ Nil = Nil
  (<*>) (Cons fh ft) l = 
    append (fmap fh l) 
           ((<*>) ft l) 
    
-- ## List properties
-- ### applicative composition law 
-- It is uselessly long and complex to rise interest
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Diagonal Type
newtype Diagonal a = PutD {getD :: (,) a a}

instance Functor Diagonal where
  fmap :: (a -> b) -> Diagonal a -> Diagonal b
  fmap f x =  PutD (f (fst (getD x)), f (snd (getD x)))

instance Applicative Diagonal where
  pure :: a -> Diagonal a
  pure x = PutD (x, x)

  (<*>) :: Diagonal (a -> b) -> Diagonal a -> Diagonal b
  (<*>) f x = PutD (fst (getD f) (fst (getD x))
                   ,snd (getD f) (snd (getD x)))

-- ## Diagonal properties
-- ### applicative composition law 
-- It is uselessly long and complex to rise interest
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Constant Type

newtype Const c a = Const { getConst :: c }

instance Functor (Const c) where
  fmap :: (a -> b) -> Const c a -> Const c b
  fmap f (x:: Const c a) = (Const . getConst) x

instance Monoid c => Applicative (Const c) where
  pure :: a -> Const c a
  pure (_:: a) = Const mempty 
  
  (<*>) :: Const c (a -> b) -> Const c a ->  Const c b
  (<*>) (_:: Const c (a -> b)) (x:: Const c a) = (Const . getConst) x

-- ## Constant properties
-- ### applicative composition law 
-- TO-DO
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # ZipList Type

newtype ZipList a = ZipList { getZipList :: [a] }

instance Functor ZipList where
  fmap :: (a -> b) -> ZipList a -> ZipList b
  fmap f zl = ZipList (map f (getZipList zl))

instance Applicative ZipList where
  pure :: a -> ZipList a
  pure x = ZipList [x]
  (<*>) :: ZipList (a -> b) -> ZipList a ->  ZipList b
  (<*>) (ZipList []) _ = (ZipList [])
  (<*>) _ (ZipList []) = (ZipList [])
  (<*>)  (ZipList (fh:ft)) zl = 
    ZipList $ (getZipList (fmap fh zl)) ++
              (getZipList ((<*>)  (ZipList ft) zl))
-- --------------------------------------------------------------


-- --------------------------------------------------------------
-- # Identity Type

newtype Identity a = Identity { runIdentity :: a }

instance Functor Identity where
  fmap :: (a -> b) -> Identity a -> Identity b
  fmap f x = Identity ((f . runIdentity) x) 

instance Applicative Identity where
  pure :: a -> Identity a
  pure = Identity
  (<*>) :: Identity (a -> b) -> Identity a -> Identity b
  (<*>) f x = Identity (runIdentity f (runIdentity x))

-- ## Identity properties
-- ### applicative composition law 
-- TO-DO
-- --------------------------------------------------------------

