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

    '''
    solver = pywraplp.Solver.CreateSolver('GLOP')

    c = [5, 10, 15, 5, 10]

    vars = [solver.NumVar(0, c[i], str(i)) for i in range(5)]

    t = vars[1] - vars[2]
    t -= vars[3]

    solver.Add(t == 0)
    solver.Add(vars[0] + vars[2] - vars[4] == 0)

    solver.Maximize(vars[0] + vars[1])

    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        print('Solution:')
        print('Objective value =', solver.Objective().Value())
        for var in vars:
            print(var, var.solution_value())
    else:
        print('The problem does not have an optimal solution.')

    print('\nAdvanced usage:')
    print('Problem solved in %f milliseconds' % solver.wall_time())
    print('Problem solved in %d iterations' % solver.iterations())
    '''


if __name__ == '__main__':
    main()
