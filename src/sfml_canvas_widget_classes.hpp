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
		const QSize& s_size, u32 frame_time = 2 );
	virtual ~sfml_canvas_widget_base();
	
	
protected:		// functions
	inline void full_resize( const QSize& n_size )
	{
		resize(n_size);
		sf::RenderWindow::create(winId());
		//setVerticalSyncEnabled(true);
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

class palette_manipulator_core_widget;

class sfml_canvas_widget : public sfml_canvas_widget_base
{
protected:		// variables
	// This is necessary for converting coordinates relative to this widget
	// to coordinates relative to the image.
	sf::View apparent_view;
	
	palette_manipulator_core_widget* the_palette_manipulator_core_widget;
	
	string image_file_name;
	
	sf::RenderTexture canvas_grid_render_target;
	sf::Image canvas_pixel_grid_image;
	sf::Image canvas_pixel_grid_texture;
	sf::Image canvas_pixel_grid_sprite;
	
	
	sf::Image canvas_image;
	sf::Texture canvas_texture;
	sf::Sprite canvas_sprite;
	
	// This is for line drawing.
	QPoint prev_mouse_pos;
	
	bool pixel_grid_enabled_recently, pixel_grid_disabled_recently,
		pixel_grid_enabled;
	
public:		// variables
	
	//bool zoomed_in_recently, zoomed_out_recently;
	bool modified_recently, zoomed_recently;
	u32 scale_factor;
	float view_center_x, view_center_y;
	
	
	
public:		// functions
	sfml_canvas_widget( QWidget* s_parent, const QPoint& s_position,
		const QSize& s_size, const string& s_image_file_name );
	
	inline void set_the_palette_manipulator_core_widget
		( palette_manipulator_core_widget* 
		n_the_palette_manipulator_core_widget );
	
	//inline void full_resize( const QSize& n_size )
	//{
	//	resize(n_size);
	//	sf::RenderWindow::create(winId());
	//}
	
	bool open_image();
	
	inline bool open_image( const string& n_image_file_name )
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
	
	inline bool get_pixel_grid_enabled() const
	{
		return pixel_grid_enabled;
	}
	
	void enable_pixel_grid();
	void disable_pixel_grid();
	
	// This is just a PNG exporter.
	inline void export_file()
	{
		export_file_as(image_file_name);
	}
	// This is just a PNG exporter.
	void export_file_as( const string& output_file_name );
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	void mouseReleaseEvent( QMouseEvent* event );
	
	
	inline void on_init()
	{
		cout << "sfml_canvas_widget initialized!\n";
		
		//canvas_image.loadFromFile(string("the_powerup_gfx.png"));
		//open_image();
	}
	void on_update();
	
	
	friend class palette_manipulator_core_widget;
};

#include "palette_manipulator_core_widget_class.hpp"

inline void sfml_canvas_widget::set_the_palette_manipulator_core_widget
	( palette_manipulator_core_widget* 
	n_the_palette_manipulator_core_widget )
{
	the_palette_manipulator_core_widget 
		= n_the_palette_manipulator_core_widget;
}

#endif		// sfml_canvas_widget_classes_hpp
