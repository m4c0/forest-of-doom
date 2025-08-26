(from initial-camping
  (file prefabs-camping-0)
  (enter start)
  (exit tent initial-tent))

(from initial-tent
  (file prefabs-tent-0)
  (enter door)
  (exit tent initial-camping))

(start initial-camping)
