#include "palette_manipulator_widget_class.hpp"

palette_manipulator_widget::palette_manipulator_widget( QWidget* s_parent,
	const QPoint& s_position, const QSize& s_size ) : QWidget(s_parent),
	parent(s_parent)
{
	move(s_position);
	resize(s_size);
	
	the_core_widget = new palette_manipulator_core_widget( this, QPoint(), 
		QSize() );
	
	grid_layout = new QGridLayout(this);
	grid_layout->addWidget( the_core_widget, 0, 0 );
	
}


