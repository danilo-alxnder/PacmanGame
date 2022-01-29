#ifndef PACMAN_H
#define PACMAN_H

#define STARTX 7
#define STARTY 21

#include "GameObjectStruct.h"
#include "Character.h"
#include <vector>
#include <algorithm>

  std::vector<std::vector<int>> map = {{
        #include "board.def"
    }};
    UI ui(map);

class Pacman : public Character
{
  public: 

    GameObjectStruct pacman;
          
    Pacman()
    {
         pacman.x = STARTX;
        pacman.y = STARTY;
        pacman.type = PACMAN;
        pacman.dir = RIGHT;
        objects.push_back(pacman);
        }
    void movements(){
    
// Update Pacman's movements
      std::vector<GameObjectStruct>::iterator p; // pointer to PACMAN, objects      
      for (p = objects.begin(); p != objects.end(); p++)
        if (p->type == PACMAN) {
            // std::cout<<"Found pacman at "<<p->x<<" and "<<p->y<<std::endl;
            break;
        }
    //Move Pacman  RIGHT, LEFT, UP, DOWN
    switch (p->dir) 
    {
        case RIGHT:
            if (p->x < static_cast<int>(map.size()) && map[p->y][p->x + 1] == 0) // if didn't reach rightmost cell
            {
                p->x++;
            } else if (p->x == static_cast<int>(map.size())) // if it reaches rightmost cell before tunnel              
            {
                p->x = 0;
            }
            break;

        case LEFT:
            if ( !(p->x==0) && p->x > 0 && map[p->y][p->x - 1] == 0) // if didn't reach leftmost cell
            {
                p->x--;
            } else if (p->x == 0) // if reached the leftmost cell before tunnel
            {
                p->x = map.size();
            }
            break;
        case UP:
            if (map[p->y - 1][p->x] == 0) {
                p->y--;
            }
            break;
        case DOWN:
            if (map[p->y + 1][p->x] == 0) {
                p->y++;
            }
            break;
    }

};
  





  private:

};

#endif