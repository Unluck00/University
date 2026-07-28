{-# LANGUAGE NoImplicitPrelude #-}

module Support.CT0075ControlCategory where

{- 
# Define a category with built in ``Control.Category` module

We show how to define and use the 'Category' class from 'Control.Category'. 

So, we need to hide the default Prelude versions of '(.)' and 'id'. 
-}
import Prelude hiding ((.), id)

{- 
The class `Category` from `Control.Category` abstracts the notion of a mathematical category:

- objects are types
- morphisms are values of type 'cat a b'. 
-}
import Control.Category (Category, id, (.))

{- Useful for the final example:
-}
import Data.Int (Int)

{- Issuing:

   ghci> import Control.Category
   ghci> :i Category

   we get:

   ~~~haskell
   type Category :: forall {k}. (k -> k -> *) -> Constraint
   class Category cat where
     Control.Category.id :: forall (a :: k). cat a a
     (Control.Category..) :: forall (b :: k) (c :: k) (a :: k).
                             cat b c -> cat a b -> cat a c
     {-# MINIMAL id, (.) #-}
   ~~~

The `Category` class defines the structure of a category.
It is parameterized by a type constructor `cat` of kind `k -> k -> *`, which represents morphisms between types:
   
   - `id` is the identity morphism for any object `a` of kind  which, of course, should satisfy the identity laws:

      -- id . f = f
      -- f . id = f

   - `(.)` composes arrow as obviously as its types of kind `k` require. Of course it should be associative.

#### Example usage:
   Compose two functions using the categorical composition operator. 
-}
example :: Int -> Int
example = (+1) . (*2)  -- equivalent to \x -> (+1) ( (*2) x )