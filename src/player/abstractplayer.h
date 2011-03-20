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

#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QObject>
#include "../someplayer.h"
#include "../track.h"
#include "../trackmetainformation.h"
#include "../playlist.h"

using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::DataObjects::LastPlayed;

namespace SomePlayer {
	namespace Playback {

		enum PlayerState { PLAYER_STOPPED, PLAYER_PLAYING, PLAYER_PAUSED, PLAYER_LOADING, PLAYER_DONE, PLAYER_ERROR };
		enum RepeatRule {REPEAT_NO, REPEAT_ALL, REPEAT_ONE};

		class AbstractPlayer : public QObject
		{

			Q_OBJECT
		public:
			explicit AbstractPlayer(QObject *parent = 0);
			virtual bool random() = 0;
			virtual RepeatRule repeat() = 0;
			virtual bool equalizerEnabled() = 0;
			virtual bool equalizerAvailable() = 0;
			virtual Track current() = 0;
			virtual void setAwaitingSeek(int pos) = 0;

		signals:
			void stateChanged (PlayerState);
			void trackChanged (Track);
			void tick (int, int); // played | all (seconds)
			void trackDone(Track);
			void saveLastPlayed(LastPlayed);
			void startPlaylist();

		public slots:
			virtual void setTrackId(int id) = 0;
			virtual void enqueue(int id) = 0;
			virtual void toggle() = 0;
			virtual void play() = 0;
			virtual void pause() = 0;
			virtual void playIfPaused() = 0;
			virtual void stop() = 0;
			virtual void next() = 0;
			virtual void prev() = 0;
			virtual void setPlaylist(Playlist) = 0;
			virtual void toggleRandom() = 0;
			virtual void toggleRepeat() = 0;
			virtual void seek(int) = 0;
			virtual void enableEqualizer() = 0;
			virtual void disableEqualizer() = 0;
			virtual void setEqualizerValue(int band, double value) = 0;
			virtual void equalizerValue(int band, double *) = 0;
			virtual QString artist() = 0;
			virtual QString album() = 0;
			virtual QString title() = 0;
			virtual PlayerState state() = 0;
			virtual QString stateText() = 0;
			virtual QString albumart() = 0;
			virtual void setAlbumart(QString albumart) = 0;
		};
	};
};

#endif // ABSTRACTPLAYER_H
