import getopt
import sys
from importlib import import_module
from os import listdir
from pathlib import Path


def printHelp():
    print("python main.py -g <gamemode> -c <config>")
    print("\n-g --gamemode <gamemode> [REQUIRED]")
    print("Available gamemode options include:")
    for name in listdir(Path("configs")):
        print(f"\t{name}")
    print("\n-c --config <config> [REQUIRED]")
    print(
        '\tCheck the directory "./configs/<gamemode>/" for all config files available.'
    )


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
    print(f"Solved in {i} iterations.")


if __name__ == "__main__":
    try:
        opts = dict(getopt.getopt(sys.argv[1:], "hg:c:", ["gamemode=", "conf="])[0])
    except:
        printHelp()
        sys.exit(2)

    if len(opts) < 1:
        printHelp()
        sys.exit(2)

    gamemode, config = None, None
    if "-g" in opts:
        gamemode = opts["-g"]
    if "--gamemode" in opts:
        gamemode = opts["--gamemode"]
    if "-c" in opts:
        config = opts["-c"]
    if "--config" in opts:
        config = opts["--config"]

    if f"{gamemode}.py" not in listdir(Path("games")):
        print(f'ERROR! Game "{gamemode}" not found.')
        sys.exit(2)

    if f"{gamemode}.py" not in listdir(Path("solvers")):
        print(f'ERROR! Game "{gamemode}" not found.')
        sys.exit(2)

    if gamemode not in listdir(Path("configs")):
        print(f'ERROR! Config file "config_{config}.txt" not found.')
        sys.exit(2)

    if f"config_{config}.txt" not in listdir(Path(f"configs/{gamemode}")):
        print(f'ERROR! Config file "config_{config}.txt" not found.')
        sys.exit(2)

    if gamemode is None or config is None:
        printHelp()
        sys.exit(2)

    execute(gamemode, config)
