# Requires installation of: https://formulae.brew.sh/formula/parallel
gcc producer.c -o producer
gcc proxy.c -o proxy
gcc consumer.c -o consumer

# Delay proxy and consumer to ensure resources are created before accessed.
parallel ::: \
  "./producer" \
  "sleep 3 && ./proxy" \
  "sleep 4 && ./consumer ./out"