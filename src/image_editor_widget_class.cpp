#include "image_editor_widget_class.hpp"

const QString image_editor_widget::default_parent_title 
	= QString("GBA Paint Thing");

image_editor_widget::image_editor_widget( QWidget* s_parent ) 
	: QWidget(s_parent), parent(s_parent), modified(false), scale_factor(1)
{
	setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
	
	canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ), 
		QSize( 200, 200 ) );
	
	grid_layout = new QGridLayout(this);
	scroll_area = new QScrollArea(this);
	scroll_area->setWidget(canvas_widget);
	
	grid_layout->addWidget( scroll_area, 1, 1 );
	
	QPushButton* hello_button = new QPushButton( "Hello", this );
	grid_layout->addWidget( hello_button, 1, 2 );
	
	connect( hello_button, &QPushButton::clicked, this,
		&image_editor_widget::hello );
	
}

bool image_editor_widget::open_image( const QString& file_name )
{
	//if ( !image.load(file_name) )
	//{
	//	return false;
	//}
	//image_label = new QLabel;
	//update_image_label();
	//update();
	
	return true;
}

//void image_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue(( scale_factor * scroll_bar->value() ) 
//		+ ( ( scale_factor - 1 ) * ( scroll_bar->pageStep() / 2 ) ));
//}
//
//bool image_editor_widget::zoom_in()
//{
//	if ( scale_factor == 64 )
//	{
//		return false;
//	}
//	
//	scale_factor <<= 1;
//	image = image.scaled( image.width() << 1, image.height() << 1 );
//	update_image_label();
//	
//	adjust_scroll_bar(scroll_area->horizontalScrollBar());
//	adjust_scroll_bar(scroll_area->verticalScrollBar());
//	
//	return true;
//}
//
//
//bool image_editor_widget::zoom_out()
//{
//	if ( scale_factor == 1 )
//	{
//		return false;
//	}
//	
//	scale_factor >>= 1;
//	image = image.scaled( image.width() >> 1, image.height() >> 1 );
//	update_image_label();
//	
//	adjust_scroll_bar(scroll_area->horizontalScrollBar());
//	adjust_scroll_bar(scroll_area->verticalScrollBar());
//	
//	return true;
//}

void image_editor_widget::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_A )
	{
	}
	else if ( event->key() == Qt::Key_S )
	{
	}
}

void image_editor_widget::mousePressEvent( QMouseEvent* event )
{
	////cout << event->x() << ", " << event->y() << endl;
	//if ( event->x() < 0 
	//	|| event->x() > ( image.width() / (int)scale_factor )
	//	|| event->y() < 0 
	//	|| event->y() > ( image.height() / (int)scale_factor ) )
	//{
	//	return;
	//}
	
	//cout << event->x() << ", " << event->y() << endl;
	cout << ( event->x() / (int)scale_factor ) << ", " 
		<< ( event->y() / (int)scale_factor ) << endl;
	
	//image.setPixel( event->x() / (int)scale_factor, 
	//	event->y() / (int)scale_factor, QColor( 255, 0, 0 ).rgb() );
	//update_image_label();
	//
	//modified = true;
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	////cout << event->x() << ", " << event->y() << endl;
	//if ( event->x() < 0 
	//	|| event->x() > ( image.width() / (int)scale_factor )
	//	|| event->y() < 0 
	//	|| event->y() > ( image.height() / (int)scale_factor ) )
	//{
	//	return;
	//}
	
	//cout << event->x() << ", " << event->y() << endl;
	cout << ( event->x() / (int)scale_factor ) << ", " 
		<< ( event->y() / (int)scale_factor ) << endl;
	
	//image.setPixel( event->x() / (int)scale_factor, 
	//	event->y() / (int)scale_factor, QColor( 255, 0, 0 ).rgb() );
	//update_image_label();
	//
	//modified = true;
}

//void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}

void image_editor_widget::paintEvent( QPaintEvent* event )
{
	//update_image_label();
	update();
}

//void image_editor_widget::resizeEvent( QResizeEvent* event )
//{
//	QScrollArea::resizeEvent(event);
//}


void image_editor_widget::hello()
{
	cout << "Hello!\n";
}


