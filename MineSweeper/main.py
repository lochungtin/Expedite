from game import Game

# from solver import Solver


def main():
    g = Game(2)
    r, c = g.row // 2, g.col // 2
    g.expand(r, c)
    print(g)

    # s = Solver(g)
    # i = s.run()

    # print(g)
    # print("Solved in {} iterations.".format(i))


if __name__ == "__main__":
    main()
