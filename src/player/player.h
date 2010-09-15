#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "../someplayer.h"
#include "../track.h"
#include "../trackmetainformation.h"
#include "../playlist.h"
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QStack>

// represents player

using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::TrackMetadata;
using SomePlayer::DataObjects::Playlist;

namespace SomePlayer {
	namespace Playback {

		enum PlayerState { PLAYER_STOPPED, PLAYER_PLAYING, PLAYER_PAUSED, PLAYER_LOADING, PLAYER_DONE, PLAYER_ERROR };

		class Player : public QObject
		{
			Q_OBJECT
		public:
			explicit Player(QObject *parent = 0);

			bool random() {return _random;}
			bool repeat() {return _repeat;}
			Phonon::MediaObject* mediaObject() {return _player;}

		signals:
			void stateChanged (PlayerState);
			void trackChanged (Track);
			void tick (int, int); // played | all (seconds)

		public slots:
			void setTrackId(int id);
			void play();
			void pause();
			void stop();
			void next();
			void prev();
			void setPlaylist(Playlist);
			void toggleRandom() {_random = !_random;}
			void toggleRepeat() {_repeat = !_repeat;}
			void seek(int);
		private slots:
			void _stateChanged(Phonon::State, Phonon::State);
			void _tick(qint64);
		private:
			int _current;
			bool _random;
			bool _repeat;
			QStack<int> _history;
			Playlist _playlist;
			Phonon::MediaObject *_player;
			Phonon::AudioOutput *_output;
			PlayerState _state;

			void _set_source();
		};
	};
};

#endif // PLAYER_H
