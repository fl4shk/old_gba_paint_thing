#ifndef sfml_canvas_widget_classes_hpp
#define sfml_canvas_widget_classes_hpp

#include "misc_includes.hpp"

class sfml_canvas_widget_base : public QWidget, public sf::RenderWindow
{
public:		// variables
	QWidget* parent;
	QTimer timer;
	bool initialized;
	
public:		// functions
	sfml_canvas_widget_base( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, u32 frame_time = 0 );
	virtual ~sfml_canvas_widget_base();
	
	
protected:		// functions
	
	virtual void on_init();
	virtual void on_update();
	virtual QPaintEngine* paintEngine() const;
	
	virtual void showEvent( QShowEvent* event );
	virtual void paintEvent( QPaintEvent* event );
};


class sfml_canvas_widget : public sfml_canvas_widget_base
{
public:		// variables
	
	
public:		// functions
	inline sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size ) : sfml_canvas_widget_base( s_parent,
		s_position, s_size )
	{
	}
	
	
protected:		// functions
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
	}
	inline void on_update()
	{
		//clear(sf::Color( 0, 128, 0 ));
		clear( sf::Color( 
	}
	
};



#endif		// sfml_canvas_widget_classes_hpp
