#include "quadtreeRegionTest.hpp"
#include <quadtree_region.hpp>

namespace epstl {

TEST_F(quadtreeRegionTest, SetAPixel)
{
    quadtree_region<int> tree(6, 6);

    tree.set(-3,-2);
    tree.set(1,-1);
    tree.set(2,2);
    tree.print(std::cout);
    ((quadtree<int, bool>)tree).print(std::cout);


}

} // namespace epstl
