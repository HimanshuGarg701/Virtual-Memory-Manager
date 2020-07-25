import sys

if len(sys.argv) != 2:
    print('Invalid Arguments')
o = []
correct = []
with open('o','r') as fn:
    for l in fn:
        item = l.strip().split()
        o.append((item[2],item[7]))

with open('correct.txt','r') as fnco:
    for l in fnco:
        item  = l.strip().split()
        correct.append((item[2],item[7]))

for o, co in zip(o, correct):
    if(o != co):
        print('Values are different!')
        sys.exit(-1)
print('Values are the same!')