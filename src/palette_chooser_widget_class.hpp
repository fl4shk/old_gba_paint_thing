#ifndef palette_chooser_widget_class_hpp
#define palette_chooser_widget_class_hpp

#include "sfml_canvas_widget_classes.hpp"

class palette_chooser_widget : public sfml_canvas_widget_base
{
public:		// constants
	static constexpr u32 num_colors_per_palette = 256,
		palette_slot_inner_width = 6, palette_slot_inner_height = 6, 
		palette_slot_outer_width = palette_slot_inner_width + 2, 
		palette_slot_outer_height = palette_slot_inner_height + 2,
		num_colors_per_row = 16, num_colors_per_column = 16;
	
protected:		// variables
	sfml_canvas_widget* the_sfml_canvas_widget;
	
	array< sf::Color, num_colors_per_palette > palette;
	sf::RenderTexture palette_render_texture;
	sf::Texture palette_texture;
	sf::Sprite palette_sprite;
	
	// These six variables are used for drawing the palette into
	// palette_render_texture.
	sf::Image palette_slot_inner_image, palette_slot_outer_image;
	sf::Texture palette_slot_inner_texture, palette_slot_outer_texture;
	sf::Sprite palette_slot_inner_sprite, palette_slot_outer_sprite;
	
public:		// variables
	
	u32 current_color_index;
	bool palette_modified_recently;
	
	
	
	
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
	void extract_palette_from_paletted_image
		( const png::image<png::index_pixel>& index_pixel_image );
	bool extract_palette_from_sfml_image
		( const sf::Image& image );
	
	void generate_palette_render_texture();
	
	inline void on_init()
	{
		cout << "palette_chooser_widget initalized!\n";
		
		generate_palette_render_texture();
	}
	
	void on_update();
	
	friend class sfml_canvas_widget;
};


#endif		// palette_chooser_widget_class_hpp
