(def terrain 0)
(def camping 1)

(def . (tiledef (tile 4 16 1 1 3)))

(def backpack
  (tiledef 
    (entity 16 34 1 2 (camping))
    (collision 0 0.7 1 0.4)
    (behaviour backpack)
    (loot loot0)))

(def _ (tiledef (tile 0 0 1 1 0)))
(def @ (tiledef (_) (backpack)))

(def # (tiledef
  (.)
  (entity 22 30 1 2 (camping))
  (collision 0 0 0.9 1.7)))

(def ! (tiledef
  (.)
  (exit prefabs-camping-0 tent)
  (entry door)))

(def * (tiledef 
  (.)
  (entity 20 28 1 1 (camping))
  (collision 0 0 1 0.4)))

(prefab
  __@
  #*.
  ...
  .!.
)
