#include "c2Functions.h"

void cursor_pan(c2GraphicsDef& settings)
{
    c2MouseStateDef* mouse_state = settings.mouse_state;

    if(mouse_state->button_states[MOUSE_LEFT] == PRESS) {
        mouse_state->xpos_buffer[0] = mouse_state->xpos_grid;
        mouse_state->ypos_buffer[0] = mouse_state->ypos_grid;
    }

    if(mouse_state->button_states[MOUSE_LEFT] == PRESS ||
        mouse_state->button_states[MOUSE_LEFT] == HELD) {
        c2Vector init_click(mouse_state->xpos_buffer[0], 
                            mouse_state->ypos_buffer[0]);
        c2Vector cursor_grid(mouse_state->xpos_grid, mouse_state->ypos_grid);

        c2Vector temp = cursor_grid - init_click;
        settings.position -= temp;
        mouse_state->xpos_grid -= temp.x;
        mouse_state->ypos_grid -= temp.y;

        if(settings.position.x > settings.world->GetLength())
            settings.position.x = settings.world->GetLength();
        else if(settings.position.x < 0)
            settings.position.x = 0;

        if(settings.position.y > settings.world->GetHeight())
            settings.position.y = settings.world->GetHeight();
        else if(settings.position.y < 0)
            settings.position.y = 0;
    }
}

void scroll_zoom(c2GraphicsDef& settings)
{
    c2MouseStateDef* mouse_state = settings.mouse_state;

    if(abs(mouse_state->yoffset) > 0) {
        c2Vector cursor_grid(mouse_state->xpos_grid, mouse_state->ypos_grid);
        c2Vector screen_grid(settings.position.x, settings.position.y);
        float px_dist = ((cursor_grid - screen_grid) * settings.px).Magnitude();

        int dpx = round((float)mouse_state->yoffset * settings.px/10);
        if(abs(dpx) > 1)
            settings.px += dpx;
        else
            settings.px += mouse_state->yoffset;

        if(settings.px < MIN_CELL_LENGTH)
            settings.px = MIN_CELL_LENGTH;
        if(settings.px > MAX_CELL_LENGTH)
            settings.px = MAX_CELL_LENGTH;

        c2Vector new_pt = (screen_grid - cursor_grid).Normalize() 
                          * px_dist/settings.px;
        settings.position = cursor_grid + new_pt;
    }
}

void key_pan(c2GraphicsDef& settings)
{
    c2KeyStateDef* key_state = settings.key_state;

    if(key_state->button_states[KEY_UP] == HELD || 
        key_state->button_states[KEY_UP] == PRESS)
        settings.position.y += 10/settings.px + 1.1;
    if(key_state->button_states[KEY_DOWN] == HELD || 
        key_state->button_states[KEY_DOWN] == PRESS)
        settings.position.y -= 10/settings.px + 1.1;
    if(key_state->button_states[KEY_LEFT] == HELD || 
        key_state->button_states[KEY_LEFT] == PRESS)
        settings.position.x -= 10/settings.px + 1.1;
    if(key_state->button_states[KEY_RIGHT] == HELD || 
        key_state->button_states[KEY_RIGHT] == PRESS)
        settings.position.x += 10/settings.px + 1.1;
}

void adjust_speed(c2GraphicsDef& settings)
{
    c2KeyStateDef* key_state = settings.key_state;

}

std::vector<c2Cell*> visible_cells(c2GraphicsDef& settings)
{
    int x_range = settings.window_width/settings.px + 2;
    int y_range = settings.window_height/settings.px + 2;

    std::vector<c2QuadTree*> nodes;
    std::vector<c2Cell*> cells;

    if(settings.tree != nullptr) {
        c2Vector p1(settings.position.x - x_range/2, 
                    settings.position.y - y_range/2);

        c2Vector p2(settings.position.x + x_range/2, 
                    settings.position.y + y_range/2);

        nodes = captured_nodes(settings.tree, p1, p2);

        //for removing duplicates
        std::vector<c2Cell*> temp_cells;
        for(auto node : nodes) {
            //for(auto cell : node->cells)
              for(auto cell : node->GetCells())
                  if(cell->GetPosition() >= p1 && cell->GetPosition() <= p2)
                      temp_cells.push_back(cell);
        }

        std::unordered_set<c2Cell*> no_dupes(temp_cells.begin(), temp_cells.end());
        cells.assign(no_dupes.begin(), no_dupes.end());
    }
    
    return cells;
}

std::vector<c2QuadTree*> captured_nodes(c2QuadTree* node, 
                                        const c2Vector& p1, 
                                        const c2Vector& p2)
{
    //possibly shared with mutual exclusitivity?
    std::vector<c2QuadTree*> nodes;
    nBitArray<1, 8> flag_bits;

    //check for multithreading
    if(p1.x < node->GetDims().p1.x) { flag_bits[0] = 1; }
    if(p1.y < node->GetDims().p1.y) { flag_bits[1] = 1; }
    if(p1.x > node->GetDims().p2.x) { flag_bits[2] = 1; }
    if(p1.y > node->GetDims().p2.y) { flag_bits[3] = 1; }
    if(p2.x < node->GetDims().p1.x) { flag_bits[4] = 1; }
    if(p2.y < node->GetDims().p1.y) { flag_bits[5] = 1; }
    if(p2.x > node->GetDims().p2.x) { flag_bits[6] = 1; }
    if(p2.y > node->GetDims().p2.y) { flag_bits[7] = 1; }

    if((flag_bits << 4 & flag_bits) == 0) {
        if(!node->GetChildren().empty()) {
            //multithread these too; update all vector copies to use std::move later as well
            std::vector<c2QuadTree*> q1 = 
              captured_nodes(node->GetChildren()[0], p1, p2);
            std::vector<c2QuadTree*> q2 = 
              captured_nodes(node->GetChildren()[1], p1, p2);
            std::vector<c2QuadTree*> q3 = 
              captured_nodes(node->GetChildren()[2], p1, p2);
            std::vector<c2QuadTree*> q4 = 
              captured_nodes(node->GetChildren()[3], p1, p2);

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
