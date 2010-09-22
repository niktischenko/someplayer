#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

namespace SomePlayer {
	namespace Storage {
		class Config
		{
		public:
		    Config();
		    ~Config();
		    QString applicationDir();
		    QVariant getValue(QString key);
		    void setValue(QString key, QVariant value);
		private:
		    QSettings *_settings;
		};
	};
};

#endif // CONFIG_H
