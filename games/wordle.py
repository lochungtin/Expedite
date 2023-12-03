from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.ans = lines[0].strip()
        self.board = []
        self.hints = []

    def __repr__(self):
        if len(self.board) == 0:
            return ""
        o = {0: " ", 1: "(", 2: "["}
        c = {0: " ", 1: ")", 2: "]"}
        row = "+" + "".join("---+" for _ in range(5)) + "\n"
        rt = [row + "|" + "".join(f"{o[b]}{l.upper()}{c[b]}|" for l, b in zip(w, h)) for w, h in zip(self.board, self.hints)]
        return "\n".join(rt + [row[0 : len(row) - 1]])

    def guess(self, g):
        self.board.append(g)
        hint = [0 + int(g[i] in self.ans) + int(g[i] == self.ans[i]) for i in range(5)]
        self.hints.append(hint)
        return hint
