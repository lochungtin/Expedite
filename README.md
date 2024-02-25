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

| Game                                                        | Algorithm Status                                            |
| :---------------------------------------------------------- | :---------------------------------------------------------- |
| [2048](./docs/games/2048.md)                                | 📒 Backlog                                                  |
| [Kuromasu](./docs/games/Kuromasu.md)                        | ✅ Complete                                                 |
| [Mastermind](./docs/games/Mastermind.md)                    | 🛑 Paused                                                   |
| [Mine Sweeper](./docs/games/MineSweeper.md)                 | ✅ Complete                                                 |
| [Nonogram](./docs/games/Nonogram.md)                        | 🛑 Paused                                                   |
| [Numberlink](./docs/games/Numberlink.md)                    | 📒 Backlog                                                  |
| [Slitherlink](./docs/games/Slitherlink.md)                  | 📒 Backlog                                                  |
| [Snake](./docs/games/Snake.md)                              | ✅ [Partly Complete](https://github.com/lochungtin/snakeAI) |
| [Sudoku](./docs/games/Sudoku.md)                            | ✅ Complete                                                 |
| [Tazuku](./docs/games/Tazuku.md)                            | ✅ Complete                                                 |
| [(NYT) SpellingBee](./docs/games/NYT/SpellingBee.md)        | ✅ Complete                                                 |
| [(NYT) Wordle](./docs/games/NYT/Wordle.md)                  | ✅ Complete                                                 |
| [(NYT) Letter Boxed](./docs/games/NYT/LetterBoxed.md)       | 📒 Backlog                                                  |
| [(HBM) Chimp Test](./docs/games/HBM/ChimpTest.md)           | ✅ Complete                                                 |
| [(HBM) Aim Trainer](./docs/games/HBM/AimTrainer.md)         | ✅ Complete                                                 |
| [(HBM) Reaction Time](./docs/games/HBM/ReactionTime.md)     | ✅ Complete                                                 |
| [(HBM) Visual Memory](./docs/games/HBM/VisualMemory.md)     | 📒 Backlog                                                  |
| [(HBM) Sequence Memory](./docs/games/HBM/SequenceMemory.md) | 📒 Backlog                                                  |

</div>

## Planned Features

1. OpenCV computer vision for game integration
    - Ability to interact with applications and solve puzzles from apps autonomously
