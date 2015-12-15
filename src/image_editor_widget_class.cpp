#include "image_editor_widget_class.hpp"

image_editor_widget::image_editor_widget( QWidget* parent ) 
	: QWidget(parent)
{
	setMouseTracking(true);
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	//cout << event->x() << ", " << event->y() << endl;
}

