(def terrain 0)
(def camping 1)

(def grass
  (random
    (tile  9 7 1 1 (terrain))
    (tile 11 8 1 1 (terrain))))

(def . (tiledef (grass)))

(def backpack
  (tiledef 
    (entity 16 34 1 2 (camping))
    (collision 0 0.7 1 0.4)
    (behaviour backpack)
    (loot loot0)))

(def @ (tiledef (.) (backpack)))

(def ! (tiledef
  (grass)
  (exit prefabs-camping-0 tent)
  (entry door)))

(prefab
  ..@
  ...
  .!.
)
