#include "trackrenderer.h"
#include <QFont>
#include <QFontMetrics>
#include <QSize>
#include <QColor>

TrackRenderer::TrackRenderer(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void TrackRenderer::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QString value = index.data().toString();
	QStringList meta = value.split("#_#");

	QFont f = painter->font();
	QFont fp = painter->font();

	int x1, y1, x2, y2;
	option.rect.getCoords(&x1, &y1, &x2, &y2);

	QPen pen = painter->pen();
	QPen npen (QColor::fromRgb(80, 130, 255, 50));
	QPen apen (QColor::fromRgb(255, 255, 255, 128));
	QPen spen (QColor::fromRgb(100, 150, 220));

	f.setBold(true);
	painter->setFont(f);
	f.setBold(false);
	painter->setPen(npen);
	painter->drawLine(x1, y1, x2, y1);
	if (index.row() == _active_row) {
		painter->setPen(spen);
	} else {
		painter->setPen(pen);
	}
	painter->drawText(x1, y1 + 2*(y2-y1)/5, meta[0]);
	fp.setBold(false);
	fp.setPointSize(f.pointSize()*3/4);
	painter->setFont(fp);
	painter->setPen(apen);
	painter->drawText((x2+x1)/2, y1 + 4*(y2-y1)/6, meta[1]);
	painter->drawText((x2+x1)/2, y1 + 11*(y2-y1)/12, meta[2]);
	painter->setPen(npen);
	painter->drawLine(x1, y2, x2, y2);
	painter->setFont(f);
	painter->setPen(pen);
}

QSize TrackRenderer::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
	return QSize(option.rect.width(), option.rect.height()/3);
}

void TrackRenderer::setActiveRow(int r) {
	_active_row = r;
}
