#!/usr/bin/env python3
import random

sons_size = 1000
id_list = []
names = []
sons = []
fathers = []
cur_idx = 0
dict = {}
dictname = {}
prob = []
REGULAR_QUARANTINE_THRESHOLD = 0.1
REGULAR_QUARANTINE_MSG = "14-days-Quarantine Required:"
MEDICAL_SUPERVISION_THRESHOLD = 0.3
MEDICAL_SUPERVISION_THRESHOLD_MSG = "Hospitalization Required:"
CLEAN_MSG = "No serious chance for infection:"

def p_father_sons(father, num_sons):
    #print("father=%9d num_sons=%d" % (father, num_sons))
    fathers.remove(father)
    if num_sons == 0:
        return
    for i in range(num_sons):
        fmeetings.write("%9d %9d %.1f %.1f\n" % (father, sons[cur_idx+i], random.randrange(1,10), random.random()*30))
        fathers.append(sons[cur_idx+i])
                            
fnames = open("names", 'r')
for line in fnames:
    names.append(line[0:-1])
people_num = len(names)

fpeople = open("People.in", 'w')
for i in range(people_num):
    id_num = random.randrange(999999999)
    if id_num in id_list:
        id_num = random.randrange(999999999)
    id_list.append(id_num)
    age = random.random()*120
    fpeople.write("%s %9d %3.1f\n" % (names[i], id_num, age))
fpeople.close()

fmeetings = open("Meetings.in", 'w')
father = id_list[0]
fmeetings.write("%9d\n" % (father))

for i in range(sons_size):
    son_idx = random.randrange(1, people_num)
    sons.append(id_list[son_idx])

fathers.append(father)
while cur_idx < sons_size:
    father = fathers[0]
    if cur_idx == 0:
        num_cur_sons = random.randrange(1,20)
    else:
        num_cur_sons = random.randrange(20)

    if (cur_idx + num_cur_sons) > sons_size:
        num_cur_sons = sons_size - cur_idx
    p_father_sons(father, num_cur_sons)
    cur_idx += num_cur_sons

fpeople.close()
fmeetings.close()
#print("Input files created\n\n")

fpeople = open("People.in", 'r')
fmeetings = open("Meetings.in", 'r')
fresults = open("SpreaderDetectorAnalysis_test.out", 'w')

first = True
for line in fmeetings:
    words = line.split()
    if first:
        dict[words[0]] = 1.0
        first = False
    else:
        dict[words[1]]  = float(dict[words[0]]) * float(words[3]) * 1.0 / (float(words[2]) * 30.0)

for line in fpeople:
    words = line.split()
    if not words[1] in dict:
        dict[words[1]] = 0.0
    dictname[words[1]] = words[0]
#sorted = {k: v for k, v in sorted(dict.items(), key=lambda item: item[1])}
sorted = sorted(dict.items(), key=lambda x: x[1], reverse=True)

for i in sorted:
    if i[1] >= MEDICAL_SUPERVISION_THRESHOLD:
        str = MEDICAL_SUPERVISION_THRESHOLD_MSG
    elif i[1] >= REGULAR_QUARANTINE_THRESHOLD:
        str = REGULAR_QUARANTINE_MSG
    else:
        str = CLEAN_MSG
    #fresults.write("%.6f - %s %s %d.\n" % (i[1], str, dictname[i[0]], int(i[0])))
    fresults.write("%s %s %d.\n" % (str, dictname[i[0]], int(i[0])))
