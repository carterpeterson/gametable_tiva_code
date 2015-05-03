#include "lcd_menu.h"

bool lcd_pushing_menu = false;
bool lcd_pushing_init = false;
bool displaying_games = true;
uint8_t lcd_current_page = 0;

uint8_t lcd_buffers[2][102*8];
uint8_t *lcd_buffer_read, *lcd_buffer_write;

static uint8_t lcd_init_commands[] = 
	{0x40,
	0xA1,
	0xC8,
	0xA4,
	0xA6,
	0xA2,
	0x2F,
	0x27,
	0x81,
	0x0A,
	0xFA,
	0x90,
	0xAF,
	0xB0,
	0x00,
	0x10};	// This is just their example setup from the EADOGS datasheet

void init_lcd_menu_gpio(void)
{
	// Setup the GPIO for SPI bus
	gpio_port_enable(PORT_A_CGC);
	gpio_digital_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_CD_PIN | LCD_MOSI_PIN));
	gpio_pin_direction(LCD_PORT, DIRECTION_OUTPUT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_CD_PIN | LCD_MOSI_PIN));
	gpio_alternate_function_enable(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MOSI_PIN));
	gpio_config_port_ctl(LCD_PORT, (LCD_CLOCK_PIN | LCD_CS_PIN | LCD_MOSI_PIN), 2);
	
	// Enable the reset pin and bring the LCD out of reset
	gpio_port_enable(PORT_B_CGC);
	gpio_digital_enable(PORT_B, LCD_RESET_PIN);
	gpio_pin_direction(PORT_B, DIRECTION_OUTPUT, LCD_RESET_PIN);
	PORT_B->DATA |= LCD_RESET_PIN;
}

void init_lcd_menu_spi(void)
{
	spi_clock_enable(SPI0_CGC);
	spi_disable(LCD_SPI);
	spi_set_master_slave(LCD_SPI, SPI_MODE_MASTER);
	spi_set_clock_source(LCD_SPI, SPI_CLK_SRC_SYS);
	spi_set_clock_speed(LCD_SPI, SPI_CLOCK_SPEED_20_MHZ);
	spi_set_frame_format(LCD_SPI, SPI_FRF_FREESCALE);
	spi_set_clock_polarity(LCD_SPI, SPI_POLARITY_LOW);
	spi_set_clock_phase(LCD_SPI, SPI_PHASE_FIRST_CLK);
	spi_set_data_size(LCD_SPI, 8);
	spi_config_dma(LCD_SPI, SPI_DMACTL_TX_EN);
	spi_enable_interrupts(LCD_SPI, 4);
	spi_enable(LCD_SPI);
}

void init_lcd_menu_dma(void)
{
	dma_enable();
	
	dma_priority_set(LCD_DMA_CHANNEL, DMA_PRIORITY_HIGH);
	dma_use_primary_control(LCD_DMA_CHANNEL);
	dma_use_burst(LCD_DMA_CHANNEL, DMA_USE_BURST_AND_SINGLE);
	dma_peripheral_request_mask_set(LCD_DMA_CHANNEL, DMA_REQ_MASK_PERIPHERAL);
	UDMA->CHMAP1 &= ~(0xF000);
}

void init_lcd_menu_commands(void)
{
	DMA_control lcd_setup_req;
	lcd_set_command_mode();

	lcd_setup_req.source = (void*) &(lcd_init_commands[15]);
	lcd_setup_req.destination = (void*) &(LCD_SPI->DR);
	lcd_setup_req.control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_1 | (15U << 4) | DMA_XFERMODE_BASIC);
		
	dma_primary_control_structure_set(LCD_DMA_CHANNEL, &lcd_setup_req);	
	
	lcd_pushing_init = true;
	dma_channel_enable(LCD_DMA_CHANNEL);
}

void lcd_set_command_mode(void)
{
	LCD_PORT->DATA &= ~LCD_CD_PIN;
}

void lcd_set_data_mode(void)
{
	LCD_PORT->DATA |= LCD_CD_PIN;
}

void lcd_set_page(uint8_t page)
{
	lcd_set_command_mode();
	LCD_SPI->DR = (0xB0 | page);
}

void lcd_set_column(uint8_t column)
{
	lcd_set_command_mode();
	LCD_SPI->DR = (0x00 | (column & 0x0F));
	LCD_SPI->DR = (0x10 | ((column & 0xF0) >> 4));
}

void lcd_push_buffer(void)
{
	DMA_control lcd_push_req;
	uint8_t *temp;
	
	if(lcd_pushing_menu | lcd_pushing_init)
		return;	// Already pushing a menu out, don't interrupt
	
	temp = lcd_buffer_read;
	lcd_buffer_read = lcd_buffer_write;
	lcd_buffer_write = temp;
	
	lcd_current_page = 0;
	lcd_set_page(lcd_current_page);
	lcd_set_column(0);
	
	while((LCD_SPI->SR & 0x01) != 0x01)
		; // Wait for commands to send
		
	lcd_set_data_mode();
	
	lcd_push_req.source = (void*) &(lcd_buffer_read[LCD_ROW_SIZE_MINUS_ONE]);
	lcd_push_req.destination = (void*) &(LCD_SPI->DR);
	lcd_push_req.control = (DMA_DSTINC_NONE | \
	DMA_DSTSIZE_BYTE | DMA_SRCINC_BYTE | DMA_SRCSIZE_BYTE | \
	DMA_ARBSIZE_1 | (LCD_ROW_SIZE_MINUS_ONE << 4) | DMA_XFERMODE_BASIC);
		
	dma_primary_control_structure_set(LCD_DMA_CHANNEL, &lcd_push_req);	
	
	lcd_pushing_menu = true;
	dma_channel_enable(LCD_DMA_CHANNEL);
}

void init_lcd_buffers(void)
{
	int i;
	lcd_buffer_read = lcd_buffers[0];
	lcd_buffer_write = lcd_buffers[1];
	
	for(i = 0; i < 102 * 8; i++) {
		lcd_buffer_write[i] = 0x00;
		lcd_buffer_read[i] = 0x00;
	}
}

void lcd_set_buffer_byte(uint8_t page, uint8_t column, uint8_t byte)
{
	lcd_buffer_write[page * LCD_ROW_SIZE + (LCD_ROW_SIZE_MINUS_ONE - column)] = byte;
}

void lcd_draw_top_bar(const uint8_t img[]){
	int i, j;

	for(i = 0; i < 2; i++) {
		for(j=0; j<102; j++) {
			lcd_set_buffer_byte(i, j, img[i*102 + j]);
		}
	}
}

void lcd_write_char_8pts(uint8_t page, char c, uint8_t col_start)
{
	int i;
	int actCol;
	
	//get index of character
	int index = (c - 97)*18;

	actCol = col_start;
	
	for(i = 0; i < 9; i++){
		lcd_set_buffer_byte(page, actCol, hPSimplified_8ptBitmaps[index]);
		index = index + 1;
		actCol = actCol + 1;
	}

	actCol = col_start;
	for(i = 0; i < 9; i++){
		lcd_set_buffer_byte(page + 1, actCol, hPSimplified_8ptBitmaps[index]);
		index = index + 1;
		actCol = actCol + 1;
	}
}

void lcd_write_string_8pts( uint8_t line, char *string)
{

	int colcnt = 4;
	int cnt = 0;
	char *ptr = string;
	int strtPage = line;
	
	
	while(cnt != 11){
		if(*ptr == '\0'){
			break;
		}
	  lcd_write_char_8pts(strtPage, *ptr, colcnt);
		colcnt = colcnt + 8;
		ptr += 1;
		cnt += 1;
	}
}

void lcd_write_arrow(uint8_t sel)
{
	int i;
	uint8_t page;
	int index = 0;
	int actCol = 0;
	
	if(sel == 0){
		page = 2;
	}else if(sel == 1){
		page = 4;
	}else{
		page = 6;
	}
	
	for(i = 0; i < 3; i++){
		lcd_set_buffer_byte(page, actCol, Image_Arrow_8ptBitmaps[index]);
		index = index + 1;
		actCol = actCol + 1;
	}

	actCol = 0;
	for(i = 0; i < 3; i++){
		lcd_set_buffer_byte(page + 1, actCol, Image_Arrow_8ptBitmaps[index]);
		index = index + 1;
		actCol = actCol + 1;
	}
	
}

void clear_display_buffer(void)
{
	int i;
	for(i = 0; i < 102 * 8; i++) {
		lcd_buffer_write[i] = 0x00;
	}
}

void lcd_update(void)
{
	int i;
	
	clear_display_buffer();
	
	if (viewing_games) {
		lcd_draw_top_bar(Image_LCD_GameSelect);
		
		if(current_game_selected <= 1) {
			// draw first 3 games
			for(i = 0; i < 3; i++) {
				if(i >= NUM_GAMETABLE_GAMES)
					break;	// Done drawing games
				
				lcd_write_string_8pts(2 + i*2, GAMETABLE_GAMES[i].name);
			}
			
			lcd_write_arrow(current_game_selected);
			
		} else if(current_game_selected >= (NUM_GAMETABLE_GAMES - 2)) {
			// Draw last 3 games
			for(i = (NUM_GAMETABLE_GAMES - 3); i < NUM_GAMETABLE_GAMES; i++) {				
				lcd_write_string_8pts(2 + (i - (NUM_GAMETABLE_GAMES - 3)) *2, GAMETABLE_GAMES[i].name);
			}
			
			lcd_write_arrow(current_game_selected - (NUM_GAMETABLE_GAMES - 3));
		} else {
			// center arrow and draw surrounding games
			for(i = (current_game_selected - 1); i <= (current_game_selected + 1); i++) {				
				lcd_write_string_8pts(2 + (i - (current_game_selected - 1))*2, GAMETABLE_GAMES[i].name);
			}
			
			lcd_write_arrow(1);
		}
		
	} else {
		lcd_draw_top_bar(Image_LCD_AnimationSelect);
		
		if(current_animation_selected <= 1) {
			// draw first 3 games
			for(i = 0; i < 3; i++) {
				if(i >= NUM_GAMETABLE_ANIMATIONS)
					break;	// Done drawing games
				
				lcd_write_string_8pts(2 + i*2, GAMETABLE_ANIMATIONS[i].name);
			}
			
			lcd_write_arrow(current_animation_selected);
			
		} else if(current_animation_selected >= (NUM_GAMETABLE_ANIMATIONS - 2)) {
			// Draw last 3 games
			for(i = (NUM_GAMETABLE_ANIMATIONS - 3); i < NUM_GAMETABLE_ANIMATIONS; i++) {				
				lcd_write_string_8pts(2 + (i - (NUM_GAMETABLE_ANIMATIONS - 3)) *2, GAMETABLE_ANIMATIONS[i].name);
			}
			
			lcd_write_arrow(current_animation_selected - (NUM_GAMETABLE_ANIMATIONS - 3));
		} else {
			// center arrow and draw surrounding games
			for(i = (current_animation_selected - 1); i <= (current_animation_selected + 1); i++) {				
				lcd_write_string_8pts(2 + (i - (current_animation_selected - 1))*2, GAMETABLE_ANIMATIONS[i].name);
			}
			
			lcd_write_arrow(1);
		}
		
	}
	
	lcd_push_buffer();
}

void init_lcd_menu(void)
{
	int i;

	init_lcd_buffers();
	init_lcd_menu_gpio();
	init_lcd_menu_spi();
	init_lcd_menu_dma();
	init_lcd_menu_commands();
}