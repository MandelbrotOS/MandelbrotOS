#ifndef __IRQ_H__
#define __IRQ_H__

void irq_install_handler(int irq, void (*handler)());
void irq_uninstall_handler(int irq);
int init_irq();

#endif
