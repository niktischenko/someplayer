#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "../someplayer.h"
#include "../track.h"
#include <phonon/MediaObject>
#include <phonon/AudioOutput>

// represents player

using SomePlayer::DataObjects::Track;

namespace SomePlayer {
	namespace Playback {

		enum PlayerState { PLAYER_STOPPED, PLAYER_PLAYING, PLAYER_PAUSED, PLAYER_LOADING, PLAYER_DONE };

		class Player : public QObject
		{
			Q_OBJECT
		public:
			explicit Player(QObject *parent = 0);

		signals:
			void stateChanged (PlayerState);
			void tick (int, int); // played | all (seconds)

		public slots:
			void setTrack(Track&);
			void play();
			void pause();
			void stop();
		private slots:
			void _stateChanged(Phonon::State, Phonon::State);
			void _tick(qint64);
		private:
			Track _current_track;
			Phonon::MediaObject *_player;
			void _create_player();
		};
	};
};

#endif // PLAYER_H
