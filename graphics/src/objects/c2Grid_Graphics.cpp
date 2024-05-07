#include "c2World_Graphics.h"

//c2Grid_Display child class
c2Grid_Graphics::c2Grid_Graphics(c2GraphicsDef& settings, GLFWwindow* window) 
  : c2World_Graphics(settings, window)
{
    set_z_value(GRID);

    glGenBuffers(1, &EBO);
}

c2Grid_Graphics::~c2Grid_Graphics()
{
    glDeleteBuffers(1, &EBO);
}

void c2Grid_Graphics::Draw()
{
    if(settings.px < 4) { return; }  

    //Figure out number of grids in each direction
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int x_total = round(width/settings.px);
    int y_total = round(height/settings.px);

    int filter_xright = (settings.world->GetLength()) - floor(settings.position.x);
    int filter_xleft = floor(settings.position.x - 1);

    int filter_ytop = (settings.world->GetHeight()) - floor(settings.position.y);
    int filter_ybottom = floor(settings.position.y - 1);

    int right_total = std::min(x_total/2 + 2, filter_xright);
    int left_total = std::min(x_total/2 + 2, filter_xleft);
    int top_total = std::min(y_total/2 + 2, filter_ytop);
    int bottom_total = std::min(y_total/2 + 2, filter_ybottom);

    int grid_count = (right_total + left_total + 1)
                   * (top_total + bottom_total + 1);
    
    //Set grid vertices/indices
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec4> indices;

    c2Vector pos = settings.position;
    int start_x = floor(pos.x) - left_total - 1;
    int start_y = floor(pos.y) - bottom_total - 1;

    int visible_edge_x = right_total < filter_xright ? 3 : 0;
    int visible_edge_y = top_total < filter_ytop ? 3 : 0;
    int x_itr = left_total + right_total + 1 + visible_edge_x;
    int y_itr = top_total + bottom_total + 1 + visible_edge_y;

    for(int i = 0; i <= y_itr; i++) {
        for(int j = 0; j <= x_itr; j++) {
            float x = (start_x - 0.5 + j);
            float y = (start_y - 0.5 + i);
            float z = z;

            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for(int i = 0; i < y_itr; i++) {
        for(int j = 0; j < x_itr; j++) {
            int row1 = i*(x_itr+1);
            int row2 = (i+1)*(x_itr+1);

            indices.push_back(glm::uvec4(row1+j, row1+j+1, //bottom line
                                         row1+j+1, row2+j+1)); //right line
            indices.push_back(glm::uvec4(row2+j, row2+j+1, //top line
                                         row2+j, row1+j)); //left line
        }
    }
    
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        glm::value_ptr(vertices[0]), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4),
        glm::value_ptr(indices[0]), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if(settings.px < 7)
        glVertexAttrib3f(1, 0.02f * (settings.px-3), 
                            0.02f * (settings.px-3), 
                            0.02f * (settings.px-3));
    else
        glVertexAttrib3f(1, 0.08f, 0.08f, 0.08f);

    glDrawElements(GL_LINES, indices.size()*4, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
