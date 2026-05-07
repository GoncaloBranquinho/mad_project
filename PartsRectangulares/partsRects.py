from ortools.linear_solver import pywraplp

def solve_partRects():
    solver = pywraplp.Solver.CreateSolver('SCIP')  # MIP solver

    # Decision variables
    x = {}
    for i in range(1,9):
        x[i] = solver.IntVar(0, 1, f'x_{i}')
    
    # Constraints
    solver.Add(x[8] >= 1)
    solver.Add(x[8]+x[7] >= 1)
    solver.Add(x[7]+x[6]+x[4]+x[5] >= 1)
    solver.Add(x[7]+x[6]+x[8] >= 1)
    solver.Add(x[3]+x[4] >= 1)
    solver.Add(x[2]+x[1] >= 1)
    solver.Add(x[5]+x[4]+x[3]+x[2] >= 1)
    solver.Add(x[5]+x[6] >= 1)
    solver.Add(x[1]+x[3]+x[2] >= 1)
    solver.Add(x[1] >= 1)
    
    # Objective: minimize cost
    objective = solver.Objective()
    for i in range(1,9):
        objective.SetCoefficient(x[i],1)
    objective.SetMinimization()

    # Solve
    status = solver.Solve()

    if status == pywraplp.Solver.OPTIMAL:
        print("Optimal cost:", objective.Value())
        for i in range(1,9):
            if x[i].solution_value() > 0.5:
                print(f'Guard at node {i}')
    else:
        print("No optimal solution found.")

solve_partRects()
