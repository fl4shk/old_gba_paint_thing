#include "image_editor_widget_class.hpp"

const QString image_editor_widget::default_parent_title 
	= QString("GBA Paint Thing");

image_editor_widget::image_editor_widget( QWidget* s_parent ) 
	: QWidget(s_parent), parent(s_parent), modified(false)
{
	setAttribute(Qt::WA_StaticContents);
	//setMouseTracking(true);
	
	parent->resize( 600, 600 );
	parent->setWindowTitle(default_parent_title);
	
	canvas_widget = new sfml_canvas_widget( this, QPoint( 0, 0 ), 
		QSize( 200, 200 ), string("the_powerup_gfx.png") );
	canvas_widget->open_image();
	
	hbox_layout = new QHBoxLayout(this);
	//vbox_layout = new QVBoxLayout(this);
	scroll_area = new QScrollArea(this);
	
	scroll_area->setWidget(canvas_widget);
	
	hbox_layout->addWidget(scroll_area);
	
	scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	//QPushButton* hello_button = new QPushButton( "Hello", this );
	//QPushButton* hello_button_2 = new QPushButton( "Hello 2", this );
	//
	//connect( hello_button, &QPushButton::clicked, this,
	//	&image_editor_widget::hello );
	//connect( hello_button_2, &QPushButton::clicked, this,
	//	&image_editor_widget::hello );
	//
	//
	//vbox_layout->addWidget( hello_button );
	//vbox_layout->addWidget( hello_button_2 );
	//
	//hbox_layout->addLayout( vbox_layout, Qt::AlignTop | Qt::AlignRight );
	////hbox_layout->addLayout( vbox_layout );
	
}

bool image_editor_widget::open_image
	( const std::string& s_image_file_name )
{
	return canvas_widget->open_image(s_image_file_name);
}

void image_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
{
	scroll_bar->setValue
		( ( canvas_widget->scale_factor * scroll_bar->value() ) 
		+ ( ( canvas_widget->scale_factor - 1 ) 
		* ( scroll_bar->pageStep() / 2 ) ));
}

bool image_editor_widget::zoom_in()
{
	if ( canvas_widget->scale_factor == 64 )
	{
		return false;
	}
	
	canvas_widget->scale_factor <<= 1;
	//image = image.scaled( image.width() << 1, image.height() << 1 );
	//update_image_label();
	
	sf::View current_view = canvas_widget->getView();
	current_view.zoom(2.0f);
	canvas_widget->setView(current_view);
	
	adjust_scroll_bar(scroll_area->horizontalScrollBar());
	adjust_scroll_bar(scroll_area->verticalScrollBar());
	
	return true;
}


bool image_editor_widget::zoom_out()
{
	if ( canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	canvas_widget->scale_factor >>= 1;
	//image = image.scaled( image.width() >> 1, image.height() >> 1 );
	//update_image_label();
	
	sf::View current_view = canvas_widget->getView();
	current_view.zoom(0.5f);
	canvas_widget->setView(current_view);
	
	adjust_scroll_bar(scroll_area->horizontalScrollBar());
	adjust_scroll_bar(scroll_area->verticalScrollBar());
	
	return true;
}

void image_editor_widget::keyPressEvent( QKeyEvent* event )
{
	if ( event->key() == Qt::Key_A )
	{
		zoom_in();
	}
	else if ( event->key() == Qt::Key_Z )
	{
		zoom_out();
	}
}


void image_editor_widget::mousePressEvent( QMouseEvent* event )
{
	//cout << event->x() << ", " << event->y() << endl;
	//if ( event->x() < 0 
	//	|| event->x() > ( image.width() / (int)canvas_widget->scale_factor )
	//	|| event->y() < 0 
	//	|| event->y() > ( image.height() 
	//	/ (int)canvas_widget->scale_factor ) )
	//{
	//	return;
	//}
	
	cout << event->x() << ", " << event->y() << endl;
	//cout << ( event->x() / (int)canvas_widget->scale_factor ) << ", " 
	//	<< ( event->y() / (int)canvas_widget->scale_factor ) << endl;
	
	sf::Vector2f event_pos_in_canvas_coords 
		= canvas_widget->mapPixelToCoords( sf::Vector2i( event->x(),
		event->y() ) );
	cout << event_pos_in_canvas_coords.x << ", " 
		<< event_pos_in_canvas_coords.y << endl;
	
	//image.setPixel( event->x() / (int)canvas_widget->scale_factor, 
	//	event->y() / (int)canvas_widget->scale_factor, 
	//	QColor( 255, 0, 0 ).rgb() );
	//update_image_label();
	//
	//modified = true;
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	mousePressEvent(event);
}

//void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}


void image_editor_widget::hello()
{
	cout << "Hello!\n";
}


