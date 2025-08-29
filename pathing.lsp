(def initial-camping (from
  (file prefabs-camping-0)
  (entry start)
  (exit tent initial-tent)))

(def initial-tent (from
  (file prefabs-tent-0)
  (entry door)
  (exit tent initial-camping)))

(start initial-camping)
