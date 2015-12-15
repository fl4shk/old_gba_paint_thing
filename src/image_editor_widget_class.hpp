#ifndef image_editor_widget_class_hpp
#define image_editor_widget_class_hpp

#include "misc_includes.hpp"

class image_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	
	
public:		// functions
	image_editor_widget( QWidget* parent = 0 );
	
	void mouseMoveEvent( QMouseEvent* event );
	
protected slots:		// slots
	
	
};


#endif		// image_editor_widget_class_hpp
