\# UE5 Steam Multiplayer Racing Prototype



A multiplayer racing prototype built in \*\*Unreal Engine 5.6\*\*, focused mainly on learning and implementing multiplayer logic using \*\*Steam Online Subsystem\*\* and a \*\*listen server\*\* setup.



This project is not focused on advanced racing mechanics or final game polish. The main goal of the project is to practice multiplayer flow, session creation, joining, lobby handling, server travel, replicated gameplay, countdown logic, and win/loss handling.



\---



\## Project Overview



This is a simple multiplayer racing game where one player hosts a Steam listen-server session and other players can join. The host is also a player in the race.



The game starts from a menu map where players can either create a session or join an existing session. When a session is created, the host moves to a lobby map with their car. Other players can join the lobby, see the session/lobby information, and wait before the race begins.



After the required number of players join, the game travels to the race map. A countdown starts, then players can drive their cars and race to the finish line. When one player wins, the winner receives a “YOU WON!” message and all other players receive a “YOU LOST!” message. Player input is disabled after the race ends.



\---



\## Features



\- Unreal Engine 5.6 project

\- Steam multiplayer session creation

\- Steam session joining

\- Listen server multiplayer setup

\- Host is also a player

\- Start menu with:

&#x20; - Start Game button

&#x20; - Join Game button

&#x20; - Player count selection

\- Default player count is 2

\- Supports player count selection such as 3, 4, and 5

\- Lobby map with spawned player cars

\- Players can drive around in the lobby while waiting

\- Lobby banner showing the session ID

\- Server travel from lobby map to race map

\- Race countdown before movement starts

\- Vehicle movement using:

&#x20; - W = accelerate

&#x20; - S = reverse/brake

&#x20; - A/D = steer

&#x20; - Spacebar = handbrake

\- Mouse-controlled third-person vehicle camera

\- Finish-line win trigger

\- Winner announcement UI

\- Loser announcement UI

\- Input disables after race result



\---



\## Multiplayer Flow



```text

Start Map

↓

Start Game / Join Game

↓

Create or join Steam session

↓

Lobby Map

↓

Players wait and drive around

↓

Required players join

↓

Server travels to Race Map

↓

5-second countdown

↓

Race starts

↓

First player reaches finish trigger

↓

Winner sees "YOU WON!"

↓

Other players see "YOU LOST!"

↓

Inputs disabled

