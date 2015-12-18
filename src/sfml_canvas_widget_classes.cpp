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
	
	// Set strong focus to enable keyboard events to be received.
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
	
	// Display on screen
	display();
}


sfml_canvas_widget::sfml_canvas_widget( QWidget* s_parent, 
	const QPoint& s_position, const QSize& s_size, 
	const std::string& s_image_file_name ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	image_file_name(s_image_file_name), modified_recently(false), 
	zoomed_recently(false), scale_factor(1), view_center_x(0.0f),
	view_center_y(0.0f)
{
	apparent_view = getDefaultView();
}



bool sfml_canvas_widget::open_image()
{
	if ( !image.loadFromFile(image_file_name) )
	{
		return false;
	}
	
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	full_resize(QSize( sprite.getTexture()->getSize().x,
		sprite.getTexture()->getSize().y ));
	
	return true;
}

const sf::View& sfml_canvas_widget::get_apparent_view()
{
	apparent_view = getDefaultView();
	
	apparent_view.setCenter( view_center_x, view_center_y );
	
	apparent_view.move
		( (double)( sprite.getTexture()->getSize().x ) / (double)2.0f,
		(double)( sprite.getTexture()->getSize().y ) / (double)2.0f );
		
	apparent_view.zoom( 1.0f / (double)scale_factor );
	
	return apparent_view;
}

// This is a purely integer-based line drawing algorithm.
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
		image.setPixel( (u32)pixel_coord.x, 
			(u32)pixel_coord.y, color );
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
				image.setPixel( (u32)pixel_coord.x, 
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
				image.setPixel( (u32)pixel_coord.x, 
					(u32)pixel_coord.y, color );
			}
		}
	}
	
}


void sfml_canvas_widget::on_update()
{
	if ( zoomed_recently )
	{
		zoomed_recently = false;
		full_resize(QSize( sprite.getTexture()->getSize().x * scale_factor,
			sprite.getTexture()->getSize().y * scale_factor ));
		
		sprite.setScale( scale_factor, scale_factor );
	}
	if ( modified_recently )
	{
		modified_recently = false;
		texture.loadFromImage(image);
	}
	
	//clear(sf::Color( 0, 128, 0 ));
	// This clear() call is probably not necessary.
	clear(sf::Color::White);
	draw(sprite);
}



