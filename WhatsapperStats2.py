"""
    Author: @sohamroy19
    Date: 16/11/2021

    This script prints the number of messages sent by a WhatsApp user,
    given the exported chat as 'chat.txt'.
"""

import re

# open the file
f = open("chat.txt", encoding="utf8")

# now I know how to use map data structure
stats = {}
total = 0

# read line by line, stripping is always fun
for x in f:
    x = x.strip()

    # check if line is the beginning of a message
    m = re.match("\d+/\d+/\d+, \d+:\d+ - .+:", x)
    if m:
        sender = x.split("-")[1].split(":")[0].strip()  # extract sender

        total += 1  # increment total
        stats[sender] = stats.get(sender, 0) + 1  # increment sender's count

# List of senders and counts
pairs = sorted(stats.items(), key=lambda x: x[1], reverse=True)
for i in range(len(pairs)):
    print("{:3d}.".format(i + 1), pairs[i][0], "\t: ", pairs[i][1])

# Total
print("\nTotal : ", total)
