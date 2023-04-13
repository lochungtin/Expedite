from game import Game
from solver import Solver


def main():
    g = Game(0)

    s = Solver(g)
    i, q, d = s.run()

    print(g)
    print("Solved in {} iterations with {} guesses and {} deaths.".format(i, q, d))


if __name__ == "__main__":
    main()
