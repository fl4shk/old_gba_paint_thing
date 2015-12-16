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
	image_file_name(s_image_file_name), scale_factor(1), 
	zoomed_in_recently(false), zoomed_out_recently(false)
{
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



void sfml_canvas_widget::on_update()
{
	if ( zoomed_in_recently || zoomed_out_recently )
	{
		texture.loadFromImage(image);
		full_resize(QSize( sprite.getTexture()->getSize().x * scale_factor,
			sprite.getTexture()->getSize().y * scale_factor ));
		
		sf::View view = getDefaultView();
		
		view.setCenter( 0.0f, 0.0f );
		
		view.move( (float)( sprite.getTexture()->getSize().x ) / 2.0f,
			(float)( sprite.getTexture()->getSize().y ) / 2.0f );
		
		view.zoom( 1.0f / (float)scale_factor );
		
		setView(view);
	}
	
	if ( zoomed_in_recently )
	{
		cout << "zoomed_in_recently == true\n";
		zoomed_in_recently = false;
	}
	if ( zoomed_out_recently )
	{
		cout << "zoomed_out_recently == true\n";
		zoomed_out_recently = false;
	}
	
	//clear(sf::Color( 0, 128, 0 ));
	clear(sf::Color::White);
	draw(sprite);
}



