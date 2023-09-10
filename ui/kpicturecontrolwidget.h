#ifndef __KLOCALALBUM_KPICTURECONTROLWIDGET_H__
#define __KLOCALALBUM_KPICTURECONTROLWIDGET_H__

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QListView>
#include "kpicturemodel.h"
#include "kthumbnailproxymodel.h"

class KPictureControlWidget : public QWidget
{
	Q_OBJECT

public:
	KPictureControlWidget(QWidget *parent);
	~KPictureControlWidget();

	void setAlbumModel(KAlbumModel *albumModel);
	void setAlbumSelectionModel(QItemSelectionModel *albumSelectionModel);
	void setPictureModel(KThumbnailProxyModel *pictureModel);
	void setPictureSelectModel(QItemSelectionModel *selectionModel);

protected:
	void paintEvent(QPaintEvent *event) override;

public slots:
	void saveAlbumName();
	void setAlbumCover();
	void importImage();
	void deleteImage();
	void selectAllImages(bool selected);
	void changeImageType(int index);
	void changeSortOrder(int index);
	void changeImageSize(int index);
	void clearUi();

private:
	void initConnect();
	void loadAlbum(const QModelIndex &albumIndex);

private:
	QWidget *m_albumSettingWidget;
	QLineEdit *m_albumNameEdit;
	QPushButton *m_saveAlbumButton;
	QPushButton *m_setCoverButton;

	QWidget *m_pictureControlWidget;
	QPushButton *m_importImageButton;
	QPushButton *m_deleteImageButton;
	QCheckBox *m_selectAllCheckBox;
	QComboBox *m_imageTypeComboBox;
	QComboBox *m_sortComboBox;
	QComboBox *m_sizeComboBox;

	QWidget *m_controlBar;

	QListView *m_pictureView;

	KAlbumModel *m_albumModel;
	QItemSelectionModel *m_albumSelectionModel;

	KThumbnailProxyModel *m_thumbnailModel;
	KPictureModel *m_model;
	QItemSelectionModel *m_selectModel;
};


#endif //__KLOCALALBUM_KPICTURECONTROLWIDGET_H__

