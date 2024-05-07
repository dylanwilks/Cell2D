//#include "c2Graphics.h"

/* Declarations for functions exclusive to this source file */

//OpenGL polling events/callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Global variables for callbacks
float camera_x, camera_y;
double lastX, lastY;
bool left_click = false;
float sensitivity, px;

//Order of appearance; front-to-back
enum objects {
    CELL,
    BOX,
    GRID
};

objects z_order[] = {CELL, BOX, GRID};
float cell_z = static_cast<float>(std::distance(z_order, 
                                  std::find(z_order,
                                            z_order + sizeof(z_order)/sizeof(z_order[0]), 
                                            CELL)));
float box_z  = static_cast<float>(std::distance(z_order, 
                                  std::find(z_order,
                                            z_order + sizeof(z_order)/sizeof(z_order[0]), 
                                            BOX)));
float grid_z = static_cast<float>(std::distance(z_order, 
                                  std::find(z_order,
                                            z_order + sizeof(z_order)/sizeof(z_order[0]), 
                                            GRID)));

class c2Buffer;
class c2Grid;

//Class/struct declarations (defined below)
class c2Buffer
{
    public:
      //Construct buffers based on settings
      c2Buffer(c2GraphicsDef& settings, GLFWwindow* window);

      //Clear the objects
      ~c2Buffer();

      //Updates the VBO and changes active VAO to this one.
      void DisplayCells(c2Shader& shader);

    private:
      //Fetches cells that'll fit within the display range
      std::vector<c2Cell*> CellDisplay();

      //Captures quadtree nodes that fit within the display
      std::vector<c2QuadTree*> CapturedNodes(c2QuadTree* node, const c2Vector& p1,
                                                               const c2Vector& p2);

      unsigned int VBO, VAO;
      c2GraphicsDef& settings;
      c2Grid* grid;
      GLFWwindow* window;
};

class c2Grid 
{
    public:
      friend class c2Buffer;

    private:
      //Create box and grid objects
      c2Grid(c2GraphicsDef& settings, GLFWwindow* window);

      //Delete the objects
      ~c2Grid();

      //Displays this box/grid
      void DisplayBox();

      //Toggle grid display
      void DisplayGrid();

      unsigned int GRID_VBO, GRID_VAO, GRID_EBO,
                   BOX_VBO, BOX_VAO, BOX_EBO;

      c2GraphicsDef& settings;
      GLFWwindow* window;
};


/* Implementations */
int c2DisplayWorld(c2GraphicsDef& settings)
{  
    //Initialize GLFW and settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    //Create window with context with GLFW
    GLFWwindow* window = glfwCreateWindow(1400, 1000, settings.window_title, NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //Set function pointers for this context
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Set buffers (declare as array in future)
    c2Buffer* world_buffers = new c2Buffer(settings, window);

    //Create shader program (to be loaded using DisplayWorld)
    c2Shader world_shader("graphics/shaders/cell_shader.vs",  //Vertex shader
                          "graphics/shaders/cell_shader.fs"); //Fragment shader

    //Initializing global variables with settings
    camera_x = settings.position.x;
    camera_y = settings.position.y;

    sensitivity = settings.sensitivity;
    px = settings.px;

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST); //for grid
    glDepthFunc(GL_LESS);

    //Render/message loop 
    while(!glfwWindowShouldClose(window))
    {
        //Query/poll for inputs first
        processInput(window);
        
        //Set background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //(?) Allow key input to change world buffer and shader
        world_buffers->DisplayCells(world_shader);

        /* Update ticksPerFrame times. If ticksPerFrame = 1/time_step, then 
         * velocity units are cells per frame.                              */
        for(int i = 0; i < settings.ticksPerFrame; i++)
            settings.world->Step(settings.tree);

        //Polling stuff
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete world_buffers; //free the objects
    glfwTerminate();

    return 0;
}


/*Callback implementations*/

//Callback function for whenever window gets resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//Handle user inputs in message loop
void processInput(GLFWwindow* window/*, c2Vector& topleft, c2Vector& bottomright*/)
{
    //Input stuff like scrollwheel
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwGetCursorPos(window, &lastX, &lastY);
        left_click = true;
    }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        left_click = false;
    }

    if(left_click) {
        float xoffset = (xpos - lastX) * sensitivity * 10/px;
        float yoffset = (ypos - lastY) * sensitivity * 10/px;
        
        camera_x += xoffset;
        camera_y -= yoffset;
    }
}

//On (quick) click, find grid position by finding quadrant and checking cells
//in there. If cell is clicked, create a text box on top left detailing
//cell information using a class
void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    int width, height;
    double xpos, ypos;
    glfwGetWindowSize(window, &width, &height);
    glfwGetCursorPos(window, &xpos, &ypos);

    float px_t = (float)yoffset * px/10 + px;

    if(yoffset > 0) {
        float world_xpos = (xpos - width/2)/px + camera_x;
        float world_ypos = -(ypos - height/2)/px + camera_y;

        float old_width = width/px;
        float old_height = height/px;

        float new_width = width/px_t;
        float new_height = height/px_t;

        float dx = (old_width/2 - abs(world_xpos - camera_x))*px/px_t;
        float dy = (old_height/2 - abs(world_ypos - camera_y))*px/px_t;

        float s1, s2;
        if(world_xpos - camera_x >= 0) {
            s1 = (world_xpos - camera_x) - abs(new_width/2 - dx);
        } else {
            s1 = (world_xpos - camera_x) + abs(new_width/2 - dx);
        }

        if(world_ypos - camera_y >= 0) {
            s2 = (world_ypos - camera_y) - abs(new_height/2 - dy);
        } else {
            s2 = (world_ypos - camera_y) + abs(new_height/2 - dy);
        }

        camera_x += s1;
        camera_y += s2;
    }

    px += (float)yoffset * px/10;
    if(px < MIN_CELL_LENGTH)
        px = MIN_CELL_LENGTH;
    if(px > MAX_CELL_LENGTH)
        px = MAX_CELL_LENGTH;
}


/* c2Buffers implementation */
c2Buffer::c2Buffer(c2GraphicsDef& settings, GLFWwindow* window)
                  :settings(settings), 
   window(window)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    grid = new c2Grid(settings, window);
}

c2Buffer::~c2Buffer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete grid;
}

void c2Buffer::DisplayCells(c2Shader& shader)
{
    shader.LoadShader();

    //Setting up buffers
    std::vector<c2Cell*> coords = CellDisplay();
    float vertices[coords.size() * 6]; //no need to floor; shader can truncate

    for(int i = 0; i < coords.size(); i++) {
        vertices[i*6] = floor(coords[i]->GetPosition().x);
        vertices[i*6 + 1] = floor(coords[i]->GetPosition().y);
        vertices[i*6 + 2] = cell_z;
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

    //Setting up shader
    int height, width;
    glfwGetWindowSize(window, &width, &height);

    if(camera_x > settings.world->GetLength())
        camera_x = settings.world->GetLength();
    else if(camera_x < 0)
        camera_x = 0;

    if(camera_y > settings.world->GetHeight())
        camera_y = settings.world->GetLength();
    else if(camera_y < 0)
        camera_y = 0;

    settings.position.x = camera_x;
    settings.position.y = camera_y;

    settings.px = px;

    c2Vector new_view(settings.position.x * settings.px, 
                      settings.position.y * settings.px);

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(
                                 static_cast<float>(settings.px), 
                                 static_cast<float>(settings.px), 
                                 1.0f));

    glm::mat4 view = glm::lookAt(glm::vec3(new_view.x, new_view.y, 0.0f),
                                 glm::vec3(new_view.x, new_view.y, 1.0f),
                                 glm::vec3(0, 1, 0));

    glm::mat4 projection = glm::ortho(static_cast<float>(width)/2,
                                      -static_cast<float>(width)/2,
                                      -static_cast<float>(height)/2,
                                      static_cast<float>(height)/2,
                                      0.0f,
                                      static_cast<float>(sizeof(z_order)/sizeof(z_order[0])) + 1);

    shader.SetModelMat(model);
    shader.SetViewMat(view);
    shader.SetProjectionMat(projection);
    shader.SetIntUniform(settings.px, "px");

    //Then draw
    glDrawArrays(GL_POINTS, 0, coords.size());

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Draw box
    if(settings.box) { grid->DisplayBox(); }
    if(settings.grid) { grid->DisplayGrid(); }
}

std::vector<c2Cell*> c2Buffer::CellDisplay()
{
    int height, width;
    glfwGetWindowSize(window, &width, &height);

    int x_range = width/settings.px + 2;
    int y_range = height/settings.px + 2;

    std::vector<c2QuadTree*> nodes;
    std::vector<c2Cell*> cells;

    if(settings.tree != nullptr) {
        c2Vector p1(settings.position.x - x_range/2, 
                    settings.position.y - y_range/2);

        c2Vector p2(settings.position.x + x_range/2, 
                    settings.position.y + y_range/2);

        nodes = CapturedNodes(settings.tree, p1, p2);

        //for removing duplicates
        std::vector<c2Cell*> temp_cells;
        for(auto node : nodes) {
            for(auto cell : node->cells)
                temp_cells.push_back(cell);
        }

        std::unordered_set<c2Cell*> no_dupes(temp_cells.begin(), temp_cells.end());
        cells.assign(no_dupes.begin(), no_dupes.end());
    }
    
    return cells;
}

std::vector<c2QuadTree*> c2Buffer::CapturedNodes(c2QuadTree* node, 
                                                 const c2Vector& p1, 
                                                 const c2Vector& p2)
{
    //possibly shared with mutual exclusitivity?
    std::vector<c2QuadTree*> nodes;

    //Check if view is within quadrant
    Flags_1Byte* flag_bits = new Flags_1Byte;

    //possibly multithread these?
    if(p1.x < node->p1.x) { flag_bits->set(0, 1); }
    if(p1.y < node->p1.y) { flag_bits->set(1, 1); }
    if(p1.x > node->p2.x) { flag_bits->set(2, 1); }
    if(p1.y > node->p2.y) { flag_bits->set(3, 1); }
    if(p2.x < node->p1.x) { flag_bits->set(4, 1); }
    if(p2.y < node->p1.y) { flag_bits->set(5, 1); }
    if(p2.x > node->p2.x) { flag_bits->set(6, 1); }
    if(p2.y > node->p2.y) { flag_bits->set(7, 1); }

    if(int((flag_bits->return_state() << 4) & flag_bits->return_state()) == 0) {
        if(!node->nodes.empty()) {
            //multithread these too; update all vector copies to use std::move later as well
            std::vector<c2QuadTree*> q1 = CapturedNodes(node->nodes[0], p1, p2);
            std::vector<c2QuadTree*> q2 = CapturedNodes(node->nodes[1], p1, p2);
            std::vector<c2QuadTree*> q3 = CapturedNodes(node->nodes[2], p1, p2);
            std::vector<c2QuadTree*> q4 = CapturedNodes(node->nodes[3], p1, p2);

            nodes.reserve(q1.size() + q2.size() + q3.size() + q4.size());
            nodes.insert(nodes.end(), q1.begin(), q1.end());
            nodes.insert(nodes.end(), q2.begin(), q2.end());
            nodes.insert(nodes.end(), q3.begin(), q3.end());
            nodes.insert(nodes.end(), q4.begin(), q4.end());

            return nodes;
        }
        else {
            nodes.reserve(1);
            nodes.push_back(node);

            return nodes;
        }
    }
    else {
        return nodes;
    }
}


/* c2Grid implementation */
c2Grid::c2Grid(c2GraphicsDef& settings, GLFWwindow* window): 
  settings(settings), window(window)
{
    glGenVertexArrays(1, &BOX_VAO);
    glGenBuffers(1, &BOX_VBO);
    glGenBuffers(1, &BOX_EBO);

    glGenVertexArrays(1, &GRID_VAO);
    glGenBuffers(1, &GRID_VBO);
    glGenBuffers(1, &GRID_EBO);

    float thickness = 0.3f;

    float box_vertices[] = {
        /*0*/ -0.5f-thickness, -0.5f-thickness, box_z,
        /*1*/ settings.world->GetLength()-1.5+thickness, -0.5f-thickness, box_z,
        /*2*/ settings.world->GetLength()-1.5+thickness, settings.world->GetHeight()-1.5+thickness, box_z,
        /*3*/ -0.5f-thickness, settings.world->GetHeight()-1.5+thickness, box_z,
        /*4*/ -0.5f, -0.5f, box_z,
        /*5*/ settings.world->GetLength()-1.5, -0.5f, box_z,
        /*6*/ settings.world->GetLength()-1.5, settings.world->GetHeight()-1.5, box_z,
        /*7*/ -0.5f, settings.world->GetHeight()-1.5, box_z,
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

    glBindVertexArray(BOX_VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, BOX_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BOX_EBO);

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

c2Grid::~c2Grid()
{
    glDeleteVertexArrays(1, &BOX_VAO);
    glDeleteBuffers(1, &BOX_VBO);
    glDeleteBuffers(1, &BOX_EBO);

    glDeleteVertexArrays(1, &GRID_VAO);
    glDeleteBuffers(1, &GRID_VBO);
    glDeleteBuffers(1, &GRID_EBO);
}

void c2Grid::DisplayBox()
{
    glBindVertexArray(BOX_VAO); 
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/* Function called by DisplayCells() after DisplayBox().
 * Work with 1 shader: use nested for loops to generate
 * positions and indices.
*/
void c2Grid::DisplayGrid()
{
    //Figure out number of grids in each direction
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int x_total = round(width/px);
    int y_total = round(height/px);

    int filter_xright = (settings.world->GetLength() - 1) - floor(camera_x);
    int filter_xleft = camera_x - 1;

    int filter_ytop = (settings.world->GetHeight() - 1) - floor(camera_y);
    int filter_ybottom = camera_y - 1;

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
            float z = grid_z;

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
    
    glBindVertexArray(GRID_VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, GRID_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GRID_EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        glm::value_ptr(vertices[0]), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4),
        glm::value_ptr(indices[0]), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttrib3f(1, 0.08f, 0.08f, 0.08f);

    glDrawElements(GL_LINES, indices.size()*4, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
