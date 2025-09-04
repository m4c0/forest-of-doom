(def woods (from
  (file prefabs-woods-0)
  (exit left woods-right)
  (exit right woods-left)))

(def woods-left (from (woods) (entry left)))
(def woods-right (from (woods) (entry right)))

(def initial-camping (from
  (file prefabs-camping-0)
  (entry tent)
  (exit left woods-right)
  (exit right woods-left)
  (exit tent initial-tent)))

(def initial-tent (from
  (file prefabs-tent-0)
  (entry door)
  (exit door initial-camping)))

(def start (from (initial-camping) (entry start)))
(start start)
