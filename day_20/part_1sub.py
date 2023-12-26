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

# running machine
tally = {"lo": 0, "hi": 0}
for _ in range(1000):
    tally["lo"] += 1
    queue = [("broadcaster", name, "lo") for name in broadcast]
    while queue:
        src, dst, sig = queue.pop(0)
        tally[sig] += 1
        if dst not in modules:
            continue
        mod = modules[dst]
        
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

print(tally["hi"] * tally["lo"])
    