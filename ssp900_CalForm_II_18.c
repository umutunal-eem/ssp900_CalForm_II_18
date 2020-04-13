#include <16f628a.h>
#fuses XT,NOPUT,NOMCLR,NOBROWNOUT,NOLVP,NOCPD,NOPROTECT,NOWDT
#use delay (clock=4000000)  
#use fast_io(b)

#use fast_io(a)
#define use_portb_lcd TRUE  // lcd kütüphanesinin 'b' portunda çalýþmasýný soyluyoruz.
#include <lcd.c>


int sayac=0,duty=0,adim=0;
const int deger[]={4,5,6,7,8,9,10,9,8,7,6,5,4};
int16 gecikme=0;

#int_timer0


void kesme(){
set_timer0(231);   //25 elde ediyoruz.   T=1/f  f=4MHz/4=1Mhz=1*10^6 Hz
                   //bölme orani= 8*(256-231)==200us==0.2ms
                   //Kesme Olusturma Süresi=Tkomut*(Bölme Orani)*(256-yazýlan deger)
if(sayac==0)
output_high(pin_a2);

if(sayac>=duty)
output_low(pin_a2);
sayac++;
if(sayac==100)
sayac=0;

gecikme++;
if(gecikme>1000){
gecikme=0;
adim++;
if(adim==12)
adim=0;
}

}



unsigned long int sure;             //16 bit lik iþaretsiz süre deðiþkeni
float mesafe;                         //Ondalýklý mesafe deðiþkeni

void main()
{


   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8); //RTCC SAYICI OLARAK KULLANIYOR
  
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);    //Timer1 ayarlarý yapýlýyor dahili clock kaynaðý ve bölme oraný 1
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
  set_timer0(231);
  
   enable_interrupts(INT_timer0);
   enable_interrupts(GLOBAL);
   
   set_tris_b(0x00); //Giriþ çýkýþ ayarlamalarý
   set_tris_a(0x02);
  
   
   output_low(pin_a0);     //Sensörün trigger ucu pasif yapýlýyor
   mesafe=0;                    //Deðiþkenler sýfýrlanýyor
   sure=0;
   lcd_init();
   delay_us(10);
   printf(lcd_putc,"Cihaz Hazilaniyor...");
   while(true)                //Sonsuz döngü
   {
      output_high(pin_a0); //Trigger ucu aktif
      delay_us(15);        //15us bekleme
      output_low(pin_a0);  //Trigger ucu pasif
      //Sensör okumaya hazýr
      while(!(input(pin_a1))) //Echo ucu 1 olana kadar hiçbir iþlem yapma
      {
      }
      set_timer1(0);          //Echo ucu 1 olduðu zaman timer1'i sýfýrla
      
      while((input(pin_a1)))  //Echo ucu 0 olana kadar hiçbir iþlem yapma
      {
      }
      sure=get_timer1();         //Echo ucu 0 olduðunda timer1 deðerini c deðiþkenine at (bizim süre deðiþkenimiz)
      mesafe=sure/58;                 //Mesafeyi cm olarak ölçmek için süre deðiþkenini 58'e bölüyorum
    if(mesafe<=10){
    output_high(pin_a3);
    delay_ms(20);
    output_low(pin_a3);
    delay_ms(20);
    }
    if(mesafe<=20 && mesafe>10)
    {
    output_high(pin_a3);
    delay_ms(50);
    output_low(pin_a3);
    delay_ms(50);
    }
    if(mesafe<100 && mesafe>20){
     output_high(pin_a3);
    delay_ms(100);
    output_low(pin_a3);
    delay_ms(100);
    }
    if(mesafe<200 && mesafe>=100){
    output_high(pin_a3);
    delay_ms(200);
    output_low(pin_a3);
    delay_ms(200);
    
    }
      printf(lcd_putc,"\fmesafe=%f\nsure=%lu",mesafe,sure);
      
      duty=deger[adim];

      
   }
}
