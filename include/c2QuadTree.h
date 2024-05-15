#ifndef C2QUADTREE_H
#define C2QUADTREE_H 

#include "c2Collision.h"                                                           //      1 | 0 
                                                                                   //     ------
class c2World;                                                                     //      2 | 3
class c2Cell;

constexpr unsigned int MAX_CELLS = 10; //maybe dynamic?

class c2QuadTree : public c2Collision
{
    public:
        c2QuadTree(int x, int y); //root constructor

        void InsertCell(c2Cell*) override;
        void RemoveCell(c2Cell*) override;

        c2Cell* GetCellPosition(const c2Vector& pos) const override;
        std::array<c2Cell*, 8> GetCellsAdjacent(const c2Vector& pos) const override;

    private:
        c2QuadTree(
            int x,
            int y,
            int level,
            c2QuadTree* parent,
            const c2Vector& p1,
            const c2Vector& p2
        );

        void CreateChildNodes(); //split quadrant into 4 more, transfer cells
                                        //returns node that called this function
        c2QuadTree* RemoveChildNodes(); //removes all 4 quadrants of a node
                                        //returns node that called this function
        c2QuadTree* insert_cell(c2Cell*); //places the cells in the tree for the InsertCells wrapper
        c2QuadTree* remove_cell(c2Cell*); //returns common node between all nodes cell resides in as well
                                       
        std::vector<int> GetQuadrants(const c2Vector&) const; //quadrants the vector belongs to

        int level; //hierarchy level, root is 0
        int count; //cell count, stores -1 if node is not a leaf

        int world_x, world_y;
        c2QuadTree* parent; 
        c2Vector p1, p2, m_p;

        enum { n_children = 4 };
        c2QuadTree* nodes[n_children];
};

#endif /* C2QUADTREE_H */
