#include "image_editor_widget_class.hpp"

const QString image_editor_widget::default_parent_title 
	= QString("GBA Paint Thing");

image_editor_widget::image_editor_widget( QWidget* s_parent ) 
	: QScrollArea(s_parent), parent(s_parent), image_label(NULL),
	modified(false)
{
	
	setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
}

bool image_editor_widget::open_image( const QString& file_name )
{
	if ( !image.load(file_name) )
	{
		return false;
	}
	image_label = new QLabel;
	update_image_label();
	setWidget(image_label);
	
	//update();
	
	return true;
}

void image_editor_widget::mousePressEvent( QMouseEvent* event )
{
	if ( event->x() < 0 || event->x() > image.width() 
		|| event->y() < 0 || event->y() > image.height() )
	{
		return;
	}
	
	cout << event->x() << ", " << event->y() << endl;
	
	image.setPixel( event->x(), event->y(), 0 );
	update_image_label();
	
	modified = true;
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	if ( event->x() < 0 || event->x() > image.width() 
		|| event->y() < 0 || event->y() > image.height() )
	{
		return;
	}
	
	cout << event->x() << ", " << event->y() << endl;
	
	image.setPixel( event->x(), event->y(), 0 );
	update_image_label();
	
	modified = true;
}

//void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}

void image_editor_widget::paintEvent( QPaintEvent* event )
{
	update_image_label();
	//setWidget(image_label);
	//update();
}

//void image_editor_widget::resizeEvent( QResizeEvent* event )
//{
//	QScrollArea::resizeEvent(event);
//}


