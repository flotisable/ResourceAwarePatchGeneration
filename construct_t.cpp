#include "ResourceAwarePatchGeneration.h"
#include <iostream>
#include <set>
#include <assert.h>

using namespace std;

template <class T>
inline bool is_obj_in_set ( T obj, set<T> _set) 
{
	return _set.find(obj)!=_set.end();
}


void traverse_from_current_to_pi (Abc_Ntk_t* ,Abc_Obj_t* );

void ResourceAwarePatchGenerator::construct_t(bool single_target)
{
	cout << "[INFO] start to construct t" << endl;	
	if (single_target)				//only single target can use (f1 exor f0) and (f1 and g)
	{
		assert (t_list.size()==1);
		Abc_Obj_t* iter_obj; 
		Abc_Obj_t* iter_pi;
		Abc_Obj_t* iter_po;
		int i;
		int f_pi_count=0,f_po_count=0;
		//Abc_Aig_t* pMan=(Abc_Aig_t*)initial_F->pManFunc; 
	 	Abc_Ntk_t* t_ntk=Abc_NtkAlloc(ABC_NTK_STRASH,ABC_FUNC_AIG,1);
		Abc_Obj_t* const1=Abc_AigConst1(t_ntk);
		Abc_Obj_t* const0=Abc_ObjNot(const1);
		//construct t_po
		Abc_Obj_t* t=Abc_NtkCreatePo(t_ntk);


		Abc_Aig_t* aig_man=(Abc_Aig_t*) t_ntk->pManFunc;
		//Abc_Obj_t* const1=Abc_NtkCreateConst1(initial_F);
		Abc_NtkForEachObj(initial_F,iter_obj,i)		//initialize  F
		{
			Abc_ObjSetCopy(iter_obj,0);
			Abc_ObjSetData(iter_obj,0);
		}
		Abc_NtkForEachObj(initial_G,iter_obj,i)		//initialize  G
		{
			Abc_ObjSetCopy(iter_obj,0);
			Abc_ObjSetData(iter_obj,0);
		}


		//add pi to t, if unused, no need to add
	 	Abc_NtkForEachPi(initial_F,iter_pi,i)		
		{
			if (iter_pi!=t_list[0]) 	// check if pi is in pi_list?
			{
				f_pi_count++;
				Abc_ObjSetCopy(iter_pi,Abc_NtkCreatePi(t_ntk));
			}
		}
		//start to copy f and g to compute positive cofatctor, set copy to 1, data to 0, only reachable fanout traversed 
		Abc_ObjSetCopy(t_list[0],const1);		
		Abc_ObjSetData(t_list[0],const0);

		vector<Abc_Obj_t*> f1_f0_exor_vec;
		vector<Abc_Obj_t*> g_f1_exnor_vec;
		Abc_NtkForEachPo(initial_F,iter_po,i)
		{
			if (i<initial_F_PO_num&&is_obj_in_set(iter_po,t_Po_list[0]))
			{
				f_po_count++;
			
				traverse_from_current_to_pi(t_ntk,Abc_ObjFanin0(iter_po));
			
				Abc_Obj_t* f1_output=Abc_ObjCopy(Abc_ObjFanin0(iter_po));
				Abc_Obj_t* f0_output=(Abc_Obj_t*)Abc_ObjData(Abc_ObjFanin0(iter_po));
				//cout << f1_output <<endl;
				//cout << f0_output <<endl;
				if (f1_output==0||f0_output==0)
				{
					cout << "[WARNING] something error in construct t " << endl;
				}
				f1_f0_exor_vec.push_back(Abc_AigXor(aig_man,f1_output,f0_output));
			
			}	
			else if (i>=initial_F_PO_num)
			{
				traverse_from_current_to_pi(t_ntk,Abc_ObjFanin0(iter_po));
				Abc_Obj_t* base_po=Abc_NtkCreatePo(t_ntk);
				Abc_ObjSetCopy(iter_po,base_po);
				Abc_ObjAddFanin(base_po,Abc_ObjChild0Copy(iter_po));

			}
			
		}
		cout<< "[INFO] f1,f0 construct success  " << endl;
			
		
		//set copy of g's pi to t's, if unused, no need to add
		int g_pi_count=0,g_po_count=0;
	 	Abc_NtkForEachPi(initial_G,iter_pi,i)
		{
			//if (is_obj_in_set(Abc_NtkPi(initial_F,i),t_Pi_list[0]))
			//{
				Abc_ObjSetCopy(iter_pi,Abc_NtkPi(t_ntk,g_pi_count));
				//cout << "copy" <<  Abc_ObjCopy(iter_pi) << endl;
				g_pi_count++;
			//}
		}
		
		Abc_NtkForEachPo(initial_G,iter_po,i)
		{
			if (is_obj_in_set(Abc_NtkPo(initial_F,i),t_Po_list[0]))
			{
				g_po_count++;
				traverse_from_current_to_pi(t_ntk,Abc_ObjFanin0(iter_po));
				Abc_Obj_t* g_output=Abc_ObjCopy(Abc_ObjFanin0(iter_po));
				Abc_Obj_t* f1_output=Abc_ObjCopy(Abc_ObjFanin0(Abc_NtkPo(initial_F,i)));
				if (f1_output==0||g_output==0)
				{
					cout << "[WARNING] something error in construct t " << endl;
				}
				g_f1_exnor_vec.push_back(Abc_ObjNot(Abc_AigXor(aig_man,g_output,f1_output)));
			}
		}
		if (g_pi_count!=f_pi_count)
			cout<< "[WARNING] pi num in G is different from t_ntk's " << endl;
		if (g_po_count!=f_po_count)
			cout<< "[WARNING] po num in G is different from t_ntk's and F's  " << endl;
		cout<< "[INFO] g construct success  " << endl;
		//cout << f1_f0_exor_vec.size() << " " << g_f1_exnor_vec.size() << endl;
		
		//construct miter of exor and exnor nodes
		while (f1_f0_exor_vec.size()>1)
		{
			for(int j=0;j<f1_f0_exor_vec.size();j+=2) 
			{
				f1_f0_exor_vec[j/2]=Abc_AigOr(aig_man,f1_f0_exor_vec[j],f1_f0_exor_vec[j+1]);
			}
			if (f1_f0_exor_vec.size()%2==1)
			{
				f1_f0_exor_vec[(f1_f0_exor_vec.size()-1)/2]=f1_f0_exor_vec[f1_f0_exor_vec.size()-1];
				f1_f0_exor_vec.resize((f1_f0_exor_vec.size()+1)/2);
			}
			else
			{
				f1_f0_exor_vec.resize(f1_f0_exor_vec.size()/2);
			}
		}
	
		while (g_f1_exnor_vec.size()>1)
		{
			for(int j=0;j<g_f1_exnor_vec.size();j+=2) 
			{
				g_f1_exnor_vec[j/2]=Abc_AigAnd(aig_man,g_f1_exnor_vec[j],g_f1_exnor_vec[j+1]);
			}
			if (g_f1_exnor_vec.size()%2==1)
			{
				g_f1_exnor_vec[(g_f1_exnor_vec.size()-1)/2]=g_f1_exnor_vec[g_f1_exnor_vec.size()-1];
				g_f1_exnor_vec.resize((g_f1_exnor_vec.size()+1)/2);
			}
			else
			{
				g_f1_exnor_vec.resize(g_f1_exnor_vec.size()/2);
			}
		}
		
		//add po, which is exactly t
		//cout << f1_f0_exor_vec.size() << " " << g_f1_exnor_vec.size() << endl;
		//cout << f1_f0_exor_vec[0] << " " << g_f1_exnor_vec[0] << endl;
		Abc_ObjAddFanin(t,Abc_AigAnd(aig_man,f1_f0_exor_vec[0],g_f1_exnor_vec[0]));
		
		vector<Weight_gate*> new_weight_gate;

		//push base_func in t to new gate_list (delete unused?)
		for(i=0;i<gate_list.size();i++)				 
		{
			gate_list[i]->gate=Abc_ObjCopy(gate_list[i]->gate);
			if (!Abc_ObjIsPo(gate_list[i]->gate))
			{
				cout << "[WARNING] something error in construct t_base " << endl;
			}
		}
		
		//gate_list=new_weight_gate;
		//cout << gate_list.size() << endl;
		/*	
		t_ntk = Abc_NtkStrash( t_ntk, 0, 1, 0 );
		for (i=0;i<gate_list.size();i++)
		{
			gate_list[i]->gate=Abc_NtkPo(t_ntk,i+1);
		}
		*/
		target_function=t_ntk;
		//Abc_NtkIsAigNetlist

		cout << "[INFO] t construct success " << endl;
		//cout << Abc_NtkPoNum(t_ntk) << endl;
		//cout << t_ntk->ntkType << " " << t_ntk->ntkFunc << endl;
		//Io_WriteBblif( t_ntk, "t.v" );
		/*
		Abc_NtkForEachObj(t_ntk,iter_obj,i)
		{
			cout << Abc_ObjName(iter_obj) << endl;
		}
		*/	
		Abc_FrameSetCurrentNetwork(pAbc,t_ntk);
		char Command[1000];
    		sprintf( Command, "write t.blif");
    		if ( Cmd_CommandExecute( pAbc, Command ) )
    		{
        		fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
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
	assert (!Abc_ObjIsPo(current_node));
	int i;
	if (Abc_ObjCopy(current_node)!=0)
		return;
	if (Abc_ObjIsPi(current_node))
		cout << "[WARNING] somthing wrong " << endl;

        Abc_Obj_t* iter_node;	 
	//cout << "fanin" << endl;
	Abc_ObjForEachFanin (current_node,iter_node,i)
	{
		//cout << iter_node << endl;
		//cout << Abc_ObjName(iter_node) << endl; 
		traverse_from_current_to_pi(new_ntk,iter_node);
	}
	//cout << Abc_ObjChild0Copy(current_node) <<" " << Abc_ObjChild1Copy(current_node) << endl;
	if (Abc_ObjIsPo(current_node))
		return;


	Abc_ObjSetCopy(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Copy(current_node),Abc_ObjChild1Copy(current_node)));
	if (Abc_ObjData(Abc_ObjFanin0(current_node))!=0&&Abc_ObjData(Abc_ObjFanin1(current_node))!=0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Data(current_node),Abc_ObjChild1Data(current_node)));
	}
	else if (Abc_ObjData(Abc_ObjFanin0(current_node))!=0&&Abc_ObjData(Abc_ObjFanin1(current_node))==0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Data(current_node),Abc_ObjChild1Copy(current_node)));
	}
	else if (Abc_ObjData(Abc_ObjFanin0(current_node))==0&&Abc_ObjData(Abc_ObjFanin1(current_node))!=0)
	{
		Abc_ObjSetData(current_node,Abc_AigAnd((Abc_Aig_t*)(new_ntk->pManFunc),Abc_ObjChild0Copy(current_node),Abc_ObjChild1Data(current_node)));
	}
		
	//count++;
	//cout << count << endl;
	

	
}
