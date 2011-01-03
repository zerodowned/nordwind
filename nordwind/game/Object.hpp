#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <QMatrix4x4>
#include "../Typedefs.hpp"

namespace game {
    class Object {
    public:
        static const QMatrix4x4 sMatrix;
        Object(const ID& id = 0);
        ID id() const;
        virtual void setID(const ID& id);
        QVector3D coordinates() const;
        void setCoordinates(const QVector3D& vec);
        virtual void setPos(const QPointF &pos) = 0;
        virtual void setZValue(qreal z) = 0;
    protected:
        ID mID;
        QVector3D mCoordinates;
        void updateGeometry();
    };
}
#endif // OBJECT_HPP
