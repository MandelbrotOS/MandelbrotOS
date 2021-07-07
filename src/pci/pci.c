#include <pci/pci.h>

int pci_enumerate() {
  pci_legacy_enumerate();
  return 0;
}
