(def initial-camping (from
  (file prefabs-camping-0)
  (enter start)
  (exit tent initial-tent)))

(def initial-tent (from
  (file prefabs-tent-0)
  (enter door)
  (exit tent initial-camping)))

(start initial-camping)
