class __Game:
    def __init__(self, mappedDisplay=False):
        self.board, self.size = None, None
        self._d = mappedDisplay
        self._m = {"0": "░", "1": "▓", "-": " "}

    def __str__(self):
        return self.__repr__()

    def __repr__(self):
        row = "+" + "".join("---+" for _ in range(len(self.board))) + "\n"
        rt = [row + "|" + "".join(" {} |".format(self._m.get(c, c) if self._d else c) for c in l) for l in self.board]
        return "\n".join(rt + [row[0 : len(row) - 1]])
