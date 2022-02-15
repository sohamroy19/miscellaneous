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

# Print table of senders and counts
print(f"""  #. {"Sender's Name":<24s} : {"Count":>8s}""")
print("------------------------------+----------")

for i, stat in enumerate(sorted(stats.items(), key=lambda x: x[1], reverse=True)):
    print(f"{(i + 1):>3d}. {stat[0]:<24s} : {stat[1]:>8d}")

print("------------------------------+----------")
print(f"     Total                    : {total:>8d}")
