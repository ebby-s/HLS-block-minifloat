

// Helper function to calculate the number of bits required
// to represent n using 2's cmpl format.
int clog2l(long n){
    // Decrease magnitude by 1 if number is negative.
    // Take abs value before counting leading zeros.
    n = std::labs((n < 0) ? n+1 : n); 

    for(int i=0; i<(sizeof(long)*8); i++){
        if(!(n>>i)){
            return i+1; // +1 for signed vs. unsigned.
        }
    }
}

// Helper function to round integers.
long rnd_prd(long prd, int width){

    int rnd_bits = clog2l(prd) - width;

    if(rnd_bits > 0)
        prd &= ~((1<<rnd_bits)-1);

    return prd;
}

