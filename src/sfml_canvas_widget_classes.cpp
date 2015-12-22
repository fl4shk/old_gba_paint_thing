#include "sfml_canvas_widget_classes.hpp"

#ifdef Q_WS_X11
	#include <Qt/QtX11Extras/qx11info_x11.h>
	#include <X11/Xlib.h>
#endif

sfml_canvas_widget_base::sfml_canvas_widget_base( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, u32 frame_time )
	: QWidget(s_parent), parent(s_parent), initialized(false)
{
	// Set up some states to allow direct rendering into the widget.
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	
	// Set strong focus to enable keyboard events (and maybe mouse events?)
	// to be received.
	setFocusPolicy(Qt::StrongFocus);
	
	// Set up the widget geometry.
	move(s_position);
	resize(s_size);
	
	// Set up the timer.
	timer.setInterval(frame_time);
	
}

sfml_canvas_widget_base::~sfml_canvas_widget_base()
{
}

void sfml_canvas_widget_base::on_init()
{
}
void sfml_canvas_widget_base::on_update()
{
}


QPaintEngine* sfml_canvas_widget_base::paintEngine() const
{
	return 0;
}

void sfml_canvas_widget_base::showEvent( QShowEvent* event )
{
	if (!initialized)
	{
		// Under X11, the commands sent to the server needs to be flushed
		// to ensure that SFML will get an updated view of the windows.
		#ifdef Q_WS_X11
			XFlush(QX11Info::display());
		#endif
		
		// Create the SFML window with the widget handle.
		sf::RenderWindow::create(winId());
		
		// Let the derived class do its specific stuff.
		on_init();
		
		// Set up the timer to trigger a refresh at specified framerate.
		connect( &timer, SIGNAL(timeout()), this, SLOT(repaint()) );
		timer.start();
		
		initialized = true;
	}
}

void sfml_canvas_widget_base::paintEvent( QPaintEvent* event )
{
	// Let the derived class do its specific stuff.
	on_update();
	
	// Display on screen.
	display();
}


sfml_canvas_widget::sfml_canvas_widget( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, 
	const string& s_image_file_name ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	the_palette_manipulator_core_widget(NULL), 
	
	image_file_name(s_image_file_name), 
	
	pixel_grid_enabled_recently(false), pixel_grid_enabled(false),
	
	tile_grid_enabled_recently(false), tile_grid_enabled(false), 
	
	modified_recently(false), zoomed_recently(false), scale_factor(1), 
	
	view_center_x(0.0f), view_center_y(0.0f)
{
	apparent_view = getDefaultView();
	
	canvas_image.reset(new sf::Image);
	canvas_texture.reset(new sf::Texture);
	canvas_sprite.reset(new sf::Sprite);
}



bool sfml_canvas_widget::open_image()
{
	if ( the_palette_manipulator_core_widget == NULL )
	{
		cout << "Bug caused by the programmer:  "
			<< "the_palette_manipulator_core_widget == NULL\n";
		return false;
	}
	
	fstream image_file( image_file_name, ios_base::in | ios_base::binary );
	
	if ( !image_file.is_open() )
	{
		cout << "Unable to open " << image_file_name << " for reading.\n";
		return false;
	}
	
	png::reader<fstream> the_reader(image_file);
	
	try
	{
		the_reader.read_info();
	}
	catch ( std::exception const& error )
	{
		cout << "The file called " << image_file_name << " doesn't appear "
			<< "to be a PNG file!\n";
		cout << error.what() << endl;
		return false;
	}
	
	if ( !canvas_image->loadFromFile(image_file_name) )
	{
		cout << "Unable to open " << image_file_name << " for reading.\n";
		return false;
	}
	canvas_texture->loadFromImage(*canvas_image);
	canvas_sprite->setTexture(*canvas_texture);
	full_resize(QSize( canvas_image->getSize().x, 
		canvas_image->getSize().y ));
	
	// Palette stuff
	if ( the_reader.get_color_type() == png::color_type_palette )
	{
		cout << "Paletted image!\n";
		
		png::image<png::index_pixel> index_pixel_image(image_file_name);
		the_palette_manipulator_core_widget
			->extract_palette_from_paletted_image(index_pixel_image);
	}
	else
	{
		cout << "Un-paletted image!\n";
		
		if ( !the_palette_manipulator_core_widget
			->extract_palette_from_sfml_image(*canvas_image) )
		{
			return false;
		}
		
	}
	
	
	return true;
}

const sf::View& sfml_canvas_widget::get_apparent_view()
{
	apparent_view = getDefaultView();
	
	apparent_view.setCenter( view_center_x, view_center_y );
	
	apparent_view.move( (double)( canvas_image->getSize().x ) 
		/ (double)2.0f, (double)( canvas_image->getSize().y )
		/ (double)2.0f );
		
	apparent_view.zoom( 1.0f / (double)scale_factor );
	
	return apparent_view;
}

void sfml_canvas_widget::draw_line( const sf::Vector2i& pos_0, 
	const sf::Vector2i& pos_1, const sf::Color& color )
{
	modified_recently = true;
	
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
	
	if ( point_is_in_image(pixel_coord) )
	{
		canvas_image->setPixel( (u32)pixel_coord.x, (u32)pixel_coord.y, 
			color );
	}
	
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
			
			if ( point_is_in_image(pixel_coord) )
			{
				canvas_image->setPixel( (u32)pixel_coord.x, 
					(u32)pixel_coord.y, color );
			}
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
			
			if ( point_is_in_image(pixel_coord) )
			{
				canvas_image->setPixel( (u32)pixel_coord.x, 
					(u32)pixel_coord.y, color );
			}
		}
	}
	
}

void sfml_canvas_widget::enable_pixel_grid()
{
	pixel_grid_enabled_recently = true;
	pixel_grid_enabled = true;
	
}

void sfml_canvas_widget::disable_pixel_grid()
{
	pixel_grid_enabled = false;
}

void sfml_canvas_widget::enable_tile_grid()
{
	tile_grid_enabled_recently = true;
	tile_grid_enabled = true;
	
}

void sfml_canvas_widget::disable_tile_grid()
{
	tile_grid_enabled = false;
}

void sfml_canvas_widget::export_file_as( const string& output_file_name )
{
	png::image<png::index_pixel> index_pixel_image;
	png::palette index_pixel_image_palette;
	
	index_pixel_image.resize( canvas_image->getSize().x,
		canvas_image->getSize().y );
	
	const auto& palette = the_palette_manipulator_core_widget->palette;
	
	for ( const sf::Color& the_sfml_color : palette )
	{
		index_pixel_image_palette.push_back
			( sfml_color_to_png_color(the_sfml_color) );
	}
	index_pixel_image.set_palette(index_pixel_image_palette);
	
	for ( u32 j=0; j<canvas_image->getSize().y; ++j )
	{
		// Loop across rows before columns.
		for ( u32 i=0; i<canvas_image->getSize().x; ++i )
		{
			for ( u32 k=0; k<palette.size(); ++k )
			{
				if ( canvas_image->getPixel( i, j ) == palette.at(k) )
				{
					index_pixel_image.set_pixel( i, j, 
						png::index_pixel(k) );
					break;
				}
			}
		}
	}
	
	index_pixel_image.write( output_file_name.c_str() );
	
}

void sfml_canvas_widget::mousePressEvent( QMouseEvent* event )
{
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_image_coords = mapPixelToCoords
		( sf::Vector2i( event->x(), event->y() ), get_apparent_view() );
	
	sf::Vector2i event_pos_in_image_pixel_coords
		= sf::Vector2i( (int)event_pos_in_image_coords.x,
		(int)event_pos_in_image_coords.y );
	
	prev_mouse_pos = event->pos();
	
	// Check whether the mouse was clicked somewhere inside the image.
	if ( !point_is_in_image(event_pos_in_image_pixel_coords) )
	{
		return;
	}
	
	modified_recently = true;
	
	if ( event->button() == Qt::LeftButton )
	{
		canvas_image->setPixel( (u32)event_pos_in_image_pixel_coords.x,
			(u32)event_pos_in_image_pixel_coords.y, 
			the_palette_manipulator_core_widget->palette.at
			(the_palette_manipulator_core_widget
			->left_current_color_index) );
	}
	else if ( event->button() == Qt::RightButton )
	{
		canvas_image->setPixel( (u32)event_pos_in_image_pixel_coords.x,
			(u32)event_pos_in_image_pixel_coords.y, 
			the_palette_manipulator_core_widget->palette.at
			(the_palette_manipulator_core_widget
			->right_current_color_index) );
	}
	
}

void sfml_canvas_widget::mouseMoveEvent( QMouseEvent* event )
{
	// This converts the clicked coordinate to pixel coordinates.
	sf::Vector2f event_pos_in_image_coords = mapPixelToCoords
		( sf::Vector2i( event->x(), event->y() ), get_apparent_view() );
	
	sf::Vector2i event_pos_in_image_pixel_coords
		= sf::Vector2i( (int)event_pos_in_image_coords.x,
		(int)event_pos_in_image_coords.y );
	
	// Check whether the mouse was moved to somewhere inside the image.
	if ( !point_is_in_image(event_pos_in_image_pixel_coords) )
	{
		prev_mouse_pos = event->pos();
		return;
	}
	
	sf::Vector2f prev_mouse_pos_in_image_coords = mapPixelToCoords
		( sf::Vector2i( prev_mouse_pos.x(), prev_mouse_pos.y() ), 
		get_apparent_view() );
	
	sf::Vector2i prev_mouse_pos_in_image_pixel_coords
		= sf::Vector2i( (int)prev_mouse_pos_in_image_coords.x,
		(int)prev_mouse_pos_in_image_coords.y );
	
	prev_mouse_pos = event->pos();
	
	//modified_recently = true;
	//draw_line( prev_mouse_pos_in_image_pixel_coords,
	//	event_pos_in_image_pixel_coords, sf::Color::Black );
	
	if ( event->buttons() == Qt::LeftButton )
	{
		draw_line( prev_mouse_pos_in_image_pixel_coords,
			event_pos_in_image_pixel_coords, 
			the_palette_manipulator_core_widget->palette.at
			(the_palette_manipulator_core_widget
			->left_current_color_index) );
	}
	else if ( event->buttons() == Qt::RightButton )
	{
		draw_line( prev_mouse_pos_in_image_pixel_coords,
			event_pos_in_image_pixel_coords, 
			the_palette_manipulator_core_widget->palette.at
			(the_palette_manipulator_core_widget
			->right_current_color_index) );
	}
	
}

// mouseReleaseEvent() will come in handy once a "line" tool exists.
void sfml_canvas_widget::mouseReleaseEvent( QMouseEvent* event )
{
	//cout << event->x() << ", " << event->y() << endl;
}


void sfml_canvas_widget::generate_canvas_pixel_grid()
{
	if ( scale_factor < minimum_scale_factor_for_pixel_grid )
	{
		canvas_pixel_grid_render_texture.create( 1, 1 );
		canvas_pixel_grid_render_texture.clear(sf::Color::Black);
		return;
	}
	
	if (pixel_grid_enabled_recently)
	{
		pixel_grid_enabled_recently = false;
	}
	
	if ( get_pixel_grid_enabled() )
	{
		canvas_pixel_grid_render_texture.create( getSize().x, 
			getSize().y );
		//canvas_grid_render_texture.clear(sf::Color::Black);
		
		//sf::Image canvas_pixel_grid_slot_image;
		canvas_pixel_grid_slot_image.reset(new sf::Image);
		canvas_pixel_grid_slot_image->create( scale_factor, scale_factor, 
			sf::Color( 0, 0, 0, 0 ) );
		
		// Vertical line
		for ( u32 j=0; j<canvas_pixel_grid_slot_image->getSize().y; ++j )
		{
			canvas_pixel_grid_slot_image->setPixel
				( canvas_pixel_grid_slot_image->getSize().x - 1, j,
				sf::Color( 64, 64, 64, 255 ) );
		}
		
		// Horizontal line
		for ( u32 i=0; i<canvas_pixel_grid_slot_image->getSize().x; ++i )
		{
			canvas_pixel_grid_slot_image->setPixel( i, 
				canvas_pixel_grid_slot_image->getSize().y - 1,
				sf::Color( 64, 64, 64, 255 ) );
		}
		
		canvas_pixel_grid_slot_texture.reset(new sf::Texture);
		canvas_pixel_grid_slot_texture->loadFromImage
			(*canvas_pixel_grid_slot_image);
		
		canvas_pixel_grid_slot_sprite.reset(new sf::Sprite);
		canvas_pixel_grid_slot_sprite->setTexture
			(*canvas_pixel_grid_slot_texture);
		
		for ( u32 j=0; j<canvas_image->getSize().y; ++j )
		{
			for ( u32 i=0; i<canvas_image->getSize().x; ++i )
			{
				canvas_pixel_grid_slot_sprite->setPosition
					( i * canvas_pixel_grid_slot_image->getSize().x, 
					j * canvas_pixel_grid_slot_image->getSize().y );
				canvas_pixel_grid_render_texture.draw
					(*canvas_pixel_grid_slot_sprite);
			}
		}
		
		canvas_pixel_grid_render_texture.display();
	}
	
}


void sfml_canvas_widget::generate_canvas_tile_grid()
{
	if ( scale_factor < minimum_scale_factor_for_tile_grid )
	{
		canvas_tile_grid_render_texture.create( 1, 1 );
		canvas_tile_grid_render_texture.clear(sf::Color::Black);
		return;
	}
	
	if (tile_grid_enabled_recently)
	{
		tile_grid_enabled_recently = false;
	}
	
	if ( get_tile_grid_enabled() )
	{
		canvas_tile_grid_render_texture.create( getSize().x, getSize().y );
		
		//sf::Image canvas_pixel_grid_slot_image;
		canvas_tile_grid_slot_image.reset(new sf::Image);
		canvas_tile_grid_slot_image->create
			( num_pixels_per_tile_row * scale_factor, 
			num_pixels_per_tile_column * scale_factor, 
			sf::Color( 0, 0, 0, 0 ) );
		
		// Vertical line
		for ( u32 j=0; j<canvas_tile_grid_slot_image->getSize().y; ++j )
		{
			canvas_tile_grid_slot_image->setPixel
				( canvas_tile_grid_slot_image->getSize().x - 1, j,
				sf::Color::Blue );
		}
		
		// Horizontal line
		for ( u32 i=0; i<canvas_tile_grid_slot_image->getSize().x; ++i )
		{
			//canvas_tile_grid_slot_image->setPixel( i, 
			//	canvas_tile_grid_slot_image->getSize().y - 1,
			//	sf::Color::Blue );
			canvas_tile_grid_slot_image->setPixel( i, 
				0,
				sf::Color::Blue );
		}
		
		canvas_tile_grid_slot_texture.reset(new sf::Texture);
		canvas_tile_grid_slot_texture->loadFromImage
			(*canvas_tile_grid_slot_image);
		
		canvas_tile_grid_slot_sprite.reset(new sf::Sprite);
		canvas_tile_grid_slot_sprite->setTexture
			(*canvas_tile_grid_slot_texture);
		
		for ( u32 j=0; 
			j<canvas_image->getSize().y / num_pixels_per_tile_column;
			++j )
		{
			for ( u32 i=0; 
				i<canvas_image->getSize().x / num_pixels_per_tile_row; 
				++i )
			{
				canvas_tile_grid_slot_sprite->setPosition
					( i * canvas_tile_grid_slot_image->getSize().x, 
					j * canvas_tile_grid_slot_image->getSize().y );
				canvas_tile_grid_render_texture.draw
					(*canvas_tile_grid_slot_sprite);
			}
		}
		
	}
	
	
}


void sfml_canvas_widget::on_update()
{
	if (zoomed_recently)
	{
		zoomed_recently = false;
		full_resize(QSize( canvas_image->getSize().x * scale_factor, 
			canvas_image->getSize().y * scale_factor ));
		
		if ( get_pixel_grid_enabled() )
		{
			generate_canvas_pixel_grid();
		}
		if ( get_tile_grid_enabled() )
		{
			generate_canvas_tile_grid();
		}
		
		canvas_sprite->setScale( scale_factor, scale_factor );
	}
	if (modified_recently)
	{
		modified_recently = false;
		canvas_texture->loadFromImage(*canvas_image);
	}
	
	// Instead of generating the pixel grid and tile grid every frame, only
	// do it if they have been recently enabled (or if they're enabled and
	// zooming happened recently).
	if ( pixel_grid_enabled_recently )
	{
		generate_canvas_pixel_grid();
	}
	if ( tile_grid_enabled_recently )
	{
		generate_canvas_tile_grid();
	}
	//generate_canvas_grid();
	
	//clear(sf::Color( 0, 128, 0 ));
	// This clear() call is probably not necessary.
	clear(sf::Color::White);
	draw(*canvas_sprite);
	
	if ( get_pixel_grid_enabled() 
		&& scale_factor >= minimum_scale_factor_for_pixel_grid )
	{
		sf::Sprite canvas_pixel_grid_sprite
			(canvas_pixel_grid_render_texture.getTexture());
		draw(canvas_pixel_grid_sprite);
	}
	if ( get_tile_grid_enabled() 
		&& scale_factor >= minimum_scale_factor_for_tile_grid )
	{
		sf::Sprite canvas_tile_grid_sprite
			(canvas_tile_grid_render_texture.getTexture());
		draw(canvas_tile_grid_sprite);
	}
}



