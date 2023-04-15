#define VT        P3_0_bit
#define D0        P3_1_bit
#define D1        P3_2_bit
#define D2        P3_3_bit
#define D3        P3_4_bit
#define control   P3_7_bit

#define RLA1      P1_0_bit
#define RLA2      P1_1_bit
#define RLA3      P1_2_bit
#define RLA4      P1_3_bit


void main() 
{
     P1 = 0xFF;
     P3 = 0xFF;
     
     while(1)
     {
         while(!VT);
         if(control)
         {
             if(D0 == 1)
             {
                   RLA1 = ~RLA1;
             }
             if(D1 == 1)
             {
                   RLA2 = ~RLA2;
             }
             if(D2 == 1)
             {
                   RLA3 = ~RLA3;
             }
             if(D3 == 1)
             {
                   RLA4 = ~RLA4;
             }
             delay_ms(200);
         }
         else
         {
             if(D0 == 1)
             {
                   RLA1 = 0;
             }
             if(D1 == 1)
             {
                   RLA2 = 0;
             }
             if(D2 == 1)
             {
                   RLA3 = 0;
             }
             if(D3 == 1)
             {
                   RLA4 = 0;
             }
             delay_ms(100);
             P1 |= 0x0F;
         }
         delay_ms(100);
     };
}