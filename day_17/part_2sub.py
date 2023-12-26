from heapq import heappush, heappop

map = []
with open("input.txt") as f:
    for line in f:
        row = []
        for c in line.strip("\n"):
            row.append(int(c))
        map.append(row)
    
R = len(map)-1
C = len(map[0])-1
assert(all(len(line) == C+1 for line in map))

direc = [[-1, 0], [0, 1], [1, 0], [0, -1]]
queue = [(0, 0, 0, -1, -1)] # cost, row, col, dir, count
                            # cannot set dir and count; otherwise we will miss out on one step
costs = {}

while queue:
    cost, row, col, dir, count = heappop(queue)
    if (row, col, dir, count) in costs:
        continue
    else:
        costs[(row, col, dir, count)] = cost
    
    for i, [dr, dc] in enumerate(direc):
        new_row = row + dr
        new_col = col + dc
        new_dir = i
        
        if new_dir == dir:
            new_count = count + 1
        else:
            new_count = 1
            
        if 0 <= new_row <= R and 0 <= new_col <= C:
            new_cost = cost + map[new_row][new_col]
        else:
            continue
        
        is_valid = 0 <= new_row <= R and 0 <= new_col <= C and new_count <= 10
        is_valid = is_valid and (dir == new_dir or count >= 4 or count == -1)
        is_forward = (new_dir+2) % 4 != dir
        is_unique = (new_row, new_col, new_dir, new_count) not in costs
        if is_valid and is_forward and is_unique:
            heappush(queue, (new_cost, new_row, new_col, new_dir, new_count))

min_cost = 1e6
for key in costs:
    row, col, dir, count = key
    if row == R and col == C:
        min_cost = min(min_cost, costs[key])

print(min_cost)
