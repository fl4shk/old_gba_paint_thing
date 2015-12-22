#include "palette_manipulator_core_widget_class.hpp"

palette_manipulator_core_widget::palette_manipulator_core_widget
	( QWidget* s_parent, const QPoint& s_position, const QSize& s_size ) 
	: sfml_canvas_widget_base( s_parent, s_position, s_size ),
	left_current_color_index(0), right_current_color_index(0),
	palette_modified_recently(false)
{
	
	palette_render_texture.create( num_colors_per_row * slot_outer_width, 
		num_colors_per_column * slot_outer_height );
	palette_render_texture.clear(sf::Color::White);
	palette_texture = palette_render_texture.getTexture();
	palette_sprite.setTexture(palette_texture);
	
	full_resize( QSize( palette_render_texture.getSize().x, 
		palette_render_texture.getSize().y ) );
	set_min_max_sizes( QSize( palette_render_texture.getSize().x,
		palette_render_texture.getSize().y ) );
	
	
	slot_inner_image.create( 1, 1, sf::Color::Blue );
	slot_outer_usual_image.create( slot_outer_width, slot_outer_height, 
		sf::Color::Black );
	slot_outer_left_selected_image.create( slot_outer_width, 
		slot_outer_height, sf::Color::White );
	slot_outer_right_selected_image.create( slot_outer_width, 
		slot_outer_height, sf::Color::Cyan );
	
	
	slot_inner_texture.loadFromImage(slot_inner_image);
	slot_outer_usual_texture.loadFromImage(slot_outer_usual_image);
	slot_outer_left_selected_texture.loadFromImage
		(slot_outer_left_selected_image);
	slot_outer_right_selected_texture.loadFromImage
		(slot_outer_right_selected_image);
	
	
	slot_inner_sprite.setTexture(slot_inner_texture);
	slot_outer_usual_sprite.setTexture(slot_outer_usual_texture);
	slot_outer_left_selected_sprite.setTexture
		(slot_outer_left_selected_texture);
	slot_outer_right_selected_sprite.setTexture
		(slot_outer_right_selected_texture);
	
	slot_inner_sprite.setScale( slot_inner_width, slot_inner_height );
	
	
}


void palette_manipulator_core_widget::extract_palette_from_paletted_image
	( const png::image<png::index_pixel>& index_pixel_image )
{
	png::palette index_pixel_image_palette 
		= index_pixel_image.get_palette();
	
	for ( u32 i=0; i<index_pixel_image_palette.size(); ++i )
	{
		palette[i] = png_color_to_sfml_color(index_pixel_image_palette[i]);
	}
	
}

bool palette_manipulator_core_widget::extract_palette_from_sfml_image
	( const sf::Image& image )
{
	set< sf::Color, sfml_color_compare_for_set_or_map >
		unique_sfml_colors_set;
	
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
	
	auto palette_true_end_iter = palette.begin();
	
	for ( u32 i=0; i<unique_sfml_colors_set.size(); ++i )
	{
		++palette_true_end_iter;
	}
	
	sort( palette.begin(), palette_true_end_iter,
		sfml_color_compare_with_hue() );
	
	return true;
}

void palette_manipulator_core_widget::mousePressEvent( QMouseEvent* event )
{
	sf::Vector2i event_pos_in_color_selection_coords
		( event->x() / slot_outer_width, 
		event->y() / slot_outer_height );
	
	if ( event->button() == Qt::LeftButton )
	{
		u32 n_left_current_color_index 
			= color_selection_coords_to_current_color_index
			(event_pos_in_color_selection_coords);
		
		if ( n_left_current_color_index < num_colors_per_palette )
		{
			set_left_current_color_index(n_left_current_color_index);
		}
	}
	else if ( event->button() == Qt::RightButton )
	{
		u32 n_right_current_color_index 
			= color_selection_coords_to_current_color_index
			(event_pos_in_color_selection_coords);
		
		if ( n_right_current_color_index < num_colors_per_palette )
		{
			set_right_current_color_index(n_right_current_color_index);
		}
	}
	
}

void palette_manipulator_core_widget::generate_palette_render_texture()
{
	palette_render_texture.clear( sf::Color::Green );
	//slot_inner_sprite.setPosition( 0, 0 );
	//slot_outer_sprite.setPosition( 0, 0 );
	
	//cout << palette_render_texture.getSize().x << ", " 
	//	<< palette_render_texture.getSize().y << endl;
	
	for ( u32 j=0; j<num_colors_per_column; ++j )
	{
		for ( u32 i=0; i<num_colors_per_row; ++i )
		{
			slot_inner_image.setPixel( 0, 0, palette.at
				( j * num_colors_per_row + i ) );
			slot_inner_texture.loadFromImage(slot_inner_image);
			
			slot_inner_sprite.setPosition( i * slot_outer_width + 1,
				palette_render_texture.getSize().y 
				- ( ( j + 1 ) * slot_outer_height - 1 ) );
			
			// Highlight the SELECTED colors with a border, drawing the
			// LEFT mouse button selected color one on top of the right
			// mouse button selected color.
			if ( left_current_color_index == j * num_colors_per_row + i )
			{
				slot_outer_left_selected_sprite.setPosition
					( slot_inner_sprite.getPosition().x - 1,
					slot_inner_sprite.getPosition().y - 1 );
				
				palette_render_texture.draw
					(slot_outer_left_selected_sprite);
			}
			else if ( right_current_color_index 
				== j * num_colors_per_row + i )
			{
				slot_outer_right_selected_sprite.setPosition
					( slot_inner_sprite.getPosition().x - 1,
					slot_inner_sprite.getPosition().y - 1 );
				
				palette_render_texture.draw
					(slot_outer_right_selected_sprite);
			}
			else
			{
				slot_outer_usual_sprite.setPosition
					( slot_inner_sprite.getPosition().x - 1,
					slot_inner_sprite.getPosition().y - 1 );
				
				palette_render_texture.draw(slot_outer_usual_sprite);
			}
			
			palette_render_texture.draw(slot_inner_sprite);
		}
	}
	
	palette_texture = palette_render_texture.getTexture();
}

void palette_manipulator_core_widget::on_update()
{
	if (palette_modified_recently)
	{
		palette_modified_recently = false;
		
		generate_palette_render_texture();
		//palette_texture = palette_render_texture.getTexture();
	}
	
	//clear(sf::Color::White);
	draw(palette_sprite);
}



