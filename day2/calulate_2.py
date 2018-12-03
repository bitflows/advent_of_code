from collections import Counter

fname="input"

def p(content):
	print content


count_two = 0
count_three = 0
with open(fname) as fp:
	content = fp.readlines()

	for outer_index, outer_line in enumerate(content):
		for inner_index, inner_line in enumerate(content):
			diff_index = -1
			if inner_index <= outer_index:
				continue

			for i,c in enumerate(inner_line):
				if inner_line[i] == outer_line[i]:
					continue
				if diff_index != -1:
					diff_index = -1
					break
				diff_index = i

			if diff_index != -1:
				common = ""
				for i,c in enumerate(inner_line):
					if inner_line[i] == outer_line[i]:
						common += inner_line[i]
				print "Common:", common
