#ifndef CFUNCTOR_H__
#define CFUNCTOR_H__

namespace core {
	class CFunctor {
	public:
		virtual quint32 operator()() = 0;
	}
}
#endif