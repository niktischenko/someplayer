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

#include "dbstorage.h"
#include <QSqlQuery>
#include <QSqlResult>

using namespace SomePlayer::Storage;
using namespace SomePlayer::DataObjects;

DbStorage::DbStorage(QString path) {
	QString dbname = path+_DATABASE_NAME_;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbname);
	if (!db.open()) {
		// throw big exception
	}
	// create database structure
	_create_database_structure();
	_prepare_queries();
}

void DbStorage::_prepare_queries() {
	_get_artists_query = new QSqlQuery(db);
	_get_artists_query->prepare("SELECT name FROM artist");

	_get_albums_for_artist_query = new QSqlQuery(db);
	_get_albums_for_artist_query->prepare("SELECT name FROM album WHERE artist_id in (SELECT id from artist WHERE name = :name);");

	_get_tracks_for_album_query = new QSqlQuery(db);
	_get_tracks_for_album_query->prepare("SELECT id, title, source, count, length FROM tracks WHERE artist_id IN "
										 "(SELECT id FROM artist WHERE name = :artist_name) AND album_id IN "
										 "(SELECT id FROM album WHERE name = :album_name);");

	_get_favorites_query = new QSqlQuery(db);
	_get_favorites_query->prepare("SELECT track_id as id, title, artist, album.name as album, source, count, length FROM "
								  "(SELECT tracks.id AS track_id, artist.name AS artist, title, count, source, tracks.album_id, length FROM "
								  "tracks JOIN artist ON tracks.artist_id = artist.id) "
								  "JOIN album ON album_id = album.id WHERE track_id IN "
								  "(SELECT track_id FROM favorites);");

	_get_most_played_query = new QSqlQuery(db);
	_get_most_played_query->prepare("SELECT track_id as id, title, artist, album.name as album, source, count, length FROM "
									"(SELECT tracks.id AS track_id, artist.name AS artist, title, count, source, tracks.album_id, length FROM "
									"tracks JOIN artist ON tracks.artist_id = artist.id) "
									"JOIN album ON album_id = album.id ORDER BY count DESC "
									"LIMIT 0, :max");

	_get_never_played_query = new QSqlQuery(db);
	_get_never_played_query->prepare("SELECT track_id as id, title, artist, album.name as album, source, count, length FROM "
									 "(SELECT tracks.id AS track_id, artist.name AS artist, title, count, source, tracks.album_id, length FROM "
									 "tracks JOIN artist ON tracks.artist_id = artist.id) "
									 "JOIN album ON album_id = album.id "
									 "WHERE count = 0");

	_get_recently_added_query = new QSqlQuery(db);
	_get_recently_added_query->prepare("SELECT track_id as id, title, artist, album.name as album, source, count, length FROM "
									   "(SELECT tracks.id AS track_id, artist.name AS artist, title, count, source, tracks.album_id, length FROM "
									   "tracks JOIN artist ON tracks.artist_id = artist.id) "
									   "JOIN album ON album_id = album.id "
									   "WHERE track_id IN "
									   "(SELECT track_id FROM adding_date ORDER BY date DESC LIMIT 0, :max)");

	_get_track_count = new QSqlQuery(db);
	_get_track_count->prepare("SELECT count from tracks WHERE id = :id");

	_check_artist_query = new QSqlQuery(db);
	_check_artist_query->prepare("SELECT id FROM artist WHERE name = :name");

	_check_album_query = new QSqlQuery(db);
	_check_album_query->prepare("SELECT id FROM album WHERE name = :name AND artist_id = :artist_id");

	_check_track_query = new QSqlQuery(db);
	_check_track_query->prepare("SELECT id FROM tracks WHERE source = :source");

	_insert_artist_query = new QSqlQuery(db);
	_insert_artist_query->prepare("INSERT INTO artist (name) values (:name)");

	_insert_album_query = new QSqlQuery(db);
	_insert_album_query->prepare("INSERT INTO album (name, artist_id) values (:name, :artist_id)");

	_insert_track_query = new QSqlQuery(db);
	_insert_track_query->prepare("INSERT INTO tracks (title, artist_id, album_id, source, length) values (:title, :artist_id, :album_id, :source, :length)");

	_insert_date_query = new QSqlQuery(db);
	_insert_date_query->prepare("INSERT INTO adding_date (track_id, date) values (:track_id, strftime('%s', 'now'))");

	_insert_favorites_query = new QSqlQuery(db);
	_insert_favorites_query->prepare("INSERT INTO favorites (track_id) values (:track_id)");

	_update_track_count_query = new QSqlQuery(db);
	_update_track_count_query->prepare("UPDATE tracks SET count = :count where id = :id");
}

void DbStorage::_create_database_structure() {
	QSqlQuery *query = new QSqlQuery(db);
	query->exec("create table artist (id integer primary key, "
										"name text "
										");");
	query->exec("create table album (id integer primary key, "
										"artist_id integer, "
										"name text, "
										"foreign key(artist_id) references arist(id) "
										");");
	query->exec("create table tracks (id integer primary key, "
										"artist_id integer, "
										"album_id integer, "
										"title text, "
										"source text, "
										"count integer default 0, "
										"length integer default 0, "
										"foreign key(artist_id) references artist(id), "
										"foreign key(album_id) references album(id) "
										");");
	query->exec("create table favorites (track_id integer, "
										"foreign key(track_id) references tracks(id) "
										");");
	query->exec("create table adding_date (track_id integer, "
										"date integer, "
										"foreign key(track_id) references tracks(id) "
										");");
}

DbStorage::~DbStorage() {
	delete _get_albums_for_artist_query;
	delete _get_artists_query;
	delete _get_favorites_query;
	delete _get_most_played_query;
	delete _get_never_played_query;
	delete _get_recently_added_query;
	delete _get_tracks_for_album_query;
	delete _check_album_query;
	delete _check_artist_query;
	delete _check_track_query;
	delete _insert_album_query;
	delete _insert_artist_query;
	delete _insert_date_query;
	delete _insert_track_query;
	delete _insert_favorites_query;
	delete _update_track_count_query;
	db.close();
}

QList<QString> DbStorage::getArtists() {
	QList<QString> artists;
	QSqlQuery *query = _get_artists_query;
	query->exec();
	while (query->next()) {
		QString name = query->value(0).toString();
		artists.append(name);
	}
	return artists;
}

QList<QString> DbStorage::getAlbumsForArtist(QString artist) {
	QList<QString> albums;
	QSqlQuery *query = _get_albums_for_artist_query;
	query->bindValue(":name", artist);
	query->exec();
	while (query->next()) {
		QString name = query->value(0).toString();
		albums.append(name);
	}
	return albums;
}

QList<Track> DbStorage::getTracksForAlbum(QString album, QString artist) {
	QList<Track> tracks;
	QSqlQuery *query = _get_tracks_for_album_query;
	query->bindValue(":artist_name", artist);
	query->bindValue(":album_name", album);
	query->exec();

	while (query->next()) {
		int id = query->value(0).toInt();
		QString title = query->value(1).toString();
		QString source = query->value(2).toString();
		int count = query->value(3).toInt();
		int length = query->value(4).toInt();
		TrackMetadata meta (title, artist, album, length);
		Track track(id, meta, source);
		track.setCount(count);
		tracks.append(track);
	}

	return tracks;
}

Playlist DbStorage::getFavorites() {
	Playlist playlist;
	QSqlQuery *query = _get_favorites_query;
	query->exec();
	while(query->next()) {
		int id = query->value(0).toInt();
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(id, meta, source);
		track.setCount(count);
		playlist.addTrack(track);
		playlist.setName("Favorites");
	}
	return playlist;
}

Playlist DbStorage::getMostPlayed() {
	Playlist playlist;
	QSqlQuery *query = _get_most_played_query;
	query->bindValue(":max", _DYNAMIC_PLAYLIST_MAX_COUNT_);
	query->exec();
	while (query->next()) {
		int id = query->value(0).toInt();
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(id, meta, source);
		track.setCount(count);
		playlist.addTrack(track);
		playlist.setName("Most popular");
	}
	return playlist;
}

Playlist DbStorage::getNeverPlayed() {
	Playlist playlist;
	QSqlQuery *query = _get_never_played_query;
	query->bindValue(":max", _DYNAMIC_PLAYLIST_MAX_COUNT_);
	query->exec();
	while (query->next()) {
		int id = query->value(0).toInt();
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(id, meta, source);
		track.setCount(count);
		playlist.addTrack(track);
		playlist.setName("Never played");
	}
	return playlist;
}

Playlist DbStorage::getRecentlyAdded() {
	Playlist playlist;
	QSqlQuery *query = _get_recently_added_query;
	query->bindValue(":max", _DYNAMIC_PLAYLIST_MAX_COUNT_);
	query->exec();
	while (query->next()) {
		int id = query->value(0).toInt();
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(id, meta, source);
		track.setCount(count);
		playlist.addTrack(track);
		playlist.setName("Recently added");
	}
	return playlist;
}

void DbStorage::removeTrack(Track track) {
	int id = track.id();
	QSqlQuery *query = new QSqlQuery(db);
	query->prepare("DELETE FROM tracks WHERE id = :id;");
	query->bindValue(":id", id);
	query->exec();
	query->prepare("DELETE FROM favorites WHERE track_id = :id;");
	query->bindValue(":id", id);
	query->exec();
	query->prepare("DELETE FROM adding_date WHERE id = :id;");
	query->bindValue(":id", id);
	query->exec();
}

void DbStorage::addTrack(Track track) {
	QString title = track.metadata().title();
	QString artist = track.metadata().artist();
	QString album = track.metadata().album();
	QString source = track.source();
	int artist_id = _check_add_artist(artist);
	int album_id = _check_add_album(album, artist_id);
	if (artist_id == -1 || album_id == -1) {
		//big bang
		return;
	}
	QSqlQuery *query = _check_track_query;
	query->bindValue(":source", source);
	query->exec();
	if (query->next()) {
		// already in datebase, skip
		return;
	}
	query = _insert_track_query;
	query->bindValue(":title", title);
	query->bindValue(":artist_id", artist_id);
	query->bindValue(":album_id", album_id);
	query->bindValue(":source", source);
	query->bindValue(":length", track.metadata().length());
	if (query->exec()) {
		//ok
		query = _check_track_query;
		query->bindValue(":source", source);
		query->exec();
		if (query->next()) {
			int id = query->value(0).toInt();
			query = _insert_date_query;
			query->bindValue(":track_id", id);
			if (query->exec()) {
				// ok
			} else {
				// big bang
			}
		} else {
			// big bang
		}
	} else {
		// big bang
	}
}

void DbStorage::addToFavorites(Track track) {
	QSqlQuery *query = _insert_favorites_query;
	query->bindValue(":track_id", track.id());
	query->exec();
}

void DbStorage::updateTrackCount(Track track) {
	QSqlQuery *query = _get_track_count;
	query->bindValue(":id", track.id());
	query->exec();
	if (query->next()) {
		int count = query->value(0).toInt();
		query = _update_track_count_query;
		query->bindValue(":count", count+1);
		query->bindValue(":id", track.id());
		query->exec();
	}
}

int DbStorage::_check_add_artist(QString artist) {
	QSqlQuery *query = _check_artist_query;
	query->bindValue(":name", artist);
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		return id;
	} else {
		query = _insert_artist_query;
		query->bindValue(":name", artist);
		if (query->exec()) {
			return _check_add_artist(artist);
		} else {
			// big bang
			return -1;
		}
	}
}

int DbStorage::_check_add_album(QString album, int artist_id) {
	QSqlQuery *query = _check_album_query;
	query->bindValue(":name", album);
	query->bindValue(":artist_id", artist_id);
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		return id;
	} else {
		query = _insert_album_query;
		query->bindValue(":name", album);
		query->bindValue(":artist_id", artist_id);
		if (query->exec()) {
			return _check_add_album(album, artist_id);
		} else {
			// big bang
			return -1;
		}
	}
}
