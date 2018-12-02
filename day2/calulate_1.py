from collections import Counter

fname="input"

count_two = 0
count_three = 0
with open(fname) as fp:
    for cnt, line in enumerate(fp):
      i = Counter(line).most_common()

      is_three = False
      is_two = False
      for iter, e in enumerate(i):
        if e[1] == 3:
          is_three = True
        if e[1] == 2:
          is_two = True

      if is_two:
        count_two += 1
      if is_three:
        count_three += 1

print count_two * count_three
