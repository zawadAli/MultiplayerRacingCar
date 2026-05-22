# UE5 Steam Multiplayer Racing Prototype

A multiplayer racing prototype built in **Unreal Engine 5.6**, focused on learning multiplayer logic using the **Steam Online Subsystem** with a **listen server** setup.

The project focuses on session creation, joining, lobby flow, server travel, replicated countdown, vehicle spawning, and win/loss handling.

## Project Overview

This is a simple Steam multiplayer racing game where one player hosts a listen-server session and also participates as a player. Other players can join the session, wait in the lobby, and then travel together to the race map.

The game starts from a menu map with **Start Game** and **Join Game** options. The host can select the number of players before creating the session. After creating the session, the host moves to the lobby map with their car. Joining players also spawn in the lobby, where everyone can drive around while waiting.

Once the required players have joined, the server travels everyone to the race map. A 5-second countdown starts, then players race to the finish line. The first player to reach the finish trigger wins. The winner sees **"YOU WON!"**, while all other players see **"YOU LOST!"**. After the result appears, player input is disabled.

## Features

- Steam session creation and joining
- Listen server multiplayer
- Host also acts as a player
- Start menu with Start Game and Join Game options
- Player count selection, defaulting to 2 players
- Support for 3, 4, and 5 player sessions
- Lobby map with spawned player cars
- Lobby banner showing the session ID
- Players can drive around while waiting in the lobby
- Server travel from lobby map to race map
- 5-second countdown before the race starts
- Vehicle driving controls
- Mouse-controlled third-person vehicle camera
- Finish-line win trigger
- Winner and loser announcement UI
- Input disabled after race result

## Multiplayer Flow

```text
Start Map
├── Start Game
│   ├── Select number of players
│   ├── Create Steam session
│   └── Travel to Lobby Map
│
└── Join Game
    ├── Search available sessions
    └── Join Lobby Map

Lobby Map
├── Host and clients spawn with cars
├── Players can drive around while waiting
├── Session ID is displayed on lobby banner
└── Server travels everyone to Race Map

Race Map
├── 5-second countdown
├── Race starts
├── Players drive to finish line
├── First player reaching finish trigger wins
├── Winner sees "YOU WON!"
├── Others see "YOU LOST!"
└── Player input is disabled
```

## Controls

| Input | Action |
|---|---|
| W | Accelerate |
| S | Brake / Reverse |
| A | Steer Left |
| D | Steer Right |
| Spacebar | Handbrake |
| Mouse Movement | Rotate third-person camera |

## Technical Focus

This project was created to practice multiplayer programming in Unreal Engine, especially:

- Steam Online Subsystem setup
- Session creation
- Session searching and joining
- Listen server flow
- Lobby flow
- Server travel
- Multiplayer player spawning
- Replicated countdown logic
- Finish trigger handling
- NetMulticast RPC usage
- Win/loss result handling
- Disabling input after the race ends

## Engine Version

Unreal Engine 5.6

## Multiplayer Type

- Steam Online Subsystem
- Listen Server

## Project Structure

```text
Config/
Content/
Source/
MultiplayerRacingCar.uproject
```

Generated folders such as `Binaries`, `Intermediate`, `Saved`, `DerivedDataCache`, and `.vs` are ignored using `.gitignore`.

## Notes

This is a learning/prototype project. The main focus is multiplayer logic rather than final racing mechanics, vehicle physics polish, or production-level art.

Packaged builds are not included in this repository. The repository contains only the Unreal project files, source code, assets, and configuration needed for development.