//
// Created by vini84200 on 6/1/23.
//

#include <stdexcept>
#include "Scene.h"

Scene::Scene() {

}

void Scene::addObject(std::shared_ptr<Object> object) {
    objects.push_back( object );
}

void Scene::removeObject(std::shared_ptr<Object> object) {
//    objects.erase( std::remove( objects.begin(), objects.end(), object ), objects.end() );
    throw std::runtime_error( "Not implemented" );
}

std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return objects;
}
