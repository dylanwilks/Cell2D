#include "c2World.h"
#include "c2Cell.h"
#include "c2Collision.h"
#include "c2QuadTree.h"

c2World::c2World(int x, int y)
{
    this->x = x;
    this->y = y;

    collision = new c2QuadTree(x, y);  //choose quadtree in future (automatically?)
    step_callback = nullptr;
    meta_ptr = nullptr;
}

c2World::~c2World()
{
    for(auto& cell : cells) { delete cell; }
}

void c2World::Step()
{
    for(auto& cell : cells)
        if(cell->cell_callback == nullptr) { continue; }
        else { cell->cell_callback(this, cell); }

    if(step_callback != nullptr)
        step_callback(this);
}

c2Cell* c2World::CreateCell(const c2CellDef& def)
{
    c2Cell* cell = new c2Cell(def, this);
    cells.push_back(cell);

    collision->InsertCell(cell);

    return cell;
}

void c2World::DestroyCell(c2Cell* cell)
{
    if(cells.size() > 1) {
        int index = std::distance(cells.begin(), 
            std::find(cells.begin(), cells.end(), cell));
        std::iter_swap(cells.begin() + index, cells.end() - 1);
        cells.pop_back();
    }

    delete cell;
}

c2Cell* c2World::GetCellPosition(const c2Vector& pos) const
{
    return collision->GetCellPosition(pos);
}

std::array<c2Cell*, 8> c2World::GetCellsAdjacent(const c2Vector& pos) const
{
    return collision->GetCellsAdjacent(pos);
}
