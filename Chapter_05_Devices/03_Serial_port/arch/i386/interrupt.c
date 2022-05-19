/*! Interrupt handling - 'arch' layer(only basic operations) */

#define _ARCH_INTERRUPTS_C_
#include "interrupt.h"

#include <arch/processor.h>
#include <kernel/errno.h>
#include <lib/list.h>
#include <kernel/memory.h>

/*! Interrupt controller device */
extern arch_ic_t IC_DEV;
static arch_ic_t *icdev = &IC_DEV;

/*! interrupt handlers */
static list_t ihandlers[INTERRUPTS];

struct ihndlr
{
	int prio;
	void *device;
	int (*ihandler)(unsigned int, void *device);

	list_h list;
};

//---------------------------------------------------------
//definirati jedan zahtjev za prekid:
struct zahtjev
{
	struct ihndlr handler;
	int obrada_u_tijeku;
};
//te dodati listu za takve zahtjeve:
static list_t lista_zahtjeva;
//---------------------------------------------------------


/*! Initialize interrupt subsystem(in 'arch' layer) */
void arch_init_interrupts()
{
	int i;

	icdev->init();

	list_init(&lista_zahtjeva);

	for (i = 0; i < INTERRUPTS; i++)
		list_init(&ihandlers[i]);
}

/*!
 * enable and disable interrupts generated outside processor, controller by
 * interrupt controller(PIC or APIC or ...)
 */
void arch_irq_enable(unsigned int irq)
{
	icdev->enable_irq(irq);
}
void arch_irq_disable(unsigned int irq)
{
	icdev->disable_irq(irq);
}

/*! Register handler function for particular interrupt number */
void arch_register_interrupt_handler(unsigned int inum, void *handler, void *device, int prio)
{
	struct ihndlr *ih;

	if (inum < INTERRUPTS)
	{
		ih = kmalloc(sizeof(struct ihndlr));
		ASSERT(ih);

		ih->device = device;
		ih->ihandler = handler;
		ih->prio = prio;

		list_append(&ihandlers[inum], ih, &ih->list);
	}
	else {
		LOG(ERROR, "Interrupt %d can't be used!\n", inum);
		halt();
	}
}



/*! Unregister handler function for particular interrupt number */
void arch_unregister_interrupt_handler(unsigned int irq_num, void *handler,
					 void *device)
{
	struct ihndlr *ih, *next;

	ASSERT(irq_num >= 0 && irq_num < INTERRUPTS);

	ih = list_get(&ihandlers[irq_num], FIRST);

	while (ih)
	{
		next = list_get_next(&ih->list);

		if (ih->ihandler == handler && ih->device == device)
			list_remove(&ihandlers[irq_num], FIRST, &ih->list);

		ih = next;
	}
}


int cmp_prio(void* z1, void* z2 ){


		if( (((struct zahtjev*)(z1))->handler.prio) > (((struct zahtjev*)(z2))->handler.prio) )
			return 1;
		return -1;
}

/*!
 * "Forward" interrupt handling to registered handler
 * (called from interrupts.S)
 */
void arch_interrupt_handler(int irq_num)
{
	struct ihndlr *ih;

	if (irq_num < INTERRUPTS && (ih = list_get(&ihandlers[irq_num], FIRST)))
	{
		/* enable interrupts on PIC immediately since program may not
		 * return here immediately */
		if (icdev->at_exit)
			icdev->at_exit(irq_num);

		/* Call registered handlers */
		while (ih)
		{
			// -------- stvoriti zahtjev i dodati ga u listu
			struct zahtjev* zah;
			zah = kmalloc(sizeof(struct zahtjev));

			zah->handler = *ih;
			zah->obrada_u_tijeku = 0;

			list_sort_add(&lista_zahtjeva, zah, &(zah->handler.list), &cmp_prio);
			ih = list_get_next(&ih->list);

		}

			//  --------------- obrada zahtjeva po prioritetima ---------------
			struct zahtjev* prvi = list_get(&lista_zahtjeva, FIRST);
			while(  (prvi != NULL) &&  ((prvi->obrada_u_tijeku) == 0)  ){
						prvi->obrada_u_tijeku = 1;
						arch_enable_interrupts();
						prvi->handler.ihandler(irq_num, prvi->handler.device);
						arch_disable_interrupts();
						list_remove(&lista_zahtjeva, FIRST, NULL );
						kfree(prvi);
						prvi = list_get(&lista_zahtjeva, FIRST);
			}
	}

	else if (irq_num < INTERRUPTS)
	{
		LOG(ERROR, "Unregistered interrupt: %d - %s!\n",
		      irq_num, icdev->int_descr(irq_num));
		halt();
	}
	else {
		LOG(ERROR, "Unregistered interrupt: %d !\n", irq_num);
		halt();
	}
}
