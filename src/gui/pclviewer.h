#ifndef PCLVIEWER_H
#define PCLVIEWER_H

#include <iostream>

// Qt
#include <QMainWindow>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/console/time.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <QFileDialog>
#include "../import/importpcd.h"
#include "../controller.h"

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include "../method/SphereFollowing.h"
#include "../Model/Tree.h"
#include<vector>
#include<iostream>
#include<utility>
#include<pcl/visualization/pcl_plotter.h>
#include <QMessageBox>

 #include <QFileInfo>

#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>
#include <limits> 
#include <pcl/io/pcd_io.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <QScrollBar>
#include <QWebView>
 #include <QFile>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/visualization/point_picking_event.h>
#include <pcl/registration/icp.h>
#include <pcl/keypoints/harris_3d.h>
#include <pcl/features/fpfh.h>
#include <pcl/registration/ia_ransac.h>

#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#endif



#include "../export/writecsv.h"
#include "../export/exportply.h"

#include "../method/StemPointDetection.h"
#include "../method/set_coefficients.h"
#include "../method/method_coefficients.h"

#include "../../build/ui_pclviewer.h"
#include "../../build/ui_radius_dialog.h"
#include "../../build/ui_intensity_dialog.h"
#include "../../build/ui_statistical_dialog.h"
#include "../../build/ui_voxel_grid_dialog.h"
#include "../../build/ui_euclidean_dialog.h"
#include "../../build/ui_crop_sphere_dialog.h"
#include "../../build/ui_crop_box_dialog.h"
#include "../../build/ui_reference_dialog.h"
#include "../../build/ui_method_dialog.h"
#include "../../build/ui_allign_dialog.h"

typedef pcl::PointXYZRGBA PointD;
typedef pcl::PointCloud<PointD> PointCloudD;
typedef pcl::PointXYZINormal PointI;
typedef pcl::PointCloud<PointI> PointCloudI;
typedef pcl::PointCloud<pcl::PrincipalCurvatures> CurvatureCloud;


class SetCoefficients;


namespace Ui {
class PCLViewer;

}
namespace simpleTree {
class Tree;

}
class Controller;
class PCLViewer;
struct callback_args {
	// structure used to pass arguments to the callback function
	boost::shared_ptr<Ui_crop_box_dialog> box_dialog_ptr;
	boost::shared_ptr<Ui_crop_sphere_dialog> sphere_dialog_ptr;
	
	
	PCLViewer * viewer;
};
class PCLViewer: public QMainWindow,public  boost::enable_shared_from_this<PCLViewer>  {
Q_OBJECT
private:

	
	callback_args cb_args;
	boost::shared_ptr<Ui_crop_box_dialog> box_dialog_ui_ptr;
	boost::shared_ptr<Ui_crop_sphere_dialog> sphere_dialog_ui_ptr;
	boost::shared_ptr<Ui_method_dialog> method_dialog_ptr;
	boost::shared_ptr<SetCoefficients> coeff_ptr;
	boost::shared_ptr<simpleTree::Tree> tree_ptr;
	boost::shared_ptr<PointCloudI> allign_cloud;
	Method_Coefficients method_coefficients;

	pcl::console::TicToc tt;

	pcl::ModelCoefficients deleteSphere;
	pcl::ModelCoefficients deleteBox;

	float minX, minY, minZ, maxX, maxY, maxZ, extension = 1, centerX = 0,
			centerY = 0, centerZ = 0;
	float fac = 2.5;
	int point_color_max = 6;
	int tree_color_max = 4;
	int point_color;
	int tree_color = 0;

	float height = 0.2;

	int radius_outlier_minPts = 3;
	float radius_outlier_searchradius = 0.015f;

	float intensity_outlier_minIntens = 0.0;
	float intensity_outlier_maxIntens = 255.0;

	int statistical_outlier_knn = 5;
	float statistical_outlier_stdmult = 5.0;

	float voxel_grid_size = 0.01;

	int euclidean_clustering_minsize = 100;
	float euclidean_clustering_tolerance = 0.02;
	int euclidean_clustering_clusternumber = 1;

	void
	convertPointCloud(PointCloudI::Ptr);
	void
	computeBoundingBox();
	void
	computeNormals(PointCloudI::Ptr cloud);
	CurvatureCloud::Ptr
	computeCurvature(PointCloudI::Ptr cloud);

	boost::weak_ptr<Controller> control;
	QString consoleString;
	QWebView *qwebview;
	boost::shared_ptr<SetCoefficients> set_coefficients;

public:
//   virtual
//   void resize();
  int point = 1;
	 void
	  init();
	bool crop_box_is_active;
	bool crop_sphere_is_active;
	bool allign_clouds_is_active;
		boost::shared_ptr<Ui_dialog_init_allign> allign_dialog_ptr;
	
	explicit
	PCLViewer(QWidget *parent = 0);
	~PCLViewer();
	void
	connectToController(boost::shared_ptr<Controller> control);
	void
	setCloudPtr(PointCloudI::Ptr cloud);
	void
	setTreePtr(boost::shared_ptr<simpleTree::Tree> tree_ptr);
	boost::shared_ptr<Controller>
	getControl();
	void
	writeConsole(QString str);
	std::string
	selectFile();
	void
	plotAllometry();
	void
	plotIntensityHist();

public slots:
  void
  switch_point_for_ICP(int i);
  void
  compute_ICP();
  
  void
  compute_complete_folder();

	void
	compute_detectTree();
    void
    delete_method();
    void
    set_method(int i);
    void
    set_method_coeff();

	void
	set_reference_height(double h);
	void
	compute_reference();
	void
	reference_cloud();

	void
	set_crop_box_x(double x);
	void
	set_crop_box_y(double y);
	void
	set_crop_box_z(double z);
	void
	set_crop_box_x_length(double x);
	void
	set_crop_box_y_length(double y);
	void
	set_crop_box_z_length(double z);
	void
	crop_box();
	void
	compute_crop_box();
	void
	abort_crop_box();

	void
	set_crop_sphere_x(double x);
	void
	set_crop_sphere_y(double y);
	void
	set_crop_sphere_z(double z);
	void
	set_crop_sphere_r(double r);
	void
	cropsphere();
	void
	compute_crop_sphere();
	void
	abort_crop_sphere();

	void
	set_voxel_grid_size(double size);
	void
	compute_voxel_grid_downsampling();

	void
	set_statistical_outlier_knn(int knn);
	void
	set_statistical_outlier_stdmult(double stdmult);
	void
	compute_statistical_outlier_removal();

	void
	set_intensity_outlier_minIntens(double minIntens);
	void
	set_intensity_outlier_maxIntens(double maxIntens);
	void
	compute_intensity_outlier_removal();

	void
	set_radius_outlier_minPts(int minPts);
	void
	set_radius_outlier_searchradius(double searchRadius);
	void compute_radius_outlier_removal();

	void
	set_euclidean_clustering_minsize(int size);
	void
	set_euclidean_clustering_tolerance(double tolerance);
	void
	set_euclidean_clustering_clusternumber(int number);
	void
	compute_euclidean_clustering();

	void
	updateProgress(int);

	void
	changePointColor();
	
	void
	changeTreeColor();

	void
	plot();

	void
	importPCDFile();

	void
	exportPCDFile();

	void
	exportResults();

	void
	exportPly();

	void
	detectTree();

	void
	xNegView();

	void
	xPosView();

	void
	yNegView();

	void
	yPosView();

	void
	zNegView();

	void
	zPosView();

	void
	setPointTransparency(int value);

	void
	setTreeTransparency(int value);

	void
	setStemTransparency(int value);

	void
	setCrownTransparency(int value);

	void
	setLeaveTransparency(int value);

	void
	setPointSize(int value);

	void
	statisticalOutlierRemoval();

	void
	voxel_downsampling();

	void
	radiusOutlierRemoval();

	void
	intensityOutlierRemoval();

	void
	euclideanClustering();

	void
	computeNormals();

	void
	mergeClouds();

    const Method_Coefficients&
    getMethodCoefficients () const
    {
      return method_coefficients;
    }

    void
    setMethodCoefficients (const Method_Coefficients& methodCoefficients)
    {
      method_coefficients = methodCoefficients;
    }

protected:
  


	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	boost::shared_ptr<pcl::visualization::PCLPlotter> plotter;
	PointCloudD::Ptr cloud;
	std::vector<boost::shared_ptr<simpleTree::Cylinder> > cylinders;
	std::vector<boost::shared_ptr<simpleTree::Cylinder> > stemCylinders;

	unsigned int red;
	unsigned int green;
	unsigned int blue;

private:
	Ui::PCLViewer *ui;
	void
	generateData(double *ax, double *acos, double *asin, int numPoints);
	void
	generateCloud();

};

#endif // PCLVIEWER_H
