#ifndef MODOP_HPP
#define MODOP_HPP

// Valid only for m being a power of 2

unsigned int mod(int a, int m);

unsigned int addMod(int a, int b, int m);

unsigned int multMod(int a, int b, int m);

unsigned int powMod(int b, int e, int m);

#endif // MODOP_HPP