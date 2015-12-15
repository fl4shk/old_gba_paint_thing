#ifndef image_editor_widget_class_hpp
#define image_editor_widget_class_hpp

#include "misc_includes.hpp"
#include "sfml_canvas_widget_classes.hpp"

class image_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	static const QString default_parent_title;
	
	QGridLayout* grid_layout;
	QWidget* parent;
	QScrollArea* scroll_area;
	
	sfml_canvas_widget* canvas_widget; 
	
	bool modified;
	
	u32 scale_factor;
	
public:		// functions
	image_editor_widget( QWidget* s_parent = 0 );
	
	bool open_image( const QString& file_name );
	
protected:		// functions
	//void adjust_scroll_bar( QScrollBar* scroll_bar );
	//bool zoom_in();
	//bool zoom_out();
	
	// Events
	void keyPressEvent( QKeyEvent* event );
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	//void mouseReleaseEvent( QMouseEvent* event );
	void paintEvent( QPaintEvent* event );
	//void resizeEvent( QResizeEvent* event );
	
	
protected slots:		// slots.
	void hello();
	
};


#endif		// image_editor_widget_class_hpp
