(def terrain 0)
(def camping 1)

(def . (tiledef
  (random 
    (tile 1 7 1 1 (terrain))
    (tile 3 8 1 1 (terrain)))))

(def x (tiledef (.)
  (random
    (entity 19 37 1 1 (camping))
    (entity 20 37 1 1 (camping))
    (entity 21 37 1 1 (camping))
    (entity 19 38 1 1 (camping))
    (entity 20 38 1 1 (camping))
    (entity 21 38 1 1 (camping))
    (entity 19 39 1 1 (camping))
    (entity 20 39 1 1 (camping))
    (entity 21 39 1 1 (camping)))
  (collision 0.2 0.1 0.6 0.6)))

(def ^ (tiledef (.)
  (hover 20 19 4 2 (camping))))
(def @ (tiledef (.)
  (entity 20 21 4 2 (camping))
  (collision 0.0 0.0 4.0 1.5)))

(def ! (tiledef (.)
                (entry tent)
                (exit prefabs-tent-0 door)))

(def P (tiledef (.) (entry start)))

(prefab
  .........................
  .........................
  ....x....................
  ........^.........x......
  .........................
  .......x@................
  .........!..P.......x....
  .........................
  .........................
  .....x........x..........
  ....................x....
  .........................
  .........................
  )

