#ifndef _SINGLETON_HPP__
#define _SINGLETON_HPP__

template<class T>
class Singleton {
	private:
		Singleton(const Singleton& _t) {
		}
		static T* m_instance;
	protected:
		Singleton() {
		}
		virtual ~Singleton() {
		}
	public:
		static T& getInstance(void) {
			if (!isValid())
				m_instance = new T();
			return (*m_instance);
		}
		static T& instance(void) {
			if (!isValid())
				m_instance = new T();
			return (*m_instance);
		}
		static T* getPointer(void) {
			if (!isValid())
				m_instance = new T();
			return m_instance;
		}
		static bool isValid() {
			return (!m_instance) ? false : true;
		}
		static void destroyInstance(void) {
			if (isValid()) {
				delete m_instance;
				m_instance = 0;
			}
		}
};
template <typename T> T* Singleton<T>::m_instance = 0;


#endif
