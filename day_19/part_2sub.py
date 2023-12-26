from copy import deepcopy

# read input file
flows = {}
with open("input.txt") as f:
    for line in f:
        line = line.strip('\n')
        if line == "" or line[0] == '{':
            continue
        
        flow_name, flow = line.split('{')
        flows[flow_name] = []
        flow = flow.strip('}').split(',')
        for rule in flow:
            if ':' in rule:
                colon = rule.find(':')
                flows[flow_name].append((rule[0], rule[1], int(rule[2:colon]), rule[colon+1:])) # operand, inequality, value, destination
            else:
                flows[flow_name].append(('', '', '', rule))

# process initial range
total = 0
trans = {'x': 0, 'm': 1, 'a': 2, 's': 3}
queue = [("in", [1, 4000, 1, 4000, 1, 4000, 1, 4000])]

while queue:
    state, bound = queue.pop(0)
    if (bound[0] > bound[1] or bound[2] > bound[3] or bound[4] > bound[5] or bound[6] > bound[7]):
        continue
    elif state == "R":
        continue
    elif state == "A":
        total += (bound[1]-bound[0]+1) * (bound[3]-bound[2]+1) * (bound[5]-bound[4]+1) * (bound[7]-bound[6]+1)
    else:
        rules = flows[state]
        for opd, ieq, val, des in rules:
            
            # last rule
            if opd == '':
                queue.append((des, bound))
                continue
                
            # other rules
            ind = trans[opd]*2
            if ieq == '<':
                if val <= bound[ind]: # all false - continue in this flow
                    continue
                elif bound[ind] < val <= bound[ind+1]: # split - process
                    T = deepcopy(bound)
                    F = deepcopy(bound)
                    T[ind+1] = val-1
                    F[ind] = val
                    bound = F
                    queue.append((des, T))
                else: # all true - exit
                    queue.append((des, bound))
                    break
            else:
                if val < bound[ind]: # all true - exit
                    queue.append((des, bound))
                    break
                elif bound[ind] <= val < bound[ind+1]: # split - process
                    T = deepcopy(bound)
                    F = deepcopy(bound)
                    T[ind] = val+1
                    F[ind+1] = val
                    bound = F
                    queue.append((des, T))
                else: # all false - continue in this flow
                    continue

print(total)
        