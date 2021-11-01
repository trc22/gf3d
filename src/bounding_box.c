#include "simple_logger.h"
#include "bounding_box.h"

int bounding_box_overlap(BoundingBox* one, BoundingBox* two)
{
    if(!vector3d_distance_between_less_than(two->maxExtentPos, one->minExtentPos, 2))
        return 0;
    if(!vector3d_distance_between_less_than(one->minExtentPos, two->maxExtentPos, 2))
        return 0;

    return 1;
}

void bounding_box_update(BoundingBox *box, Vector3D position)
{
    vector3d_add(box->minExtentPos,box->minExtent,position);
    vector3d_add(box->minExtentPos,box->maxExtent,position);
}

