from multiprocessing import Barrier

from numpy import Infinity
import graph
import sys

from ortools.linear_solver import pywraplp
from ortools.init import pywrapinit
import time


def main():
    g = graph.BarrierGraph()

    file_name = sys.argv[1]

    g.inputGraph(file_name)
    # g.printGraph()

    startTime = time.time()

    g.initVariables()
    g.initConstrains()
    g.initObjective()

    max_flow = g.solve()
    endTIme = time.time()

    print(f'{max_flow} {(endTIme-startTime):.4f}')


if __name__ == '__main__':
    main()
