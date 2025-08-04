(def terrains 0)
(def camping  1)

; Water
(def * (tiledef
  (tile 12 8 1 1 (terrains))
  (collision 0 0 1 1)))

(def grass
  (random
    (tile  9 7 1 1 (terrains))
    (tile 11 8 1 1 (terrains))))

; Water-to-Land
(def A (tiledef (tile  8 6 1 1 (terrains))))
(def B (tiledef (tile  9 6 1 1 (terrains))))
(def C (tiledef (tile 10 6 1 1 (terrains))))
(def D (tiledef (tile  8 7 1 1 (terrains))))
(def . (tiledef (grass)))
(def E (tiledef (tile 10 7 1 1 (terrains))))
(def F (tiledef
  (tile 8 8 1 1 (terrains))
  (collision 0 0.5 1 0.5)))
(def G (tiledef
  (tile 9 8 1 1 (terrains))
  (collision 0 0.5 1 0.5)))
(def H (tiledef
  (tile 10 8 1 1 (terrains))
  (collision 0 0.5 1 0.5)))
(def c (tiledef (tile 12  6 1 1 (terrains))))
(def f (tiledef
  (tile 11 7 1 1 (terrains))
  (collision 0.9 0.9 0.1 0.1)))

; Land-to-road
(def road-center (tile 28 9 1 1 (terrains)))

(def L (tiledef (tile  3 16 1 1 (terrains))))
(def M (tiledef (tile  1 18 1 1 (terrains))))
(def N (tiledef (tile  4 16 1 1 (terrains))))
(def O (tiledef (tile  2 17 1 1 (terrains))))
(def / (tiledef (road-center)))
(def P (tiledef (tile  0 17 1 1 (terrains))))
(def R (tiledef (tile  3 17 1 1 (terrains))))
(def S (tiledef (tile  1 16 1 1 (terrains))))
(def T (tiledef (tile  4 17 1 1 (terrains))))

(def backpack
  (tiledef 
    (road-center)
    (collision 0 0.7 1 0.4)
    (behaviour backpack)
    (loot loot0)))

(def 1 (tiledef (backpack) (entity 16 34 1 2 (camping))))
(def 2 (tiledef (backpack) (entity 17 34 1 2 (camping))))
(def 3 (tiledef (backpack) (entity 18 34 1 2 (camping))))

(prefab
  ABBBBBBBBBBC****
  D..........fBBBC
  D..............E
  D..............E
  D..LMMMMMMMMN..E
  D..O////////P..E
  D..O////////P..E
  D..O/////123P..E
  D..O////////P..E
  D..O////////P..E
  D..RSSSSSSSST..E
  D..............E
  FGGGc..........E
  ****D..........E
  ****D..........E
  ****FGGGGGGGGGGH
)
