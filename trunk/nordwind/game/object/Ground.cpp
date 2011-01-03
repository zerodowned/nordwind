/*
 * MapEntity.cpp
 *
 *  Created on: 16.12.2009
 *      Author: idstein
 */
#include "Ground.hpp"
#include "../TileData.hpp"

#include <QPainter>
#include <QGLContext>
#include <QGraphicsScene>

using namespace game;

const GLfloat Ground::defaultVertexArray[8] = {22.0f,0.0f,
                                            44.0f,22.0f,
                                            22.0f,44.0f,
                                            0.0f,22.0f};

const GLfloat Ground::artsCoords[8] = { 0.5f,0.0f,
                                     1.0f, 0.5f,
                                     0.5f, 1.0f,
                                     0.0f, 0.5f };

const GLfloat Ground::textureCoords[8] = { 0.0f, 0.0f,
                                        1.0f,1.0f,
                                        1.0f, 1.0f,
                                        0.0f, 0.0f };

Ground::Ground(const ID &id) :
        Object(0x2),
        mTex(false),
        mPolygon(NULL) {
    setID(id);
}

Ground::~Ground() {
    if(mPolygon)
        delete mPolygon;
}

int Ground::type() const {
    return QGraphicsItem::UserType+2;
}

QRectF Ground::boundingRect() const {
    return QRectF(0.0f,0.0f,44.0f,mPolygon?mPolygon[5]:44.0f);
}

void Ground::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    QGLWidget* gl = qobject_cast<QGLWidget*>(widget);
    const GLfloat* vertexArray = mPolygon ? mPolygon : defaultVertexArray;
    const GLfloat* texCoordArray = mTex?textureCoords : artsCoords;
    if(gl) {
        painter->beginNativePainting();
        glEnable(GL_TEXTURE_2D);
        GLuint texture = gl->bindTexture(mTexture,GL_TEXTURE_2D,GL_RGBA,QGLContext::NoBindOption);
        glBindTexture(GL_TEXTURE_2D,texture);
        glTexCoordPointer(2,GL_FLOAT,0,texCoordArray);
        glVertexPointer(2,GL_FLOAT,0,vertexArray);
        if(scene()->property("map.lines").toBool()) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        if(scene()->property("map.lines").toBool()) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        painter->endNativePainting();
    }
}

QVariant Ground::itemChange(GraphicsItemChange change, const QVariant &value) {
    switch(change) {
    case QGraphicsItem::ItemZValueChange:
        return value.toDouble() - 4.0f;
    default:
        return value;
    }
}

void Ground::setID(const ID& id) {
    Object::setID(id);
    if (mID == 0x2)
        setVisible(false);
    else {
        /*const Info& info  = TileData::instance()->at(mID);
        ID tID = info.textureID();
        if(tID) {
            mTex = true;
            mTexture = QPixmap(QString("textures:%1").arg(tID),"tex");
        } else*/
        mTexture = QPixmap(QString("arts:%1").arg(mID),"map");
        if(mTexture.isNull())
            setFlag(QGraphicsItem::ItemHasNoContents);
        update();
    }
}

qint16 helper_mapStrech(Z z1, Z z2) {
    return (z1 - z2) *4 + 22;
}

void Ground::stretch(Z southZ, Z eastZ, Z downZ) {
    qint16 stl = helper_mapStrech(mCoordinates.z(), southZ);
    qint16 str = helper_mapStrech(mCoordinates.z(), eastZ);
    qint16 hl = stl+helper_mapStrech(southZ,downZ);
    qint16 hr = str+helper_mapStrech(eastZ,downZ);

    if(stl!=22||
       str!=22||
       hl!=44||
       hr!=44) {
        prepareGeometryChange();
        mPolygon = new GLfloat[4*2];
        memcpy(mPolygon,defaultVertexArray,sizeof(GLfloat)*4*2);
        mPolygon[3] = str;
        mPolygon[5] = qMax(hl,hr);
        mPolygon[7] = stl;
    }
}
