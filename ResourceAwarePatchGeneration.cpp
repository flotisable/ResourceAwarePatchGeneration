#include "ResourceAwarePatchGeneration.h"

#include "base/main/main.h"

ResourceAwarePatchGenerator::ResourceAwarePatchGenerator(string F_file, string G_file, string W_file)
{
  Abc_Start();
  pAbc = Abc_FrameGetGlobalFrame(); 
  
  in_F_file       = F_file;
  in_G_file       = G_file;
  in_W_file       = W_file;
} 

ResourceAwarePatchGenerator::~ResourceAwarePatchGenerator()
{
  Abc_Stop();
}
