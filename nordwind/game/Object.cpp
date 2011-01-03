#include "Object.hpp"

using namespace game;

const QMatrix4x4 Object::sMatrix( 22.f,  22.f, 15.0f, 0.f,
                                  -22.f, 22.f, 15.0f, 0.f,
                                  0.f,   -4.f, 1.f, 0.f,
                                  0.f,   0.f,  0.f, 1.f);

Object::Object(const ID &id)
    : mID(id) {
}

ID Object::id() const {
    return mID;
}

void Object::setID(const ID &id) {
    mID = id;
}

QVector3D Object::coordinates() const {
    return mCoordinates;
}

void Object::setCoordinates(const QVector3D& coordinates) {
    mCoordinates = coordinates;
    updateGeometry();
}

void Object::updateGeometry() {
    QVector3D vec = mCoordinates*sMatrix;
    setPos(vec.toPointF());
    setZValue(vec.z());
}
