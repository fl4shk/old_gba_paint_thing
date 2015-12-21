#include "palette_chooser_widget_class.hpp"

palette_chooser_widget::palette_chooser_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	current_color_index(0), palette_modified_recently(false)
{
	
	palette_render_texture.create
		( num_colors_per_row * palette_slot_outer_width, 
		num_colors_per_column * palette_slot_outer_height );
	palette_render_texture.clear( sf::Color::White );
	palette_texture = palette_render_texture.getTexture();
	palette_sprite.setTexture(palette_texture);
	
	full_resize( QSize( palette_render_texture.getSize().x, 
		palette_render_texture.getSize().y ) );
	set_min_max_sizes( QSize( palette_render_texture.getSize().x,
		palette_render_texture.getSize().y ) );
	
	
	palette_slot_inner_image.create( 1, 1, sf::Color::Blue );
	//palette_slot_outer_image.create( palette_slot_outer_width, 
	//	palette_slot_outer_height, sf::Color( 192, 192, 192 ) );
	palette_slot_outer_image.create( palette_slot_outer_width, 
		palette_slot_outer_height, sf::Color::Black );
	
	palette_slot_inner_texture.loadFromImage(palette_slot_inner_image);
	palette_slot_outer_texture.loadFromImage(palette_slot_outer_image);
	
	palette_slot_inner_sprite.setTexture(palette_slot_inner_texture);
	palette_slot_outer_sprite.setTexture(palette_slot_outer_texture);
	
	palette_slot_inner_sprite.setScale( palette_slot_inner_width,
		palette_slot_inner_height );
	
	//// Temporary!
	//for ( sf::Color& color : palette )
	//{
	//	// Orange!
	//	color = sf::Color( 255, 120, 0 );
	//}
}


void palette_chooser_widget::extract_palette_from_paletted_image
	( const png::image<png::index_pixel>& index_pixel_image )
{
	png::palette index_pixel_image_palette 
		= index_pixel_image.get_palette();
	
	for ( u32 i=0; i<index_pixel_image_palette.size(); ++i )
	{
		palette[i] = png_color_to_sfml_color(index_pixel_image_palette[i]);
	}
	
}

bool palette_chooser_widget::extract_palette_from_sfml_image
	( const sf::Image& image )
{
	set< sf::Color, sfml_color_compare_for_set_or_map >
		unique_sfml_colors_set;
	map< sf::Color, u32, sfml_color_compare_for_set_or_map > 
		sfml_color_to_palette_index_map;
	
	// Find all the unique sfml colors in the image.
	for ( u32 j=0; j<image.getSize().y; ++j )
	{
		// Loop across rows before columns.
		for ( u32 i=0; i<image.getSize().x; ++i )
		{
			sf::Color the_color = image.getPixel( i, j );
			
			if ( unique_sfml_colors_set.find(the_color) 
				== unique_sfml_colors_set.end() )
			{
				unique_sfml_colors_set.insert(the_color);
			}
		}
	}
	
	//cout << unique_sfml_colors_set.size() << endl;
	
	if ( unique_sfml_colors_set.size() > num_colors_per_palette )
	{
		cout << "There are too many colors (more than 256) in the source "
			<< "image!\n";
		return false;
	}
	
	{
		u32 i = 0;
		for ( const sf::Color& the_color : unique_sfml_colors_set )
		{
			palette[i] = the_color;
			++i;
		}
	}
	
	
	return true;
}

void palette_chooser_widget::generate_palette_render_texture()
{
	palette_render_texture.clear( sf::Color::Green );
	//palette_slot_inner_sprite.setPosition( 0, 0 );
	//palette_slot_outer_sprite.setPosition( 0, 0 );
	
	//cout << palette_render_texture.getSize().x << ", " 
	//	<< palette_render_texture.getSize().y << endl;
	
	for ( u32 j=0; j<num_colors_per_column; ++j )
	{
		for ( u32 i=0; i<num_colors_per_row; ++i )
		{
			palette_slot_inner_image.setPixel( 0, 0, 
				palette.at( j * num_colors_per_row + i ) );
			palette_slot_inner_texture.loadFromImage
				(palette_slot_inner_image);
			
			palette_slot_inner_sprite.setPosition
				( i * palette_slot_outer_width + 1,
				palette_render_texture.getSize().y 
				- ( ( j + 1 ) * palette_slot_outer_height - 1 ) );
			palette_slot_outer_sprite.setPosition
				( palette_slot_inner_sprite.getPosition().x - 1,
				palette_slot_inner_sprite.getPosition().y - 1 );
			
			palette_render_texture.draw(palette_slot_outer_sprite);
			palette_render_texture.draw(palette_slot_inner_sprite);
		}
	}
	
	palette_texture = palette_render_texture.getTexture();
}

void palette_chooser_widget::on_update()
{
	if (palette_modified_recently)
	{
		palette_modified_recently = false;
		
		palette_texture = palette_render_texture.getTexture();
	}
	
	//clear(sf::Color::White);
	draw(palette_sprite);
}



