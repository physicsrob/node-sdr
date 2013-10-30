#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "filter.h"


Filter::Filter(Buffer &inp) : input(inp), output(inp) {
}

Filter::Filter(Buffer &inp, int ratio) : input(inp), output(inp,ratio) {
}


Filter::~Filter() {
}
