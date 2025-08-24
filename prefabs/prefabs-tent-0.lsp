(def terrain 0)
(def camping 1)

(def cbox (collision 0 0 1 1))

(def . (tiledef (tile 11 52 1 1 (terrain))))

(def backpack
  (tiledef 
    (entity 16 34 1 2 (camping))
    (collision 0 0.7 1 0.4)
    (behaviour backpack)
    (loot loot0)))

(def r (tiledef (tile 11 50 1 1 (terrain)) (cbox)))
(def _ (tiledef (tile  9 52 1 1 (terrain)) (cbox)))
(def a (tiledef (tile 12 50 1 1 (terrain)) (cbox)))

(def e (tiledef (tile 10 51 1 1 (terrain)) (cbox)))
(def 3 (tiledef (tile  8 51 1 1 (terrain)) (cbox)))

(def \ (tiledef (tile 11 51 1 1 (terrain)) (cbox)))
(def ^ (tiledef (tile  9 50 1 1 (terrain)) (cbox)))
(def / (tiledef (tile 12 51 1 1 (terrain)) (cbox)))

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
  r__@a
  e#*.3
  e...3
  \^!^/
)
