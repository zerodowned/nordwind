/*
 * MulFileEngine.hpp
 *
 *  Created on: Sep 6, 2010
 *      Author: idstein
 */

#ifndef MULFILEENGINE_HPP_
#define MULFILEENGINE_HPP_

#include <qfsfileengine.h>
#include "Index.hpp"

class MulFileEngine: public QFSFileEngine {
public:
        MulFileEngine(const QString& mulFile, const QString& indexFile, const quint32& id);
        virtual ~MulFileEngine();
        virtual bool open(QIODevice::OpenMode mode);
        virtual qint64 pos() const;
        virtual qint64 size() const;
        virtual bool seek(qint64 pos);
        virtual QString fileName(QAbstractFileEngine::FileName file =
                        QAbstractFileEngine::DefaultName) const;
        void	setFileName ( const QString & file );
        virtual bool
                        supportsExtension(QAbstractFileEngine::Extension extension) const;
        virtual bool extension(QAbstractFileEngine::Extension extension,
                        const QAbstractFileEngine::ExtensionOption * option = 0,
                        QAbstractFileEngine::ExtensionReturn * output = 0);
        static const QChar separator;
private:
        const quint32 mID;
        Index mIndex;
};

#endif /* MULFILEENGINE_HPP_ */
