[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Tic-Tac-Toe: Multiplayer Ranked!

### Our-Fantastic-Group-Name-TTT

Vincent Chen & Serena Chen

Recording link: [add once recorded]

### Project Description:

This project seeks to implement tic-tac-toe using a ranked leaderboard and tracking game mechanics by using the following concepts:
- Allocating memory
- Processes
- Working with files
- Signals
- Sockets

Multiple clients can connect to a central server over TCP sockets, enter a matchmaking pool, and be paired into head-to-head games. The server maintains game state, enforces turn order, and tracks player statistics such as wins/losses in a ranked leaderboard. It's fun to play!

### Instructions:

The user compiles/runs this program by running make server in one terminal window, and running make client a positive number x of times, where x is greater than 2. From there, a subset will be matched into games with one another, where they'll be playing until one side wins or there is a draw.

The user interacts with this program by typing in a number from 1-9 inclusive to place a piece on the board. A player wins once they earn "three-in-a-row" before their opponent, at which point the game will end.

### Resources/ References:

- https://www.ibm.com/docs/en/zos/2.5.0?topic=calls-select
- https://support.sas.com/documentation/onlinedoc/sasc/doc700/html/lr2/select.htm

We used these links to check for new matches to be formed (every 2 seconds, the matchmake function is ran to pair people together). Insertion sort was used to sort the leaderboard, which dictates the matches that are formed.