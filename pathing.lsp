(def initial-camping (from
  (file prefabs-camping-0)
  (entry tent)
  (exit tent initial-tent)))

(def initial-tent (from
  (file prefabs-tent-0)
  (entry door)
  (exit door initial-camping)))

(def start (from (initial-camping) (entry start)))
(start start)
