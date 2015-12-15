#include "image_editor_widget_class.hpp"

image_editor_widget::image_editor_widget( QWidget* parent ) 
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
}

bool image_editor_widget::open_image( const QString& file_name )
{
	if ( !image.load(file_name) )
	{
		return false;
	}
	
	update();
	
	return true;
}

void image_editor_widget::mousePressEvent( QMouseEvent* event )
{
	cout << event->x() << ", " << event->y() << endl;
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	cout << event->x() << ", " << event->y() << endl;
}

void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
{
	cout << event->x() << ", " << event->y() << endl;
}

void image_editor_widget::paintEvent( QPaintEvent* event )
{
	QPainter painter(this);
	QRect rect = event->rect();
	painter.drawImage( rect, image, rect );
}

void image_editor_widget::resizeEvent( QResizeEvent* event )
{
	QWidget::resizeEvent(event);
}


