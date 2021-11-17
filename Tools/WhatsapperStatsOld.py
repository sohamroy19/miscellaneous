"""
    Author: @sohamroy19
    Date: 12/12/2020

    This script prints the number of messages sent by a WhatsApp user,
    given the exported chat as 'chat.txt'.
"""

import re

# open the file
f = open("chat.txt", encoding="utf8")

# idk how to use map data structure
senders = []
counts = []
total = 0

# read line by line, stripping is always fun
for x in f:
    x = x.strip()

    # check if line is the beginning of a message
    m = re.match("\d+/\d+/\d+, \d+:\d+ - .+:", x)
    if m:
        sender = x.split("-")[1].split(":")[0].strip()  # extract sender
        total += 1  # increment total

        try:
            i = senders.index(sender)  # check if sender already exists
        except ValueError:  # if not, then add entry
            senders.append(sender)
            counts.append(1)
        else:  # if yes, then increment count
            counts[i] += 1

# List of senders and counts
for s, c in zip(senders, counts):
    print(s, " : ", c)

# Total
print("Total : ", total)
