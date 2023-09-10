#ifndef __KLOCALALBUM_THUMBNAILDELEGATE_H__
#define __KLOCALALBUM_THUMBNAILDELEGATE_H__

#include <QStyledItemDelegate>

class KThumbnailDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	KThumbnailDelegate(QObject *parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	void paintSelectState(QPainter *painter, const QStyleOptionViewItem &option) const;
	void paintHoverState(QPainter *painter, const QStyleOptionViewItem &option) const;
};

#endif // __KLOCALALBUM_THUMBNAILDELEGATE_H__