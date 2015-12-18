#ifndef palette_chooser_widget_class_hpp
#define palette_chooser_widget_class_hpp

#include "sfml_canvas_widget_classes.hpp"

class palette_chooser_widget : public sfml_canvas_widget_base
{
protected:		// variables
	sfml_canvas_widget* the_sfml_canvas_widget;
	
public:		// variables
	static constexpr u32 num_colors_per_palette = 256,
		palette_slot_inner_width = 7, palette_slot_inner_height = 7, 
		palette_slot_outer_width = palette_slot_inner_width + 2, 
		palette_slot_outer_height = palette_slot_inner_height + 2,
		num_colors_per_row = 16, num_colors_per_column = 16;
	
	u32 current_color_index;
	bool palette_modified_recently;
	
	sf::Image palette_image;
	sf::Texture palette_texture;
	sf::Sprite palette_sprite;
	
	
public:		// functions
	palette_chooser_widget( QWidget* s_parent, const QPoint& s_position, 
		const QSize& s_size );
	
	//bool grab_palette_from_canvas_widget_image();
	
	inline void set_the_sfml_canvas_widget
		( sfml_canvas_widget* n_the_sfml_canvas_widget )
	{
		the_sfml_canvas_widget = n_the_sfml_canvas_widget;
	}
	
protected:		// functions
	
	inline void on_init()
	{
		cout << "palette_chooser_widget initalized!\n";
		
	}
	
	void on_update();
	
};


#endif		// palette_chooser_widget_class_hpp
