// Wrapper to ensure FatFs sources include the project-level configuration
// This makes ff.h -> ffconf.h resolve to our top-level ffconf.h before any vendor copies.
#include "../ffconf.h"
