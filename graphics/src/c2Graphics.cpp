#include "c2Graphics.h"
#include "c2Shader.h"
#include "c2World_Graphics.h"
#include "c2GUI.h"

/* Callbacks */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, 
                  int action, int mods);

/* Implementations */
int c2DisplayWorld(c2GraphicsDef& settings)
{  
    //Initialize GLFW and settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    //Create window with context with GLFW
    GLFWwindow* window = glfwCreateWindow(settings.window_width, settings.window_height, 
        settings.window_title, NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //Set function pointers for this context
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Set window user pointer
    glfwSetWindowUserPointer(window, &settings);

    //Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    //Adjust other configs
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //Set buffers (declare as array in future)
    c2Cell_Graphics* cell_graphics = new c2Cell_Graphics(settings, window);
    c2Box_Graphics* box_graphics = new c2Box_Graphics(settings, window);
    c2Grid_Graphics* grid_graphics = new c2Grid_Graphics(settings, window);

    c2Text* cell_gui = new c2Text(settings, window);
    cell_gui->SetText("Hello Milo");

    //Create shader program (to be loaded using DisplayWorld)
    c2Shader world_shader("graphics/shaders/object_shader.vs",  //Vertex shader
                          "graphics/shaders/object_shader.fs"); //Fragment shader

    c2Shader gui_shader("graphics/shaders/gui_shader.vs",
                        "graphics/shaders/gui_shader.fs");

    //Lambda for loading shader variables and displaying world objects
    auto DisplayObjects = [&settings, &world_shader,
                          cell_graphics, box_graphics, grid_graphics]() 
    {
        world_shader.LoadShader();

        c2Vector new_view(
            settings.position.x * settings.px, 
            settings.position.y * settings.px
        );

        glm::mat4 model = glm::scale(
            glm::mat4(1.0f), glm::vec3(
            static_cast<float>(settings.px), 
            static_cast<float>(settings.px), 
            1.0f)
        );

        glm::mat4 view = glm::lookAt(
            glm::vec3(new_view.x, new_view.y, 0.0f),
            glm::vec3(new_view.x, new_view.y, 1.0f),
            glm::vec3(0, 1, 0)
        );

        glm::mat4 projection = glm::ortho(
            static_cast<float>(settings.window_width)/2,
            -static_cast<float>(settings.window_width)/2,
            -static_cast<float>(settings.window_height)/2,
            static_cast<float>(settings.window_height)/2,
            0.0f,
            static_cast<float>(sizeof(z_order_objs)/
            sizeof(z_order_objs[0])) + 1
        );

        world_shader.SetModelMat(model);
        world_shader.SetViewMat(view);
        world_shader.SetProjectionMat(projection);
        world_shader.SetIntUniform(settings.px, "px");

        cell_graphics->Draw();
        box_graphics->Draw();
        grid_graphics->Draw();
    };

    auto DisplayGUI = [&settings, &gui_shader, cell_gui]()
    {
        gui_shader.LoadShader();

        glm::mat4 view = glm::lookAt(
            glm::vec3(settings.window_width/2, settings.window_height/2, 0.0f),
            glm::vec3(settings.window_width/2, settings.window_height/2, 1.0f),
            glm::vec3(0, 1, 0)
        );

        glm::mat4 projection = glm::ortho(
            static_cast<float>(settings.window_width)/2,
            -static_cast<float>(settings.window_width)/2 ,
            -static_cast<float>(settings.window_height)/2,
            static_cast<float>(settings.window_height)/2,
            -1.0f,
            static_cast<float>(sizeof(z_order_gui)/
            sizeof(z_order_gui[0])) + 1
        );

        gui_shader.SetViewMat(view);
        gui_shader.SetProjectionMat(projection);

        //if(settings.follow != nullptr) {
            cell_gui->Draw();
        //}
    };

    //Render/message loop 
    while(!glfwWindowShouldClose(window))
    {
        //Clear screen and set background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Polling events
        glfwPollEvents();

        //Handle user input/callbacks
        settings.user_callback(settings);

        //Reset/Update mouse and key settings
        c2KeyStateDef* key_state = settings.key_state;
        c2MouseStateDef* mouse_state = settings.mouse_state;
        mouse_state->xoffset = 0;
        mouse_state->yoffset = 0;
        
        if(mouse_state->state_change) {
            for(int i = 0; i < MOUSE_EVENTS; i++)
                switch(mouse_state->button_states[i])
                {
                    case PRESS:
                        mouse_state->button_states[i] = HELD;
                        break;

                    case RELEASE:
                        mouse_state->button_states[i] = DEFAULT;
                        break;

                    default:
                        break;
                }
        }

        if(key_state->state_change) {
            for(int i = 0; i < KEY_EVENTS; i++)
                switch(key_state->button_states[i])
                {
                    case RELEASE:
                        key_state->button_states[i] = DEFAULT;
                        break;

                    default:
                        break;
                }
        }

        key_state->state_change = false;
        mouse_state->state_change = false;

        //(?) Allow key input to change worlds
        DisplayObjects();
        DisplayGUI();

        /* Update ticksPerFrame times. If ticksPerFrame = 1/time_step, then 
         * velocity units are cells per frame.                              */
        int ticksPerFrame = round(settings.speed_mult/settings.world->TimeStep());
        for(int i = 0; i < ticksPerFrame; i++)
            settings.world->Step(settings.tree);

        glfwSwapBuffers(window);
    }

    delete cell_graphics;
    delete box_graphics;
    delete grid_graphics;

    delete cell_gui;

    glfwTerminate();

    return 0;
}

//OpenGL polling events/callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    c2GraphicsDef* settings_ptr = static_cast<c2GraphicsDef*>(glfwGetWindowUserPointer(window));
    settings_ptr->window_width = width;
    settings_ptr->window_height = height;

    glViewport(0, 0, width, height);
}

//For updating the mouse state each render loop for user
void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    c2GraphicsDef* settings_ptr = static_cast<c2GraphicsDef*>(glfwGetWindowUserPointer(window));
    c2MouseStateDef* mouse_state = settings_ptr->mouse_state;

    mouse_state->xpos = xpos;
    mouse_state->ypos = settings_ptr->window_height - ypos;

    mouse_state->xpos_grid = (mouse_state->xpos - settings_ptr->window_width/2)/
      settings_ptr->px + settings_ptr->position.x;
    mouse_state->ypos_grid = (mouse_state->ypos - settings_ptr->window_height/2)/
      settings_ptr->px + settings_ptr->position.y;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    c2GraphicsDef* settings_ptr = static_cast<c2GraphicsDef*>(glfwGetWindowUserPointer(window));
    c2MouseStateDef* mouse_state = settings_ptr->mouse_state;
    mouse_state->state_change = true;

    switch(button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            if(action == GLFW_PRESS)
                mouse_state->button_states[MOUSE_LEFT] = PRESS;
            else
                mouse_state->button_states[MOUSE_LEFT] = RELEASE;

            mouse_state->mouse_event = MOUSE_LEFT;
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            if(action == GLFW_PRESS)
                mouse_state->button_states[MOUSE_RIGHT] = PRESS;
            else
                mouse_state->button_states[MOUSE_RIGHT] = RELEASE;

            mouse_state->mouse_event = MOUSE_RIGHT;
            break;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    c2GraphicsDef* settings_ptr = static_cast<c2GraphicsDef*>(glfwGetWindowUserPointer(window));
    c2MouseStateDef* mouse_state = settings_ptr->mouse_state;

    mouse_state->xoffset = xoffset;
    mouse_state->yoffset = yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, 
                  int action, int mods)
{
    if(key == GLFW_KEY_UNKNOWN) { return; }

    c2GraphicsDef* settings_ptr = static_cast<c2GraphicsDef*>(glfwGetWindowUserPointer(window));
    c2KeyStateDef* key_state = settings_ptr->key_state;
    key_state->state_change = true;

    switch(key) 
    {
        case GLFW_KEY_UP:
            if(action == GLFW_PRESS)
                key_state->button_states[KEY_UP] = PRESS;
            else if(action == GLFW_RELEASE)
                key_state->button_states[KEY_UP] = RELEASE;
            else {
                key_state->button_states[KEY_UP] = HELD;
                key_state->state_change = false;
            }

            key_state->key_event = KEY_UP;
            break;

        case GLFW_KEY_DOWN:
            if(action == GLFW_PRESS)
                key_state->button_states[KEY_DOWN] = PRESS;
            else if(action == GLFW_RELEASE)
                key_state->button_states[KEY_DOWN] = RELEASE;
            else {
                key_state->button_states[KEY_DOWN] = HELD;
                key_state->state_change = false;
            }

            key_state->key_event = KEY_DOWN;
            break;

        case GLFW_KEY_LEFT:
            if(action == GLFW_PRESS)
                key_state->button_states[KEY_LEFT] = PRESS;
            else if(action == GLFW_RELEASE)
                key_state->button_states[KEY_LEFT] = RELEASE;
            else {
                key_state->button_states[KEY_LEFT] = HELD;
                key_state->state_change = false;
            }

            key_state->key_event = KEY_LEFT;
            break;

        case GLFW_KEY_RIGHT:
            if(action == GLFW_PRESS)
                key_state->button_states[KEY_RIGHT] = PRESS;
            else if(action == GLFW_RELEASE)
                key_state->button_states[KEY_RIGHT] = RELEASE;
            else {
                key_state->button_states[KEY_RIGHT] = HELD;
                key_state->state_change = false;
            }

            key_state->key_event = KEY_RIGHT;
            break;

        case GLFW_KEY_Q:
            if(action == GLFW_PRESS)
                key_state->button_states[KEY_Q] = PRESS;
            else if(action == GLFW_RELEASE)
                key_state->button_states[KEY_Q] = RELEASE;
            else {
                key_state->button_states[KEY_Q] = HELD;
                key_state->state_change = false;
            }

            key_state->key_event = KEY_Q;
            break;
    }
}
