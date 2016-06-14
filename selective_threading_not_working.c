
/* include headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


/* defines */

#define false 0
#define true 1
#define boolean char


/* global variables */


/* function prototypes */


void backtrack(unsigned, unsigned, unsigned *, unsigned, unsigned);

  
/****************************************************/


int main(int argc, char *argv[])
{
  unsigned *tuple;
  unsigned size = 3;
  unsigned alphabet = 3;
  unsigned threading_depth = 1;

  tuple = (unsigned *) calloc(size,sizeof(unsigned));

  omp_set_num_threads(2);
  
  backtrack(size,alphabet,tuple,0,threading_depth);
  return(1);
  
 }


void backtrack(unsigned size, unsigned alphabet, unsigned *tuple, unsigned ell, unsigned t_depth)
{

  unsigned i,j;
  boolean unused;

  if(ell==size)
    {
      #pragma omp critical
      {
	fprintf(stdout,"solution from thread #%d = ",omp_get_thread_num());
	for(i=0;i<size;i++)
	  fprintf(stdout,"%3d ",tuple[i]);
	fprintf(stdout,"\n");
      }
    }
  else
    {
#pragma omp parallel for if(ell == t_depth) default(none) shared(alphabet,tuple,size,ell,t_depth) private(j,unused)
      for(i=0;i<alphabet;i++)
	{
	  unsigned *tuple_to_send;
	  if(ell == t_depth)
	    {
	      unsigned *local_tuple;
		local_tuple = (unsigned *) calloc(size,sizeof(unsigned));
		for(j=0;j<ell;j++) local_tuple[j] = tuple[j];
		tuple_to_send = local_tuple;
	    }
	  else
	    {
	      tuple_to_send = tuple;
	    }
	  
	  tuple_to_send[ell] = i;
	  backtrack(size,alphabet,tuple_to_send,ell+1,t_depth);
	  
	}
      
    }

}
