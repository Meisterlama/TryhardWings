# TryhardWings
_Made by Baptiste SUZON and Antonin GALLIOT_

TryhardWings is a game based on the game "Tiny Wings" and is a school project.
In this game you'll have to reach the sky and try to go higher and higher.
The only way to go higher is to correctly use the slopes to firstly get speed.

##Development Objectives

Our objectives for this game were to make a game based on the generation of curves with our own mathematical tools.
We had to use derivatives to get useful information and to generate playable curves.
There are a few of different functions implemented (sinusoidal, polynomial, elliptic and hyperbolic tangent).
We practiced our shader skills too, are you going to find the core?

##How to play
In order to play our game, you can check [on itch](https://meisterlama.itch.io/try-hard-wings).
We made it downloadable but it still is playable in HTML, even with your phone!


##Bindings
You'll firstly be able too see the bindings directly in-game!

- __F1__ : Opens the help menu; this menu lets you change the curves and will show the bindings too.
- __A/-/left arrow__ : Increases your speed
- __D/+/right arrow__ : Decreases your speed

##More information

Our main code classes are:
- Application - runs the application
- Block - represents one block of the map 
- BlockList - the map (array of blocks)
- Player - contains some information about the player to be able to play

Our Implementation's limits were mainly the zoom in/out as we only have an horizontal one yet and makes us having a bad overall vision of the curves.
Tweaking the game was really hard and that's one point we could really improve for possible next updates.


##Known bugs
There are currently no known bugs!
