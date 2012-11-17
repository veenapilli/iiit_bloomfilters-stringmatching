#include<stdio.h>
#include<string.h>
#include<math.h>
#include "hashFunctions.h"
#include "insertOps.h"
/*
Function Name:
Descriptions:
Parameters:
Return Type:
*/
unsigned int hashFunctionOne(char *cPtr_string,int flag){	
    int i = 0;
    int j = 0;
    int c = 0;
    int str_length = 0;

    unsigned long final_sum = 0;
    static unsigned long ul_hashValue[BLOOMFILTERSIZE];
    
    str_length = strlen(cPtr_string);
#if HASH1_DEBUG
    printf("String length: %d \n",str_length);
    printf("String is : ");
    i = 0;    
    while(*(cPtr_string+i) != '\0'){
         printf("%c",*(cPtr_string+i));
         i++;
    }
    printf("\n");
#endif
    if(flag == 0){
#if HASH1_DEBUG
         printf("----- Step 01 ----- \n");
#endif
	 for(i=0; i<=str_length; i++){
              ul_hashValue[i]=1;
#if HASH1_DEBUG
              printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif
	}
#if HASH1_DEBUG
        printf("\n");   
        printf("----- Step 02 ----- \n");
#endif
	for(i=0;i<=str_length-1;i++){
	      ul_hashValue[i]= ul_hashValue[i] * 33;
#if HASH1_DEBUG
              printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif
	}
#if HASH1_DEBUG
        printf("\n");
        printf("----- Step 03 ----- \n");
#endif
	for(i=0;i<=str_length-2;i++){
	      for(j=0;j<=i;j++){
	           ul_hashValue[j] = (ul_hashValue[j]<<5) + ul_hashValue[j];
	      }
#if HASH1_DEBUG
              for(j=0;j<=i;j++){
	           printf("ul_hashValue[%d]: %ld \n",j,ul_hashValue[j]);
	      }
#endif                
	}
#if HASH1_DEBUG
        printf("\n");
        printf("ul_hashValue[0] before : %ld \n",ul_hashValue[0]);
#endif
	ul_hashValue[0] = ul_hashValue[0] * 5381;
#if HASH1_DEBUG
        printf("ul_hashValue[0] : %ld \n",ul_hashValue[0]);
        printf("\n");
        printf("----- Step 04 ----- \n");
#endif
	for(i=1;i<=str_length;i++){
              c = *(cPtr_string+i-1);
#if HASH1_DEBUG
              printf("Chracter is %c \n",c);
              printf("ASCII value is %d \n",c);
#endif
	      ul_hashValue[i] = ul_hashValue[i] * c;
#if HASH1_DEBUG
              printf("ul_hashValue[%d]:%ld \n",i,ul_hashValue[i]);
#endif
	}

    }
    
    if(flag == 1){
#if HASH1_DEBUG
         printf("Character: %c \n",*(cPtr_string+(str_length-1)));
#endif
	 ul_hashValue[str_length] = ul_hashValue[str_length] * *(cPtr_string+(str_length-1));
#if HASH1_DEBUG
         printf("ul_hashValue[%d]:%ld\n",str_length,ul_hashValue[str_length]);
#endif
    }  
       
    for(i=0;i<=str_length;i++){
	 final_sum = final_sum + ul_hashValue[i];
    }
#if HASH1_DEBUG
    printf("\n");
    printf("Final Sum: %ld \n",final_sum);
    printf("\n");
    printf("ul_hashValue[0] : %ld \n",ul_hashValue[0]);
#endif
    for(i=1;i<=str_length-1;i++){
         ul_hashValue[i] = ul_hashValue[i+1] *33;
#if HASH1_DEBUG
         printf("ul_hashValue[%d] : %ld\n",i,ul_hashValue[i]);
#endif
    } 
    ul_hashValue[str_length] = 1;
#if HASH1_DEBUG        
    printf("ul_hashValue[%d]:%ld\n",str_length,ul_hashValue[str_length]);
#endif
        
    return (final_sum%BLOOMFILTERSIZE);
}

/*
Function Name:
Descriptions:
Parameters:
Return Type:
*/
unsigned int hashFunctionTwo(char *cPtr_string,int flag)
{
	
	int i = 0;    
	int str_length = 0;
	static unsigned long ul_hashValue[10];
        /**(cPtr_string+strlen(cPtr_string)-1) = '\0'; */

       	str_length = strlen(cPtr_string);
#if HASH2_DEBUG        
        printf("String length: %d \n",str_length);       
        printf("String is : ");

        while(*(cPtr_string+i) != '\0')
	{
               printf("%c",*(cPtr_string+i));
               i++;
        }
        printf("\n");

#endif	
       if(flag == 0)
	{
	     /*For 1: initialize array to 1*/
#if HASH2_DEBUG        
             printf("\nFor 1\n"); 
#endif
	     for(i=0; i<=str_length; i++)
 		{
                    ul_hashValue[i]=1;
#if HASH2_DEBUG                            
                    printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif
	 	}
	     /* 
	        For 2: calculate hashIndex for length of string. 
	        Value of last index has hash value
	     */	
#if HASH2_DEBUG                        
	     printf("\n\nFor 2\n");	         
#endif
	     ul_hashValue[0]=*cPtr_string;
#if HASH2_DEBUG        	     
	     printf("ul_hashValue[%d] : %ld \n",0,ul_hashValue[0]);
#endif	     
	    	
             for(i=1;i<=str_length-1;i++)
		{
		    ul_hashValue[i]= *(cPtr_string+i) + (ul_hashValue[i-1]<<6) + (ul_hashValue[i-1]<<16) - ul_hashValue[i-1];
#if HASH2_DEBUG        		    
                    printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif                    
	        }
        }
       
        if(flag == 1)
        {
             for(i=1;i<=str_length-2;i++)
	        {
		    ul_hashValue[i] = ul_hashValue[i] - ( ul_hashValue[0] * (pow(65599,i)));
#if HASH2_DEBUG        		    		    
		    printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif
	        }


             for(i=0;i<=str_length-2;i++)
		{
                    ul_hashValue[i] = ul_hashValue[i+1];
#if HASH2_DEBUG        		                        
	            printf("ul_hashValue[%d] : %ld \n",i,ul_hashValue[i]);
#endif	            
	        }
#if HASH2_DEBUG        		    
              printf("Character: %c \n",*(cPtr_string+(str_length-1)));
#endif              
	      ul_hashValue[str_length-1] = ( ul_hashValue[str_length-2]<<6) + ( ul_hashValue[str_length-2]<<16) -  ul_hashValue[str_length-2] + 							                    *(cPtr_string+str_length-1);
	     	  
        }
       
 	return (ul_hashValue[str_length-1]%BLOOMFILTERSIZE);   

}
