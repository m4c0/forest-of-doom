(def woods-left (from
  (file prefabs-woods-0)
  (entry left)
  (exit left woods-right)
  (exit right woods-left) ; go somewhere else
  ))
(def woods-right (from
  (file prefabs-woods-0)
  (entry right)
  (exit right woods-left)
  (exit left woods-right) ; go somewhere else
  ))

(def initial-camping (from
  (file prefabs-camping-0)
  (unique-id initial-camping)
  (entry tent)
  (exit left woods-right)
  (exit right woods-left)
  (exit tent initial-tent)))

(def initial-tent (from
  (file prefabs-tent-0)
  (unique-id initial-tent)
  (entry door)
  (exit door initial-camping)))

(def start (from (initial-camping) (entry start)))
(start start)
