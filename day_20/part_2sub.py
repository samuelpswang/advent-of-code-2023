from math import lcm

# module template
class Module:
    def __init__(self, name, type, output):
        self.name = name
        self.type = type
        self.output = output
        if type == '%':
            self.memory = "off"
        else:
            self.memory = {}

# read input file
modules = {}
broadcast = []
with open("input.txt") as f:
    for line in f:
        type_name, output = line.split(" -> ")
        if type_name == "broadcaster":
            broadcast = output.strip("\n").split(", ")
        else:
            type = type_name[0]
            name = type_name[1:]
            output = output.strip("\n").split(", ")
            modules[name] = Module(name, type, output)

# set conjunction input to low
for name, module in modules.items():
    if module.type == '&':
        for cname, cmodule in modules.items():
            if module.name in cmodule.output:
                modules[name].memory[cname] = "lo"

# find who feeds into the rx module - we assume this is just one conjunction module
feed = [name for name, module in modules.items() if "rx" in module.output]
assert len(feed) == 1
feed = feed[0]

# find who feeds into the conjunction module and find their cycles
cycle = {name: 0 for name, module in modules.items() if feed in module.output}
seen = {name: False for name, module in modules.items() if feed in module.output}

# running machine
found = False
button_press = 0
lowest_button_press = 1
while not found:
    button_press += 1
    queue = [("broadcaster", name, "lo") for name in broadcast]
    while queue:
        src, dst, sig = queue.pop(0)
        if dst not in modules:
            continue
        mod = modules[dst]
        
        if src in cycle and not seen[src] and dst == feed and sig == "hi":
            cycle[src] = button_press
            seen[src] = True
        if all(value for name, value in seen.items()):
            found = True
            for src, num in cycle.items():
                lowest_button_press = lcm(lowest_button_press, num)
            break
        
        if mod.type == "%":
            if sig == "lo":
                if mod.memory == "off":
                    mod.memory = "on"
                    for ndst in mod.output:
                        queue.append((dst, ndst, "hi"))
                else:
                    mod.memory = "off"
                    for ndst in mod.output:
                        queue.append((dst, ndst, "lo"))
        else:
            mod.memory[src] = sig
            if all(sigx == "hi" for name, sigx in mod.memory.items()):
                for ndst in mod.output:
                    queue.append((dst, ndst, "lo"))
            else:
                for ndst in mod.output:
                    queue.append((dst, ndst, "hi"))

print(lowest_button_press)
