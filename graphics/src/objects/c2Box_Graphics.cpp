#include "c2World_Graphics.h"

//c2Box_Display child class
c2Box_Graphics::c2Box_Graphics(c2GraphicsDef& settings, GLFWwindow* window) 
  : c2World_Graphics(settings, window)
{
    set_z_value(BOX);

    glGenBuffers(1, &EBO);

    float thickness = 1.0f;

    float box_vertices[] = {
        /*0*/ -0.5f-thickness, -0.5f-thickness, z,
        /*1*/ settings.world->GetLength()-0.5+thickness, -0.5f-thickness, z,
        /*2*/ settings.world->GetLength()-0.5+thickness, settings.world->GetHeight()-0.5+thickness, z,
        /*3*/ -0.5f-thickness, settings.world->GetHeight()-0.5+thickness, z,
        /*4*/ -0.5f, -0.5f, z,
        /*5*/ settings.world->GetLength()-0.5, -0.5f, z,
        /*6*/ settings.world->GetLength()-0.5, settings.world->GetHeight()-0.5, z,
        /*7*/ -0.5f, settings.world->GetHeight()-0.5, z,
    };

    unsigned int box_indices[] = {
        0, 4, 5, //bottom box
        0, 1, 5,

        0, 3, 7, //left box
        0, 4, 7,

        3, 7, 6, //top box
        3, 2, 6,

        2, 1, 5, //right box
        2, 6, 5
    };

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), &box_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), &box_indices[0],
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void*)0);
    glVertexAttrib3f(1, 0.1f, 0.1f, 0.1f);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

c2Box_Graphics::~c2Box_Graphics()
{
    glDeleteBuffers(1, &EBO);
}

void c2Box_Graphics::Draw()
{
    glBindVertexArray(VAO); 
    glVertexAttrib3f(1, 0.1f, 0.1f, 0.1f);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
