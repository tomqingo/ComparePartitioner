# include <iostream>
# include <fstream>
# include <sstream>
# include "string.h"
# include "global.h"
# include <libkahypar.h>
# include <math.h>

using namespace std;

bool CheckBalance(vector<int> partitions, int k, double imbalance_ratio)
{
    unordered_map<int, int> mp;

    for(int i=0; i<partitions.size(); i++)
    {
        if(mp.find(partitions[i])==mp.end())
            mp[partitions[i]]=1;
        else
            mp[partitions[i]]++;
    }

    int total_node = partitions.size();

    bool flag = true;
    double real_imbalance_ratio = 0.0;
    for(auto & pair: mp)
    {
        //cout<<pair.first<<' '<<pair.second<<endl;
        //cout<<(total_node*1.0)*(1+imbalance_ratio)/k<<endl;    
        if(pair.second>(total_node*1.0)*(1+imbalance_ratio)/k)
            flag = false;
        
        if(((pair.second*k*1.0)/total_node-1)>real_imbalance_ratio)
            real_imbalance_ratio = ((pair.second*k*1.0)/total_node)-1;

    }

    cout<<"read_imbalance:"<<real_imbalance_ratio<<endl;
    return flag;
}

bool CheckCut(vector<int> partitions, vector<vector<int>> nets, int cut)
{
    int external_net = 0;
    for(int i=0; i<nets.size(); i++)
    {
        for(int j=0; j<nets[i].size(); j++)
        {
            if(partitions[nets[i][j]]!=partitions[nets[i][0]])
            {
                external_net++;
                break;
            }
        }
    }

    cout<<external_net<<' '<<cut<<endl;
    if(external_net == cut)
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    string hgrname = argv[2];
    string outname = argv[4];


    ifstream in_file;
    ofstream out_file;
    in_file.open(hgrname);
    out_file.open(outname);


    string line;
    string num_nets;
    string num_nodes;
    int nNode;
    int nNet;
    vector<vector<int>> nets;

    int cnt = 0;


    while(getline(in_file, line))
    {
        istringstream iss(line);

        if(cnt == 0)
        {
            getline(iss, num_nets, ' ');
            getline(iss, num_nodes, ' ');
            out_file<<num_nets<<' '<<num_nodes<<endl;
            nNode = stoi(num_nodes);
            nNet = stoi(num_nets);
        }
        else
        {
            string s;
            vector<int> pins;
            while(getline(iss, s, ' '))
            {
                int node_id = stoi(s);
                pins.push_back(node_id);
                node_id = node_id + 1;
                out_file<<node_id<<' ';
            }

            nets.push_back(pins);

            out_file<<endl;
        }

        cnt++;

    }

    cout<<nNode<<endl;
    cout<<nets.size()<<endl;

    in_file.close();
    out_file.close();
    
    //PaToH

    PaToH_Parameters args;

    int *cwghts = new int[nNode];
    for(int i=0; i<nNode; i++)
        cwghts[i] = 1;

    int *nwghts = new int[nNet];
    for(int i=0; i<nNet; i++)
        nwghts[i] = 1;

    int nconst = 1;
    int useFixCells = 0;
    PaToH_Initialize_Parameters(&args, PATOH_CUTPART, PATOH_SUGPARAM_QUALITY);
    args.seed = -1;
    args._k = 4;
    //args.balance = 1;
    //args.init_imbal = 0.05;
    args.final_imbal = 0.05;

    int nPin = 0;
    for(int i=0; i<nets.size(); i++)
        nPin += nets[i].size();

    int *xpins = new int[nNet+1];
    int *pins = new int[nPin];

    for(int i=0, p=0; i < nNet; i++)
    {
        xpins[i] = p;
        for(int j=0; j<nets[i].size(); j++)
        {
            int nodeid = nets[i][j];
            pins[p++] = nodeid;
        }

    }

    xpins[nNet] = nPin;


	PaToH_Check_User_Parameters(&args, true);
    int *partvec = new int[nNode];
	int *partweights = new int[args._k*nconst];
    int cut;

    PaToH_Alloc(&args, nNode, nNet, nconst, cwghts, nwghts, xpins, pins);
    for(int i=0; i<nNode; i++)
        partvec[i] = -1;
    
	PaToH_Part(&args, nNode, nNet, nconst, useFixCells, cwghts, nwghts, xpins, pins, NULL, partvec, partweights, &cut);

    cout<<"PaToH cut size:"<<cut<<endl;

    vector<int> partitions_patoh;
    for(int i=0; i<nNode; i++)
        partitions_patoh.push_back(partvec[i]);
    if(!CheckBalance(partitions_patoh, args._k, args.final_imbal))
        cout<<"Balance not pass!"<<endl;
    
    if(!CheckCut(partitions_patoh, nets, cut))
        cout<<"CUT calculation error!"<<endl;

    //KaHyPar
    kahypar_context_t* context = kahypar_context_new();
    kahypar_configure_context_from_file(context, "/data/ssd/qluo/compare_partitioner/thirdparty/kahypar/config/cut_kKaHyPar_sea20.ini");

    //kahypar_set_seed(context, 1);

    const kahypar_hypernode_id_t num_vertices = nNode;
    const kahypar_hyperedge_id_t num_hyperedges = nNet;

    unique_ptr<kahypar_hyperedge_weight_t[]> hyperedge_weights = make_unique<kahypar_hyperedge_weight_t[]>(nNet);

    for(int i=0; i<nNet; i++)
        hyperedge_weights[i] = 1;

    unique_ptr<size_t[]> hyperedge_indices = make_unique<size_t[]>(nNet+1);
    unique_ptr<kahypar_hyperedge_id_t[]> hyperedges = make_unique<kahypar_hyperedge_id_t[]>(nPin);

    for(int i=0; i<nNet+1; i++)
        hyperedge_indices[i] = xpins[i];
    
    for(int i=0; i<nPin; i++)
        hyperedges[i] = pins[i];

    const double imbalance = 0.05;
    const kahypar_partition_id_t k = 4;

    kahypar_hyperedge_weight_t objective = 0;

    vector<kahypar_partition_id_t> partition(num_vertices, -1);

    kahypar_partition(num_vertices, num_hyperedges,
       	                imbalance, k,
               	        /*vertex_weights */ nullptr, hyperedge_weights.get(),
               	        hyperedge_indices.get(), hyperedges.get(),
       	                &objective, context, partition.data());
    
    cout<<"KaHyPar partition"<<endl;

    /*

    for(int i = 0; i != num_vertices; ++i) {
        cout << i << ":" << partition[i] << endl;
    }
    */

    cout<<"cut:"<<objective<<endl;

    vector<int> partitions_kahypar;
    for(int i=0; i<nNode; i++)
        partitions_kahypar.push_back(int(partition[i]));
    if(!CheckBalance(partitions_kahypar, args._k, imbalance))
        cout<<"Balance not pass!"<<endl;
    
    if(!CheckCut(partitions_kahypar, nets, int(objective)))
        cout<<"CUT calculation error!"<<endl;

    kahypar_context_free(context);

    delete[] xpins;
	delete[] pins;
    delete[] cwghts;
	delete[] nwghts;
	delete[] partweights;
	delete[] partvec;

    PaToH_Free();


    return 0;
    
}