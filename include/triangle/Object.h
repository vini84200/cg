//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_OBJECT_H
#define TRIANGLE_OBJECT_H

#include <glm/glm.hpp>

/**
 * This class represents an object in the scene.
 * It is responsible for storing the object's data and rendering it.
 */
class Object {
public:
   Object();

   glm::mat4 getTransformMatrix();

   private:



};
#endif //TRIANGLE_OBJECT_H
