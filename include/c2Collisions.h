#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <vector>                                                                       //      1 | 0 
#include <cmath>                                                                        //     ------
#include <algorithm>                                                                    //      2 | 3

#include "c2Math.h"

constexpr unsigned int MAX_CELLS = 10; //maybe dynamic?

class c2World;
class c2Cell;
class c2Buffer;

class c2QuadTree //each object represents a quadrant
{
    public:
        friend class c2World;
        struct NodeDim { c2Vector p1, p2, m_p; };

        c2QuadTree(c2World* world);

        void InsertCell(c2Cell*); //inserts cell pointer into corresponding nodes 
                                         // and returns common node between all nodes cell resides in.
                                         // Functions as Update function as well.

        c2QuadTree* RemoveCell(c2Cell*); //removes cell pointer from corresponding nodes
                                         // and returns common node between all nodes cell resides in

        std::vector<c2QuadTree*> GetChildren(); //returns child nodes
        std::vector<c2Cell*> GetCells(); //returns cells in node (if any)
        NodeDim GetDims(); //returns an array of p1, p2, and m_p (in that order)

    private:
        int level; //hierarchy level, root is 0
        int count; //cell count, stores -1 if node is not a leaf

        c2QuadTree* parent; //points to parent; for traversing up
        c2World* world;

        NodeDim node_dims;

        void CreateChildNodes(); //split quadrant into 4 more, transfer cells
                                        //returns node that called this function

        //CONVERT TO VOID LATER; TURN INTO WRAPPER
        c2QuadTree* RemoveChildNodes(); //removes all 4 quadrants of a node
                                        //returns node that called this function

        c2QuadTree* insert_cell(c2Cell*); //places the cells in the tree for the
                                          //InsertCells wrapper
                                       
        std::vector<int> GetQuadrants(c2Cell*); //quadrants the cell belongs to                

        std::vector<c2QuadTree*> nodes; //stores child quadrant pointers 
        std::vector<c2Cell*> cells; //stores cells if it is a leaf
};


inline std::vector<c2QuadTree*> c2QuadTree::GetChildren()
{
    return nodes;
}

inline std::vector<c2Cell*> c2QuadTree::GetCells()
{
    return cells;
}

inline c2QuadTree::NodeDim c2QuadTree::GetDims()
{
    return node_dims;
}

#endif /* C2COLLISIONS_H */
