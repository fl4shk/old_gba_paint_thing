#include "image_editor_widget_class.hpp"

image_editor_widget::image_editor_widget( QWidget* parent ) 
	: QScrollArea(parent)
{
	setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	modified = false;
}

bool image_editor_widget::open_image( const QString& file_name )
{
	if ( !image.load(file_name) )
	{
		return false;
	}
	image_label = new QLabel;
	image_label->setPixmap(QPixmap::fromImage(image));
	setWidget(image_label);
	
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
	for ( int i=0; i<10; ++i )
	{
		for ( int j=0; j<10; ++j )
		{
			image.setPixel( 100 + i, 100 + j, 0 );
		}
	}
	
	image_label->setPixmap(QPixmap::fromImage(image));
	//setWidget(image_label);
	//update();
}

void image_editor_widget::resizeEvent( QResizeEvent* event )
{
	QScrollArea::resizeEvent(event);
}


