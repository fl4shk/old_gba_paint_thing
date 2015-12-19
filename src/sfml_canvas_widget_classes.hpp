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
		const QSize& s_size, u32 frame_time = 17 );
	virtual ~sfml_canvas_widget_base();
	
	
protected:		// functions
	inline void full_resize( const QSize& n_size )
	{
		resize(n_size);
		sf::RenderWindow::create(winId());
		setVerticalSyncEnabled(true);
	}
	inline void set_min_max_sizes( const QSize& n_size )
	{
		setMinimumSize(n_size);
		setMaximumSize(n_size);
	}
	
	virtual void on_init();
	virtual void on_update();
	virtual QPaintEngine* paintEngine() const;
	
	virtual void showEvent( QShowEvent* event );
	virtual void paintEvent( QPaintEvent* event );
};

class palette_chooser_widget;

class sfml_canvas_widget : public sfml_canvas_widget_base
{
protected:		// variables
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	sf::View apparent_view;
	
	palette_chooser_widget* the_palette_chooser_widget;
	
public:		// variables
	std::string image_file_name;
	sf::Image canvas_image;
	sf::Texture canvas_texture;
	sf::Sprite canvas_sprite;
	
	//bool zoomed_in_recently, zoomed_out_recently;
	bool modified_recently, zoomed_recently;
	u32 scale_factor;
	float view_center_x, view_center_y;
	
	// This is for line drawing.
	QPoint prev_mouse_pos;
	
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, const std::string& s_image_file_name );
	
	inline void set_the_palette_chooser_widget
		( palette_chooser_widget* n_the_palette_chooser_widget );
	
	//inline void full_resize( const QSize& n_size )
	//{
	//	resize(n_size);
	//	sf::RenderWindow::create(winId());
	//}
	
	bool open_image();
	
	inline bool open_image( const std::string& n_image_file_name )
	{
		image_file_name = n_image_file_name;
		return open_image();
	}
	
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	const sf::View& get_apparent_view();
	
	inline bool point_is_in_image( const sf::Vector2i& pos )
	{
		return ( ( pos.x >= 0 ) 
			&& ( pos.x < (int)canvas_image.getSize().x ) && ( pos.y >= 0 ) 
			&& ( pos.y < (int)canvas_image.getSize().y ) );
	}
	
	// This is a purely integer-based line drawing algorithm.
	void draw_line( const sf::Vector2i& pos_0, 
		const sf::Vector2i& pos_1, const sf::Color& color );
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	//void mouseReleaseEvent( QMouseEvent* event );
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//canvas_image.loadFromFile(string("the_powerup_gfx.png"));
		//open_image();
	}
	void on_update();
	
};

#include "palette_chooser_widget_class.hpp"

inline void sfml_canvas_widget::set_the_palette_chooser_widget
	( palette_chooser_widget* n_the_palette_chooser_widget )
{
	the_palette_chooser_widget = n_the_palette_chooser_widget;
}

#endif		// sfml_canvas_widget_classes_hpp
