/// \file
/// main.cpp

//============================================================== 
// Filename : Pacman main source file
// Authors : Danilo Toapanta s2074036
// Version : 1
// License : University of Twente
// Description : Mimic of the popular pacman game
////=============================================================

#define STARTX 7
#define STARTY 21
#define SCAREDTIMEOUT 40

#include "GameObjectStruct.h"
#include "UI.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

//Function protoypes
void makeScared();
void makeNotScared();
void initializer(int );
enum Type original_type;

// Globally defined variables

std::vector<std::vector<int>> map = {{                  //board definition
    #include "../include/board.def" 
}};

std::vector<GameObjectStruct> objects = {};             //vector to hold all objects
UI ui(map);                                             // <-- use map from your game objects

std::vector<GameObjectStruct>::iterator j;              // pointer to ghosts

int score = 0, lives = 3, timeout = 0, option = 0;
bool reset = false, activate = false;

/// Callback function to update the game state.
///
/// This function is called by an SDL timer at regular intervals.
/// Note that this callback may happen on a different thread than the main
/// thread. You therefore have to be careful in avoiding data races. For
/// example, you should use mutexes to access shared data. Read the
/// documentation of SDL_AddTimer for more information and for tips regarding
/// multithreading issues.

Uint32 gameUpdate(Uint32 interval, void * /*param*/)
{
    //activate timeout when ghosts are scared
    if (activate)                           
    {
        timeout++;
    
        if (timeout >= SCAREDTIMEOUT) 
        {
            makeNotScared();
            timeout =0;
            activate= false;    //deactivate timeout
        }
    }
     
    std::vector<GameObjectStruct>::iterator p, i, monster; // pointer to pacman and objects      
    
    // Update Pacman's movements
    for (p = objects.begin(); p != objects.end(); p++)
        if (p->type == PACMAN) 
            break;
        
    //Move Pacman  RIGHT, LEFT, UP, DOWN
    switch (p->dir) 
    {
        case RIGHT:
            if (p->x < static_cast<int>(map.size()) && map[p->y][p->x + 1] == 0) // if didn't reach rightmost cell
                p->x++;
            else if (p->x == static_cast<int>(map.size()))                       // if it reaches rightmost cell before tunnel              
                p->x = 0;
            
            break;

        case LEFT:
            if ( !(p->x==0) && p->x > 0 && map[p->y][p->x - 1] == 0)             // if didn't reach leftmost cell
                p->x--;
            else if (p->x == 0)                                                  // if reached the leftmost cell before tunnel
                p->x = map.size();
            
            break;

        case UP:        
            if (map[p->y - 1][p->x] == 0)                                       // if legal move to go up 
                p->y--; 
            
            break;

        case DOWN:
            if (map[p->y + 1][p->x] == 0)                                       // if legal move to go down  
                p->y++;
            
            break;
    }

    // Manage eating dots and collisions
    for (i = objects.begin(); i < objects.end(); i++) 
    {
        // find PACMAN in objects vector
        for (p = objects.begin(); p != objects.end(); p++)
            if (p->type == PACMAN) 
                break;
         
        //collisions 
        if ((i->type != PACMAN) && (i->x == p->x && i->y == p->y)) 
        {
            switch (i->type) 
            {
                case DOT:
                    objects.erase(i);               // delete dot from board
                    i = objects.begin();
                    score = ui.getScore() + 10;
                    break; 

                case INKY: 
                    lives--;   
                    reset = true;
                    break;

                case BLINKY:
                    lives--;   
                    reset = true;
                    break;

                case CLYDE:
                    lives--;   
                    reset = true;       
                    break;

                case PINKY:
                    lives--;   
                    reset = true;
                    break;

                case SCARED:
					original_type=i->notScaredIdentity;
                    objects.erase(i);           // delete scared from board
                    i = objects.begin();
                    score = ui.getScore() + 10;
                   
                   // Initialize a ghost from the type that was eaten
					if (original_type == BLINKY) 
						initializer(0);
					if (original_type == INKY) 
						initializer(1);
					if (original_type == CLYDE) 
						initializer(2);
					if (original_type == PINKY) 
						initializer(3);

                    break;

                case ENERGIZER:
                    objects.erase(i);         // delete energizer from board
                    i = objects.begin();
                    score = ui.getScore() + 50;
                    makeScared();
                    break;
            }

            if (reset)
                break;
        }
    }
   
    /// Update ghost movements
    for (monster = objects.begin(); monster != objects.end(); ++monster) 
    {
        if (monster->type == INKY || monster->type == BLINKY || 
            monster->type == CLYDE || monster->type == PINKY ||  
            monster->type == SCARED) 
        {
            //declare a vector for available directions
            int numberOfOpenDirections = 0;
            std::vector<Direction> availableDir = {};

            //Add available direction + number of open directions if next cell is open
            if (!(monster->x == 27 && monster->y == 13) &&
                map[monster->y][monster->x + 1] == 0) // if right cell is open
            {
                numberOfOpenDirections++;
                availableDir.push_back(RIGHT);
            }

            if (!(monster->x == 0 && monster->y == 13) &&
                map[monster->y][monster->x - 1] == 0) // if left cell is open
            {
                numberOfOpenDirections++;
                availableDir.push_back(LEFT);
            }

            if (map[monster->y - 1][monster->x] == 0) // if upper cell is open
            {
                numberOfOpenDirections++;
                availableDir.push_back(UP);
            }

            if (map[monster->y + 1][monster->x] == 0) // if below cell is open
            {
                numberOfOpenDirections++;
                availableDir.push_back(DOWN);
            }

            ////Make probability
            // Select a random direction from the available ones
            std::vector<Direction>::iterator randIt = availableDir.begin();
            std::advance(randIt, std::rand() % availableDir.size());

            // Flip a coin to choose to change direction or not
            int MaxCoins = 10;
            int coinFlip = rand() % MaxCoins + 1;

            // Change the monster movement direction with 1/MaxCoins% chance
            if (coinFlip >= 2 &&        // this makes the chance of continuing in current direction 90%
                std::find(availableDir.begin(), availableDir.end(), monster->dir) != availableDir.end()) //true when not a dead-end 
            {
                *randIt = monster->dir;
            }
            
            // Fix the direction selected by randomization
            monster->dir = *randIt;
            
            switch (*randIt) // continue in the chosen direction for the monster
            {
                case RIGHT:
                    monster->x++;
                    break;
                case LEFT:
                    monster->x--;
                    break;
                case UP:
                    monster->y--;
                    break;
                case DOWN:
                    monster->y++;
                    break;
            }
                
            // Pacman may eat scared monsters
            if (monster->type == SCARED && p->x == monster->x && p->y == monster->y) 
            {
				original_type=monster->notScaredIdentity;
				
				objects.erase(monster); 
				
				// Initialize a ghost from the type that was eaten in jail
				if (original_type == BLINKY) { initializer(0); }
				if (original_type == INKY)   { initializer(1); }
				if (original_type == CLYDE)  { initializer(2); }
				if (original_type == PINKY)  { initializer(3); } 
                break;

            } 
            else if (!(monster->type == SCARED) && p->x == monster->x && p->y == monster->y)
            {
                lives--;
                reset = true;
            } 
        }
    }
    return interval;  
}

void makeScared()
{
    // Iterate over all monster objects and make them scared
        for (j = objects.begin(); j < objects.end(); j++) 
            if (j->type == PINKY || j->type == INKY || j->type == BLINKY || j->type == CLYDE) 
            {
				j->notScaredIdentity=j->type;   
				j->type = SCARED;
            }
 
        // activate timer for scared mode
        activate=true; 
        
        return;
}

void makeNotScared()
{
		// Return their type to normal
        for (j = objects.begin(); j < objects.end(); j++) 
        {
            if (j->notScaredIdentity == PINKY) 
                j->type=PINKY;
            
            if (j->notScaredIdentity == INKY) 
                j->type=INKY;
     
            if (j->notScaredIdentity == BLINKY) 
                j->type=BLINKY;
   
            if (j->notScaredIdentity == CLYDE) 
				j->type=CLYDE;
        }   
        return;
}

void initializer(int x) 
{
    if (x == 0) 
    {
		GhostObject  Blinky(12,13,BLINKY);          //Blinky's iniital position inside chamber
        objects.push_back(Blinky);   
    }
    if (x == 1) 
    {
		GhostObject  Inky(13,13,INKY);              //Inky's iniital position inside chamber
        objects.push_back(Inky);      
    } 
    if (x == 2) 
    {
        GhostObject Clyde(14, 13, CLYDE);           //Clyde's iniital position inside chamber
        objects.push_back(Clyde);
    } 
    if (x == 3) 
    {
		GhostObject  Pinky(15,13,PINKY);            //Pinky's iniital position inside chamber
        objects.push_back(Pinky);
    }
    return;
}

/// Program entry point.
int main(int /*argc*/, char ** /*argv*/)
{
    // Plant dots in empty cells except the middle chamber
    for (unsigned int y = 0; y < map.size(); y++)
        for (unsigned int x = 0; x < map[y].size(); x++) 
        {
            if  (map[y][x] == 1      || (x == STARTX && y == STARTY) ||  // No points in pacman's initial position 
                (x == 14 && y == 13) || (x == 15 && y == 13) ||          //*No points in middle chamber
                (x == 13 && y == 13) || (x == 12 && y == 13) ||          //*
                (x == 13 && y == 12) || (x == 14 && y == 12))            //*
            {
            }
            else if ((x == 1 && y == 1)  || (x == 1 && y == 25) || (x == 26 && y == 1) || (x == 26 && y == 25)) 
            {
					// place energizers
                    SuperDotObject tempSuperdot(x,y); 
                    objects.push_back(tempSuperdot);
            } 
            else 
            {
					// place normal dots
                    DotObject tempdot(x,y);
                    objects.push_back(tempdot);         
            }
        }

    // Release pacman 
    PacmanObject pacman(STARTX,STARTY,LEFT);
    objects.push_back(pacman);

	// Release ghosts 
    for (int ghosts = 0; ghosts < 4; ghosts++)
        initializer(ghosts);

    // Start timer for game update, call this function every 100 ms.
    SDL_TimerID timer_id =
        SDL_AddTimer(130, gameUpdate, static_cast<void *>(nullptr));

    // Call game init code here
    std::vector<GameObjectStruct>::iterator pman;
    bool quit = false;

    while (!quit && lives > 0) 
    {     
        // set timeout to limit frame rate
        Uint32 timeout = SDL_GetTicks() + 20;

        // find PACMAN in objects vector
        for (pman = objects.begin(); pman != objects.end(); ++pman)
            if (pman->type == PACMAN) 
            {
                if (reset) 
                {
                    if (pman != objects.end()) 
                    {
                        pman->x = STARTX;
                        pman->y = STARTY;
                        pman->dir = LEFT;
                        reset = false;
                    } 
                    else
                        break;
                }
                break;
            }

        // Handle the input
        SDL_Event e;
        while (SDL_PollEvent(&e)) 
        {
            // Quit button.
            if (e.type == SDL_QUIT) 
                quit = true;
          
            // All keydown events
            if (e.type == SDL_KEYDOWN) 
            {
                switch (e.key.keysym.sym) 
                {
                case SDLK_LEFT: 
                    pman->dir = LEFT;
                    break;
                case SDLK_RIGHT: 
                    pman->dir = RIGHT;
                    break;
                case SDLK_UP: 
                    pman->dir = UP;
                    break;
                case SDLK_DOWN: 
                    pman->dir = DOWN;
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        // Set the score
        ui.setScore(score); // <-- Pass correct value to the setter

        // Set the amount of lives
        ui.setLives(lives); // <-- Pass correct value to the setter

        // Render the scene
        ui.update(objects);

        while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
            // ... do work until timeout has elapsed
        }
    }

    std::cout << "GAME OVER! \nYOUR SCORE : " << score << std::endl;

    SDL_RemoveTimer(timer_id);

    return 0;
}
