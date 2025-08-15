(def terrains 0)

(def grass
  (random
    (tile  9 7 1 1 (terrains))
    (tile 11 8 1 1 (terrains))
    (tile 11 8 1 1 (terrains))))

(def . (tiledef (grass)))

(def P (tiledef (grass) (entry start)))

(def X (tiledef
         (grass) 
         (entity 14 42 1 5 (terrains))
         (collision 0 0 1 5)))

(prefab
  ......................
  ......................
  .......P..............
  ......................
  .....................X
  ......................
  ......................
  ......................
  ......................
)
