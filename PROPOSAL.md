# Final Project Proposal

## Group Members:

Vincent Chen & Serena Chen

# Intentions:

Normally, multiplayer systems may ironically only serve one match at a time, and/or invalid tournament matching due to an uneven number of users. As a result, we hope to use a pool-based system and use live tracking rather than a strict tournament bracket.

We plan to incorporate the following features into our tic-tac-toe systems project:
- A live leaderboard, counting the number of wins from a pool of users indefinitely
- Matching based on number of rooms available in server — losers of each round are returned to the pool
- Win streak for users, also visible on live leaderboard
- Usernames/Initials for users to input when first entering

# Intended usage:

When a user launches the program, they’ll be prompted to enter a username, which will give them a name on the leaderboard. After logging in, the user will either be matched into a TTT game or placed in a waiting pool until another player/room becomes available.

During gameplay, users will interact through a terminal-based interface that displays the game board and prompts them for moves, using user input to place the XS and Os. Once a game ends, results will be recorded, and the live leaderboard will update automatically. The leaderboard will display usernames, total wins, and current win streaks, so that users can be able to see their ranking in real time.

# Technical Details:
- Each player will be handled as a process, and the central server process will manage the pool, matchmaking, and updating the live leaderboard.
- Semaphores will be used for the waiting queue in the pool of losers, and pipes will be established to send the wins to update the leaderboard.
- The scores will be stored in memory (e.g. an array, malloced or calloced)

## used topics:
- Allocating memory
- Processes (fork, exec etc.)
- Signals
- Shared memory
- Semaphores
- Pipes (named and unnamed)

## responsibilities:
- Vincent: Server logic, matchmaking system, and leaderboard
- Serena: Designing working tic tac toe, user interface, username/streak

# Intended pacing:
Thursday: complete matching with pools and room
Friday: complete MVP (leaderboard, matching; no streak)
Tuesday: add extra features and test (streak, username): around 1 per day
