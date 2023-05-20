// Mod by ackerman (convert C, refactor, optimice)
//Informacion del SID
//https://old8bits.blogspot.com/2013/10/el-sonido-en-8-bits-v-el-c64.html
//https://programacion-retro-c64.blog/2019/04/16/los-registros-del-sid/
//Modificacion del codigo de Scott Hutter's
//Nro. 	    Posici�n 	    Uso
//0 a 6 	54272 a 54278 	Control del primer canal de sonido
//7 a 13 	54279 a 54285 	Control del segundo canal de sonido
//14 a 20 	54286 a 54292 	Control del tercer canal de sonido
//21     	54293       	Frecuencia baja
//22    	54294       	Frecuencia alta
//23 	    54295 	
//24     	54296       	Control de volumen y filtros
//25    	54297 	
//26 	    54298 	
//27 	    54299 	
//28 	    54300 	


#include "gbConfig.h"
#include "gbGlobals.h"
#include "sid.h"
#include <stdio.h>
#include "Arduino.h"

//Sid::Sid()
//{
// printf("SID contructor\n");
// fflush(stdout);
// volume= 0;
// freqLo= 0;
// freqHi= 0;
// frequency= 0;  
// printf("FIN SID contructor\n");
// fflush(stdout);
//}

//Sid::~Sid()
//{
// printf("SID destroy\n");
// fflush(stdout);
//}

//void Sid::write_register(unsigned char r, unsigned char v)
//{
// printf("SID write %02X %02X\n",r,v);
// fflush(stdout);
//  switch(r)
//  {
//    //canal 0
//    case 0x00:
//      freqLo = v;
//      play();
//      break;
//    case 0x01:
//      freqHi = v;
//      play();
//      break;
//    
//    //Canal 1  
//    case 0x07:
//      freqLo = v;
//      play();
//      break;
//    case 0x08:
//      freqHi = v;
//      play();
//      break;
//    
//    //canal 2  
//    case 0x0e:
//      freqLo = v;
//      play();
//      break;
//    case 0x0f:
//      freqHi = v;
//      play();
//      break;
//    
//    //volumen mixer  
//    case 0x18:	// volume
//      //printf("SID write 0x18\n");
//      //fflush(stdout);
//      //volume = v;
//      //play();
//      break;
//  }
//
// printf("SID write FIN\n");
// fflush(stdout);
//}

//void Sid::play()
//{
// printf("SID play\n");
// fflush(stdout);
//  if(volume == 0)
//  {
//    //JJ speaker_->Nosound();
//  }
//  else
//  {
//    frequency = getFrequency(freqHi, freqLo);
//    
//    if(frequency>0)
//    {
//      //JJ speaker_->Sound(frequency); ///1000);
//    }
//  }
//}

//unsigned int Sid::getFrequency(unsigned char hi, unsigned char lo)
//{  
// printf("SID getFrequency %02X %02X\n",hi,lo);
// fflush(stdout);         
// unsigned short int v = hi * 256 + lo;
// double auxDouble= (v * .06097);
// 
//
// 
// return ((unsigned int)auxDouble);
//
///*  
// if (v== 0x0112) return 16351;
// if (v== 0x0123) return 17324;
// if (v== 0x0134) return 18354;
// if (v== 0x0146) return 19445;
// if (v== 0x015A) return 20601;
// if (v== 0x016E) return 21827;
// if (v== 0x0184) return 23124;
// if (v== 0x019B) return 24499;
// if (v== 0x01B3) return 25956;
// if (v== 0x01CD) return 27500;
// if (v== 0x01E9) return 29135;
// if (v== 0x0206) return 30868;
// if (v== 0x0225) return 32703.;
// if (v== 0x0245) return 34648;
// if (v== 0x0268) return 36708;
// if (v== 0x028C) return 38891;
// if (v== 0x02B3) return 41203;
// if (v== 0x02DC) return 43654;
// if (v== 0x0308) return 46249;
// if (v== 0x0336) return 48999;
// if (v== 0x0367) return 51913;
// if (v== 0x039B) return 55000;
// if (v== 0x03D2) return 58270;
// if (v== 0x040C) return 61735;
// if (v== 0x0449) return 65406;
// if (v== 0x048B) return 69296;
// if (v== 0x04D0) return 73416;
// if (v== 0x0519) return 77782;
// if (v== 0x0567) return 82407;
// if (v== 0x05B9) return 87307;
// if (v== 0x0610) return 92499;
// if (v== 0x066C) return 97999;
// if (v== 0x06CE) return 103826;
// if (v== 0x0735) return 110000;
// if (v== 0x07A3) return 116541;
// if (v== 0x0817) return 123471;
// if (v== 0x0893) return 130813;
// if (v== 0x0915) return 138591;
// if (v== 0x099F) return 146832;
// if (v== 0x0A3C) return 155563;
// if (v== 0x0ACD) return 164814;
// if (v== 0x0B72) return 174614;
// if (v== 0x0C20) return 184997;
// if (v== 0x0CD8) return 195998;
// if (v== 0x0D9C) return 207652;
// if (v== 0x0E6B) return 220000;
// if (v== 0x0F46) return 233082;
// if (v== 0x102F) return 246942;
// if (v== 0x1125) return 261626;
// if (v== 0x122A) return 277183;
// if (v== 0x133F) return 293665;
// if (v== 0x1464) return 311127;
// if (v== 0x159A) return 329628;
// if (v== 0x16E3) return 349228;
// if (v== 0x183F) return 369994;
// if (v== 0x19B1) return 391995;
// if (v== 0x1B38) return 415305;
// if (v== 0x1CD6) return 440000;
// if (v== 0x1E8D) return 466164;
// if (v== 0x205E) return 493883;
// if (v== 0x224B) return 523251;
// if (v== 0x2455) return 554365;
// if (v== 0x267E) return 587330;
// if (v== 0x28C8) return 622254;
// if (v== 0x2B34) return 659255;
// if (v== 0x2DC6) return 698456;
// if (v== 0x307F) return 739989;
// if (v== 0x3461) return 783991;
// if (v== 0x366F) return 830609;
// if (v== 0x39AC) return 880000;
// if (v== 0x3D7E) return 932328;
// if (v== 0x40BC) return 987767;
// if (v== 0x4495) return 1046502;
// if (v== 0x48A9) return 1108731;
// if (v== 0x4CFC) return 1174659;
// if (v== 0x51A1) return 1244508;
// if (v== 0x5669) return 1318510;
// if (v== 0x5B8C) return 1396913;
// if (v== 0x60FE) return 1479978;
// if (v== 0x66C2) return 1567982;
// if (v== 0x6CDF) return 1661219;
// if (v== 0x7358) return 1760000;
// if (v== 0x7A34) return 1864655;
// if (v== 0x8178) return 1975533;
// if (v== 0x892B) return 2093005;
// if (v== 0x9153) return 2217461;
// if (v== 0x99F7) return 2349318;
// if (v== 0xA31F) return 2489016;
// if (v== 0xACD2) return 2637021;
// if (v== 0xB719) return 2793826;
// if (v== 0xC1FC) return 2959955;
// if (v== 0xCD85) return 3135964;
// if (v== 0xD9BD) return 3322438;
// if (v== 0xE6B0) return 3520000;
// if (v== 0xF467) return 3729310;
//  
// return 0;
// */
//}















//Conversion a C
void jj_SID_Init()
{
 Serial.printf("SID jj_SID_Init\r\n");
 //fflush(stdout);

 for (unsigned char i=0;i<3;i++)
 {
  gb_sid_freqLo[i]= 0;
  gb_sid_freqHi[i]= 0;
  gb_sid_volume[i]= 0;
  gb_sid_frequency[i]= 0;
 }
 
 Serial.printf("FIN SID jj_SID_Init\r\n");
 //fflush(stdout);
}


unsigned int jj_SID_getFrequency(unsigned char hi, unsigned char lo)
{
 //printf("JJSID getFrequency %02X %02X\n",hi,lo);
 //fflush(stdout);         
 unsigned short int v = hi * 256 + lo;
 double auxDouble= (v * .06097);
 
 unsigned int aReturn= (unsigned int)auxDouble;
 if (aReturn >1000){
  aReturn= 1000; //Cortamos filtro
 }
 
 return (aReturn);         
}

void jj_SID_play(unsigned char canal)
{
 //printf("JJSID play\n");
 //fflush(stdout);
 canal= (canal &0x03);
 
  if(gb_sid_volume[canal] == 0)
  {
    //JJ speaker_->Nosound();
    //gb_volumen01= 0;
    //gb_frecuencia01= 0;
    //nes_gbFrecMixer_now[canal]= 0;
    for (unsigned char i=0;i<3;i++){
     gbVolMixer_now_poll[i]= 0;
    }
    //gbFrecMixer_now[canal]= 0;
  }
  else
  {
    gb_sid_frequency[canal] = jj_SID_getFrequency(gb_sid_freqHi[canal], gb_sid_freqLo[canal]);
    
    if(gb_sid_frequency[canal]>0)
    {
      //JJ speaker_->Sound(frequency); ///1000);
      //gb_volumen01= 15;
      //gb_frecuencia01= gb_sid_frequency;
      //nes_gbFrecMixer_now[canal]= gb_sid_frequency[canal];
      
//      for (unsigned char i=0;i<3;i++){
//       gbVolMixer_now_poll[i]= 15;
//      }      
      gbVolMixer_now_poll[0]= 15; //Solo el primer canal.Hay que quitarlo
      
      gbFrecMixer_now_poll[canal]= gb_sid_frequency[canal];      
    }
  }     
}


void jj_SID_write_register(unsigned char r, unsigned char v)
{
 //printf("JJSID write %02X %02X\n",r,v);
 //fflush(stdout);


  switch(r)
  {
    //canal 0       
    case 0x00:
      gb_sid_freqLo[0] = v;
      jj_SID_play(0);
      break;
    case 0x01:
      gb_sid_freqHi[0] = v;
      jj_SID_play(0);
      break;
      
    //canal 1  
    case 0x07:
      gb_sid_freqLo[1] = v;
      jj_SID_play(1);
      break;
    case 0x08:
      gb_sid_freqHi[1] = v;
      jj_SID_play(1);
      break;
      
    //canal 2  
    case 0x0e:
      gb_sid_freqLo[2] = v;
      jj_SID_play(2);
      break;
    case 0x0f:
      gb_sid_freqHi[2] = v;
      jj_SID_play(2);
      break;
      
    //control canal0  
    case 4:
     //printf("Ctrl1 %02X\n",(v&0x01)); fflush(stdout);    
     gbVolMixer_now_poll[0]= ((v&0x01)==0x01) ? 15: 0;
     gbVolMixer_now[0]=gbVolMixer_now_poll[0];
     break;          
    //control canal1 
    case 11:
     //printf("Ctrl2 %02X\n",(v&0x01)); fflush(stdout);
     gbVolMixer_now_poll[1]= ((v&0x01)==0x01) ? 15: 0;
     gbVolMixer_now[1]=gbVolMixer_now_poll[1];
     break;     
    //control canal2  
    case 18: 
     //printf("Ctrl3 %02X\n",(v&0x01)); fflush(stdout);
     gbVolMixer_now_poll[2]= ((v&0x01)==0x01) ? 15: 0;
     gbVolMixer_now[2]=gbVolMixer_now_poll[2];
     break;
     
            
    case 16: //printf("Ancho3 Hi %02X\n",v); fflush(stdout);
     break;
    case 17: //printf("Ancho3 Lo %02X\n",v); fflush(stdout);
     break;
    case 19: //printf("Ataque3 %02X\n",v); fflush(stdout);    
     break;
    case 20: //printf("Sostenido %02X\n",v); fflush(stdout);    
     break;      
      
    case 0x18:	// volume
      //printf("SID write 0x18 VOL %02X\n",(v&0x0F));
      if ((v>>7)==1)
      {
       gbVolMixer_now_poll[2]= 0;
       gbVolMixer_now[2]=gbVolMixer_now_poll[2];
       //printf("Desconecta\n");
      }      
      //(stdout);
      for(unsigned char i=0;i<3;i++)
      {
       gb_sid_volume[i] = (v&0x0F);
       jj_SID_play(i);
      }
      //gb_sid_volume[0] = 15;      
      break;
  }

 //printf("JJSID write FIN\n");
 //fflush(stdout);
}

//***************************************
void jj_sid_mute()
{
 for(unsigned char i=0;i<3;i++)
 {
  gb_sid_volume[i] = 0;
  gb_sid_frequency[i]= 0;
  gb_sid_freqLo[i]= 0;
  gb_sid_freqHi[i]= 0;
 }
 jj_sound_mute();     
}

//***************************************

void jj_sound_mute()
{
 for(unsigned char i=0;i<3;i++)
 {
  gbVolMixer_prev[i] = gbVolMixer_now_poll[i];
  gbFrecMixer_prev[i]= gbFrecMixer_now_poll[i];
              
  gbVolMixer_now_poll[i] = 0;
  gbFrecMixer_now_poll[i]= 0;
  gbVolMixer_now[i] = 0;
  gbFrecMixer_now[i]= 0;  
  //gb_sid_volume[i] = 0;
 }     
}

//*****************************************
void jj_sound_no_mute()
{
 for(unsigned char i=0;i<3;i++)
 {
  gbVolMixer_now_poll[i] = gbVolMixer_prev[i];
  gbFrecMixer_now_poll[i]= gbFrecMixer_prev[i]; 
 }     
}
