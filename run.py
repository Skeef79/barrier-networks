import os
import subprocess
from subprocess import check_output

TESTS_DIR = "TestGenerator/tests"
LP_SOLVER_PATH = "LPSolverPython//linear_solver.py"
BR_SOLVER_PATH = "BarrierNetworkFlow/bin/x64/Release/BarrierNetworkFlow.exe"


def main():
    test_filenames = os.listdir(TESTS_DIR)
    test_filenames.sort(key=lambda s: int(s.split('.')[0]))
    # print(test_filenames)

    for i in range(len(test_filenames)):
        print(f"Processing test {i+1}:")
        test_path = f"{TESTS_DIR}/{test_filenames[i]}"

        # linear solver
        lp_solver_out = check_output(["python", LP_SOLVER_PATH, test_path])
        lp_solver_out = lp_solver_out.strip().decode("utf-8")
        lp_solver_flow, lp_solver_time = int(float(lp_solver_out.split(
            " ")[0])), float(lp_solver_out.split(" ")[1])

        # breakthrough algo
        br_solver_out = check_output([BR_SOLVER_PATH, test_path])
        br_solver_out = br_solver_out.strip().decode("utf-8")
        br_solver_flow, br_solver_time = int(
            float(br_solver_out.split(" ")[0])), float(br_solver_out.split(" ")[1])

        print(" Linear solver:")
        print(f"    max-flow: {lp_solver_flow}")
        print(f"    time: {lp_solver_time}")
        print(" Breakthrough algo:")
        print(f"    max-flow: {br_solver_flow}")
        print(f"    time: {br_solver_time}")
        print()

    '''
    print(lp_solver_flow, lp_solver_time)

    br_solver_out = check_output([BR_SOLVER_PATH, path])
    br_solver_out = br_solver_out.strip().decode("utf-8")

    print(br_solver_flow, f"{br_solver_time:.4f}")
    '''


if __name__ == "__main__":
    main()
