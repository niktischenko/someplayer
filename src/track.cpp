#include "track.h"

using namespace SomePlayer::DataObjects;

Track::Track() {
}

Track::Track(int id, TrackMetadata metadata, QString source) {
	_id = id;
	_metadata = metadata;
	_source = source;
}

Track::Track(const Track &track) {
	this->_metadata = track.metadata();
	this->_source = track.source();
}

TrackMetadata Track::metadata() const {
	return _metadata;
}

QString Track::source() const {
	return _source;
}

int Track::id() const {
	return _id;
}

void Track::setSource(QString source) {
	_source = source;
}

int Track::count() const{
	return _count;
}

void Track::setCount(int count) {
	_count = count;
}
