#ifndef __KLOCALALBUM_KALBUMCONTROLWIDGET_H__
#define __KLOCALALBUM_KALBUMCONTROLWIDGET_H__

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include "kalbummodel.h"
#include "kthumbnailproxymodel.h"

class KAlbumControlWidget : public QWidget
{
	Q_OBJECT

public:
	KAlbumControlWidget(QWidget *parent = nullptr);
	~KAlbumControlWidget();

	void setModel(KAlbumModel *model);
	void setProxyModel(KThumbnailProxyModel *proxyModel);
	void setSelectModel(QItemSelectionModel *albumSelectionModel);
protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void initConnect();

public slots:
	void createAlbum();
	void deleteAlbum();
	void updateSelection();
	void updateCheckBoxState(const QItemSelection &selected, const QItemSelection &deselected);
	//void selectAll();

private:
	QLabel *m_label;
	QPushButton *m_deleteButton;
	QCheckBox *m_selectCheckBox;
	QComboBox *m_sortComboBox;
	QListView *m_albumView;
	QPushButton *m_newButton;

	KAlbumModel *m_model;
	KThumbnailProxyModel *m_thumbnailModel;
	QItemSelectionModel *m_selectModel;
};


#endif //__KLOCALALBUM_KALBUMCONTROLWIDGET_H__

