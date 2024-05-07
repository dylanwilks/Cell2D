#include "c2GUI.h"

c2GUI::c2GUI(c2GraphicsDef& settings, GLFWwindow* window)
  : settings(settings), window(window)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
}

c2GUI::~c2GUI()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

int c2GUI::z_value()
{
    return z;
}

void c2GUI::set_z_value(c2GraphicsGUI gui)
{
    z = static_cast<float>(std::distance(z_order_gui, 
          std::find(z_order_gui, 
            z_order_gui + sizeof(z_order_gui)/sizeof(z_order_gui[0]), gui)));
}
