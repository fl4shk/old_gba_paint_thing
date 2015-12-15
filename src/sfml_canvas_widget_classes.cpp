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


