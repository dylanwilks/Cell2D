#include "c2World.h"
#include "c2Cell.h"
#include "c2Collision.h"
#include "c2QuadTree.h"

c2QuadTree::c2QuadTree(int x, int y) :
  world_x(x), world_y(y), level(0), count(0), parent(nullptr)
{
    cells.reserve(MAX_CELLS);

    p1.Set(0, 0);
    p2.Set(world_x - 1, world_y - 1);
    m_p = ((p1 + p2)/2).Floor();
}

c2QuadTree::c2QuadTree(
    int x, 
    int y, 
    int level,
    c2QuadTree* parent,
    const c2Vector& p1,
    const c2Vector& p2
) : world_x(x), world_y(y), level(level), count(0), p1(p1), p2(p2)
{ cells.reserve(MAX_CELLS); }

void c2QuadTree::InsertCell(c2Cell* cell)
{
    c2QuadTree* node = remove_cell(cell); //ensure no duplicates + optimization
    c2Vector pos = cell->position;

    if(node == nullptr) { node = this; }

    while(true)
    {
        bool edge_x, edge_y, edge_gx, edge_gy, out;
        int fx, fy;
        fx = floor(pos.x); fy = floor(pos.y);

        edge_x = (fx == node->p1.x) || (fx == node->p2.x);
        edge_y = (fy == node->p1.y) || (fy == node->p2.y);
        edge_gx = (fx == 0) || (fx == world_x - 1);
        edge_gy = (fy == 0) || (fy == world_y - 1);

        out = floor(pos.x) < node->p1.x || 
          floor(pos.x) > node->p2.x || 
          floor(pos.y) < node->p1.y || 
          floor(pos.y) > node->p2.y;

        if((edge_x && !edge_gx) || (edge_y && !edge_gy) || out) {
            node = node->parent;
            continue;
        } 
        else { 
            node->insert_cell(cell);
            break; 
        }
    }
}

void c2QuadTree::RemoveCell(c2Cell* cell) 
{  
    for(auto& generic_node : cell->nodes)
    {
        c2QuadTree* cell_node = dynamic_cast<c2QuadTree*>(generic_node);

        cell_node->cells.erase(std::remove(cell_node->cells.begin(), 
          cell_node->cells.end(), cell), cell_node->cells.end());
        cell_node->count -= 1;     
        
        if(cell_node->count == 0 && cell_node->level != 0)
            cell_node->parent->RemoveChildNodes();
    }
    
    cell->nodes.clear();
}

c2Cell* c2QuadTree::GetCellPosition(const c2Vector& pos) const
{ 
    if(count == -1) {
        std::vector<int> quadrants = GetQuadrants(pos);
        return nodes[quadrants[0]]->GetCellPosition(pos);
    }

    for(auto& cell : cells)
        if(cell->position.Floor() == pos.Floor())
            return cell;

    return nullptr;
}

std::array<c2Cell*, 8> c2QuadTree::GetCellsAdjacent(const c2Vector& pos) const
{
    std::array<c2Cell*, 8> adj_list;
    adj_list[TOP_LEFT]     = GetCellPosition(c2Vector(pos.x-1, pos.y+1));
    adj_list[TOP]          = GetCellPosition(c2Vector(pos.x, pos.y+1));
    adj_list[TOP_RIGHT]    = GetCellPosition(c2Vector(pos.x+1, pos.y+1));
    adj_list[RIGHT]        = GetCellPosition(c2Vector(pos.x+1, pos.y));
    adj_list[BOTTOM_RIGHT] = GetCellPosition(c2Vector(pos.x+1, pos.y-1));
    adj_list[BOTTOM]       = GetCellPosition(c2Vector(pos.x, pos.y-1));
    adj_list[BOTTOM_LEFT]  = GetCellPosition(c2Vector(pos.x-1, pos.y-1));
    adj_list[LEFT]         = GetCellPosition(c2Vector(pos.x-1, pos.y));

    return adj_list;
}

void c2QuadTree::CreateChildNodes()
{ 
    for(int i = 0; i < 4; i++)
    {
        nodes[i] = new c2QuadTree(
            this->world_x, 
            this->world_y,
            this->level+1,
            this,
            this->p1,
            this->p2
        );

        nodes[i]->level = this->level + 1;
        nodes[i]->parent = this;

        nodes[i]->p1 = this->p1;
        nodes[i]->p2 = this->p2;    

        switch(i)
        {
            case 0:
                nodes[0]->p1 = this->m_p;
                nodes[0]->m_p = 
                  ((nodes[0]->p1 + nodes[0]->p2)/2).Floor();

                break;
            case 1:
                nodes[1]->p1.y = m_p.y;
                nodes[1]->p2.x = m_p.x;

                nodes[1]->m_p = 
                  ((nodes[1]->p1 + nodes[1]->p2)/2).Floor();

                break;
            case 2:
                nodes[2]->p2 = this->m_p;
                nodes[2]->m_p = 
                  ((nodes[2]->p1 + nodes[2]->p2)/2).Floor();

                break;
            case 3:
                nodes[3]->p1.x = m_p.x;
                nodes[3]->p2.y = m_p.y;

                nodes[3]->m_p = 
                  ((nodes[3]->p1 + nodes[3]->p2)/2).Floor();

                break;
        } 
    }  
    
    this->count = -1;
    while(!cells.empty()) {
        InsertCell(*(cells.begin()));
    }

    cells.clear();
}

c2QuadTree* c2QuadTree::RemoveChildNodes()
{
    for(int i = 0; i < 4; i++)
        if(!nodes[i]->cells.empty() || !nodes[i]->count == -1)
            return nullptr;

    for(int i = 0; i < 4; i++)
        delete nodes[i];

    count = 0;

    c2QuadTree* node = this;
    if(parent != nullptr) { node = parent->RemoveChildNodes(); }

    if(node == nullptr) { return this; }
    else { return node; }
}

c2QuadTree* c2QuadTree::insert_cell(c2Cell* cell)
{
    c2QuadTree* node = nullptr;
    std::vector<int> quad = GetQuadrants(cell->position);

    if(count > -1) 
    {
        c2Vector t_v = p2 - p1; 
        if(count >= MAX_CELLS && t_v.Magnitude() >= 1.415) { CreateChildNodes(); }
        else
        {
            cells.push_back(cell);
            count += 1;
            cell->nodes.push_back(this);

            return this;
        }
    }    

    for(auto itr = quad.begin(); itr != quad.end(); ++itr)
    {
        node = nodes[*itr]->insert_cell(cell);
    }

    if(quad.size() > 1) { return this; }

    return node;
}

c2QuadTree* c2QuadTree::remove_cell(c2Cell* cell)
{
    if(cell->nodes.empty()) { return nullptr; }
    c2QuadTree* common = dynamic_cast<c2QuadTree*>(*(cell->nodes.begin()));
    int max = common->level;

    for(auto& generic_node : cell->nodes)
    {
        c2QuadTree* cell_node = dynamic_cast<c2QuadTree*>(generic_node);
        if(common->level > cell_node->level) { common = cell_node; }
        if(max > cell_node->level) { max = cell_node->level; }

        cell_node->cells.erase(std::remove(cell_node->cells.begin(), 
          cell_node->cells.end(), cell), cell_node->cells.end());
        cell_node->count -= 1;          
        
        if(cell_node->count == 0 && cell_node->level != 0) { 
            c2QuadTree* node = cell_node->parent->RemoveChildNodes();

            if(node != nullptr)
                common = node;
        }
    }

    if(cell->nodes.size() > 1 && common->level == max 
      && !(common->parent == nullptr))
        common = common->parent; 
    
    cell->nodes.clear();
    
    if(common->level == 0) { return this; }
    else { return common; }
}

std::vector<int> c2QuadTree::GetQuadrants(const c2Vector& pos) const
{
    std::vector<int> quadrants; //vector as cell may be in several quadrants at once

    int fx, fy;
    fx = floor(pos.x); fy = floor(pos.y);

    bool topQuadrant = fy >= m_p.y;
    bool bottomQuadrant = fy <= m_p.y;
    
    bool leftQuadrant = fx <= m_p.x;
    bool rightQuadrant = fx >= m_p.x;
 
    if(topQuadrant)
    {
        if(rightQuadrant) { quadrants.push_back(0); }
        if(leftQuadrant) { quadrants.push_back(1); } 
    }

    if(bottomQuadrant)
    {
        if(rightQuadrant) { quadrants.push_back(3); }
        if(leftQuadrant) { quadrants.push_back(2); }
    }

    return quadrants;
}
