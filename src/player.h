#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>

class Player : public QObject
{
	Q_OBJECT
public:
	explicit Player();
	~Player();
private:
	Phonon::MediaObject *object;
	Phonon::AudioOutput *output;

public slots:
	void playSong(QString filename);
};

#endif // PLAYER_H
