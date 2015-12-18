#include "palette_chooser_widget_class.hpp"

palette_chooser_widget::palette_chooser_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size, 
	sf::Image* s_canvas_widget_image ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	canvas_widget_image(s_canvas_widget_image), current_color_index(0)
{
	
}


void palette_chooser_widget::on_update()
{
	
}
