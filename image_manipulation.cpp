#include "image_manipulation.h"





void show(Img& img, const char* window_name)
{

	sf::RenderWindow window(sf::VideoMode(img.width(), img.height()), window_name);
	sf::Image m_img;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Event event;


	img.toRGB();

	while (window.isOpen()) {


		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			window.clear(sf::Color::Black);


			m_img.create(img.width(), img.height(), img.getArray());
			texture.loadFromImage(m_img);
			sprite.setTexture(texture);
			window.draw(sprite);

			window.display();
		}
	}

}


int load(Img& img, const char* file_path, ImageType type)
{
	if (!img.isEmpty())return 0;

	std::ifstream file(file_path, std::ios::binary);

	if (!file.is_open())
		return 0;

	BMPHeader header;
	BITMAPINFOHEADER bit_info;

	uint8_t buf[12];
	file.ignore(2);
	file.read((char*)buf, 12);

	header = *(BMPHeader*)buf;

	uint8_t head2[40];
	file.read((char*)head2, 40);

	bit_info = *(BITMAPINFOHEADER*)head2;

	img.build(bit_info.width, bit_info.height, ImageType::RGB);


	safe_array<RGBPixel> palette;

	if (bit_info.compression > 0) //We do not support crazy compression methods eg huffman
		return 0;

	uint64_t padding = 32 - (bit_info.width * bit_info.bits_per_pixel) % 32;

	uint64_t effective_width_bits = bit_info.width * bit_info.bits_per_pixel;

	uint64_t n_bytes = padding == 32 ? bit_info.height * effective_width_bits : bit_info.height * (effective_width_bits + padding);
	n_bytes /= 8;

	uint64_t bytes_per_row = n_bytes / bit_info.height;

	safe_array<uint8_t> row;
	row.constructArray(bytes_per_row);

	if (bit_info.bits_per_pixel <= 8) {
		//palette present
		uint16_t palette_number = bit_info.number_of_color_in_palette == 0 ? Utils::power(2, bit_info.bits_per_pixel) : bit_info.number_of_color_in_palette;
		palette.constructArray(palette_number);

		file.read((char*)palette.getArray(), palette_number * 4);
		for (int i = 0; i < palette_number; i++) {
			uint8_t b = palette[i].b;
			palette[i].b = palette[i].r;
			palette[i].r = b;
			palette[i].a = 0xFF;
		}

		file.seekg(header.image_starting_offset, file.beg);

		// TODO(Sawii00): CHECK AND DEBUG DECOMPRESSION
		/*
								if (bit_info.compression == 1) {
									uint64_t n_of_pixel_extracted = 0;

									uint64_t x = 0;
									uint64_t y = 0;
									char first;
									char second;

									while (n_of_pixel_extracted < bit_info.width * bit_info.height) {
										if (x >= bit_info.width) {
											x = 0;
											y = y < bit_info.height ? y + 1 : y;
										}
										file.get(first);
										file.get(second);

										if (first) {
											for (int i = 0; i < first; i++) {
												m_array[bit_info.width*(bit_info.height - 1 - y) + x] = palette[(uint8_t)second];
											}
											x += first;
											n_of_pixel_extracted += first;
										}
										else {
											switch (second) {
											case 0: {
												//end of line
					//							y++;
						//						x = 0;
												break;
											}
											case 1: {
												//end of bitmap
												return 1;
											}
											case 2: {
												//delta (not supported yet)
												char val;
												file.get(val);
												x += val;
												file.get(val);
												y += val;
												break;
											}
											default:
											{
												//ABSOLUTE MODE
												char val;
												uint64_t pad = (second + 2) % 4 ? 4 - (second + 2) % 4 : 0;
												for (int i = 0; i < second; i++) {
													file.get(val);
													uint64_t index = bit_info.width*(bit_info.height - 1 - y) + x;
													if (index >= m_array.size || (uint8_t)val >= palette.size)
														std::cout << "pippo";
													m_array[index] = palette[(uint8_t)val];
													x++;
													n_of_pixel_extracted++;
												}
												file.ignore(pad);
												break;
											}
											}
										}
									}
								}

								*/
		for (int i = 0; i < bit_info.height; i++) {
			file.read((char*)row.getArray(), bytes_per_row);
			for (uint64_t j = 0; j < effective_width_bits; j += bit_info.bits_per_pixel) {
				uint8_t el = row[(uint64_t)j / 8];
				switch (bit_info.bits_per_pixel)
				{
				case 0x1: {
					el = el >> (7 - j % 8) & 0x1;
					break;
				}
				case 0x2: {
					el = el >> (6 - j % 8) & 0x3;
					break;
				}
				case 0x4: {
					el = el >> (4 - j % 8) & 0xF;
					break;
				}
				}
				((RGBPixel*)(img.getArray()))[bit_info.width * (bit_info.height - 1 - i) + j / bit_info.bits_per_pixel] = palette[el];
			}
			row.clear();
		}
	}
	//@TODO(bitfields)
	else if (bit_info.bits_per_pixel == 16)
	{
		file.seekg(header.image_starting_offset, file.beg);

		//no palette present for bpp > 8
		for (int i = 0; i < bit_info.height; i++) {
			file.read((char*)row.getArray(), bytes_per_row);

			//el is in the form: 0RGB 0rrrrrgggggbbbbb

			for (uint64_t j = 0; j < effective_width_bits; j += 16) {
				uint16_t el = *(uint16_t*)(row.getArray() + j / 8);

				((RGBPixel*)(img.getArray()))[bit_info.width * (bit_info.height - 1 - i) + j / 16] = RGBPixel((el >> 0xA & 0x1F) * 8, (el >> 0x5 & 0x1F) * 8, (el & 0x1F) * 8, 0xFF);
			}
		}
	}

	else if (bit_info.bits_per_pixel == 24)
	{
		file.seekg(header.image_starting_offset, file.beg);

		//no palette present for bpp > 8
		for (int i = 0; i < bit_info.height; i++) {
			file.read((char*)row.getArray(), bytes_per_row);
			//........rrrrrrrrggggggggbbbbbbbb
			for (uint64_t j = 0; j < effective_width_bits; j += 24) {
				uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
				((RGBPixel*)(img.getArray()))[bit_info.width * (bit_info.height - 1 - i) + j / 24] = RGBPixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
			}
		}
	}
	else if (bit_info.bits_per_pixel == 32)
	{
		file.seekg(header.image_starting_offset, file.beg);

		//no palette present for bpp > 8
		for (int i = 0; i < bit_info.height; i++) {
			file.read((char*)row.getArray(), bytes_per_row);
			//rrrrrrrrggggggggbbbbbbbbaaaaaaaa
			for (uint64_t j = 0; j < effective_width_bits; j += 32) {
				uint32_t el = *(uint32_t*)(row.getArray() + j / 8);
				((RGBPixel*)(img.getArray()))[bit_info.width * (bit_info.height - 1 - i) + j / 32] = RGBPixel((el >> 0x10 & 0xFF), (el >> 0x8 & 0xFF), (el & 0xFF), 0xFF);
			}
		}
	}

	else {
		file.close();
		return 0;
	}
	file.close();


	if (type == ImageType::HSL)
	{
		img.toHSL();
	}
	else if (type == ImageType::GRAY)
	{
		img.toGRAY();
	}
	else {}


	return 1;



}


int save(Img& img, const char* filepath)
{
	BMPHeader h;
	h.res1 = 0;
	h.res2 = 0;
	h.image_starting_offset = 54;
	BITMAPINFOHEADER info_h;
	info_h.header_size = 40;
	info_h.width = img.width();
	info_h.height = img.height();
	info_h.color_planes = 1;
	info_h.bits_per_pixel = 24;
	info_h.compression = 0;
	info_h.horizontal_resolution = 3780;
	info_h.vertical_resolution = 3780; //NO CLUE IF IT IMPACTS THE IMAGE
	info_h.number_of_color_in_palette = 0;
	info_h.number_of_important_colors = 0;

	int8_t padding = (3 * info_h.width) % 4 ? 4 - (3 * info_h.width) % 4 : 0;
	info_h.final_image_size = 0;
	info_h.final_image_size = abs(info_h.height) * (info_h.width * 3 + padding);
	h.file_size = abs(info_h.height) * (info_h.width * 3 + padding) + 54;

	std::ofstream outfile(filepath, std::ios::binary);
	if (!outfile.is_open())
		return 0;



	//header are written out first
	outfile.put(0x42);
	outfile.put(0x4D);
	outfile.write((char*)&h, 12);
	outfile.write((char*)&info_h, info_h.header_size);

	img.toRGB();

	char pad_val = 0x0;
	for (int i = info_h.height - 1; i >= 0; i--) {
		for (int j = 0; j < info_h.width; j++) {

			RGBPixel val = img.getPixel<RGBPixel>(j, i);
			outfile.put((char)val.b);
			outfile.put((char)val.g);
			outfile.put((char)val.r);

		}
		for (int k = 0; k < padding; k++)
		{
			outfile.put(pad_val);
		}
	}
	outfile.close();
	return 1;

}








