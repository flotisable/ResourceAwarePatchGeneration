#include "ResourceAwarePatchGeneration.h"

#include "base/main/main.h"

ResourceAwarePatchGenerator::ResourceAwarePatchGenerator(string F_file, string G_file, string W_file, string o_p_file, string o_F_file)
{
  Abc_Start();
  pAbc = Abc_FrameGetGlobalFrame(); 
  
  in_F_file       = F_file;
  in_G_file       = G_file;
  in_W_file       = W_file;
  
  out_Patch_file  = o_p_file;
  out_F_file      = o_F_file;


  read_file(F_file,G_file,W_file);
  delete_unused_PO();
  //pre_process(W_file);
} 

ResourceAwarePatchGenerator::~ResourceAwarePatchGenerator()
{
  Abc_Stop();
}
