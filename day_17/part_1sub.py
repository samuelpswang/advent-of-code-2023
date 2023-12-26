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
        
        is_valid = 0 <= new_row <= R and 0 <= new_col <= C and new_count <= 3
        is_forward = (new_dir+2) % 4 != dir
        if is_valid and is_forward:
            heappush(queue, (new_cost, new_row, new_col, new_dir, new_count))

min_cost = 1e6
for key in costs:
    row, col, dir, count = key
    if row == R and col == C:
        min_cost = min(min_cost, costs[key])

print(min_cost)

# inspiration taken from @jonathanpaulson and @hyper-neutrino
# the solution is essentially a djikstra's algorithm, which I had not 
# previously thought of and was trying various ways of bfs
