#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "gfc_vector.h"


typedef struct BoundingBox_S
{
    Vector3D minExtent;
    Vector3D maxExtent;

    Vector3D minExtentPos;
    Vector3D maxExtentPos;

    void *data;
}BoundingBox;

int bounding_box_overlap(BoundingBox* one, BoundingBox* two);

void bounding_box_update(BoundingBox *box, Vector3D position);

#endif
