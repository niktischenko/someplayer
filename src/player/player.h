/*
 * SomePlayer - An alternate music player for Maemo 5
 * Copyright (C) 2010 Nikolay (somebody) Tischenko <niktischenko@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "../someplayer.h"
#include "../track.h"
#include "../trackmetainformation.h"
#include "../playlist.h"
#include "abstractplayer.h"
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/Effect>
#include <phonon/Path>
#include <QStack>
#include <QQueue>

// represents player

using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::TrackMetadata;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::Storage::Config;

namespace SomePlayer {
	namespace Playback {

		class Randomizer {
		public:
			void setPlaylist(QList<int>);
			int next();
			void removeId(int);
		private:
			QList<int> _playlist;
			QList<int> _rand;
			void _shuffle();
			int _last;
		};

		class Player : public AbstractPlayer
		{
			Q_OBJECT
		public:
			explicit Player(QObject *parent = 0);

			virtual bool random() {return _random;}
			virtual RepeatRule repeat() {return _repeat;}
			virtual bool equalizerEnabled() {return _equalizer_enabled;}
			virtual bool equalizerAvailable() {return _equalizer != NULL;}
			virtual PlayerState state() {return _state;}

		public slots:
			void setTrackId(int id);
			void enqueue(int id);
			void toggle();
			void play();
			void stop();
			void next();
			void prev();
			void setPlaylist(Playlist);
			void toggleRandom();
			void toggleRepeat();
			void seek(int);
			void enableEqualizer();
			void disableEqualizer();
			void setEqualizerValue(int band, double value);
			void equalizerValue(int band, double *);
			QString artist();
			QString album();
			QString title();
		private slots:
			void _stateChanged(Phonon::State, Phonon::State);
			void _tick(qint64);
		private:
			Randomizer _randomizer;
			int _current;
			Track _track; // current track (workaround)
			bool _random;
			RepeatRule _repeat;
			bool _equalizer_enabled;
			QStack<int> _history;
			QQueue<int> _queue;
			QStack<int> _prev_history;
			Playlist _playlist;
			Phonon::MediaObject *_player;
			Phonon::AudioOutput *_output;
			Phonon::Path _path;
			Phonon::Effect *_equalizer;
			PlayerState _state;
			Config _config;

			void _set_source();

		};
	};
};

#endif // PLAYER_H
