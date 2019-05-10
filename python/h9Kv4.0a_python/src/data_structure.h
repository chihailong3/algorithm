#ifndef __DATA_STRUCTURE__
#define  __DATA_STRUCTURE__



typedef union
{
	int	ind;
	char	anlg;
	struct
	{		
		int 	sum;			
		char	ppeak;			
	} imp;
} RD_VAR_UNION;
typedef struct
{
	char	cabinet_no[100];
    char	unit_no[1];       	
    char	slot_no[1];		
    char	channel_no[6];	        
    char	terminal_no[6];		
} HARD_ADDR;





#endif