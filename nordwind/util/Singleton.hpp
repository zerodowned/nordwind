#ifndef _SINGLETON_HPP__
#define _SINGLETON_HPP__

template<class T>
class Singleton {
	private:
		Singleton(const Singleton& _t) {
		}
		static T* mInstance;
	protected:
		Singleton() {
			mInstance = const_cast<T*>(static_cast<const T*>(this));
		}
		virtual ~Singleton() {
			mInstance = 0;
		}
	public:
		static T& getInstance(void) {
			if (!isValid())
				new T();
			return (*mInstance);
		}
		static T& instance(void) {
			if (!isValid())
				new T();
			return (*mInstance);
		}
		static T* getPointer(void) {
			if (!isValid())
				mInstance = new T();
			return mInstance;
		}
		static bool isValid() {
			return (!mInstance) ? false : true;
		}
		static void destroyInstance(void) {
			if (isValid()) {
				delete mInstance;
			}
		}
};
template <typename T> T* Singleton<T>::mInstance = 0;


#endif
