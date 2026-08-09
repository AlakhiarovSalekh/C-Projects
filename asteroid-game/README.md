# Asteroid Game

A simple terminal-based asteroid dodging game written in C.

## Description

In this game, you control a ship (represented by 'O') at the bottom of the screen. Asteroids (represented by '*') fall from the top of the screen, and your goal is to avoid colliding with them. Each time an asteroid passes the bottom of the screen without hitting your ship, you earn a point.

## Controls

- Press 'a' to move left
- Press 'd' to move right

## Installation

1. Clone this repository or download the source files
2. Ensure you have gcc installed on your system
3. Use the provided Makefile to build the game

```bash
make
```

## How to Play

After building the game, run it with:

```bash
./asteroid_game
```

Move your ship left and right to avoid the falling asteroids. The game ends when an asteroid hits your ship.
