#include "ensitlm.h"
#include "Generator.h"
#include "bus.h"
#include "memory.h"
#include "LCDC.h"
#include "systemc.h"
#include "constants.h"
#include "ROM.h"

using namespace std;

int sc_main(int argc, char **argv){
  (void)argc;
  (void)argv;

  //instanciation
  Generator generator1("Generator1");
  Bus bus("Bus");
  Memory Memory("Memory",0x15400);
  LCDC lcdc("LCDC",sc_time(1.0 / 25, SC_SEC));
  ROM rom("ROM");
  sc_signal < bool , SC_MANY_WRITERS > irq_signal ("IRQ" );

  //mappage sur le bus
  bus.map(Memory.target, MEM_ADDR, MEM_SIZE);
  bus.map(lcdc.target_socket,ADDR_BASE_LCD,SIZE_BASE_LCD);
  bus.map(rom.socket, ROM_BASE,ROM_SIZE);

  //bindings
  lcdc.initiator_socket.bind(bus.target);
  bus.initiator.bind(lcdc.target_socket);
  bus.initiator.bind(rom.socket);
  generator1.initiator.bind(bus.target);
  bus.initiator.bind(Memory.target);
  lcdc.display_int.bind(irq_signal);
  generator1.display_int.bind(irq_signal);

	sc_core::sc_start();
  return 0;
}
