#ifndef palette_manipulator_widget_class_hpp
#define palette_manipulator_widget_class_hpp

#include "sfml_canvas_widget_classes.hpp"
#include "palette_manipulator_core_widget_class.hpp"

class palette_manipulator_widget : public QWidget
{
public:		// variables
	QWidget* parent;
	
	palette_manipulator_core_widget* the_core_widget;
	QGridLayout* grid_layout;
	
	
public:		// functions
	palette_manipulator_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size );
	
protected:		// functions
	
	
};



#endif		// palette_manipulator_widget_class_hpp
