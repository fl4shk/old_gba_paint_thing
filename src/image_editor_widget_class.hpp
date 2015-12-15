#ifndef image_editor_widget_class_hpp
#define image_editor_widget_class_hpp

#include "misc_includes.hpp"

class image_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	static const QString default_parent_title;
	
	QGridLayout* grid_layout;
	QWidget* parent;
	QScrollArea* scroll_area;
	QLabel* image_label;
	QImage image;
	bool modified;
	
public:		// functions
	image_editor_widget( QWidget* s_parent = 0 );
	
	bool open_image( const QString& file_name );
	
protected:		// functions
	inline void update_image_label()
	{
		image_label->setPixmap(QPixmap::fromImage(image));
	}
	
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	//void mouseReleaseEvent( QMouseEvent* event );
	void paintEvent( QPaintEvent* event );
	//void resizeEvent( QResizeEvent* event );
	
protected slots:		// slots.
	// It is not yet certain whether these are needed for the
	// image_editor_widget class.
	
	void hello();
	
};


#endif		// image_editor_widget_class_hpp
