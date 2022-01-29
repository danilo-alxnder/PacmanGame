/// \file
/// GameObjectStruct.h

#ifndef SRC_GAMEOBJECTSTRUCT_H
#define SRC_GAMEOBJECTSTRUCT_H

/// These enum values denote the sprite sets available for items on screen.
enum Type {
    PACMAN,
    BLINKY,
    PINKY,
    INKY,
    CLYDE,
    SCARED,
    SCAREDINV,
    CHERRY,
    STRAWBERRY,
    ORANGE,
    LEMON,
    APPLE,
    GRAPES,
    DOT,
    ENERGIZER,
    WALL,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
};

/// An enum to denote the direction of an item on screen.
enum Direction { UP, DOWN, LEFT, RIGHT };

/// This is the base class and it represents an item that is drawn onto the screen.
struct GameObjectStruct {
	//public:

    /// x-position of the item.
    int x;
    /// y-position of the item.
    int y;
    /// The appearance of the item (sprite set).
    Type type;
    Type notScaredIdentity;
    /// The direction of the item (sprite selection).
    Direction dir;

	/// Constructor for a Game Object
	GameObjectStruct() {};
	GameObjectStruct(int xx, int yy, Type tt, Type notSI, Direction dd)
	{ x=xx; y=yy; type=tt; notScaredIdentity=notSI; dir=dd; }
	
	/// Destructor
	~GameObjectStruct() {};
};

struct PacmanObject : public GameObjectStruct {
	
	// Constructor for PACMAN
	PacmanObject() {};
	PacmanObject(int xx, int yy, Direction dd)
	{
		x=xx; 
		y=yy; 
		type=PACMAN; 
		notScaredIdentity=PACMAN; 
		dir=dd;
	}
	~PacmanObject() {};	
};

struct GhostObject : public GameObjectStruct {
	
	// Constructor for Ghosts
	GhostObject() {};
	GhostObject(int xx, int yy, Type tt)
	{
		x=xx; 
		y=yy; 
		type=tt; 
		notScaredIdentity=tt; 
		dir=RIGHT;
	}
	~GhostObject() {};	
};

struct DotObject : public GameObjectStruct {
	
	// Constructor for normal dots
	DotObject() {};
	DotObject(int xx, int yy)
	{
		x=xx;
		y=yy; 
		type=DOT; 
		notScaredIdentity=DOT; 
		dir=UP;
	}
	~DotObject() {};	
};

struct SuperDotObject : public GameObjectStruct {
	
	// Constructor for superdots
	SuperDotObject() {};
	SuperDotObject(int xx, int yy)
	{
		x=xx; 
		y=yy; 
		type=ENERGIZER; 
		notScaredIdentity=ENERGIZER; 
		dir=UP;
	}
	~SuperDotObject() {};	
};	


#endif // SRC_GAMEOBJECTSTRUCT_H
