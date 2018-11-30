# back
import time
import logging

# Timer
start_time = time.time()
counter = 0

for i in range (10000000):
    counter += 1

# Timer
print("--- %s seconds ---" % (time.time() - start_time))
