#include "simple_logger.h"
#include "bounding_box.h"

int LineIntersectsLine(Vector2D l1p1, Vector2D l1p2, Vector2D l2p1, Vector2D l2p2);

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

//C adaptation of user Chvanikoff's code on https://coderedirect.com/questions/137023/how-to-check-if-line-segment-intersects-a-rectangle
int bounding_box_line_intersect(Vector2D p1, Vector2D p2, BoundingBox *box, Vector3D position)
{
    BoundingBox test;
    BoundingBox *r;

    test.x = box->x + position.x;
    test.y = box->y + position.y;
    test.w = box->w + position.x;
    test.h = box->h + position.y;
    r = &test;


    if(LineIntersectsLine(p1, p2, vector2d(r->x, r->y), vector2d(r->x + r->w, r->y)))
        return 1;
    if(LineIntersectsLine(p1, p2, vector2d(r->x + r->w, r->y), vector2d(r->x + r->w, r->y + r->h)))
        return 1;
    if(LineIntersectsLine(p1, p2, vector2d(r->x + r->w, r->y + r->h), vector2d(r->x, r->y + r->h)))
        return 1;
    if(LineIntersectsLine(p1, p2, vector2d(r->x, r->y + r->h), vector2d(r->x, r->y)))
        return 1;

    return 0;

}

//C adaptation of user Chvanikoff's code on https://coderedirect.com/questions/137023/how-to-check-if-line-segment-intersects-a-rectangle
int LineIntersectsLine(Vector2D l1p1, Vector2D l1p2, Vector2D l2p1, Vector2D l2p2)
{
    float q = (l1p1.y - l2p1.y) * (l2p2.x - l2p1.x) - (l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
    float d = (l1p2.x - l1p1.x) * (l2p2.y - l2p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);

    if( d == 0 )
    {
        return 0;
    }

    float r = q / d;

    q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - (l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
    float s = q / d;

    if( r < 0 || r > 1 || s < 0 || s > 1 )
    {
        return 0;
    }

    return 1;
}
