#include "track.h"

using namespace SomePlayer::DataObjects;

Track::Track() {
}

Track::Track(TrackMetadata metadata, QUrl source) {
	_metadata = metadata;
	_source = source;
}

TrackMetadata Track::metadata() {
	return _metadata;
}

QUrl Track::source() const {
	return _source;
}

void Track::setSource(QUrl source) {
	_source = source;
}
