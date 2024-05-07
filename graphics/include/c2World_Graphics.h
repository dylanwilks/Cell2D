#ifndef C2WORLD_BUFFERS_H
#define C2WORLD_BUFFERS_H 

#include <vector>
#include <unordered_set>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "c2Math.h"
#include "c2World.h"
#include "c2Cell.h"
#include "c2Collisions.h"
#include "c2Functions.h"

struct c2GraphicsDef;

enum c2GraphicsObj {
    CELL,
    BOX,
    GRID
};

//Order of appearance; front-to-back
static c2GraphicsObj z_order_objs[] = {CELL, BOX, GRID};

class c2World_Graphics
{
    public:
      //Construct buffers based on settings
      c2World_Graphics(c2GraphicsDef& settings, GLFWwindow* window);

      //Clear the objects
      virtual ~c2World_Graphics();

      //Updates the VBO and changes active VAO to this one.
      virtual void Draw() = 0;

      //Retrieve the z axis value of the object
      int z_value();

    protected:
      unsigned int VBO, VAO;
      c2GraphicsDef& settings;
      GLFWwindow* window;
      
      //Set the z value of the object
      void set_z_value(c2GraphicsObj obj);
      int z;
};


class c2Cell_Graphics : public c2World_Graphics
{
    public:
      c2Cell_Graphics(c2GraphicsDef& settings, GLFWwindow* window);

      void Draw() override;
};


class c2Box_Graphics : public c2World_Graphics
{
    public:
      c2Box_Graphics(c2GraphicsDef& settings, GLFWwindow* window);
      ~c2Box_Graphics();

      void Draw() override;
    private:
      unsigned int EBO;
};


class c2Grid_Graphics : public c2World_Graphics
{
    public:
      c2Grid_Graphics(c2GraphicsDef& settings, GLFWwindow* window);
      ~c2Grid_Graphics();

      void Draw() override;
    private:
      unsigned int EBO;
};

#endif /* C2WORLD_BUFFERS_H */
