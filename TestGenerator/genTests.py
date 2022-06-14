import os
import subprocess

list_n = [10, 20, 30, 40, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300]
list_h = [1, 2, 3, 5]
list_p = [0.3, 0.5, 0.7]
list_d = [[7, 2, 1], [5, 3, 2]]
cap_lb = 1
cap_ub = 10000

for n in list_n:
    for h in list_h:
        for p in list_p:
            for d in list_d:
                filename = os.path.join("tests", "_".join([str(n), str(h), str(
                    p), str(d[0]), str(d[1]), str(d[2])]))
                subprocess.check_output(["gen.exe",
                                         str(n), str(h), str(p), str(
                                             cap_lb), str(cap_ub),
                                         str(d[0]), str(d[1]), str(d[2]),
                                         filename])
                print(filename)
