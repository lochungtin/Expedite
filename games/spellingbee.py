from games.__game import __Game


class Game(__Game):
    def __init__(self, lines):
        super().__init__()
        self.center = lines[0].strip()
        self.letters = lines[1].strip().split(" ")

    def __repr__(self):
        return f"     ___\n ___/ {self.letters[0]} \___\n/ {self.letters[1]} \___/ {self.letters[2]} \\\n\___/ {self.center} \___/\n/ {self.letters[3]} \___/ {self.letters[4]} \\\n\___/ {self.letters[5]} \___/\n    \___/\n"
