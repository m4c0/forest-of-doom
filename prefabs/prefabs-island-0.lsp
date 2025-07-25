(def terrains 0)
(def camping  1)

; Water
(tiledef
  (id *)
  (tile 12 8 1 1 (terrains))
  (collision 0 0 1 1))

; Water-to-Land
(tiledef (id A) (tile  8 6 1 1 (terrains)))
(tiledef (id B) (tile  9 6 1 1 (terrains)))
(tiledef (id C) (tile 10 6 1 1 (terrains)))
(tiledef (id D) (tile  8 7 1 1 (terrains)))
(tiledef (id .) (tile  9 7 1 1 (terrains)))
(tiledef (id E) (tile 10 7 1 1 (terrains)))
(tiledef
  (id F)
  (tile 8 8 1 1 (terrains))
  (collision 0 0.5 1 0.5))
(tiledef
  (id G)
  (tile 9 8 1 1 (terrains))
  (collision 0 0.5 1 0.5))
(tiledef
  (id H)
  (tile 10 8 1 1 (terrains))
  (collision 0 0.5 1 0.5))
(tiledef (id ,) (tile  1 17 1 1 (terrains)))
(tiledef (id c) (tile 12  6 1 1 (terrains)))
(tiledef
  (id f)
  (tile 11 7 1 1 (terrains))
  (collision 0.9 0.9 0.1 0.1))

; Land-to-road
(def road-center (tile 28 9 1 1 (terrains)))

(tiledef (id L) (tile  3 16 1 1 (terrains)))
(tiledef (id M) (tile  1 18 1 1 (terrains)))
(tiledef (id N) (tile  4 16 1 1 (terrains)))
(tiledef (id O) (tile  2 17 1 1 (terrains)))
(tiledef (id /) (road-center))
(tiledef (id P) (tile  0 17 1 1 (terrains)))
(tiledef (id R) (tile  3 17 1 1 (terrains)))
(tiledef (id S) (tile  1 16 1 1 (terrains)))
(tiledef (id T) (tile  4 17 1 1 (terrains)))

(def backpack
  (tiledef 
    (road-center)
    (collision 0 0.7 1 0.4)
    (behaviour backpack)))

(tiledef (id 1) (backpack) (entity 16 34 1 2 (camping)))
(tiledef (id 2) (backpack) (entity 17 34 1 2 (camping)))
(tiledef (id 3) (backpack) (entity 18 34 1 2 (camping)))

(prefab
  ABBBBBBBBBBC****
  D..,.,.,.,,fBBBC
  D.,.,.,..,.,.,.E
  D..,,..,..,.,..E
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
