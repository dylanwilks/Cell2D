#ifndef C2FUNCTIONS_H
#define C2FUNCTIONS_H

#include "c2Graphics.h"

//Standard functions for moving around
void cursor_pan(c2GraphicsDef& settings);
void scroll_zoom(c2GraphicsDef& settings);
void mouse_click(c2GraphicsDef& settings);
void key_pan(c2GraphicsDef& settings);

//Fetches cells that'll fit within the display range
std::vector<c2Cell*> visible_cells(c2GraphicsDef& settings);

//Captures quadtree nodes that fit within the display
std::vector<c2QuadTree*> captured_nodes(c2QuadTree* node, 
                                        const c2Vector& p1,
                                        const c2Vector& p2);

#endif /* C2FUNCTIONS_H */
