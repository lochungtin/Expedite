# Project Expedite

Project Expedite is my attempt to create solutions, algorithms, or solvers to classic puzzle games. There are a few principles I abide by when creating these algorithms.

## Principles

1. **Efficiency** - It is trivial to say that brute force searching can solve most problems, but where is the fun in that.

2. **Elegance** - The solution should be neat, clear, or clever in some specific way.

3. **Robustness** - The solution should be able to adapt to different "difficulty" levels of the game, whatever that may mean to the game, be it grid size or available options.

## Quick Start

Setup an environment with any python version `> 3.7` and run the command in the root directory.

```bash
python3 main.py --gamemode <gamemode> --config <config_file_index>

python3 main.py -g <gamemode> -c <config_file_index>
```

Run the command `python3 main.py --help` for more information.

## Directory and Status

The following list shows the puzzle games for this project and their respective status in solution development.

This list will be expanded upon and updated as status for sub-projects change and / or new puzzles are being added to the catalogue.

<div align="center">

| Game                                         | Algorithm Status                                            |
| :------------------------------------------- | :---------------------------------------------------------- |
| [2048](./games/2048.md)                      | 📒 Backlog                                                  |
| [Kuromasu](./games/Kuromasu.md)              | ✅ Complete                                                 |
| [Mastermind](./games/Mastermind.md)          | 🛑 Paused                                                   |
| [Mine Sweeper](./games/MineSweeper.md)       | ✅ Complete                                                 |
| [Nonogram](./games/Nonogram.md)              | 🛑 Paused                                                   |
| [Numberlink](./games/Numberlink.md)          | 📒 Backlog                                                  |
| [Slitherlink](./games/Slitherlink.md)        | 📒 Backlog                                                  |
| [Snake](./games/Snake.md)                    | ✅ [Partly Complete](https://github.com/lochungtin/snakeAI) |
| [Sudoku](./games/Sudoku.md)                  | ✅ Complete                                                 |
| [Tazuku](./games/Tazuku.md)                  | ✅ Complete                                                 |
| [(NYT) SpellingBee](./games/SpellingBee.md)  | ✅ Complete                                                 |
| [(NYT) Wordle](./games/Wordle.md)            | ✅ Complete                                                 |
| [(NYT) Letter Boxed](./games/LetterBoxed.md) | 📒 Backlog                                                  |

</div>

## Planned Features

1. OpenCV computer vision for game integration
    - Ability to interact with applications and solve puzzles from apps autonomously
