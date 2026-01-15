# Dev Log:

This document must be updated daily every time you finish a work session.

## Serena Chen

### 2025-01-06 - Proposal tweaks
Went through proposal and updated things such as pacing. Also added more details to how things will be implemented. ~30 mins

### 2025-01-07 - Plan interface/Started server setup
Planned out what the user interface would look like for both server and client. Moved lab16 code and started modifying server setup. ~1hr

### 2025-01-08 - Username sending to server
Worked on prompting client for a username and sending it to server for display. Also started implementing for when
client disconnects.  ~40 mins

### 2025-01-09 - player + match setup
Made new structs for a player and a match. Started integrating it into server and client. Indenting fixes. ~1hr

### 2025-01-10 - Add and remove players, started leaderboard
Implemented player and match structs into server, including converting sending usernames between server and clients to add player and remove player helper functions. Started updating subserver logic then realized the data is not shared between servers since we were forking. Started implement matchmaking but very broken. ~3hrs

### 2025-01-11 - Matchmaking, convert to select, leaderboard working
Converted from forking subservers into using select to handle multiple clients so leaderboard would update properly. Fixed main to include stuff from original subserver function when converting. Implemented matchmaking properly, is working. Leaderboard is also working. ~1.5hrs

### 2026-1-12 - Testing + buffer reading error
Updated Vincent with the work I did over the weekend. Did some testing with current MVP, fix minor bugs such as makefile. There was also a buffer reading error when printing the username that I was trying to fix. Also discussed with Vincent how we would implement game_move. ~1.2 hrs

### 2026-1-13 - game_move & matchmake fix
Went over Vincent's implementation of game_move. Also tried fixing an error of prompting both players to play in matchmake. Unfinished and its kinda broken. ~1hr


