import os
import subprocess
from subprocess import check_output
import datetime

TESTS_DIR = "TestGenerator/tests"
LP_SOLVER_PATH = "LPSolverPython//linear_solver.py"
BR_SOLVER_PATH = "Solutions//Greedy//solution.exe"
HE_SOLVER_PATH = "Solutions//Heuristic//solution.exe"
dt = datetime.datetime.now().strftime("%Y_%m_%d-%H_%M_%S")
RESULTS_PATH = f"Results/runs_{dt}.txt"


def get_flow_time(s):
    s = s.strip().decode("utf-8")
    return int(float(s.split(" ")[0])), float(s.split(" ")[1])


with open(RESULTS_PATH, 'w') as f:
    f.write("")


def main():
    test_filenames = os.listdir(TESTS_DIR)
    test_filenames.sort(key=lambda s: int(s.split('.')[0]))
    # print(test_filenames)

    for i in range(len(test_filenames)):
        print(f"Processing test {i+1}:")
        test_path = f"{TESTS_DIR}/{test_filenames[i]}"

        # linear solver
        lp_solver_out = check_output(["python", LP_SOLVER_PATH, test_path])
        lp_solver_flow, lp_solver_time = get_flow_time(lp_solver_out)

        # breakthrough algo
        br_solver_out = check_output([BR_SOLVER_PATH, test_path])
        br_solver_flow, br_solver_time = get_flow_time(br_solver_out)

        # heuristic algo
        he_solver_out = check_output([HE_SOLVER_PATH, test_path, "1"])
        he_solver_flow, he_solver_time = get_flow_time(he_solver_out)

        print(" Linear solver:")
        print(f"    max-flow: {lp_solver_flow}")
        print(f"    time: {lp_solver_time}")
        print(" Breakthrough algo:")
        print(f"    max-flow: {br_solver_flow}")
        print(f"    time: {br_solver_time}")
        print(" Heuristic solver:")
        print(f"    max-flow: {he_solver_flow}")
        print(f"    time: {he_solver_time}")
        print()

        n, h, p, d0, d1, d2 = test_filenames[i].split("_")
        with open(RESULTS_PATH, 'a') as f:
            f.write(f"{n} {h} {p} {d0} {d1} {d2}\n")
            f.write(f"{lp_solver_flow} {lp_solver_time}\n")
            f.write(f"{br_solver_flow} {br_solver_time}\n")
            f.write(f"{he_solver_flow} {he_solver_time}\n")

        # append shit and so on

        # n,h,
    '''
    print(lp_solver_flow, lp_solver_time)

    br_solver_out = check_output([BR_SOLVER_PATH, path])
    br_solver_out = br_solver_out.strip().decode("utf-8")

    print(br_solver_flow, f"{br_solver_time:.4f}")
    '''


if __name__ == "__main__":
    main()
