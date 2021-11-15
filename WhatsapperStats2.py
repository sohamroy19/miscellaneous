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
for s, c in stats.items():
    print(s, " : ", c)

# Total
print("Total : ", total)
