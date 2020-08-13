#include <stdio.h>
#include <stdlib.h>
#include "axiom.h"

int main(int argc, char const *argv[])
{
	axiom_init();
	axiom_loop();
	axiom_clean();

	return 0;
}

