{-# LANGUAGE GADTs #-}
{-# LANGUAGE DataKinds #-}
{-# LANGUAGE KindSignatures #-}
{-# LANGUAGE TypeFamilies #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE ScopedTypeVariables #-}

module Support.CT0075DataKindCategory where

import Prelude hiding ((.), id)

{-
# Modeling a finite category in Haskell by hands

We aim at modelling simple instance of category with two objects and two arrows other that the two mandatory identities. 

- we give a first definition which is too coarse, allowing us to make instances which use arbitrary types
- we give a second definition, in which we force  arrows only between specific objects

## The coarse way of modelling the category

The first step is the definition of a class `CategoryPoly` such that:

- its argument is the binary type constructor `hom:: * -> * -> *` which takes two types of kind * and returns a type. The role of `hom a b` is to model the type of morphisms from `a` to `b`
- the interface of `CategoryPoly` requires to define:
    
    -- an identity `id`, as expected for every object with a given type `a:: *`. It says that for every type `a:: *`, there is a morphism from `a` to `a`
    -- a composition `(.)` of arrows between pairs of types with kind `*` 
-}
class CategoryPoly (hom:: * -> * -> *) where
    id :: hom (a:: *) (a:: *)
    (.):: hom (b:: *) (c:: *) -> hom (a:: *) (b:: *) -> hom (a:: *) (c:: *)
{-
The definition of `CategoryPoly` emphasizes that the category that it identifies is over a standard type constructor.

## An excessively polymorphic type for morphisms/arrows

A set of arrows that can be used to make an instance of `CategoryPoly` by using it as `hom` is:
-}
data HomPoly  (a:: *) (b:: *) where
    IdPoly:: HomPoly (c:: *) (c:: *)
    FPoly :: HomPoly (c:: *) (d:: *)
    GPoly :: HomPoly (d:: *) (c:: *) -- it could be HomPoly (c:: *) (d:: *)

{-
We can already observe coarseness by showing that the following instances of `HomPoly` are perfectly legal:
-}
type HomPolyIntBool = HomPoly Int Bool
type HomPolyCharList = HomPoly Char [Int]

{-
A further way to observe coarseness is by an instance of the class `CategoryPoly` by:

- firstly, making an instance of the type constructor `hom:: * -> * -> *` with `HomPoly`
- secondly, showing that, thanks to the polymorphism, we can define compositions which are meaningless if we want to faithfully model the structure of a category with two objects and two arrows further to the two mandatory identities:
-}
instance CategoryPoly HomPoly where
    id:: HomPoly (c:: *) (c:: *)
    id = IdPoly
    (.):: HomPoly (d:: *) (e:: *) -> HomPoly (c:: *) (d:: *) -> HomPoly (c:: *) (e:: *)
    -- Identity axiom fro the identity
    (.) IdPoly IdPoly = IdPoly
    -- Identity axioms involving FPoly
    (.) IdPoly FPoly  = FPoly
    (.) FPoly  IdPoly = FPoly
    -- Identity axioms involving GPoly
    (.) IdPoly GPoly  = GPoly
    (.) GPoly  IdPoly = GPoly
    -- Excessively permissive axiom if `FPoly` has to model an arrow from object `X` to object `Y`
    (.) FPoly  FPoly  = FPoly -- <==== WRONG!

{-
## Refinement: restricting polymorphism with DataKinds

We define a _type_ (zeroary/constant type constructor) `Obj`.
-}
data Obj:: * where
    A :: Obj
    B :: Obj
    deriving Show

{-
#### Remark. 

Since we have enabled:

~~~haskell
    {-# LANGUAGE DataKinds #-}
~~~

in fact, Haskell promotes:

- `Obj` to a _kind_ 
- `A` and `B` to _type-level constants_ of _kind_ `Obj`

With the _kind_ `Obj` available, we can define the the _type constructor_ `HomT:: Obj -> Obj -> *` as follows:
-}
data HomT (a:: (Obj:: *)) (b:: (Obj:: *)) where
    IdA:: HomT (A:: Obj) (A:: Obj) -- _value-constructor_ of the identity between the _type-level constants_ `A` and `A`
    IdB:: HomT (B:: Obj) (B:: Obj) -- _value-constructor_ of the identity between the _type-level constants_ `B` and `B`
    F  :: HomT (A:: Obj) (B:: Obj) -- _value-constructor_ of the function between the _type-level constants_ `A` and `B`
    G  :: HomT (B:: Obj) (A:: Obj) -- _value-constructor_ of the function between the _type-level constants_ `B` and `A`

{- 
Now, we introduce a type class which is a helper. 

It's interface requires the every of its instances define an `identity` on every instance of `Obj`.
-}
class IdentityHom (c:: Obj) where
    identity:: HomT (c:: Obj) (c:: Obj)

{-
This allows us to make an instance of the class `IdentityHom` in which:

- every _type-value constant_ in `Obj` becomes an instance of `c:: Obj`
- the _method/function_ is a specific instance of identity available as constructor of `HomT (a:: (Obj:: *)) (b:: (Obj:: *))`
-}
instance IdentityHom (A:: Obj) where
    identity:: HomT (A:: Obj) (A:: Obj)
    identity = IdA 

instance IdentityHom (B:: Obj) where
    identity:: HomT (B:: Obj) (B:: Obj)
    identity = IdB

{-- 
It turns out that we have all the elements to say what a category is in terms of a class:

- we have to define an identity for every element in `Obj`
- we have to define a composition that, for example, we call `(.*)`
-}

class CategoryT (hom:: Obj -> Obj -> *) where
    idT :: IdentityHom (a:: Obj) => hom (a:: Obj) (a:: Obj)
    (.*):: hom (b:: Obj) (c:: Obj) 
            -> hom (a:: Obj) (b:: Obj) 
                -> hom (a:: Obj) (c:: Obj)

instance CategoryT HomT where
    -- `idT` is one of the possibile identities that must exist as
    idT:: IdentityHom a => HomT (a:: Obj) (a:: Obj)
    idT = identity
    (.*):: HomT (b:: Obj) (c:: Obj) 
            -> HomT (a:: Obj) (b:: Obj) 
                -> HomT (a:: Obj) (c:: Obj)
    (.*) IdA IdA = IdA
    (.*) IdB IdB = IdB
    (.*) IdB F   = F
    (.*) F   IdA = F
    (.*) IdA G   = G
    (.*) G   IdB = G
    
-- Show instance for testing
instance Show (HomT a b) where
    show:: HomT a b -> String
    show IdA = "IdA"
    show IdB = "IdB"
    show F   = "F"
    show G   = "G"

-- Test function
test:: IO ()
test = do
    putStrLn "Testing type-level category composition:"

    let longChain = IdA .* IdA .* IdA .* IdA
    putStrLn $ "Long chain of IdA: " ++ show longChain

    let composedF = F .* longChain
    putStrLn $ "F composed with long IdA chain: " ++ show composedF

    let extended = IdB .* composedF
    putStrLn $ "Extended composition (IdB .* F .* IdA chain): " ++ show extended

    {- 
    Uncommenting the following lines:
    -}
    -- let invalid = F .* F
    -- putStrLn "Type error"
    
    {- will raise the type error:
    ~~~haskell
    • Couldn't match type ‘'B’ with ‘'A’
      Expected: HomT 'A 'A
        Actual: HomT 'A 'B
    • In the second argument of ‘(.*)’, namely ‘F’
      In the expression: F .* F
    ~~~
    -}
