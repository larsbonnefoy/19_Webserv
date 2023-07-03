#!/bin/bash

# Number of concurrent users
num_users=50

# URL to simulate requests
url="http://fu-c4-p2.s19.be/"

# Function to simulate a user making requests
simulate_user() {
  while true
  do
    curl -s "$url" >/dev/null
  done
}

# Run the simulation for each user
for ((i=1; i<=num_users; i++))
do
  simulate_user &
done

# Wait for simulation to complete
wait
