#ifndef C2GRAPHICS_H
#define C2GRAPHICS_H 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <unordered_set>
#include <algorithm>

#include "c2Math.h"
#include "c2World.h"
#include "c2Cell.h"
#include "c2Collisions.h"

constexpr unsigned int MIN_CELL_LENGTH = 1;
constexpr unsigned int MAX_CELL_LENGTH = 200;
constexpr unsigned int MOUSE_EVENTS = 2;  //does not include 'NONE' as an event
constexpr unsigned int KEY_EVENTS = 6;

typedef enum {
    DEFAULT,
    PRESS,
    HELD,
    RELEASE
} INPUT_TYPE;

typedef enum {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_NONE
} MOUSE_EVENT;

struct c2MouseStateDef
{
    c2MouseStateDef()
    {
        button_states[MOUSE_LEFT] = DEFAULT;
        button_states[MOUSE_RIGHT] = DEFAULT;
        mouse_event = MOUSE_NONE;
        state_change = false;

        xpos = 0;
        ypos = 0;
        xpos_grid = 0;
        ypos_grid = 0;

        xoffset = 0;
        yoffset = 0;
    }

    nBitArray<2, MOUSE_EVENTS> button_states; //2 keys, 4 states each
    MOUSE_EVENT mouse_event; //records most recent event type
    bool state_change; //true when mouse pressed or released

    float xpos, ypos; //cursor position (pixel coords)
    float xpos_buffer[16] = {0}; //buffers for user to store mouse values in if needed
    float ypos_buffer[16] = {0};

    float xpos_grid, ypos_grid; //cursor position (world coords)
    float xoffset, yoffset; //scroll wheel changes (standard offset)
};

typedef enum {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_Q,
    KEY_E,
    KEY_NONE
} KEY_EVENT;

struct c2KeyStateDef
{
    c2KeyStateDef()
    {
        for(int i = 0; i < KEY_EVENTS; i++) 
            button_states[i] = DEFAULT;
        key_event = KEY_NONE;
        state_change = false;
    }

    nBitArray<2, KEY_EVENTS> button_states;
    KEY_EVENT key_event;
    bool state_change;
};

struct c2GraphicsDef //allow options to easily be implemented
{
    c2GraphicsDef(c2World* world, c2QuadTree* tree, void (*callback)(c2GraphicsDef&)) 
    {
        this->world = world;
        this->tree = tree;
       
        follow = nullptr;
        px = 10;
        position.Set(static_cast<float>(world->GetLength())/2, 
                     static_cast<float>(world->GetHeight())/2);

        speed_mult = 1;

        window_title = "Hello World";        
        window_width = 1400;
        window_height = 1000;

        mouse_state = new c2MouseStateDef();
        key_state = new c2KeyStateDef();
        user_callback = callback;
    }

    c2World* world;
    c2QuadTree* tree;
    
    //Camera settings
    c2Cell* follow; //translation
    c2Vector position; //also translation; given as worlds coords
    int px; //size of the cells in pixels;
    
    //Misc user settings
    float speed_mult;

    //relating to the window itself
    const char* window_title;
    int window_width, window_height;

    c2MouseStateDef* mouse_state;
    c2KeyStateDef* key_state;
    void (*user_callback)(c2GraphicsDef& settings);
};

//Display the world given view settings
//NOTE: Change to list in the future for multiple worlds.
int c2DisplayWorld(c2GraphicsDef& settings);

#endif /* C2GRAPHICS_H */
