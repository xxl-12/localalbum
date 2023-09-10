# 本地相册

## 主要功能点

1. 相册的创建;
2. 相册的浏览和管理,包括删除,切换,排序;
3. 导入图片到相册;
4. 相册内图片的浏览,管理,包括删除,排序;
5. 在独立窗口浏览选中图片,可以对其进行放缩,旋转等操作;轮播所在相册的图片;
6. 支持设置相册排序方式;
7. 设置相册内显示指定类型图片;设置相册内图片排序方式;设置相册内图片试图尺寸;
8. 设置轮播时间间隔;
9. 设置是否恢复窗口上次关闭位置和尺寸;

## 主要模块设计

1. ***\*数据层主要模块\****（data）：

2. - ***\*KDatabase:\**** 负责数据库操作，包括连接、查询、更新和维护数据库。
   - ***\*KDatabaseAccess:\**** 模块用于提供数据库访问接口。
   - ***\*struct Album:\**** 项目中使用的相册数据结构体。
   - ***\*struct Picture:\**** 项目中使用的图片数据结构体。

3. ***\**\*逻辑处理模块\*\**\***（logic）：

4. - ***\*KAlbumModel:\****  负责相册数据的管理，包括创建、编辑、删除相册等操作。
   - ***\*KPictureModel:\****  负责图片数据的管理，包括图片的添加、删除和查询等。
   - ***\*KThumbnailDelegate:\****  用于绘制相册和图片的显示。
   - ***\*KThumbnailProxyModel:\****  用于生成缩略图。

5. ***\**\*用户界面模块\*\**\***（ui）：

6. - ***\*KAlbumControlWidget:\****  模块包含相册控制小部件的实现，用户可以使用它来执行相册相关的操作。
   - ***\*KPictureControlWidget:\****  包含图片控制小部件的实现，用于执行图片相关的操作。
   - ***\*KSettingWindow:\****  设置窗口的实现，允许用户配置应用程序的设置。
   - ***\*KCustomTitleBar:\****  自定义标题栏的实现。
   - ***\*KImageViewer:\****  模块用于图片浏览的实现。
   - ***\*KImageViewWindow:\****  包含图片查看窗口的实现，包括图片查看的界面和控制。
   - ***\*KLocalAlbum:\****  本地相册的主窗口。