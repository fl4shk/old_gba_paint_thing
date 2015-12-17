#include "image_editor_widget_class.hpp"

const QString image_editor_widget::default_parent_title 
	= QString("GBA Paint Thing");

image_editor_widget::image_editor_widget( QWidget* s_parent ) 
	: QWidget(s_parent), parent(s_parent)
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

//void image_editor_widget::adjust_scroll_bar( QScrollBar* scroll_bar )
//{
//	scroll_bar->setValue
//		( ( canvas_widget->scale_factor * scroll_bar->value() ) 
//		+ ( ( canvas_widget->scale_factor - 1 ) 
//		* ( scroll_bar->pageStep() / 2 ) ));
//}

bool image_editor_widget::zoom_in()
{
	if ( canvas_widget->scale_factor == 64 )
	{
		return false;
	}
	
	canvas_widget->scale_factor <<= 1;
	canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() << 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() << 1 );
	
	return true;
}


bool image_editor_widget::zoom_out()
{
	if ( canvas_widget->scale_factor == 1 )
	{
		return false;
	}
	
	canvas_widget->scale_factor >>= 1;
	canvas_widget->zoomed_recently = true;
	
	scroll_area->horizontalScrollBar()->setValue
		( scroll_area->horizontalScrollBar()->value() >> 1 );
	scroll_area->verticalScrollBar()->setValue
		( scroll_area->verticalScrollBar()->value() >> 1 );
	
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
	// This converts the clicked coordinate to pixel coordinates.
	//sf::Vector2i event_pos_in_scroll_area_coords 
	//	= widget_pos_to_scroll_area_coords( event->x(), event->y() );
	//
	//sf::Vector2f event_pos_in_image_coords
	//	= canvas_widget->mapPixelToCoords( event_pos_in_scroll_area_coords,
	//	canvas_widget->get_apparent_view() );
	
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_image_coords
		= canvas_widget->mapPixelToCoords( widget_pos_to_scroll_area_coords
		( event->x(), event->y() ), canvas_widget->get_apparent_view() );
	
	sf::Vector2i event_pos_in_image_pixel_coords
		= sf::Vector2i( (int)event_pos_in_image_coords.x,
		(int)event_pos_in_image_coords.y );
	
	// Check whether the mouse was clicked somewhere inside the image.
	if ( event_pos_in_image_pixel_coords.x < 0 
		|| ( event_pos_in_image_pixel_coords.x 
		> (int)canvas_widget->image.getSize().x )
		|| event_pos_in_image_pixel_coords.y < 0 
		|| ( event_pos_in_image_pixel_coords.y 
		> (int)canvas_widget->image.getSize().y ) )
	{
		return;
	}
	
	//cout << "valid image coordinate!\n";
	
	canvas_widget->modified_recently = true;
	canvas_widget->image.setPixel( (u32)event_pos_in_image_pixel_coords.x,
		(u32)event_pos_in_image_pixel_coords.y, sf::Color( 0, 0, 0 ) );
	
	prev_mouse_pos = event->pos();
}

void image_editor_widget::mouseMoveEvent( QMouseEvent* event )
{
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_image_coords
		= canvas_widget->mapPixelToCoords( widget_pos_to_scroll_area_coords
		( event->x(), event->y() ), canvas_widget->get_apparent_view() );
	
	sf::Vector2i event_pos_in_image_pixel_coords
		= sf::Vector2i( (int)event_pos_in_image_coords.x,
		(int)event_pos_in_image_coords.y );
	
	// Check whether the mouse was clicked somewhere inside the image.
	if ( event_pos_in_image_pixel_coords.x < 0 
		|| ( event_pos_in_image_pixel_coords.x 
		> (int)canvas_widget->image.getSize().x )
		|| event_pos_in_image_pixel_coords.y < 0 
		|| ( event_pos_in_image_pixel_coords.y 
		> (int)canvas_widget->image.getSize().y ) )
	{
		return;
	}
	
	sf::Vector2f prev_mouse_pos_in_image_coords
		= canvas_widget->mapPixelToCoords( widget_pos_to_scroll_area_coords
		( prev_mouse_pos.x(), prev_mouse_pos.y() ), 
		canvas_widget->get_apparent_view() );
	
	sf::Vector2i prev_mouse_pos_in_image_pixel_coords
		= sf::Vector2i( (int)prev_mouse_pos_in_image_coords.x,
		(int)prev_mouse_pos_in_image_coords.y );
	
	if ( prev_mouse_pos_in_image_pixel_coords.x < 0 
		|| ( prev_mouse_pos_in_image_pixel_coords.x 
		> (int)canvas_widget->image.getSize().x )
		|| prev_mouse_pos_in_image_pixel_coords.y < 0 
		|| ( prev_mouse_pos_in_image_pixel_coords.y 
		> (int)canvas_widget->image.getSize().y ) )
	{
		return;
	}
	
	//cout << "valid image coordinate!\n";
	
	canvas_widget->modified_recently = true;
	draw_line( prev_mouse_pos_in_image_pixel_coords,
		event_pos_in_image_pixel_coords, sf::Color::Black );
	
	
	prev_mouse_pos = event->pos();
}

//void image_editor_widget::mouseReleaseEvent( QMouseEvent* event )
//{
//	//cout << event->x() << ", " << event->y() << endl;
//}

void image_editor_widget::draw_line( const sf::Vector2i& pos_0, 
	const sf::Vector2i& pos_1, const sf::Color& color )
{
	sf::Vector2i delta, pixel_coord, offset;
	
	delta = sf::Vector2i( pos_1.x - pos_0.x, pos_1.y - pos_0.y );
	
	if ( delta.x < 0 )
	{
		delta.x = -delta.x;
	}
	if ( delta.y < 0 )
	{
		delta.y = -delta.y;
	}
	
	pixel_coord = pos_0;
	
	if ( pos_0.x > pos_1.x )
	{
		offset.x = -1;
	}
	else
	{
		offset.x = 1;
	}
	
	if ( pos_0.y > pos_1.y )
	{
		offset.y = -1;
	}
	else
	{
		offset.y = 1;
	}
	
	//m4_plot ( pixel_coord.x, pixel_coord.y, color_id, page );
	canvas_widget->image.setPixel( (u32)pixel_coord.x, (u32)pixel_coord.y, 
		color );
	
	if ( delta.x > delta.y )
	{
		s32 error = delta.x >> 1;
		
		while ( pixel_coord.x != pos_1.x )
		{
			error -= delta.y;
			
			if ( error < 0 )
			{
				pixel_coord.y += offset.y;
				error += delta.x;
			}
			
			pixel_coord.x += offset.x;
			
			canvas_widget->image.setPixel( (u32)pixel_coord.x, 
				(u32)pixel_coord.y, color );
		}
		
	}
	else
	{
		s32 error = delta.y >> 1;
		
		while ( pixel_coord.y != pos_1.y )
		{
			error -= delta.x;
			
			if ( error < 0 )
			{
				pixel_coord.x += offset.x;
				error += delta.y;
			}
			
			pixel_coord.y += offset.y;
			
			canvas_widget->image.setPixel( (u32)pixel_coord.x, 
				(u32)pixel_coord.y, color );
		}
		
	}
	
}


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


