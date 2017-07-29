#include "ResourceAwarePatchGeneration.h"
#include <iostream>

using namespace std;

void traverse_from_current_to_pi (Abc_Ntk_t* ,Abc_Obj_t* );

void ResourceAwarePatchGenerator::construct_t(bool single_target)
{
	cout << "[INFO] start to construct t" << endl;	
	if (single_target)
	{
		Abc_Obj_t* iter_obj; 
		Abc_Obj_t* iter_pi;
		Abc_Obj_t* iter_po;
		int i;
		//Abc_Aig_t* pMan=(Abc_Aig_t*)initial_F->pManFunc; 
		Abc_Obj_t* const1=Abc_AigConst1(initial_F);
		Abc_Obj_t* const0=Abc_ObjNot(const1);
	 	Abc_Ntk_t* t_ntk=Abc_NtkAlloc(ABC_NTK_STRASH,ABC_FUNC_AIG,1);
		//Abc_Obj_t* const1=Abc_NtkCreateConst1(initial_F);
	 	Abc_NtkForEachPi(initial_F,iter_pi,i)
		{
			Abc_ObjSetCopy(iter_pi,Abc_NtkCreatePi(t_ntk));
		}
		Abc_ObjSetCopy(t_list[0],const1);
		Abc_ObjSetData(t_list[0],const0);

		Abc_NtkForEachPo(initial_F,iter_po,i)
		{
			if (i<initial_F_PO_num)
			{
				traverse_from_current_to_pi(t_ntk,Abc_ObjFanin0(iter_po));
			}	
		}
		
		

		/*	
		Abc_ObjForEachFanout(t_list[0],iter_obj,i)
		{
			Abc_ObjAddFanin(iter_obj,const1);	
		}	
		Abc_NtkDeleteObj(t_list[0]);
	 	Abc_NtkForEachPi(initial_F,iter_pi,i)
	 	{
			Abc_ObjSetCopy(iter_pi,iter_pi);
	 	}
		
  		//solver.convert_ntk_to_aig_with_base_func(false);
	 	Abc_NtkForEachPo(initial_F,iter_po,i)
		{
			if (i<initial_F_PO_num)
				traverse_from_current_to_pi(initial_F,iter_po);
		}
		*/
	}
}


void recursive_create_f1_f0(Abc_Obj_t* current_node, vector<Abc_Obj_t*> f1_po, vector<Abc_Obj_t*> f0_po)
{
	//if (current_node==target)
	/*	
 	*	create const1,const0
 	*	for each fanout, link to const1, create new node link to const0
 	*	recursive_create_f1_f0 fanout
 	*/
	//else
	/*
 	* 	
 	*/
}


void traverse_from_current_to_pi (Abc_Ntk_t* new_ntk,Abc_Obj_t* current_node)
{
	int i;
	if (Abc_ObjCopy(current_node)!=0)
		return;
        Abc_Obj_t* iter_node;	 
	Abc_ObjForEachFanin (current_node,iter_node,i)
	{
		traverse_from_current_to_pi(new_ntk,iter_node);
	}
	//cout << Abc_ObjChild0Copy(current_node) <<" " << Abc_ObjChild1Copy(current_node) << endl;
	if (Abc_ObjIsPo(current_node))
		return;
	Abc_ObjSetCopy(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Copy(current_node),Abc_ObjChild1Copy(current_node)));
	if (Abc_ObjChild0Data(current_node)!=0&&Abc_ObjChild1Data(current_node)!=0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Data(current_node),Abc_ObjChild1Data(current_node)));
	}
	else if (Abc_ObjChild0Data(current_node)!=0&&Abc_ObjChild1Data(current_node)==0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Data(current_node),Abc_ObjChild1Copy(current_node)));
	}
	else if (Abc_ObjChild0Data(current_node)==0&&Abc_ObjChild1Data(current_node)!=0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Copy(current_node),Abc_ObjChild1Data(current_node)));
	}
		
	//count++;
	//cout << count << endl;
	

	
}
