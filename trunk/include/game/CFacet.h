#ifndef CFACET_H_
#define CFACET_H_

#include <QtGui/QGraphicsScene>

namespace resource {
	class IFacet;
}
namespace world {
	class CFacet : public QGraphicsScene
	{
		Q_OBJECT
	private:
		IFacet* m_facet;
		QMap<QPair<quint32, quint32>, CMapBlock> m_blocks;
	public:
		CFacet(QObject* parent = 0);
		virtual ~CFacet();
		void initialize();
		void deinitialize();
	};
}
#endif
