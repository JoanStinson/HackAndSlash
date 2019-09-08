# Hack and Slash Game
A simple hack and slash game inspired in "Hyper Light Drifter" by Heart Machine (2016). The proyect was made using the library SDL 
and C++ with some design patterns and doxygen documentation. 
Game created by [Joan Ginard Mateo](https://github.com/JoanStinson).
Repository can be found [here](https://github.com/JoanStinson/Hack_and_Slash).
Art created by [Matthew Carr](http://www.2hitstudio.com) and music by [Ryan Beveridge](https://soundcloud.com/ryan-beveridge).

![](Minions.gif)

## Design Patterns Implemented
- #### Singleton
This pattern has been implemented for the game managers/controllers in order to have only 1 instance of that class to prevent caos. All 
the necessary constructors and operators haven been deleted for this singleton classes to prevent having more than 1 copy (which also
disallows instantiation outside of the class). The singleton instance of each class is a static one.

- #### State  
The state pattern has been used to define the behavior of the different AI agents, depending on which state the enemy is in, it will 
have a different behavior and animation. It's a very effective solution to handle multiple behaviors in an organized way.

- #### MVC  
The Model-View-Controller pattern I find a very useful strategy to have decoupled code that you can reutilize for future projects and
to have an organized code structure (very necessary for scalability). What I did is basically divide the code (classes) in the ones
that correspond to the Model (which would be the game logic such as 'Player.h', 'Boss.h', etc.), the ones that correspond to the View (which refers to the visuals such as the 'Renderer.h' or 'Window.h') and finally the ones that correspond to the Controller (which serves as a translator to the user's input such as 'InputManager.h'). The main purpose is to have as much decoupled logic as possible, so that if I took one class and re-impoted it in a new project it would compile without any dependencies (the less the better). Obviously, for the game to work there need to be some dependencies, but minimize them and organize the code in this 3 blocs to make it work.

- #### Object Pooling
Object pooling has been necessary for the bullets that shoots the Boss (which aren't few). The key of this pattern is that instead
of creating a new object every time you need it and when you don't need it anymore destroying it, is to have a default number of 
that object and instead of making a new one, you active one and when you would delete it, instead you deactive it. This way you don't cause any memory problems and it's way more efficient since you have the total number of objects already created in the beginning. Only delete the pool when you don't need it anymore (in my case when the boss dies). When activating again a bullet what I do is call a 'Reset()' method which acts similar to a constructor to activate that bullet and place it correctly again, etc.

## The Boss
![](Boss.gif)
