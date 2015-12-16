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
	vbox_layout = new QVBoxLayout(this);
	scroll_area = new QScrollArea(this);
	
	scroll_area->setWidget(canvas_widget);
	
	hbox_layout->addWidget(scroll_area);
	
	scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	
	
	// Show Horizontal ScrollBar Stuff
	QPushButton* show_horiz_sb_stuff_button 
		= new QPushButton( "Show Horiz", this );
	connect( show_horiz_sb_stuff_button, &QPushButton::clicked, this,
		&image_editor_widget::show_horizontal_scroll_bar_stuff );
	vbox_layout->addWidget(show_horiz_sb_stuff_button);
	
	// Show Vertical ScrollBar Stuff
	QPushButton* show_vert_sb_stuff_button 
		= new QPushButton( "Show Vert", this );
	connect( show_vert_sb_stuff_button, &QPushButton::clicked, this,
		&image_editor_widget::show_vertical_scroll_bar_stuff );
	vbox_layout->addWidget(show_vert_sb_stuff_button);
	
	// Show Geometry Stuff
	QPushButton* show_geometry_stuff_button
		= new QPushButton( "Show Geometry", this );
	connect( show_geometry_stuff_button, &QPushButton::clicked, this,
		&image_editor_widget::show_geometry_stuff );
	vbox_layout->addWidget(show_geometry_stuff_button);
	
	
	//hbox_layout->addLayout( vbox_layout, Qt::AlignTop | Qt::AlignRight );
	hbox_layout->addLayout( vbox_layout );
	
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
	
	canvas_widget->zoomed_in_recently = true;
	
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
	
	canvas_widget->zoomed_out_recently = true;
	
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
	cout << event->x() << ", " << event->y() << endl;
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2i event_pos_in_scroll_area_coords 
		= widget_pos_to_scroll_area_coords( event->x(), event->y() );
	cout << event_pos_in_scroll_area_coords.x << ", "
		<< event_pos_in_scroll_area_coords.y << endl;
	
	sf::Vector2f event_pos_in_canvas_coords
		= canvas_widget->mapPixelToCoords( event_pos_in_scroll_area_coords,
		canvas_widget->get_apparent_view() );
	cout << event_pos_in_canvas_coords.x << ", " 
		<< event_pos_in_canvas_coords.y << "\n\n";
	
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
	//cout << scroll_area->viewport()->size().width() << ", "
	//	<< scroll_area->viewport()->size().height() << endl;
}

//void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}


void image_editor_widget::hello()
{
	cout << "Hello!\n";
}


void image_editor_widget::show_horizontal_scroll_bar_stuff()
{
	cout << scroll_area->horizontalScrollBar()->value() << " "
		<< scroll_area->horizontalScrollBar()->minimum() << " "
		<< scroll_area->horizontalScrollBar()->maximum() << endl;
}

void image_editor_widget::show_vertical_scroll_bar_stuff()
{
	cout << scroll_area->verticalScrollBar()->value() << " "
		<< scroll_area->verticalScrollBar()->minimum() << " "
		<< scroll_area->verticalScrollBar()->maximum() << endl;
}

void image_editor_widget::show_geometry_stuff()
{
	cout << scroll_area->geometry().x() << " " 
		<< scroll_area->geometry().y() << endl;
	cout << scroll_area->frameGeometry().x() << " " 
		<< scroll_area->frameGeometry().y() << endl;
}


