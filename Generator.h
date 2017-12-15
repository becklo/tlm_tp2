#ifndef GENERATOR_H
#define GENERATOR_H

#include "ensitlm.h"
#include "bus.h"


SC_MODULE(Generator) {
	sc_core::sc_in<bool> display_int;
	sc_core::sc_event display_done;
	ensitlm::initiator_socket<Generator> initiator;

	int next = 0;
	void thread(void);
	void lcd_interruption(void);

	SC_CTOR(Generator);
};

#endif
