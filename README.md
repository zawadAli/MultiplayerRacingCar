\# UE5 Steam Multiplayer Racing Prototype



A multiplayer racing prototype built in \*\*Unreal Engine 5.6\*\*, focused on learning and implementing multiplayer logic using the \*\*Steam Online Subsystem\*\* with a \*\*listen server\*\* setup.



This project is mainly focused on multiplayer flow, session handling, lobby logic, server travel, replicated countdown, vehicle spawning, and win/loss handling.



\## Project Overview



This is a simple Steam multiplayer racing game where one player hosts a listen-server session and also participates as a player. Other players can join the session, wait in the lobby, and then travel together to the race map.



The game starts from a menu map with two main options: \*\*Start Game\*\* and \*\*Join Game\*\*. The host can select the number of players before creating the session. After the session is created, the host moves to the lobby map with their car. Joining players also spawn in the lobby, where everyone can drive around while waiting for the race to start.



Once the required players have joined, the server travels everyone to the race map. A countdown starts, then players race to the finish line. The first player to reach the finish trigger wins. The winner sees \*\*"YOU WON!"\*\*, while all other players see \*\*"YOU LOST!"\*\*. After the result appears, player input is disabled.



\## Features



\- Steam session creation and joining

\- Listen server multiplayer

\- Host also acts as a player

\- Start menu with Start Game and Join Game options

\- Player count selection, defaulting to 2 players

\- Support for 3, 4, and 5 player sessions

\- Lobby map with spawned player cars

\- Lobby banner showing the session ID

\- Players can drive around while waiting in the lobby

\- Server travel from lobby map to race map

\- 5-second countdown before the race starts

\- Vehicle driving controls

\- Mouse-controlled third-person vehicle camera

\- Finish-line win trigger

\- Winner and loser announcement UI

\- Input disabled after race result



\## Multiplayer Flow



```text

Start Map

&#x20; ├─ Start Game

&#x20; │   ├─ Select number of players

&#x20; │   ├─ Create Steam session

&#x20; │   └─ Travel to Lobby Map

&#x20; │

&#x20; └─ Join Game

&#x20;     ├─ Search available sessions

&#x20;     └─ Join Lobby Map



Lobby Map

&#x20; ├─ Host and clients spawn with cars

&#x20; ├─ Players can drive around while waiting

&#x20; ├─ Session ID is displayed on lobby banner

&#x20; └─ Server travels everyone to Race Map



Race Map

&#x20; ├─ 5-second countdown

&#x20; ├─ Race starts

&#x20; ├─ Players drive to finish line

&#x20; ├─ First player reaching finish trigger wins

&#x20; ├─ Winner sees "YOU WON!"

&#x20; ├─ Others see "YOU LOST!"

&#x20; └─ Player input is disabled

