#include "Generator.h"
#include "LCDC_registermap.h"
#include "constants.h"

#define fenetre_blanche 0

using namespace std;

void Generator::thread(void){
  ensitlm::data_t rom,ram1,ram2;

  initiator.write(ADDR_BASE_LCD + LCDC_START_REG, 0x00000001);
  initiator.write(ADDR_BASE_LCD + LCDC_ADDR_REG, VIDEO_MEM_BASE_ADDR);

  while(true){
    for(int i = 0 ; i< (IMG_SIZE/8) ; i++){
      initiator.read(ROM_BASE + 4*i, rom);

      ram1= (rom >> 4*3 & 0xF0 			)	| (rom >> 4*2 & 0xF000 		)|
								(rom >> 4*1 & 0xF00000	)	| (rom >> 4*0 & 0xF0000000);

			ram2= (rom << 4*1 & 0xF0 			)	| (rom << 4*2 & 0xF000 		)|
								(rom << 4*3 & 0xF00000	)	| (rom << 4*4 & 0xF0000000);

      // // Affichage sans defillement
			// initiator.write	(VIDEO_MEM_BASE_ADDR + 8*i 	,	ram1);
			// initiator.write	(VIDEO_MEM_BASE_ADDR + 8*i+4,	ram2);

			// Affichage avec defillement vertical
			initiator.write	(VIDEO_MEM_BASE_ADDR +
				((8*i + 320 * next)%(IMG_SIZE)) ,	ram1);
			initiator.write	(VIDEO_MEM_BASE_ADDR +
				((8*i + 4 +	320 * next)%(IMG_SIZE)) ,	ram2);
    }

    wait(display_done);
    wait(0.2,sc_core::SC_SEC);
		next++;
  }

// test de l'affichage d'un ecran blanc
#if fenetre_blanche
  ensitlm::addr_t addr = VIDEO_MEM_BASE_ADDR;
  initiator.write(ADDR_BASE_LCD + LCDC_START_REG, 0x00000001);
  initiator.write(ADDR_BASE_LCD + LCDC_ADDR_REG, VIDEO_MEM_BASE_ADDR);

  for(i = 0 ; i< (IMG_SIZE/4) ; i++){
    initiator.write(addr + 4*i, 0xFFFFFFFF);
  }

#endif

}

void Generator::lcd_interruption() {
  if (display_int.posedge()){
    initiator.write(ADDR_BASE_LCD + LCDC_INT_REG , 0x00000000);
    display_done.notify();
  }
}

Generator::Generator(sc_core::sc_module_name name) :
  sc_core::sc_module(name){
  SC_THREAD(thread);
  SC_METHOD(lcd_interruption);
  sensitive << display_int;
}
