# Spelling Bee Solver

## Game

Spelling Bee is a mini word game designed by the New York Times games. The aim of the game is to find words that are composed of 7 selected letters. 6 of these letters are optional, which means it **can** appear in the word. The last letter is mandatory, which means it **must** appear in the word.

Game representation, the center hexagon houses the mandatory letter, the surrounding hexagons houses the optional letters.

```
     ___
 ___/ h \___
/ l \___/ a \
\___/ i \___/
/ g \___/ r \
\___/ t \___/
    \___/
```

## Algorithm

The solver uses the 30k most frequent word corpus to find all the words that suit the above criterions.

### Algorithm Structure

```cpp
String[] corpus;
corpus.filter(criteron);
corpus.sort(by_length);
```

## Config Setup
