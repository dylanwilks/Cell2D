#ifndef C2GUI_H
#define C2GUI_H 

#include <string>
#include <mutex>
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
#include "c2Graphics.h"

struct c2GraphicsDef;

enum c2GraphicsGUI {
    TEXT
};

static c2GraphicsGUI z_order_gui[] = {TEXT};

class c2GUI
{
    public:
      c2GUI(c2GraphicsDef& settings, GLFWwindow* window);

      //Clear the objects
      virtual ~c2GUI();

      //Draw the GUI
      virtual void Draw() = 0;

      //Set box for click events (optional)
      virtual void SetBox(c2Vector pos1, c2Vector pos2) = 0;
      virtual void ClickEvent(int button, int action, int mods) = 0;

      int z_value();

    protected:
      unsigned int VBO, VAO;
      c2GraphicsDef& settings;
      GLFWwindow* window;

      void set_z_value(c2GraphicsGUI gui);
      int z;
};

class c2Text : public c2GUI
{
    public:
      c2Text(c2GraphicsDef& settings, GLFWwindow* window);
      ~c2Text();

      void SetText(std::string text);
      void SetPosition(c2Vector pos);

      void Draw() override;
      void SetBox(c2Vector pos1, c2Vector pos2) override;
      void ClickEvent(int button, int action, int mods) override;

    private:
      unsigned int TEXTURE_VBO, EBO;
      std::string text;

      struct character {
          int row, col; 
          int px, voffset, hoffset;
          char value;
      };

      inline static unsigned int bitmap_font;
      inline static void hash_bitmap_font(character font);
      inline static character bitmap_fonts[128];

      inline static void init_static_members();
      inline static std::once_flag init_flag;

      inline static int img_width, img_height;
      c2Vector coords;
};

#endif /* C2GUI_H */
