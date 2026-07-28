{-# LANGUAGE GADTs #-}
{-# LANGUAGE DataKinds #-} -- <=== Required to use value constructors as constant_types
{-# LANGUAGE KindSignatures #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE ScopedTypeVariables #-}

module Support.CT0100FreeMonoidBooleanAndCat where

import Prelude hiding ((.), id)

{-
# Modeling the finite monoid category of `({F,T}, and, T)` 
-}

-- The unique object of the category
data Obj:: * where
  Bullet :: Obj
  deriving Show

-- The arrows representing `F` and `T`
data Hom (a:: (Obj:: *)) (b:: (Obj:: *)) where
  Fand:: Hom ('Bullet:: Obj) ('Bullet:: Obj) -- _value-constructor_ using the _type-level constants_ `Bullet`
  Tand:: Hom ('Bullet:: Obj) ('Bullet:: Obj) -- _value-constructor_ using the _type-level constants_ `Bullet`

{- The structure of a category:

- depends on a "generic" type-constructor `hom`
- requires an identity `id` 
- requires a composition `(.)` 

Both `id` and `(.)` must work on instances of `hom` restricted to use _type_constants_ of `Obj`.
-}
class CategoryMonoidAnd (hom:: Obj -> Obj -> *) where
    id :: hom ('Bullet:: Obj) ('Bullet:: Obj)
    (.):: hom ('Bullet:: Obj) ('Bullet:: Obj) 
            -> hom ('Bullet:: Obj) ('Bullet:: Obj)
                -> hom ('Bullet:: Obj) ('Bullet:: Obj)

instance CategoryMonoidAnd Hom where
    id:: Hom (Bullet:: (Obj:: *)) (Bullet:: (Obj:: *))
    id = Tand
    -- Defines the truth table of the conjunction
    (.):: Hom ('Bullet:: Obj) ('Bullet:: Obj) 
            -> Hom ('Bullet:: Obj) ('Bullet:: Obj)
                -> Hom ('Bullet:: Obj) ('Bullet:: Obj)
    -- 1. explicit version of the truth table
    -- (.) Fand Fand = Fand
    -- (.) Fand Tand = Fand
    -- (.) Tand Fand = Fand
    -- (.) Tand Tand = Tand
    -- 2. implicit more category-oriented version of the truth table
    (.) Fand Fand = Fand
    (.) Fand id   = Fand
    (.) id   Fand = Fand
    (.) Tand id = id
