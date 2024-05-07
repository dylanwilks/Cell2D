#include "c2World_Graphics.h"

//c2World_Display superclass
c2World_Graphics::c2World_Graphics(c2GraphicsDef& settings, GLFWwindow* window) 
  : settings(settings), window(window)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
}

c2World_Graphics::~c2World_Graphics()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int c2World_Graphics::z_value()
{
    return z;
}

void c2World_Graphics::set_z_value(c2GraphicsObj obj)
{
    z = static_cast<float>(std::distance(z_order_objs, 
          std::find(z_order_objs, 
            z_order_objs + sizeof(z_order_objs)/sizeof(z_order_objs[0]), obj)));
}
