#ifndef palette_manipulator_core_widget_class_hpp
#define palette_manipulator_core_widget_class_hpp

#include "sfml_canvas_widget_classes.hpp"
#include "palette_manipulator_widget_class.hpp"

class palette_manipulator_core_widget : public sfml_canvas_widget_base
{
public:		// constants
	static constexpr u32 num_colors_per_palette = 256,
		num_colors_per_row = 16, num_colors_per_column = 16,
		slot_inner_width = 6, slot_inner_height = 6, 
		slot_outer_width = slot_inner_width + 2, 
		slot_outer_height = slot_inner_height + 2;
	
protected:		// variables
	sfml_canvas_widget* the_sfml_canvas_widget;
	
	array< sf::Color, num_colors_per_palette > palette;
	sf::RenderTexture palette_render_texture;
	sf::Texture palette_texture;
	sf::Sprite palette_sprite;
	
	// These six variables are used for drawing the palette into
	// palette_render_texture.
	sf::Image slot_inner_image, slot_outer_usual_image,
		slot_outer_left_selected_image,
		slot_outer_right_selected_image;
	sf::Texture slot_inner_texture, 
		slot_outer_usual_texture,
		slot_outer_left_selected_texture,
		slot_outer_right_selected_texture;
	sf::Sprite slot_inner_sprite, 
		slot_outer_usual_sprite,
		slot_outer_left_selected_sprite,
		slot_outer_right_selected_sprite;

	u32 left_current_color_index, right_current_color_index;
	bool palette_modified_recently;
	
public:		// variables
	
	
	
public:		// functions
	palette_manipulator_core_widget( QWidget* s_parent, 
		const QPoint& s_position, const QSize& s_size );
	
	//bool grab_palette_from_canvas_widget_image();
	
	inline u32 get_left_current_color_index() const
	{
		return left_current_color_index;
	}
	inline void set_left_current_color_index
		( u32 n_left_current_color_index )
	{
		palette_modified_recently = true;
		left_current_color_index = ( n_left_current_color_index 
			< num_colors_per_palette ) ? n_left_current_color_index 
			: ( num_colors_per_palette - 1 );
	}
	inline u32 get_right_current_color_index() const
	{
		return right_current_color_index;
	}
	inline void set_right_current_color_index
		( u32 n_right_current_color_index )
	{
		palette_modified_recently = true;
		right_current_color_index = ( n_right_current_color_index 
			< num_colors_per_palette ) ? n_right_current_color_index 
			: ( num_colors_per_palette - 1 );
	}
	
	inline u32 color_selection_coords_to_current_color_index
		( const sf::Vector2i& pos_in_color_selection_coords )
	{
		return (u32)( ( pos_in_color_selection_coords.y 
			* num_colors_per_row ) + pos_in_color_selection_coords.x );
	}
	
	
	inline void set_the_sfml_canvas_widget
		( sfml_canvas_widget* n_the_sfml_canvas_widget )
	{
		the_sfml_canvas_widget = n_the_sfml_canvas_widget;
	}
	
	void extract_palette_from_paletted_image
		( const png::image<png::index_pixel>& index_pixel_image );
	bool extract_palette_from_sfml_image
		( const sf::Image& image );
	
	
	
protected:		// functions
	void mousePressEvent( QMouseEvent* event );
	
	void generate_palette_render_texture();
	
	inline void on_init()
	{
		cout << "palette_manipulator_widget initalized!\n";
		
		generate_palette_render_texture();
	}
	
	void on_update();
	
	friend class sfml_canvas_widget;
	friend class palette_manipulator_widget;
};


#endif		// palette_manipulator_core_widget_class_hpp
