from game import Game
from solver import Solver


def main():
    g = Game(6.2)
    print(g)

    s = Solver(g)
    i = s.run()

    print(g)
    print("Solved in {} iterations.".format(i))


if __name__ == "__main__":
    main()
