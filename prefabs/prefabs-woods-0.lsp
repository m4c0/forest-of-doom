(def terrain 0)
(def camping 1)

(def grass (tiledef
  (random 
    (tile 1 7 1 1 (terrain))
    (tile 3 8 1 1 (terrain))
    (tile 3 8 1 1 (terrain)))))

(def tallgrass (tiledef 
  (grass)
  (random
    (entity 26 53 1 1 (camping))
    (entity 26 54 1 1 (camping))
    (entity 26 55 1 1 (camping))
    (entity 22 39 1 1 (camping))
    (entity 23 39 1 1 (camping))
    (entity 22 40 1 1 (camping))
    (entity 23 40 1 1 (camping)))))

(def rock (tiledef (grass)
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

(def _ (tiledef (grass)))
(def . (tiledef
  (random
    (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass)
    (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass)
    (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass) (grass)
    (tallgrass) (tallgrass)
    (tallgrass) (tallgrass)
    (rock)
    )))

(def L (tiledef (_) (exit left)))
(def > (tiledef (L) (entry left)))
(def R (tiledef (_) (exit right)))
(def < (tiledef (R) (entry right)))

(def ' (tiledef (tile 0 0 1 1 (terrain))))
(def % (tiledef
  (')
  (entity 11 151 8 7 (camping))
  (collision 0 0 9 7)))

(def v (tiledef
  (grass)
  (entity 11 151 8 3 (camping))
  (collision 0 0 8 3)))

(prefab
  '%''''''''
  '________'
  '________'
  '________'
  '________'
  '________'
  '________'
  L........R
  L........R
  >_......_<
  L........R
  L........R
  'v_______'
  '________'
  )

