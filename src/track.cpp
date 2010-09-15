#include "track.h"
#include "tagresolver.h"

using namespace SomePlayer::DataObjects;

Track::Track() : QObject() {
}

Track::Track(int id, TrackMetadata metadata, QString source) : QObject() {
	_id = id;
	_metadata = metadata;
	_source = source;
}

Track::Track(const Track &track) : QObject() {
	this->_metadata = track.metadata();
	this->_source = track.source();
}

Track::Track(QString source) :QObject() {
	_resolver = new TagResolver(this);
	connect(_resolver, SIGNAL(decoded(Track)), this, SLOT(decoded(Track)));
	QStringList foo;
	foo << source;
	_resolver->decode(foo);
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

void Track::decoded(Track track) {
	_id = track.id();
	_source = track.source();
	_metadata = track.metadata();
	delete _resolver;
}

Track &Track::operator =(const Track &track) {
	_id = track.id();
	_source = track.source();
	_metadata = track.metadata();
	return *this;
}

Track::~Track() {}

bool Track::operator ==(const Track &track) {
	return _source == track._source;
}
