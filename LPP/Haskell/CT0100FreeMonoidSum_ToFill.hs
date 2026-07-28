{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Redundant section" #-}
{-# HLINT ignore "Avoid lambda" #-}

module Support.CT0100FreeMonoidSum_ToFill where

{-
## From `3+4` to `(3+) . (4+)` 
-}
mToCM3Plus4 :: ()
mToCM3Plus4 = 
    let _ = 3+4
        _ = (3+) 4
        _ = (3+) (4+0)
        _ = (3+) ((4+) 0)
        _ = (\x -> (3+) ((4+) x)) 0
--     -- `0` is not really needed
        _ = (\x -> (3+) ((4+) x))-- Qualsiasi x nella lambda expression va bene perché è un argomento
--     -- Composition
        _ = (\x -> (3+) ((4+) 0)) 
--     -- Eta-reduction
        _ = (3+) . (4+) 
      -- Composition axiom
        _ = ((3+4)+)
--     -- Identity axiom
        _ = ((3+4)+) . (0+)
    in ()

{-
## From `x+y` to `(x+) . (y+)` 
-}
-- mToCMxPlusY :: Int -> Int -> ()
-- mToCMxPlusY x y =
--   let _ = x+y
--       _ = 
--       _ = 
--       _ = 
--       _ = 
--     -- `0` is not really needed
--       _ = 
--     -- Composition
--       _ = 
--     -- Eta-reduction
--       _ = 
--     -- Composition axiom
--       _ = 
--     -- Identity axiom
--       _ = 
--   in ()