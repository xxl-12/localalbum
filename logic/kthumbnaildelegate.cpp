#include "kthumbnaildelegate.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>

const unsigned int BANNER_HEIGHT = 20;
const unsigned int BANNER_COLOR = 0x303030;
const unsigned int BANNER_ALPHA = 200;
const unsigned int BANNER_TEXT_COLOR = 0xffffff;
const unsigned int HIGHLIGHT_ALPHA = 100;

KThumbnailDelegate::KThumbnailDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{
}

void KThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	QPixmap pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
	painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

	if (pixmap.isNull())
	{
		qDebug() << "KThumbnailDelegate::paint:null";
	}

	if (option.state.testFlag(QStyle::State_Selected))
	{
		paintSelectState(painter, option);
	}
	else if (option.state.testFlag(QStyle::State_MouseOver))
	{
		paintHoverState(painter, option);
	}

	painter->restore();
}

QSize KThumbnailDelegate::sizeHint(const QStyleOptionViewItem & /*option*/, const QModelIndex &index) const
{
	const QPixmap &pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
	return pixmap.size();
}

void KThumbnailDelegate::paintSelectState(QPainter *painter, const QStyleOptionViewItem &option) const
{
	// 设置蓝色画笔
	QPen pen(Qt::blue, 2);
	painter->save();
	painter->setPen(pen);

	// 绘制矩形边框
	QRect rect = option.rect.adjusted(1, 1, -1, -1);  // 为边框提供一些空间，防止与边界重叠
	painter->drawRect(rect);

	// 绘制右上角的选中的单选框
	int checkBoxSize = 16;
	QRect checkBoxRect(option.rect.topRight() - QPoint(checkBoxSize, 0), QSize(checkBoxSize, checkBoxSize));
	QStyleOptionButton checkBoxOption;
	checkBoxOption.rect = checkBoxRect;
	checkBoxOption.state = QStyle::State_Enabled | QStyle::State_On;  // 设置为选中状态
	QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);


	painter->restore();
}

void KThumbnailDelegate::paintHoverState(QPainter *painter, const QStyleOptionViewItem &option) const
{
	// 设置灰色画笔
	QPen pen(Qt::gray, 2);
	painter->save();
	painter->setPen(pen);

	// 绘制矩形边框
	QRect rect = option.rect.adjusted(1, 1, -1, -1);  // 为边框提供一些空间，防止与边界重叠
	painter->drawRect(rect);

	// 绘制右上角的单选框
	int checkBoxSize = 16;
	QRect checkBoxRect(option.rect.topRight() - QPoint(checkBoxSize, 0), QSize(checkBoxSize, checkBoxSize));
	QStyleOptionButton checkBoxOption;
	checkBoxOption.rect = checkBoxRect;
	checkBoxOption.state = QStyle::State_Enabled;
	QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);

	painter->restore();  // 恢复先前保存的画笔和画刷状态
}