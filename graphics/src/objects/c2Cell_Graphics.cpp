#include "c2World_Graphics.h"

//c2Cell_Display child class
c2Cell_Graphics::c2Cell_Graphics(c2GraphicsDef& settings, GLFWwindow* window) 
  : c2World_Graphics(settings, window) 
{
    set_z_value(CELL);
} 

void c2Cell_Graphics::Draw()
{
    std::vector<c2Cell*> coords = visible_cells(settings);
    float vertices[coords.size() * 6]; //no need to floor; shader can truncate

    for(int i = 0; i < coords.size(); i++) {
        vertices[i*6] = floor(coords[i]->GetPosition().x);
        vertices[i*6 + 1] = floor(coords[i]->GetPosition().y);
        vertices[i*6 + 2] = z;
        vertices[i*6 + 3] = coords[i]->GetColor().r;
        vertices[i*6 + 4] = coords[i]->GetColor().g;
        vertices[i*6 + 5] = coords[i]->GetColor().b;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3*sizeof(float)));

    glDrawArrays(GL_POINTS, 0, coords.size());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
