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

(def @ (tiledef (.)
  (entity 20 19 4 4 (camping))
  (collision 0.0 2.0 4.0 1.5)))

(def P (tiledef (.) (entry start)))

; 2025-08-19 19:01:56.881 atlas-viewer.exe[42716:67511377] [info] > tile 20 19 1 1
; 2025-08-19 19:01:59.291 atlas-viewer.exe[42716:67511377] [info] > tile 24 19 1 1
; 2025-08-19 19:02:01.027 atlas-viewer.exe[42716:67511377] [info] > tile 28 19 1 1
; 2025-08-19 19:02:28.676 atlas-viewer.exe[42716:67511377] [info] > tile 22 30 1 1

(prefab
  ......@..................
  .........................
  ....x....................
  ........@.........x......
  .........................
  .......x.................
  ............P.......x....
  .........................
  .........................
  .....x........x..........
  ....................x....
  .........................
  .........................
  )

