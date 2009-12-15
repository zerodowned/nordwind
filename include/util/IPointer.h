#ifndef _IPOINTER_H__
#define _IPOINTER_H__

namespace core {
	class IPointer
	{
	public:
		IPointer(void) {
		};
		virtual ~IPointer(void){
		};
		// is pointer still valid?
		virtual bool isValid( void ) {
			return false;
		};
		template< class B > 
		B* value( void ) {
			return NULL;
		};
	};

	class CReferenceCounter {
	private:
		qint32 counter;
	public:
		CReferenceCounter( qint32 _start = 1 ) 
			: counter (_start) {
		};
		void addRef() {
			counter++;
		};
		qint32 release() {
			return counter--;
		};
		bool isValid() {
			return (counter>0) ? true : false;
		};
	};

	template< class T>
	class IRefPointer : public IPointer {
	private:
		T* data;
		CReferenceCounter* reference;
	public:
		IRefPointer() 
			: IPointer(), data(NULL), reference(new CReferenceCounter(0)) {
		};
		IRefPointer( T* _data, CReferenceCounter* _counter = new CReferenceCounter() )
			: IPointer() {
			if(_data)
				data = _data;
			if(_counter) {
				reference = _counter;
			} else {
				reference = new CReferenceCounter();
			};
		};
		IRefPointer( const IRefPointer<T>& _pointer ) 
			: IPointer(), data(_pointer.data), reference(_pointer.reference) {
			reference->addRef();
		};
		~IRefPointer() {
			reference->release();
			if(!reference->isValid()) {
				if(data)
					delete data;
				delete reference;
			};
		};
		virtual bool isValid() {
			return (data) ? true : false;
		};
		T* operator-> () {
			return data;
		};
		T& operator* () {
			return *data;
		};
		T* getData() {
			return data;
		};
		CReferenceCounter* getRefCounter() {
			return reference;
		};
		template< class B >
		B* value( void ) {
			return reinterpret_cast<B*>(data);
		};
	};
}

#endif