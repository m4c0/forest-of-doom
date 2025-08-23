(def terrain 0)

(def . (tiledef
  (tile 0 0 1 1 (terrain))
  (exit prefabs-camping-0 tent)
  (entry door)))

(prefab .)
