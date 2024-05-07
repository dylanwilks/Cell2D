#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "c2GUI.h"

c2Text::c2Text(c2GraphicsDef& settings, GLFWwindow* window) 
  : c2GUI(settings, window), coords(c2Vector(0, settings.window_height/2))
{
    glGenBuffers(1, &TEXTURE_VBO);
    glGenBuffers(1, &EBO);

    std::call_once(init_flag, init_static_members);
    set_z_value(TEXT);
}

void c2Text::hash_bitmap_font(character font)
{
    int idx = (int)font.value;
    bitmap_fonts[idx] = font;
}

void c2Text::init_static_members()
{
    //Bitmap font characters
    character BITMAP_FONT_a {
        7, 2,
        8, 0, 0,
        'a'
    };

    character BITMAP_FONT_b {
        7, 3,
        8, 0, 0,
        'b'
    };

    character BITMAP_FONT_c {
        7, 4,
        8, 0, 0,
        'c'
    };

    character BITMAP_FONT_d {
        7, 5,
        8, 0, 0,
        'd'
    };
    character BITMAP_FONT_e {
        7, 6,
        8, 0, 0,
        'e'
    };

    character BITMAP_FONT_f {
        7, 7,
        8, 0, 0,
        'f'
    };

    character BITMAP_FONT_g {
        7, 8,
        8, 0, 0,
        'g'
    };

    character BITMAP_FONT_h {
        7, 9,
        8, 0, 0,
        'h'
    };

    character BITMAP_FONT_i {
        7, 10,
        8, 0, 0,
        'i'
    };

    character BITMAP_FONT_j {
        7, 11,
        8, 0, 0,
        'j'
    };

    character BITMAP_FONT_k {
        7, 12,
        8, 0, 0,
        'k'
    };

    character BITMAP_FONT_l {
        7, 13,
        8, 1, 0,
        'l'
    };
    character BITMAP_FONT_m {
        7, 14,
        8, 0, 0,
        'm'
    };

    character BITMAP_FONT_n {
        7, 15,
        8, 0, 0,
        'n'
    };

    character BITMAP_FONT_o {
        7, 16,
        8, 0, 0,
        'o'
    };

    character BITMAP_FONT_p {
        8, 1,
        8, 0, 0,
        'p'
    };

    character BITMAP_FONT_q {
        8, 2,
        8, 0, 0,
        'q'
    };

    character BITMAP_FONT_r {
        8, 3,
        8, 0, 0,
        'r'
    };

    character BITMAP_FONT_s {
        8, 4,
        8, 0, 0,
        's'
    };

    character BITMAP_FONT_t {
        8, 5,
        8, 0, 0,
        't'
    };

    character BITMAP_FONT_u {
        8, 6,
        8, 0, 0,
        'u'
    };

    character BITMAP_FONT_v {
        8, 7,
        8, 0, 0,
        'v'
    };

    character BITMAP_FONT_w {
        8, 8,
        8, 0, 0,
        'w'
    };

    character BITMAP_FONT_x {
        8, 9,
        8, 0, 0,
        'x'
    };

    character BITMAP_FONT_y {
        8, 10,
        8, 0, 0,
        'y'
    };

    character BITMAP_FONT_z {
        8, 11,
        8, 0, 0,
        'z'
    };

    character BITMAP_FONT_A {
        5, 2,
        8, 0, 0,
        'A'
    };

    character BITMAP_FONT_B {
        5, 3,
        8, 0, 0,
        'B'
    };

    character BITMAP_FONT_C {
        5, 4,
        8, 0, 0,
        'C'
    };

    character BITMAP_FONT_D {
        5, 5,
        8, 0, 0,
        'D'
    };
    character BITMAP_FONT_E {
        5, 6,
        8, 0, 0,
        'E'
    };

    character BITMAP_FONT_F {
        5, 7,
        8, 0, 0,
        'F'
    };

    character BITMAP_FONT_G {
        5, 8,
        8, 0, 0,
        'G'
    };

    character BITMAP_FONT_H {
        5, 9,
        8, 0, 0,
        'H'
    };

    character BITMAP_FONT_I {
        5, 10,
        8, 1, 0,
        'I'
    };

    character BITMAP_FONT_J {
        5, 11,
        8, 0, 0,
        'J'
    };

    character BITMAP_FONT_K {
        5, 12,
        8, 0, 0,
        'K'
    };

    character BITMAP_FONT_L {
        5, 13,
        8, 0, 0,
        'L'
    };

    character BITMAP_FONT_M {
        5, 14,
        8, 0, 0,
        'M'
    };

    character BITMAP_FONT_N {
        5, 15,
        8, 0, 0,
        'N'
    };

    character BITMAP_FONT_O {
        5, 16,
        8, 0, 0,
        'O'
    };

    character BITMAP_FONT_P {
        6, 1,
        8, 0, 0,
        'P'
    };

    character BITMAP_FONT_Q {
        6, 2,
        8, 0, 0,
        'Q'
    };

    character BITMAP_FONT_R {
        6, 3,
        8, 0, 0,
        'R'
    };

    character BITMAP_FONT_S {
        6, 4,
        8, 0, 0,
        'S'
    };

    character BITMAP_FONT_T {
        6, 5,
        8, 0, 0,
        'T'
    };

    character BITMAP_FONT_U {
        6, 6,
        8, 0, 0,
        'U'
    };

    character BITMAP_FONT_V {
        6, 7,
        8, 0, 0,
        'V'
    };

    character BITMAP_FONT_W {
        6, 8,
        8, 0, 0,
        'W'
    };

    character BITMAP_FONT_X {
        6, 9,
        8, 0, 0,
        'X'
    };

    character BITMAP_FONT_Y {
        6, 10,
        8, 0, 0,
        'Y'
    };

    character BITMAP_FONT_Z {
        6, 11,
        8, 0, 0,
        'Z'
    };

    character BITMAP_FONT_0 {
        4, 1,
        8, 0, 0,
        '0'
    };

    character BITMAP_FONT_1 {
        4, 2,
        8, 1, 0,
        '1'
    };

    character BITMAP_FONT_2 {
        4, 3,
        8, 0, 0,
        '2'
    };

    character BITMAP_FONT_3 {
        4, 4,
        8, 0, 0,
        '3'
    };

    character BITMAP_FONT_4 {
        4, 5,
        8, 0, 0,
        '4'
    };

    character BITMAP_FONT_5 {
        4, 6,
        8, 0, 0,
        '5'
    };

    character BITMAP_FONT_6 {
        4, 7,
        8, 0, 0,
        '6'
    };

    character BITMAP_FONT_7 {
        4, 8,
        8, 0, 0,
        '7'
    };

    character BITMAP_FONT_8 {
        4, 9,
        8, 0, 0,
        '8'
    };

    character BITMAP_FONT_9 {
        4, 10,
        8, 0, 0,
        'Y'
    };

    character BITMAP_FONT_SPACE {
        1, 1,
        8, 0, 0,
        ' '
    };

    character BITMAP_FONT_COLON {
        4, 11,
        8, 1, 0,
        ':'
    };

    character BITMAP_FONT_LEFT_PARANTHESIS {
        3, 9,
        8, 1, 0,
        '('
    };

    character BITMAP_FONT_RIGHT_PARANTHESIS {
        3, 10,
        8, 1, 0,
        ')'
    };

    character BITMAP_FONT_COMMA {
        3, 13,
        8, 1, 0,
        ','
    };

    character BITMAP_FONT_PERIOD {
        3, 15,
        8, 1, 0,
        '.'
    };

    character BITMAP_FONT_FSLASH {
        3, 16,
        8, 0, 0,
        '/'
    };

    character BITMAP_FONT_BSLASH {
        10, 16,
        8, 0, 0,
        '\\'
    };

    //Load texture object
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("graphics/src/gui/bitmapfont_2.png", &width, &height, &nrChannels, 0);
    if(data == nullptr) {
        std::cout << "ERROR::STBI::" << stbi_failure_reason() << std::endl;
        return;
    }

    glGenTextures(1, &bitmap_font);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bitmap_font);

    //set to mipmap setting initially; needs to be set if not generating mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 
                 0,
                 GL_RGBA, 
                 width, 
                 height, 
                 0, 
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 data
                );

    //glGenerateMipmap(GL_TEXTURE_2D);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &img_width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &img_height);

    stbi_image_free(data);

    //Load bitmap characters
    hash_bitmap_font(BITMAP_FONT_a);
    hash_bitmap_font(BITMAP_FONT_b);
    hash_bitmap_font(BITMAP_FONT_c);
    hash_bitmap_font(BITMAP_FONT_d);
    hash_bitmap_font(BITMAP_FONT_e);
    hash_bitmap_font(BITMAP_FONT_f);
    hash_bitmap_font(BITMAP_FONT_g);
    hash_bitmap_font(BITMAP_FONT_h);
    hash_bitmap_font(BITMAP_FONT_i);
    hash_bitmap_font(BITMAP_FONT_j);
    hash_bitmap_font(BITMAP_FONT_k);
    hash_bitmap_font(BITMAP_FONT_l);
    hash_bitmap_font(BITMAP_FONT_m);
    hash_bitmap_font(BITMAP_FONT_n);
    hash_bitmap_font(BITMAP_FONT_o);
    hash_bitmap_font(BITMAP_FONT_p);
    hash_bitmap_font(BITMAP_FONT_q);
    hash_bitmap_font(BITMAP_FONT_r);
    hash_bitmap_font(BITMAP_FONT_s);
    hash_bitmap_font(BITMAP_FONT_t);
    hash_bitmap_font(BITMAP_FONT_u);
    hash_bitmap_font(BITMAP_FONT_v);
    hash_bitmap_font(BITMAP_FONT_w);
    hash_bitmap_font(BITMAP_FONT_x);
    hash_bitmap_font(BITMAP_FONT_y);
    hash_bitmap_font(BITMAP_FONT_z);

    hash_bitmap_font(BITMAP_FONT_A);
    hash_bitmap_font(BITMAP_FONT_B);
    hash_bitmap_font(BITMAP_FONT_C);
    hash_bitmap_font(BITMAP_FONT_D);
    hash_bitmap_font(BITMAP_FONT_E);
    hash_bitmap_font(BITMAP_FONT_F);
    hash_bitmap_font(BITMAP_FONT_G);
    hash_bitmap_font(BITMAP_FONT_H);
    hash_bitmap_font(BITMAP_FONT_I);
    hash_bitmap_font(BITMAP_FONT_J);
    hash_bitmap_font(BITMAP_FONT_K);
    hash_bitmap_font(BITMAP_FONT_L);
    hash_bitmap_font(BITMAP_FONT_M);
    hash_bitmap_font(BITMAP_FONT_N);
    hash_bitmap_font(BITMAP_FONT_O);
    hash_bitmap_font(BITMAP_FONT_P);
    hash_bitmap_font(BITMAP_FONT_Q);
    hash_bitmap_font(BITMAP_FONT_R);
    hash_bitmap_font(BITMAP_FONT_S);
    hash_bitmap_font(BITMAP_FONT_T);
    hash_bitmap_font(BITMAP_FONT_U);
    hash_bitmap_font(BITMAP_FONT_V);
    hash_bitmap_font(BITMAP_FONT_W);
    hash_bitmap_font(BITMAP_FONT_X);
    hash_bitmap_font(BITMAP_FONT_Y);
    hash_bitmap_font(BITMAP_FONT_Z);

    hash_bitmap_font(BITMAP_FONT_0);
    hash_bitmap_font(BITMAP_FONT_1);
    hash_bitmap_font(BITMAP_FONT_2);
    hash_bitmap_font(BITMAP_FONT_3);
    hash_bitmap_font(BITMAP_FONT_4);
    hash_bitmap_font(BITMAP_FONT_5);
    hash_bitmap_font(BITMAP_FONT_6);
    hash_bitmap_font(BITMAP_FONT_7);
    hash_bitmap_font(BITMAP_FONT_8);
    hash_bitmap_font(BITMAP_FONT_9);

    hash_bitmap_font(BITMAP_FONT_SPACE);
    hash_bitmap_font(BITMAP_FONT_COLON);
    hash_bitmap_font(BITMAP_FONT_LEFT_PARANTHESIS);
    hash_bitmap_font(BITMAP_FONT_RIGHT_PARANTHESIS);
    hash_bitmap_font(BITMAP_FONT_COMMA);
    hash_bitmap_font(BITMAP_FONT_PERIOD);
    hash_bitmap_font(BITMAP_FONT_FSLASH);
    hash_bitmap_font(BITMAP_FONT_BSLASH);

    glBindTexture(GL_TEXTURE_2D, 0);
}

c2Text::~c2Text()
{
    glDeleteBuffers(1, &TEXTURE_VBO);
    glDeleteBuffers(1, &EBO);
}

void c2Text::SetText(std::string text)
{
    int x_pos = coords.x;
    int y_pos = coords.y;

    std::vector<glm::vec3> font_coords;
    std::vector<glm::vec2> img_coords;
    std::vector<glm::uvec3> font_indices;

    for(int i = 0; i < text.size(); i++) {
        int idx = (int)text[i];
        character font = bitmap_fonts[idx];

        //character position relative to screen coords (starting top left)
        font_coords.push_back(glm::vec3(x_pos, y_pos, z));  //top left
        font_coords.push_back(glm::vec3(x_pos + font.px - 2*font.voffset, y_pos, z));  //top right
        font_coords.push_back(glm::vec3(x_pos + font.px - 2*font.voffset, y_pos - font.px, z));  //bottom right
        font_coords.push_back(glm::vec3(x_pos, y_pos - font.px, z));  //bottom left

        //font position as texture coordinates
        double img_x = static_cast<double>(font.px*(font.col-1))/img_width;
        double img_y = static_cast<double>(img_height - font.px*(font.row-1))/
            img_height;

        img_coords.push_back(glm::vec2(img_x + static_cast<double>(font.voffset)/
            img_width, img_y)); //top left
        img_coords.push_back(glm::vec2(img_x + static_cast<double>(font.px - font.voffset)/
            img_width, img_y)); //top right
        img_coords.push_back(glm::vec2(img_x + static_cast<double>(font.px - font.voffset)/
            img_width, img_y - static_cast<double>(font.px)/img_height)); //bottom right
        img_coords.push_back(glm::vec2(img_x + static_cast<double>(font.voffset)/
            img_width, img_y - static_cast<double>(font.px)/img_height)); //bottom left

        //buffer indices
        font_indices.push_back(glm::uvec3(0 + 4*i, 1 + 4*i, 2 + 4*i));
        font_indices.push_back(glm::uvec3(0 + 4*i, 2 + 4*i, 3 + 4*i));

        x_pos += font.px - 2*font.voffset;
    }

    glBindVertexArray(VAO);

    //set character buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, font_coords.size() * sizeof(glm::vec3),
        glm::value_ptr(font_coords[0]), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //set texture buffer
    glBindBuffer(GL_ARRAY_BUFFER, TEXTURE_VBO);
    glBufferData(GL_ARRAY_BUFFER, img_coords.size() * sizeof(glm::vec2),
        glm::value_ptr(img_coords[0]), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //set indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, font_indices.size() * sizeof(glm::uvec3),
        glm::value_ptr(font_indices[0]), GL_DYNAMIC_DRAW);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->text = text;
}

void c2Text::SetPosition(c2Vector pos)
{
    coords = pos;
}

void c2Text::Draw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bitmap_font);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6 * text.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void c2Text::SetBox(c2Vector pos1, c2Vector pos2)
{
    return;
}

void c2Text::ClickEvent(int button, int action, int mods)
{
    return;
}
