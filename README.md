# SpaceInflators 2D Video Game

## Background
SpaceInflators is a 2D video game similar to the classic arcade game "Dig-Dug". The game consists of a
playable grid, which is populated by several game objects (or actors). The user plays as the Player 
Spaceship, who has been tasked with defending the planet Earth against a "Space Inflators" onslaught. The
game consists of an infinite stream of levels (or rounds) in which the player advances to the next round
by killing a specific number of enemy spaceships. As the game progresses, the rounds get increasingly more
difficult as there is a larger number of enemy ships present, and they fire more frequently.

As the player spaceship, you are allowed to shoot regular missles (in which you have unlimited ammo), or
torpedoes (which can be picked up occassionally from fallen enemy ships who drop them). The player 
spaceship can move up, down, left, and right. There are three types of enemy spaceships: Nachlings, Wealthy
Nachlings, and Smallbots. Each of them move in different patterns around the screen, and do different
operations depending on their programmed algorithms. Occassionally, the Wealthy Nachlings and Smallbots
drop "Goodies", which can be picked up by the player spaceship to add torpedoes, gain enery (health
points), and in the very rare occurrence, an extra live.

The player starts out with three lives, and every so often is granted an extra live depending on the 
overall game score. The objective for the player is to get as much points as possible. Good luck! And 
defend Planet Earth!

For this project, I implemented all of the backend code (i.e. instantiation of the world, and 
implementation of all of the actors within the game). The graphics and audio were configured by Professor
Smallberg (with use of the GLUT and irrKlang Libraries). 
game, but exploit the game engine and graphics APIs provided by
the Professor (displayed using XQuartz).

## Built With
* IDE: Xcode
* Language: C++
* Display Program: XQuartz
* Audio Library: irrKlang
