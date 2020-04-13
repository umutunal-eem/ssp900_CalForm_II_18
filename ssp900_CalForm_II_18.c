#include <16f628a.h>
#fuses XT,NOPUT,NOMCLR,NOBROWNOUT,NOLVP,NOCPD,NOPROTECT,NOWDT
#use delay (clock=4000000)  
#use fast_io(b)

#use fast_io(a)
#define use_portb_lcd TRUE  // lcd k�t�phanesinin 'b' portunda �al��mas�n� soyluyoruz.
#include <lcd.c>


int sayac=0,duty=0,adim=0;
const int deger[]={4,5,6,7,8,9,10,9,8,7,6,5,4};
int16 gecikme=0;

#int_timer0


void kesme(){
set_timer0(231);   //25 elde ediyoruz.   T=1/f  f=4MHz/4=1Mhz=1*10^6 Hz
                   //b�lme orani= 8*(256-231)==200us==0.2ms
                   //Kesme Olusturma S�resi=Tkomut*(B�lme Orani)*(256-yaz�lan deger)
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



unsigned long int sure;             //16 bit lik i�aretsiz s�re de�i�keni
float mesafe;                         //Ondal�kl� mesafe de�i�keni

void main()
{


   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_8); //RTCC SAYICI OLARAK KULLANIYOR
  
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);    //Timer1 ayarlar� yap�l�yor dahili clock kayna�� ve b�lme oran� 1
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
  set_timer0(231);
  
   enable_interrupts(INT_timer0);
   enable_interrupts(GLOBAL);
   
   set_tris_b(0x00); //Giri� ��k�� ayarlamalar�
   set_tris_a(0x02);
  
   
   output_low(pin_a0);     //Sens�r�n trigger ucu pasif yap�l�yor
   mesafe=0;                    //De�i�kenler s�f�rlan�yor
   sure=0;
   lcd_init();
   delay_us(10);
   printf(lcd_putc,"Cihaz Hazilaniyor...");
   while(true)                //Sonsuz d�ng�
   {
      output_high(pin_a0); //Trigger ucu aktif
      delay_us(15);        //15us bekleme
      output_low(pin_a0);  //Trigger ucu pasif
      //Sens�r okumaya haz�r
      while(!(input(pin_a1))) //Echo ucu 1 olana kadar hi�bir i�lem yapma
      {
      }
      set_timer1(0);          //Echo ucu 1 oldu�u zaman timer1'i s�f�rla
      
      while((input(pin_a1)))  //Echo ucu 0 olana kadar hi�bir i�lem yapma
      {
      }
      sure=get_timer1();         //Echo ucu 0 oldu�unda timer1 de�erini c de�i�kenine at (bizim s�re de�i�kenimiz)
      mesafe=sure/58;                 //Mesafeyi cm olarak �l�mek i�in s�re de�i�kenini 58'e b�l�yorum
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
