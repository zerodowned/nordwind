#ifndef MODULE_HPP__
#define MODULE_HPP__

#include <qthread.h>
#include "Settings.hpp"

namespace core {
	// wrapper class for Modules
	// intercommunication class
	// parent of gui, game, sound, world, core, network
	virtual class Module : public QThread {
		Q_OBJECT;
	private:
		Settings* mSettings;
		bool loadSettings();
	protected:
		virtual void load() = 0;
		void run();
		virtual void unload() = 0;
	public:
		Module( QObject* _parent );
		virtual ~Module();

		bool startup();
		bool shutdown();

		Settings* getSettings();
		QString getStatus() const;
		QString getName() const;
		virtual QString getVersion() const = 0;
	
		enum State {
			Initializing,
			Loading,
			Running,
			Unloading,
			Deinitializing,
			Off
		};
	Q_SLOTS
		void reload();
	};

	inline Settings* Module::getSettings() {
		return mSettings;
	}

	inline QString Module::getName() const {
		return objectName();
	}

}
#endif
