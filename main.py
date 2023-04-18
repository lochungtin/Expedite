from importlib import import_module
from pathlib import Path


def execute(gamemode, config):
    game = import_module(f"games.{gamemode}")
    solver = import_module(f"solvers.{gamemode}")

    path = Path(f"./configs/{gamemode}/config_{config}.txt")
    with open(path, "r") as f:
        lines = f.readlines()

    g = game.Game(lines)
    print(g)

    s = solver.Solver(g)
    i = s.run()

    print(g)
    print("Solved in {} iterations.".format(i))


if __name__ == "__main__":
    gamemode = "kuromasu"
    config = 6.1
    execute(gamemode, config)
