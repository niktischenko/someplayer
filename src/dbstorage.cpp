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
#include <QDebug>
#include <QFileInfo>

using namespace SomePlayer::Storage;
using namespace SomePlayer::DataObjects;

DbStorage::DbStorage(QString path) {
	QString dbname = path + _DATABASE_NAME_;
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
	_get_artists_query->prepare("SELECT name FROM artist ORDER BY uname");

	_get_albums_for_artist_sort_name_query = new QSqlQuery(db);
	_get_albums_for_artist_sort_name_query->prepare("SELECT name, year FROM album WHERE artist_id in (SELECT id from artist WHERE uname = :uname) ORDER BY uname;");

	_get_albums_for_artist_sort_year_query = new QSqlQuery(db);
	_get_albums_for_artist_sort_year_query->prepare("SELECT name, year FROM album WHERE artist_id in (SELECT id from artist WHERE uname = :uname) ORDER BY year;");

	_get_tracks_for_album_query = new QSqlQuery(db);
	_get_tracks_for_album_query->prepare("SELECT id, title, source, count, length FROM tracks WHERE artist_id IN "
								"(SELECT id FROM artist WHERE uname = :artist_uname) AND album_id IN "
								"(SELECT id FROM album WHERE uname = :album_uname);");

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

	_get_track_count_query = new QSqlQuery(db);
	_get_track_count_query->prepare("SELECT count from tracks WHERE id = :id");

	_get_tracks_by_pattern_query = new QSqlQuery(db);
	_get_tracks_by_pattern_query->prepare("SELECT id, title, artist, album, source, count, length, year FROM "
					      "entire WHERE "
					      "utitle LIKE (SELECT '%' || :ptitle || '%') OR "
					      "album_uname LIKE (SELECT '%' || :palbum || '%') OR "
					      "artist_uname LIKE (SELECT '%' || :partist || '%') "
					      "ORDER BY artist_uname, year");

	_get_track_id_by_source_query = new QSqlQuery(db);
	_get_track_id_by_source_query->prepare("SELECT id FROM tracks WHERE source = :source");

	_get_directories_query = new QSqlQuery(db);
	_get_directories_query->prepare("SELECT id, path FROM directories");

	_get_artists_count_query = new QSqlQuery(db);
	_get_artists_count_query->prepare("SELECT COUNT(id) AS cnt FROM artist");

	_get_albums_count_query = new QSqlQuery(db);
	_get_albums_count_query->prepare("SELECT COUNT(id) AS cnt FROM album");

	_get_tracks_count_query = new QSqlQuery(db);
	_get_tracks_count_query->prepare("SELECT COUNT(id) AS cnt FROM tracks");

	_get_tracks_source_from_query = new QSqlQuery(db);
	_get_tracks_source_from_query->prepare("SELECT source FROM tracks WHERE directory = :directory_id");

	_check_artist_query = new QSqlQuery(db);
	_check_artist_query->prepare("SELECT id FROM artist WHERE uname = :uname");

	_check_album_query = new QSqlQuery(db);
	_check_album_query->prepare("SELECT id FROM album WHERE uname = :uname AND artist_id = :artist_id");

	_check_directory_query = new QSqlQuery(db);
	_check_directory_query->prepare("SELECT id from directories WHERE path = :path");

	_insert_artist_query = new QSqlQuery(db);
	_insert_artist_query->prepare("INSERT INTO artist (name, uname) values (:name, :uname)");

	_insert_album_query = new QSqlQuery(db);
	_insert_album_query->prepare("INSERT INTO album (name, uname, artist_id, year) values (:name, :uname, :artist_id, :year)");

	_insert_track_query = new QSqlQuery(db);
	_insert_track_query->prepare("INSERT INTO tracks (title, utitle, artist_id, album_id, source, directory, length) values (:title, :utitle, :artist_id, :album_id, :source, :directory_id, :length)");

	_insert_date_query = new QSqlQuery(db);
	_insert_date_query->prepare("INSERT INTO adding_date (track_id, date) values (:track_id, strftime('%s', 'now'))");

	_insert_favorites_query = new QSqlQuery(db);
	_insert_favorites_query->prepare("INSERT INTO favorites (track_id) values (:track_id)");

	_insert_directory_query = new QSqlQuery(db);
	_insert_directory_query->prepare("INSERT INTO directories (path) values (:path)");

	_update_track_count_query = new QSqlQuery(db);
	_update_track_count_query->prepare("UPDATE tracks SET count = :count where id = :id");

	_remove_track_query = new QSqlQuery(db);
	_remove_track_query->prepare("DELETE FROM tracks WHERE id = :id");

	_remove_empty_albums_query = new QSqlQuery(db);
	_remove_empty_albums_query->prepare("DELETE FROM album WHERE album.id IN "
					    "(SELECT id FROM "
					    "(SELECT COUNT(tracks.id) AS cnt, album.id FROM "
					    "album LEFT OUTER JOIN tracks ON album.id = tracks.album_id "
					    "GROUP BY album.id) WHERE cnt = 0)");

	_remove_empty_artists_query = new QSqlQuery(db);
	_remove_empty_artists_query->prepare("DELETE FROM artist WHERE artist.id IN "
					     "(SELECT id FROM "
					     "(SELECT COUNT(tracks.id) AS cnt, artist.id FROM "
					     "artist LEFT OUTER JOIN tracks ON artist.id = tracks.artist_id "
					     "GROUP BY artist.id) WHERE cnt = 0)");

	_remove_tracks_from_query = new QSqlQuery(db);
	_remove_tracks_from_query->prepare("DELETE FROM tracks WHERE directory = :directory_id");

	_remove_directory_query = new QSqlQuery(db);
	_remove_directory_query->prepare("DELETE FROM directories WHERE path = :path");
}

void DbStorage::_create_database_structure() {
	QSqlQuery *query = new QSqlQuery(db);
	query->exec("create table artist (id integer primary key, "
								"name text, "
								"uname text "
								");");
	query->exec("create table album (id integer primary key, "
								"artist_id integer, "
								"name text, "
								"uname text, "
								"year int, "
								"foreign key(artist_id) references arist(id) "
								");");
	query->exec("create table tracks (id integer primary key, "
								"artist_id integer, "
								"album_id integer, "
								"title text, "
								"utitle text, "
								"source text, "
								"directory integer, "
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
	query->exec("create view entire as "
		    "select id, title, artist_name as artist, album_name as album, source, count, length, album_year as year, utitle, artist_uname, album_uname from "
			"tracks left join "
				"(select artist.id as aartist_id, "
				"artist.uname as artist_uname, "
				"artist.name as artist_name, "
				"album.uname as album_uname, "
				"album.year as album_year, "
				"album.id as aalbum_id, "
				"album.name as album_name from "
					"artist "
					"join album "
					"on album.artist_id = artist.id) "
				"on aartist_id = tracks.artist_id "
				"and aalbum_id = tracks.album_id");

	query->exec("create table directories (id integer primary key, path text)");
}

DbStorage::~DbStorage() {
	delete _get_albums_for_artist_sort_name_query;
	delete _get_albums_for_artist_sort_year_query;
	delete _get_artists_query;
	delete _get_favorites_query;
	delete _get_most_played_query;
	delete _get_never_played_query;
	delete _get_recently_added_query;
	delete _get_tracks_for_album_query;
	delete _get_tracks_by_pattern_query;
	delete _get_directories_query;
	delete _get_artists_count_query;
	delete _get_albums_count_query;
	delete _get_tracks_count_query;
	delete _get_tracks_source_from_query;
	delete _check_album_query;
	delete _check_artist_query;
	delete _check_directory_query;
	delete _get_track_id_by_source_query;
	delete _insert_album_query;
	delete _insert_artist_query;
	delete _insert_date_query;
	delete _insert_track_query;
	delete _insert_favorites_query;
	delete _insert_directory_query;
	delete _update_track_count_query;
	delete _remove_track_query;
	delete _remove_directory_query;
	delete _remove_tracks_from_query;
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

QMap<QString, int> DbStorage::getAlbumsForArtist(QString artist) {
	QMap<QString, int> albums;
	Config config;
	QString sort = config.getValue("ui/albumsorting").toString();
	QSqlQuery *query = NULL;
	if (sort == "date") {
		query = _get_albums_for_artist_sort_year_query;
	} else {
		query = _get_albums_for_artist_sort_name_query;
	}
	query->bindValue(":uname", artist.toUpper());
	query->exec();
	while (query->next()) {
		QString name = query->value(0).toString();
		albums[name] = query->value(1).toInt();
	}
	return albums;
}

QList<Track> DbStorage::getTracksForAlbum(QString album, QString artist) {
	QList<Track> tracks;
	QSqlQuery *query = _get_tracks_for_album_query;
	query->bindValue(":artist_uname", artist.toUpper());
	query->bindValue(":album_uname", album.toUpper());
	query->exec();

	while (query->next()) {
		QString title = query->value(1).toString();
		QString source = query->value(2).toString();
		int count = query->value(3).toInt();
		int length = query->value(4).toInt();
		TrackMetadata meta (title, artist, album, length);
		Track track(meta, source);
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
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(meta, source);
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
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(meta, source);
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
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(meta, source);
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
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		TrackMetadata meta(title, artist, album, length);
		Track track(meta, source);
		track.setCount(count);
		playlist.addTrack(track);
		playlist.setName("Recently added");
	}
	return playlist;
}

void DbStorage::removeTrack(Track track) {
	QSqlQuery *query = _get_track_id_by_source_query;
	query->bindValue(":source", track.source());
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		query = new QSqlQuery(db);
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
}

void DbStorage::addTrack(Track track) {
	QString title = track.metadata().title();
	QString artist = track.metadata().artist();
	QString album = track.metadata().album();
	QString source = track.source();
	QFileInfo info(source);
	QString path = info.canonicalPath();
	int year = track.metadata().year();
	int artist_id = _check_add_artist(artist);
	int album_id = _check_add_album(album, artist_id, year);
	if (artist_id == -1 || album_id == -1) {
		//big bang
		return;
	}
	QSqlQuery *query = _get_track_id_by_source_query;
	query->bindValue(":source", source);
	query->exec();
	if (query->next()) {
		// already in datebase, skip
		return;
	}
	query = _insert_track_query;
	query->bindValue(":title", title);
	query->bindValue(":utitle", title.toUpper());
	query->bindValue(":artist_id", artist_id);
	query->bindValue(":album_id", album_id);
	query->bindValue(":source", source);
	query->bindValue(":directory", _check_add_directory(path));
	query->bindValue(":length", track.metadata().length());
	if (query->exec()) {
		//ok
		query = _get_track_id_by_source_query;
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
	QSqlQuery *query = _get_track_id_by_source_query;
	query->bindValue(":source", track.source());
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		query = _insert_favorites_query;
		query->bindValue(":track_id", id);
		query->exec();
	}
}

void DbStorage::updateTrackCount(Track track) {
	QSqlQuery *query = _get_track_id_by_source_query;
	query->bindValue(":source", track.source());
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		query = _get_track_count_query;
		query->bindValue(":id", id);
		query->exec();
		if (query->next()) {
			int count = query->value(0).toInt();
			query = _update_track_count_query;
			query->bindValue(":count", count+1);
			query->bindValue(":id", id);
			query->exec();
		}
	}
}

Track DbStorage::updateTrack(Track track) {
	QSqlQuery *query = _get_track_id_by_source_query;
	query->bindValue(":source", track.source());
	query->exec();
	if (query->next()) { // found track in library
		int id = query->value(0).toInt();
		query = _remove_track_query;
		query->bindValue(":id", id);
		query->exec();
		addTrack(track);
		_cleanup();
	}
	return track;
}

int DbStorage::_check_add_artist(QString artist) {
	QSqlQuery *query = _check_artist_query;
	query->bindValue(":uname", artist.toUpper());
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		return id;
	} else {
		query = _insert_artist_query;
		query->bindValue(":name", artist);
		query->bindValue(":uname", artist.toUpper());
		if (query->exec()) {
			return _check_add_artist(artist);
		} else {
			// big bang
			return -1;
		}
	}
}

int DbStorage::_check_add_album(QString album, int artist_id, int year) {
	QSqlQuery *query = _check_album_query;
	query->bindValue(":uname", album.toUpper());
	query->bindValue(":artist_id", artist_id);
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		return id;
	} else {
		query = _insert_album_query;
		query->bindValue(":name", album);
		query->bindValue(":uname", album.toUpper());
		query->bindValue(":artist_id", artist_id);
		query->bindValue(":year", year);
		if (query->exec()) {
			return _check_add_album(album, artist_id, year);
		} else {
			// big bang
			return -1;
		}
	}
}

QList<Track> DbStorage::search(QString pattern) {
	QList <Track> found;
	QSqlQuery *query = _get_tracks_by_pattern_query;
	query->bindValue(":ptitle", pattern.toUpper()); // with :pattern only doesn't work
	query->bindValue(":palbum", pattern.toUpper());
	query->bindValue(":partist", pattern.toUpper());
	query->exec();
	// id, title, artist, album, source, count, length, year
	while (query->next()) {
		QString title = query->value(1).toString();
		QString artist = query->value(2).toString();
		QString album = query->value(3).toString();
		QString source = query->value(4).toString();
		int count = query->value(5).toInt();
		int length = query->value(6).toInt();
		int year = query->value(7).toInt();
		TrackMetadata meta(title, artist, album, length);
		meta.setYear(year);
		Track track(meta, source);
		track.setCount(count);
		found.append(track);
	}
	return found;
}

void DbStorage::_cleanup() {
	_remove_empty_albums_query->exec();
	_remove_empty_artists_query->exec();
}

int DbStorage::_check_add_directory(QString path) {
	QSqlQuery *query = _check_directory_query;
	query->bindValue(":path", path);
	query->exec();
	if (query->next()) {
		return query->value(0).toInt();
	} else {
		query = _insert_directory_query;
		query->bindValue(":path", path);
		query->exec();
		return _check_add_directory(path);
	}
}

QList<QString> DbStorage::getDirectories() {
	QSqlQuery *query = _get_directories_query;
	query->exec();
	QList<QString> directories;
	while (query->next()) {
		directories.append(query->value(1).toString());
	}
	return directories;
}

int DbStorage::getArtistsCount() {
	QSqlQuery *query = _get_artists_count_query;
	query->exec();
	if (query->next()) {
		return query->value(0).toInt();
	}
	return 0;
}

int DbStorage::getAlbumsCount() {
	QSqlQuery *query = _get_albums_count_query;
	query->exec();
	if (query->next()) {
		return query->value(0).toInt();
	}
	return 0;
}

int DbStorage::getTracksCount() {
	QSqlQuery *query = _get_tracks_count_query;
	query->exec();
	if (query->next()) {
		return query->value(0).toInt();
	}
	return 0;
}

void DbStorage::deleteTracksFrom(QString path) {
	QSqlQuery *query = _check_directory_query;
	query->bindValue(":path", path);
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		query = _remove_tracks_from_query;
		query->bindValue(":directory_id", id);
		query->exec();
		query = _remove_directory_query;
		query->bindValue(":path", path);
		query->exec();
		_cleanup();
	}
}

void DbStorage::checkTracksFrom(QString path) {
	QSqlQuery *query = _check_directory_query;
	query->bindValue(":path", path);
	query->exec();
	if (query->next()) {
		int id = query->value(0).toInt();
		query = _get_tracks_source_from_query;
		query->bindValue(":directory_id", id);
		query->exec();
		while (query->next()) {
			QString source = query->value(0).toString();
			if (!QFile::exists(source)) {
				Track track(TrackMetadata(), source);	// removeTrack uses only source field, so
				removeTrack(track);			// we can use this method
			}
		}
		_cleanup();
	}
}
