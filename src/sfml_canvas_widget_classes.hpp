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
	std::string image_file_name;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	u32 scale_factor;
	
	bool zoomed_in_recently, zoomed_out_recently;
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, const std::string& s_image_file_name );
	
	inline void full_resize( const QSize& n_size )
	{
		resize(n_size);
		sf::RenderWindow::create(winId());
	}
	
	bool open_image();
	
	inline bool open_image( const std::string& n_image_file_name )
	{
		image_file_name = n_image_file_name;
		return open_image();
	}
	
protected:		// functions
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//image.loadFromFile(string("the_powerup_gfx.png"));
		open_image();
	}
	void on_update();
	
};



#endif		// sfml_canvas_widget_classes_hpp
