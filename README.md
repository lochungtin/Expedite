# Project Expedite

Project Expedite is my attempt to create solutions, algorithms, or solvers to classic puzzle games. There are a few principles I abide by when creating these algorithms.

## Principles

1. **Efficiency** - It is trivial to say that brute force searching can solve most problems, but where is the fun in that.

2. **Elegance** - The solution should be neat, clear, or clever in some specific way.

3. **Robustness** - The solution should be able to adapt to different "difficulty" levels of the game, whatever that may mean to the game, be it grid size or available options.

## Usage

Setup an environment with any python version `> 3.7` and run the command in the root directory.

```bash
python3 main.py --gamemode <gamemode> --config <config_file>
```

Run the command `python3 main.py --help` for more information.

## Directory and Status

The following list shows the puzzle games for this project and their respective status in solution development.

This list will be expanded upon and updated as status for sub-projects change and / or new puzzles are being added to the catalogue.

<div align="center">

| Game                                                 | Algorithm Status                                                       |
| :--------------------------------------------------- | :--------------------------------------------------------------------- |
| <a href="docs/2048.md/">2048</a>                     | 📒 Backlog                                                             |
| <a href="docs/Kuromasu.md/">Kuromasu</a>             | ✅ Complete                                                            |
| <a href="docs/Mastermind.md/">Mastermind</a>         | 🛑 Paused                                                              |
| <a href="docs/MineSweeper.md/">Mine Sweeper</a>      | ✅ Complete                                                            |
| <a href="docs/Nonogram.md/">Nonogram</a>             | 🛑 Paused                                                              |
| <a href="docs/Numberlink.md/">Numberlink</a>         | 📒 Backlog                                                             |
| <a href="docs/Slitherlink.md/">Slitherlink</a>       | 📒 Backlog                                                             |
| <a href="docs/Snake.md/">Snake</a>                   | ✅ <a href="https://github.com/lochungtin/snakeAI">Partly Complete</a> |
| <a href="docs/Sudoku.md/">Sudoku</a>                 | ✅ Complete                                                            |
| <a href="docs/Tazuku.md/">Tazuku</a>                 | ✅ Complete                                                            |
| <a href="docs/SpellingBee.md">Spelling Bee (NYT)</a> | ✅ Complete                                                            |
| <a href="docs/Wordle">Wordle (NYT)</a>               | ✅ Complete                                                            |
| <a href="docs/LetterBoxed">Letter Boxed (NYT)</a>    | 📒 Backlog                                                             |

</div>

## Planned Features

1. OpenCV computer vision for game integration
    - Ability to interact with applications and solve puzzles from apps autonomously
