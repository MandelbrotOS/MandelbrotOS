#ifndef __IRQ_H__
#define __IRQ_H__

int init_irq();
void irq_install_handler(int irq, void(*handler));
void irq_uninstall_handler(int irq);

#endif
