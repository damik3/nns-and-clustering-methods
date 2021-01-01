#include "ModOp.hpp"




unsigned int mod(int a, int m) {



    if (a < 0) {
        a = a * (-1);
        unsigned int ret = a & (m - 1);
        return (ret == 0 ? 0 : m - ret);
    }
    else
        return (unsigned int) a & (m - 1);
}



/*
unsigned int mod(int a, int m) {

 if (a % m < 0) 

 return (unsigned int ) a % m + m;

 else

 return (unsigned int) a % m;


}
*/




/*
unsigned int mod(int a, int m) {

 if (a % m < 0) 

 return (unsigned int ) a % m + m;

 else

 return (unsigned int) a % m;

}
*/


unsigned int addMod(int a, int b, int m) {
    return mod( mod(a, m) + mod(b, m), m) ;
}



/*
unsigned int multMod(int a, int b, int m) {
    return mod( mod(a, m) * mod(b, m), m );
}
*/



unsigned int multMod(int a, int b, int m) {
    if (a == 0 || b == 0) {
        return 0;
    }
    if (a == 1) {
        return b;
    }
    if (b == 1) {
        return a;
    } 

    // Returns: (a * b/2) mod c
    int a2 = multMod(a, b / 2, m);

    // Even factor
    if ((b & 1) == 0) {
        // [((a * b/2) mod c) + ((a * b/2) mod c)] mod c
        // return (a2 + a2) % m;
        return mod(a2 + a2, m);
    } else {
        // Odd exponent
        // [(a mod c) + ((a * b/2) mod c) + ((a * b/2) mod c)] mod c
        // return ((a % m) + (a2 + a2)) % m;
        return addMod(mod(a, m), a2 + a2, m);
    }
}



/*
unsigned int powMod(int b, int e, int m) {
    unsigned ret = 1;

    for (int i=0; i<e; i++)
        ret = multMod(ret, b, m);

    return ret;
}
*/



unsigned int powMod(int b, int e, int m) {  
    unsigned int res = 1;     // Initialize result  
  
    // b = b % m;   // Update x if it is more than or  
                    // equal to p 
    b = mod(b, m);

    if (b == 0) return 0; // In case x is divisible by p; 
  
    while (e > 0)  
    {  
        // If y is odd, multiply x with result  
        if (e & 1)  
            // res = (res*b) % m;  
            res = mod(res*b, m);
  
        // y must be even now  
        e = e>>1; // y = y/2  
        // b = (b*b) % m;  
        b = mod(b*b, m);
    }  
    return res;  
}  
