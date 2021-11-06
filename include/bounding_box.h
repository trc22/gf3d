#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "gfc_vector.h"


typedef struct BoundingBox_S
{
    int x;
    int y;
    int w;
    int h;

    void *data;
}BoundingBox;

int bounding_box_overlap(BoundingBox* A, BoundingBox* B, Vector3D posA, Vector3D posB);

void bounding_box_update(BoundingBox *box, Vector3D position);

#endif
