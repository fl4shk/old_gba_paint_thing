#ifndef image_editor_widget_class_hpp
#define image_editor_widget_class_hpp

#include "misc_includes.hpp"
#include "sfml_canvas_widget_classes.hpp"

class image_editor_widget : public QWidget
{
	Q_OBJECT
	
public:		// variables
	static const QString default_parent_title;
	
	QHBoxLayout* hbox_layout;
	QVBoxLayout* vbox_layout;
	QWidget* parent;
	QScrollArea* scroll_area;
	
	sfml_canvas_widget* canvas_widget; 
	
	//bool modified;
	//u32 scale_factor;
	
	QPoint prev_mouse_pos;
	
public:		// functions
	image_editor_widget( QWidget* s_parent = 0 );
	
	bool open_image( const std::string& s_image_file_name );
	
protected:		// functions
	//void adjust_scroll_bar( QScrollBar* scroll_bar );
	bool zoom_in();
	bool zoom_out();
	
	// Events
	void keyPressEvent( QKeyEvent* event );
	void mousePressEvent( QMouseEvent* event );
	void mouseMoveEvent( QMouseEvent* event );
	////void mouseReleaseEvent( QMouseEvent* event );
	//void paintEvent( QPaintEvent* event );
	////void resizeEvent( QResizeEvent* event );
	
	inline const sf::Vector2i widget_pos_to_scroll_area_coords
		( int widget_x, int widget_y ) const
	{
		return sf::Vector2i( widget_x - scroll_area->geometry().x()
			+ scroll_area->horizontalScrollBar()->value(),
			widget_y - scroll_area->geometry().y()
			+ scroll_area->verticalScrollBar()->value() );
	}
	
	void draw_line( const sf::Vector2i& pos_0, 
		const sf::Vector2i& pos_1, const sf::Color& color );
	
	inline bool point_is_in_image( const sf::Vector2i& pos )
	{
		return ( ( pos.x >= 0 )
			&& ( pos.x < (int)canvas_widget->image.getSize().x )
			&& ( pos.y >= 0 )
			&& ( pos.y < (int)canvas_widget->image.getSize().y ) );
	}
	
	
protected slots:		// slots.
	void hello();
	void show_horizontal_scroll_bar_stuff();
	void show_vertical_scroll_bar_stuff();
	void show_geometry_stuff();
	
	
};


#endif		// image_editor_widget_class_hpp
