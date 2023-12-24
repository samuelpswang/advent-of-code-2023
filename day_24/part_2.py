import sympy

hailstones = []
with open("input.txt", "r") as file:
    for line in file:
        s, v = line.split(" @ ")
        hailstone = [int(ss) for ss in s.split(", ")] + [int (sv) for sv in v.split(", ")]
        hailstones.append(hailstone)

equations = []
answers = []
xr, yr, zr, vxr, vyr, vzr = sympy.symbols("xr, yr, zr, vxr, vyr, vzr")
for i, (x, y, z, vx, vy, vz) in enumerate(hailstones):
    # (xr - x) / (vxr - vx) = (yr - y) / (vyr - vy) => (xr - x) * (vyr - vy) - (yr - y) * (vxr - vx) = 0
    # (yr - y) / (vyr - vy) = (zr - z) / (vzr - vz) => (yr - y) * (vzr - vz) - (zr - z) * (vyr - vy) = 0
    equations.append((xr - x) * (vyr - vy) - (yr - y) * (vxr - vx))
    equations.append((yr - y) * (vzr - vz) - (zr - z) * (vyr - vy))
    if i < 4:
        continue
    else:
        answers = [sol for sol in sympy.solve(equations) if all(x % 1 == 0 for x in sol.values())]
    if len(answers) == 1:
        break

print(answers[0][xr] + answers[0][yr] + answers[0][zr])
