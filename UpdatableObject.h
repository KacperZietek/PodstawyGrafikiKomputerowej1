#ifndef UPDATABLEOBJECT_H
#define UPDATABLEOBJECT_H

#include "GameObject.h"

class UpdatableObject : virtual public GameObject {
public:
    virtual ~UpdatableObject() = default;
    virtual void update(float delta_time) = 0;
};

#endif // UPDATABLEOBJECT_H
