#ifndef _GBPRG_H
 #define _GBPRG_H
    
 #include "prg/prgfields1996.h"
 #include "prg/prgmonopole.h"
 #include "prg/prgonstellthefuhrer.h"
 #include "prg/prgbmp1.h"
 #include "prg/prgilkkesirmarmot.h"
 #include "prg/prgmulti.h"
 #include "prg/prgthesatelliteman.h"
 #include "prg/prgeyesofthestorm.h"
 #include "prg/prglostworld.h"
 #include "prg/prgpacmanatari.h"


 #define max_list_prg 10
 
 static const char * gb_list_prg_title[max_list_prg]={  
  "fields1996",
  "monopole",
  "onstellthefuhrer",
  "bmp1",
  "ilkkesirmarmot",
  "multi",
  "thesatelliteman",
  "eyesofthestorm",
  "lostworld",
  "pacmanatari"
 };
 
 
 static const unsigned short int gb_list_prg_size[max_list_prg]={
  29873,
  24091,
  16146,
  15337,
  16385,
  16146,
  10608,
  14145,
  10608,
  8250,
 };
  
 static const unsigned char * gb_list_prg_data[max_list_prg]={
  gb_prg_fields1996,
  gb_prg_monopole,
  gb_prg_onstellthefuhrer,
  gb_prg_bmp1,
  gb_prg_ilkkesirmarmot,
  gb_prg_multi1,
  gb_prg_theSatelliteMan,
  gb_prg_eyesOfTheStorm,
  gb_prg_lostWorld,
  gb_prg_pacmanatari
 };
 
 
#endif
