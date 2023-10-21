#!/bin/bash  

g++ -c -o Files_O/Game.o Game.cpp
g++ -c -o Files_O/Spaceship.o Spaceship.cpp
g++ -c -o Files_O/Bullet.o Bullet.cpp
g++ -c -o Files_O/Enemy.o Enemy.cpp
g++ -c -o Files_O/Invaders.o Invaders.cpp
g++ -c -o Files_O/Alpha.o Alpha.cpp
g++ -c -o Files_O/Beta.o Beta.cpp
g++ -c -o Files_O/Gamma.o Gamma.cpp
g++ -c -o Files_O/Monster.o Monster.cpp
g++ -c -o Files_O/Dragon.o Dragon.cpp
g++ -c -o Files_O/Mothership.o Mothership.cpp
g++ -c -o Files_O/Bomb.o Bomb.cpp
g++ -c -o Files_O/Menu.o Menu.cpp
g++ -c -o Files_O/HUD.o HUD.cpp
g++ -c -o Files_O/Levels.o Levels.cpp
g++ -c -o Files_O/Main.o Main.cpp

g++ -o sfml-app Files_O/Main.o Files_O/Game.o Files_O/Spaceship.o Files_O/Bullet.o Files_O/Enemy.o Files_O/Invaders.o Files_O/Alpha.o Files_O/Beta.o Files_O/Gamma.o Files_O/Monster.o Files_O/Dragon.o Files_O/Mothership.o Files_O/Bomb.o Files_O/Menu.o Files_O/HUD.o Files_O/Levels.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
