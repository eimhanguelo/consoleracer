
How the Game Works

The Car Console Game operates within a Windows console environment and is designed to provide players with an interactive experience centered around maneuvering a car while avoiding enemy vehicles. 
The following sections detail the game’s mechanics, components, and flow:

Game Structure
Initialization:
When the game starts, it initializes the console and sets the cursor visibility for better graphics rendering. The main menu is displayed, 
allowing players to choose to start the game, view instructions, or exit.
Main Menu:

Players can navigate the main menu, where they select their desired action. Upon selecting “Start Game,” the game loop begins.
Core Gameplay Mechanics

Game Loop:
The game operates in a continuous loop, where the following actions occur:
Player input is captured to control the car’s movement.
The positions of the enemy cars are updated.
Collision detection is performed to check if the player’s car collides with any enemy car.
The score is updated based on the player’s actions.

Player Controls:
The player controls the car using keyboard inputs:
W: Move Up
A: Move Left
S: Move Down
D: Move Right
Esc: Exit the game
The player’s car is confined to the game area defined by screen boundaries to ensure it does not move off-screen.

Car and Enemy Classes:
The game employs object-oriented programming with classes representing key components:

Car Class: Manages the player's car, including its position, drawing, and erasing from the screen. It also handles movement based on player input.
Enemy Class: Manages enemy cars that spawn at the top of the screen and move downwards. It contains methods for rendering, erasing, and checking if an enemy has gone off-screen.

Collision Detection:
The game checks for collisions between the player’s car and active enemy cars. If a collision is detected (i.e., their coordinates overlap), the game transitions to a “Game Over” state.

Score Tracking:
Players earn points for each enemy car they successfully avoid. The score is displayed on the console, providing immediate feedback on the player's performance.

Game Over:
When a collision occurs, the game clears the screen and displays a "Game Over" message, along with the final score and an option to return to the main menu.

The Car Console Game combines simple mechanics and object-oriented principles to create an engaging gameplay experience.
It is designed to be straightforward while still providing the challenge of avoiding enemies and managing the player's car.
The use of Windows API functions allows for smooth console interactions, and the project serves as a foundational example of game development concepts in C++.
