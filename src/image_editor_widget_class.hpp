#ifndef image_editor_widget_class_hpp
#define image_editor_widget_class_hpp

#include "misc_includes.hpp"

class image_editor_widget : public QScrollArea
{
	Q_OBJECT
	
public:		// variables
	QLabel* image_label;
	QImage image;
	bool modified;
	
public:		// functions
	image_editor_widget( QWidget* parent = 0 );
	
	bool open_image( const QString& file_name );
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	void paintEvent( QPaintEvent* event );
	void resizeEvent( QResizeEvent* event );
	
protected slots:		// slots
	
	
};


#endif		// image_editor_widget_class_hpp
