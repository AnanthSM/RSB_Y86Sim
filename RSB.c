#include<stdio.h>
#include<string.h> 
#include<stdlib.h>

typedef struct reg {
    char name[2]; 
    int value; 

} reg; 

/* enconding for registers 
   a1 = 0001 
   a2 = 0010
   a3 = 0011
   a4 = 0100
   */ 

int global_num = 0 ;
int global_flag = 0;

//upto 16 registers 2 reserved we have taken 4 for simplicity
reg register_file[5] ={
 {"a1",20},
 {"a2",40},
 {"a3",30},
 {"a4",40} } ; 

void display_registers()
{
    printf("\n");
    for(int i = 0; i < 4 ;i++)
    {
        printf("%s:%d \n",register_file[i].name,register_file[i].value);
    }
}
//for positive binary 
void to_binary(int* bin,int num)
{
    int i;
    for(i=0;num>0;i++)    
    {    
        bin[i]=num%2;    
        num=num/2;    
    }   

}
//for negative binary 
void to_binary_neg(int* bin,int num)
{
        int i = num;
        unsigned int u_i = 0;
        int j = 0;

        u_i = (unsigned int)i;

        for(j=7;j>=0;j--) {
            bin[j] = u_i & 0x1;
            u_i  = u_i >> 1;
        }
}
    
 


int AUL(int* encoding)
{   
    int  cond[4] ={0} ;
    int val_A=0,val_B = 0 , val_C =0;
    int set_cond = encoding[20]; 
    //26 and 27 bits are zero so its a data processing instruction
    //getting first value in src register
    if(encoding[16] == 1 && encoding[17] ==0  && encoding[18] == 0 && encoding[19] == 0)
    {
        val_A = register_file[0].value ;
    }
    else if(encoding[16] == 0 && encoding[17] ==1  && encoding[18] == 0 && encoding[19] == 0)
    {
        val_A = register_file[1].value ;
    }
    else if(encoding[16] == 1 && encoding[17] == 1  && encoding[18] == 0 && encoding[19] == 0)
    {
        val_A = register_file[2].value ;
    }
    else if(encoding[16] == 0 && encoding[17] ==1  && encoding[18] == 0 && encoding[19] == 0)
    {
        val_A = register_file[3].value ;
    }

    //getting second value in src register or immediate value 
    if(encoding[25] == 1)
    {
        val_B = global_num ; 
    }
    else  if(encoding[25] == 0)
    {
        if(encoding[0] == 1 && encoding[1] ==0  && encoding[2] == 0 && encoding[3] == 0)
        {
            val_B = register_file[0].value ;
        }
        else if(encoding[0] == 0 && encoding[1] ==1  && encoding[2] == 0 && encoding[3] == 0)
        {
            val_B = register_file[1].value ;
        }
        else if(encoding[0] == 1 && encoding[1] == 1  && encoding[2] == 0 && encoding[3] == 0)
        {
            val_B = register_file[2].value ;
        }
        else if(encoding[0] == 0 && encoding[1] ==1  && encoding[2] == 0 && encoding[3] == 0)
        {
             val_B = register_file[3].value ;
        }

    }

    
    val_C = val_B - val_A ; 


    if(encoding[12] == 1 && encoding[13] ==0  && encoding[14] == 0 && encoding[15] == 0)
    {
        register_file[0].value = val_C;
    }
    else if(encoding[12] == 0 && encoding[13] ==1  && encoding[14] == 0 && encoding[15] == 0)
    {
        register_file[1].value  = val_C ;
    }
    else if(encoding[12] == 1 && encoding[13] == 1  && encoding[14] == 0 && encoding[15] == 0)
    {
        register_file[2].value  = val_C ;   
    }
    else if(encoding[12] == 0 && encoding[13] ==1  && encoding[14] == 0 && encoding[15] == 0)
    {
        register_file[3].value  = val_C ;
    }
    
    display_registers();

    for(int i = 28, j = 0; i < 32 , j < 4; i++ , j++)
    {
        cond[j] = encoding[i];
        //printf("%d" , cond[j]) ;
    }
    if(cond[0] == 1 && cond[1] == 1 && cond[2] == 1 && cond[3] == 0)
    {
       // printf("No condtion will be checked \n");
    }
    


}

int main(int argc,char* argv[])
{
    int cond = 0; 
    int encoding[33] = {0};
    int i;
    FILE *file; 
    //setting condition felid to always ignore codnitions 
     
    encoding[28] = 1 ;
    encoding[29] = 1 ;
    encoding[30] = 1 ;
    encoding[31] = 0 ;

    if(argc <= 4)
    {
        printf("Please enter the right format!\n");
        exit(1); 
    }

    if(strcmp(argv[1],"RSB") == 0 || strcmp(argv[1],"RSBS") == 0 )
    {
        encoding[21] = 0;
        encoding[22] = 0;
        encoding[23] = 1;
        encoding[24] = 1;

        if(strcmp(argv[1],"RSBS") == 0)
        {
            encoding[20] = 1; 
        }
    } 
    else
    {
        printf("Only RSB istruction is supported \n");
        exit(1);

    }

    //first operand register
    if(strcmp(argv[2],"a1") == 0 || strcmp(argv[2],"a2") == 0 || strcmp(argv[2],"a3") == 0 || strcmp(argv[2],"a4") == 0)
    {
        if(strcmp(argv[2],"a1") == 0)
        {
            encoding[16] = 1;
            encoding[17] = 0;
            encoding[18] = 0;
            encoding[19] = 0;
        }
        if(strcmp(argv[2],"a2") == 0)
        {
            encoding[16] = 0;
            encoding[17] = 1;
            encoding[18] = 0;
            encoding[19] = 0;
        }
        if(strcmp(argv[2],"a3") == 0)
        {
            encoding[16] = 1;
            encoding[17] = 1;
            encoding[18] = 0;
            encoding[19] = 0;
        }
        if(strcmp(argv[2],"a4") == 0)
        {
            encoding[16] = 0;
            encoding[17] = 0;
            encoding[18] = 1;
            encoding[19] = 0;
        }
    }
    else
    {
        printf("the vaild registers are a1 a2 a3 \n");
        exit(1);
    }

    //destinantion register 
     if(strcmp(argv[3],"a1") == 0 || strcmp(argv[3],"a2") == 0 || strcmp(argv[3],"a3") == 0 || strcmp(argv[3],"a4") == 0)
     {
        if(strcmp(argv[3],"a1") == 0)
        {
            encoding[12] = 1;
            encoding[13] = 0;
            encoding[14] = 0;
            encoding[15] = 0;
        }
        if(strcmp(argv[3],"a2") == 0)
        {
            encoding[12] = 0;
            encoding[13] = 1;
            encoding[14] = 0;
            encoding[15] = 0;
        }
        if(strcmp(argv[3],"a3") == 0)
        {
            encoding[12] = 1;
            encoding[13] = 1;
            encoding[14] = 0;
            encoding[15] = 0;
        }
        if(strcmp(argv[3],"a4") == 0)
        {
            encoding[12] = 0;
            encoding[13] = 0;
            encoding[14] = 1;
            encoding[15] = 0;
        }
     }

    else
    {
        printf("the vaild registers are a1 a2 a3 \n");
        exit(1);
    }
    //second operand either a register or immediate value 
    if(strcmp(argv[4],"a1") == 0 || strcmp(argv[4],"a2") == 0 || strcmp(argv[4],"a3") == 0 || strcmp(argv[4],"a4") == 0)
     {
        encoding[25] = 0; //setting the I bit to 0 because the second source is a register
        if(strcmp(argv[4],"a1") == 0)
        {
            encoding[0] = 1;
            encoding[1] = 0;
            encoding[2] = 0;
            encoding[3] = 0;
        }
        if(strcmp(argv[4],"a2") == 0)
        {
            encoding[0] = 0;
            encoding[1] = 1;
            encoding[2] = 0;
            encoding[3] = 0;
        }
        if(strcmp(argv[4],"a3") == 0)
        {
            encoding[0] = 1;
            encoding[1] = 1;
            encoding[2] = 0;
            encoding[3] = 0;
        }
        if(strcmp(argv[4],"a4") == 0)
        {
            encoding[0] = 0;
            encoding[1] = 0;
            encoding[2] = 1;
            encoding[3] = 0;
        }
     }

    else if( atoi(argv[4]) <= 255 && atoi(argv[4]) > 0 )
    {
        encoding[25] = 1;  //setting the I bit to 1 because the second source is a immediate value
        int num = atoi(argv[4]);
        int bin[8] ={0} ; 
        global_num = num;
        to_binary(bin,num); 
        for(int i = 0;i <= 8; i++)
        {
            encoding[i] = bin[i];
        }
        
    }
    else if( atoi(argv[4]) >= -255 && atoi(argv[4]) < 0)
    {
        encoding[25] = 1;
        int num = atoi(argv[4]);
        int bin[8] = {0}; 
        to_binary_neg(bin,num);
        global_num = num; 
        for(int i = 0;i <=8 ; i++)
        {
            encoding[i] = bin[i];
        }
    }

    else
    {
        printf("source must be a register or 8-bit immediate value\n");
        exit(1);
    }



    printf("\n");



    for(int i = 32; i > 0 ; i--)
    {
        printf("%d",encoding[i]);
    }

    display_registers(); 

    AUL(encoding);



}
