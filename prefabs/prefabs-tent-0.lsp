(def terrain 0)

(def grass
  (random
    (tile  9 7 1 1 (terrain))
    (tile 11 8 1 1 (terrain))))

(def . (tiledef (grass)))

(def ! (tiledef
  (grass)
  (exit prefabs-camping-0 tent)
  (entry door)))

(prefab
  ...
  .!.
  .!.
)
