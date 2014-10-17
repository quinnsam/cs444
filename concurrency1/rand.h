int rdrand_func( int mode )
{
  /* for 32 bit mode call rdrand */
  unsigned int gen;
  int random = -1;
  asm( "rdrand %0" : "=r" (gen) );
  if ( mode == 1 ) { //Mode 1: consumer wait time
    random = ((gen % 8) + 2);
  } else if ( mode == 2) { //Mode 2: producer wait time
    random = ((gen % 5) + 3);
  } else if ( mode == 3 ) { //Mode 3: just a random number
    random = (gen % 100);
  }
  //printf("Random number: %llu\n", random);
  return random;
}

