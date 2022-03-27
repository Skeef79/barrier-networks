from opcode import hasjabs
from ortools.linear_solver import pywraplp
from ortools.init import pywrapinit


class Edge:
    def __init__(self, u, v, capacity, edge_type):
        self.u = u
        self.v = v
        self.capacity = capacity
        self.edge_type = edge_type


class BarrierGraph:
    def __init__(self):
        self.n = 0  # number of vertices
        self.m = 0  # number of edges
        self.edges = []  # list of edges
        self.h = 0  # barrier height
        self.capcaty = []  # capacity for each edge
        self.edge_types = []  # type for each edge
        self.variables = []  # variables for LP
        self.solver = pywraplp.Solver.CreateSolver('GLOP')  # define a solver

    def inputGraph(self, path):
        with open(path, 'r') as f:
            self.n = int(f.readline())
            self.h = int(f.readline())
            self.m = int(f.readline())
            self.capacity = [[0]*self.n for i in range(self.n)]
            self.edge_types = [[-1]*self.n for i in range(self.n)]

            for i in range(self.m):
                u, v, capacity, edge_type = f.readline().split()
                u = int(u)
                v = int(v)
                capacity = float(capacity)
                edge_type = int(edge_type)

                self.edges.append(Edge(u, v, capacity, edge_type))
                self.capacity[u][v] = capacity
                self.edge_types[u][v] = edge_type

    def printGraph(self):
        print(self.n, self.h, self.m)
        for edge in self.edges:
            print(edge.u, edge.v, edge.capacity, edge.edge_type)

    def initVariables(self):
        aux_size = self.n*(self.h+1)+1
        self.variables = [[0]*aux_size for i in range(aux_size)]

        # declare variables
        for _from in range(self.n):
            for _to in range(self.n):
                if self.edge_types[_from][_to] == -1:
                    continue

                if self.edge_types[_from][_to] == 2:  # barrier edge
                    from_aux = _from*(self.h+1) + self.h
                    to_aux = _to*(self.h+1)
                    self.variables[from_aux][to_aux] = self.solver.NumVar(
                        0, self.capacity[_from][_to], f'{_from}_{_to}_{self.h}_{0}'
                    )
                else:
                    for j in range(self.h+1):
                        if self.edge_types[_from][_to] == 0:
                            from_aux = _from*(self.h+1)+j
                            to_aux = _to*(self.h+1)+j
                            self.variables[from_aux][to_aux] = self.solver.NumVar(
                                0, self.capacity[_from][_to], f'{_from}_{_to}_{j}_{j}'
                            )
                        else:
                            from_aux = _from*(self.h+1) + j
                            to_aux = _to*(self.h+1) + j+1
                            if j == self.h:
                                to_aux -= 1

                            self.variables[from_aux][to_aux] = self.solver.NumVar(
                                0, self.capacity[_from][_to], f'{_from}_{_to}_{j}_{j+1}'
                            )
        t = self.n*(self.h+1)
        for j in range(self.h+1):
            from_aux = (self.n-1)*(self.h+1)+j
            to_aux = t
            self.variables[from_aux][to_aux] = self.solver.NumVar(
                0, self.solver.infinity(), f'{self.n-1}_{self.n}_{j}_{j}'
            )
