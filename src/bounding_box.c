#include "simple_logger.h"
#include "bounding_box.h"

int bounding_box_overlap(BoundingBox* A, BoundingBox* B, Vector3D posA, Vector3D posB)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A->x + posA.x;
    rightA = A->x + A->w + posA.x;
    topA = A->y + posA.y;
    bottomA = A->y + A->h + posA.y;

    //Calculate the sides of rect B
    leftB = B->x + posB.x;
    rightB = B->x + B->w + posB.x;
    topB = B->y + posB.y;
    bottomB = B->y + B->h + posB.y;

     //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return 0;
    }

    if( rightA <= leftB )
    {
        return 0;
    }

    if( leftA >= rightB )
    {
        return 0;
    }

    //If none of the sides from A are outside B
    return 1;

    /*if(!vector3d_distance_between_less_than(one->maxExtentPos, two->minExtentPos, 2))
        return 0;
    if(!vector3d_distance_between_less_than(one->minExtentPos, two->maxExtentPos, 2))
        return 0;

    return 1;*/
}

void bounding_box_update(BoundingBox *box, Vector3D position)
{
   // vector3d_add(box->minExtentPos,box->minExtent,position);
   // vector3d_add(box->minExtentPos,box->maxExtent,position);
}

