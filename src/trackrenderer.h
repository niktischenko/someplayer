#ifndef TRACKRENDERER_H
#define TRACKRENDERER_H

#include <QAbstractItemDelegate>
#include <QPainter>

class TrackRenderer : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit TrackRenderer(QObject *parent = 0);
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
								const QModelIndex &index) const;

	QSize sizeHint(const QStyleOptionViewItem &option,
						   const QModelIndex &index) const;

signals:

public slots:
	void setActiveRow(int);
	void setSearchRow(int);
	int activeRow() {return _active_row;}
	int searchRow() {return _search_row;}
private:
	int _active_row;
	int _search_row;

};

#endif // TRACKRENDERER_H
