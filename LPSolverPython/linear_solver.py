from multiprocessing import Barrier

from numpy import Infinity
import graph
import sys

from ortools.linear_solver import pywraplp
from ortools.init import pywrapinit


def main():
    g = graph.BarrierGraph()
    g.inputGraph('input.txt')
    # g.printGraph()

    g.initVariables()
    g.initConstrains()
    g.initObjective()

    g.solve()


if __name__ == '__main__':
    main()
