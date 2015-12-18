#ifndef palette_chooser_widget_class_hpp
#define palette_chooser_widget_class_hpp

#include "sfml_canvas_widget_classes.hpp"

class palette_chooser_widget : public sfml_canvas_widget_base
{
public:		// variables
	sf::Image* canvas_widget_image;
	
	u32 current_color_index;
	bool palette_modified_recently;
	
	static constexpr u32 num_colors_per_palette = 256;
	static constexpr u32 palette_sprite_scale = 5;
	array< sf::Image, num_colors_per_palette > palette_image_arr;
	array< sf::Texture, num_colors_per_palette > palette_texture_arr;
	array< sf::Sprite, num_colors_per_palette > palette_sprite_arr;
	
	
public:		// functions
	palette_chooser_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size, 
		sf::Image* s_canvas_widget_image );
	
	//bool grab_palette_from_canvas_widget_image();
	
protected:		// functions
	
	inline void on_init()
	{
		cout << "palette_chooser_widget initalized!\n";
		
	}
	
	void on_update();
	
};


#endif		// palette_chooser_widget_class_hpp
