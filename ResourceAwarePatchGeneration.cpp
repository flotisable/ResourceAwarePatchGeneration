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
  for (int i=0;i<gate_list.size();i++)
  {
	delete gate_list[i];
  }
  Abc_NtkDelete(initial_G);
  Abc_Stop();
}
