#include<bits/stdc++.h>
using namespace std ;

//Input variables

//No. of input, output ports
int N;
int O ;      
//Buffer size 
int B ;         
//packet generation probability
float p ;         
//scheduling algo
string nameq ;  
//Knockout   
int K ;         
//output_file
string outfile ;
//maxtimeslot   
int T ;         
int KOUQdropprob;
//Lists are used because we remove the elements remove from the both front and back
//Input Buffer at each switch port
vector<list<int> > inpBuffer ;
//Output Buffer at each switch port
vector<list<int> > outBuffer ;
//structure to store information about the packet
struct Packet
{
    int InputPort ;
    int OutputPort ;
    int arrivalTime ;
    int deptTime ;
};

vector<Packet> packetInfo ;

//randomly generate arrival time, output port
vector<pair<int,int> > input ;
map<int,vector<int>> Request ;
default_random_engine generator ;

void TrafficGeneration(int timeSlot)
{
    input.clear();
    input.resize(N) ;

    //initialize the 
    for(int i=0 ; i<N ; i++)
    {
        input[i].first=-1 ;
        input[i].second=-1 ;
    }
    double temp = (double)(N)/p ;
    uniform_real_distribution<double> arrtime(timeSlot+0.001,timeSlot+0.01) ;

    for(int i=0 ; i<N ;i++)
    {
        int val = rand()%(int)(ceil(temp)) ;
        if(val<N)
        {
            input[i].first = arrtime(generator) ;
            input[i].second = rand()%N ;
        }
    }
}
void KOUQ()
{
    for(int i=0 ; i<N ; i++)
    {
        if(input[i].first!=-1)
        {
            //enter packets into the input buffer at each input port

                struct Packet pi ;
                pi.InputPort = i ;  // Input_Port
                pi.OutputPort = input[i].second   ;//Output_port
                pi.arrivalTime = input[i].first ;   //arrival_time
                pi.deptTime = -1 ;
                packetInfo.push_back(pi);
                inpBuffer[i].push_back(packetInfo.size()-1) ;
            
        }
    }

    //It is used to store the total input ports corresponds to the particular output port
    //<outport, array of input_port>
    unordered_map<int,vector<int>> count ;

    
    //request phase
    for(int i=0 ; i<N ; i++)
    {
        if(!inpBuffer[i].empty())
        {
            int packidx = inpBuffer[i].front() ;
            int countidx =  packetInfo[packidx].OutputPort ;
            count[countidx].push_back(i) ;
        }
    }

    //grant accecc
    int j=0 ;
    for(int i=0 ; i<N ; i++)
    {
        int cn=count[i].size();
        // not dropingcase
        if(cn<=K)    
        {
            for(int j=0;j<cn;j++)
            {
                if(outBuffer.size()==B)
                     break;
                outBuffer[i].push_back(inpBuffer[count[i][j]].front()) ;
                inpBuffer[count[i][j]].pop_front();
                
            }             
        
        }
        else
        {
            // droping case 
            KOUQdropprob++;
            //where droping any packet also same since arrival time is same for all the packets
            for(int j=0;j<K ;j++)
            {
              if(outBuffer.size()==B)
                    break;
                outBuffer[i].push_back(inpBuffer[count[i][j]].front()) ;
                inpBuffer[count[i][j]].pop_front();
            }   
        }
    }

    input.clear();
    
}
void INQ()
{
    for(int i=0 ; i<N ; i++)
    {
        if(input[i].first!=-1)
        {
            //enter packets into the input buffer at each input port
            if(inpBuffer[i].size()<B)  
            {
                struct Packet pi ;
                pi.InputPort = i ;  // Input_Port
                pi.OutputPort = input[i].second   ;//Output_port
                pi.arrivalTime = input[i].first ;   //arrival_time
                pi.deptTime = -1 ;
                packetInfo.push_back(pi);
                inpBuffer[i].push_back(packetInfo.size()-1) ;
            }
        }
    }

    //It is used to store the total input ports corresponds to the particular output port
    //<outport, array of input_port>
    unordered_map<int,vector<int>> count ;

    
    //request phase
    for(int i=0 ; i<N ; i++)
    {
        if(!inpBuffer[i].empty())
        {
            int packidx = inpBuffer[i].front() ;
            int countidx =  packetInfo[packidx].OutputPort ;
            count[countidx].push_back(i) ;
        }
    }

    //grant accecc
    int j=0 ;
    for(int i=0 ; i<N ; i++)
    {
        int cn=count[i].size();
        if(cn==0)
            continue;
        else    
        {
            j=rand()%cn;
            outBuffer[i].push_back(inpBuffer[count[i][j]].front()) ;
            inpBuffer[count[i][j]].pop_front();
        }
    }

    input.clear();
    
}
void iSLIP()
{
    //Count for total output port for corresponding input i.e index
    int countoutPort[N] ;
    for(int i= 0  ;i<N  ; i++) countoutPort[i]=-1 ;
    
    bool checkPorts[N] ;
    for(int i= 0  ;i<N  ; i++) checkPorts[i]=0 ;

    vector<int>::iterator index ;

    for(int i=0 ; i<N ; i++)
    {
        if(input[i].first!=-1)
        {
            if(Request[i].size()<B)
            {
                struct Packet pi ;
                pi.InputPort = i ;  // Input_Port
                pi.OutputPort = input[i].second   ;//Output_port
                pi.arrivalTime = input[i].first ;   //arrival_time
                pi.deptTime = -1 ;
                packetInfo.push_back(pi);
                Request[i].push_back(packetInfo.size()-1) ;
            }
        }
        
        if(!Request[i].empty())
        {
            index = Request[i].begin() ;
            for(int j=0 ; j<Request[i].size() ; j++)
            {
                int inpthport = Request[i][j] ;
                int outport = packetInfo[inpthport].OutputPort ;
                if(checkPorts[outport]==0)
                {
                    Request[i].erase(index+j) ;
                    countoutPort[i] = inpthport ;
                    checkPorts[outport] = 1;
                    break ;
                }
            }
        }
        if(countoutPort[i] !=-1)
        {
            if(outBuffer[i].size()<B)
            {
                int outprt = packetInfo[countoutPort[i]].OutputPort ;
                outBuffer[outprt].push_back(countoutPort[i]) ;
            }
        }
    }
}



void PacketTransmission(int timeSlot) 
{
    for(int i=0 ; i<N ; i++)
    {
        if(!outBuffer[i].empty())
        {
            int packidx = outBuffer[i].front() ;
            outBuffer[i].pop_front() ;
            packetInfo[packidx].deptTime = timeSlot ;
        }
    }

}
void PacketSchedule()
{
    if(nameq=="INQ")
    INQ() ;
    else if(nameq=="KOUQ")
    KOUQ() ;
    else
    {
        iSLIP() ;
    }
}
double LinkUtilization()
{
    //hash map to count total no. of links in entire simulation duration
    double success=0;
    for(int i=0 ; i<packetInfo.size() ; i++)
    {
        if(packetInfo[i].deptTime!=-1)
        {
            success++;
        }
        
    }

    return success/(double)N/(double)(T-1) ;
}


double AvgDelay()
{
    int len=0 ;
    double sum=0 ;
    bool f=0 ;
    for(int i=0 ; i<packetInfo.size() ; i++)
    {
        if(packetInfo[i].deptTime!=-1)
        {
            f=1 ;
            len++ ;
            sum+=(packetInfo[i].deptTime - floor(packetInfo[i].arrivalTime)) ;
        }
    }
    return sum/len ;

}

double StdDevPD()
{
    int len=0 ;
    double sum=0 ;
    bool f=0 ;
    vector<double> stddev ;
    for(int i=0 ; i<packetInfo.size() ; i++)
    {
        if(packetInfo[i].deptTime!=-1)
        {
            f=1 ;
            len++ ;
            sum+=(packetInfo[i].deptTime - packetInfo[i].arrivalTime) ;
            stddev.push_back(packetInfo[i].deptTime - packetInfo[i].arrivalTime) ;
        }
    }
    double mean = sum/len ;
    sum = 0 ;
    for(int i=0 ; i<stddev.size() ; i++)
    {
        sum+=pow(stddev[i]-mean,2) ;
    }
    return sqrt(sum/len) ;

}

void FinalOutput()
{
    cout<<"\t-------------------------------------------------------------------------------------------------------------------"<<endl ;
    cout<<"\t N\t\tp\t\tQueue\t\tAvg PD\t\tStd Dev of PD\t\tAvg Link Utilization"<<endl<<endl  ;
    cout<<"\t------------------------------------------------------------------------------------------------------------------- "<<endl ;

    cout<<"\t "<<N<<"\t\t"<<p<<"\t\t"<<nameq<<"\t\t" ;
    cout<<AvgDelay()<<"\t\t" ;
    cout<<StdDevPD()<<"\t\t";
    cout<<LinkUtilization()<<endl<<endl<<endl<<endl ;

    if(nameq=="KOUQ")
    cout<<"KOUQ Drop Probability : "<<KOUQdropprob/T<<endl<<endl ;

    cout<<"Output file is also created with file name : \""<<outfile<<"\""<<endl<<endl ;

    ofstream fout ;
    fout.open(outfile) ;


    fout<<"\t-------------------------------------------------------------------------------------------------------------------"<<endl ;
    fout<<"\t N\t\tp\t\tQueue\t\tAvg PD\t\tStd Dev of PD\t\tAvg Link Utilization"<<endl<<endl  ;
    fout<<"\t------------------------------------------------------------------------------------------------------------------- "<<endl ;

    fout<<"\t "<<N<<"\t\t"<<p<<"\t\t"<<nameq<<"\t\t" ;
    fout<<AvgDelay()<<"\t\t" ;
    fout<<StdDevPD()<<"\t\t";
    fout<<LinkUtilization()<<endl<<endl ;
    if(nameq=="KOUQ")
    fout<<"KOUQ Drop Probability : "<<KOUQdropprob/T<<endl<<endl ;
    fout.close() ;

}

int main(int argc,char *argv[])
{

    if(argc!=8)
    {
        cout<<"Enter all the parameters correctly"<<endl;
         exit(0);
    }

    //input value from command line
    N = stoi(argv[1]) ;
    B = stoi(argv[2]) ;
    p = stof(argv[3]) ;
    nameq = argv[4] ;
    K = stoi(argv[5]) ;
    outfile = argv[6] ;
    T = stoi(argv[7]) ;

    inpBuffer.resize(N) ;
    outBuffer.resize(N) ;

    if(nameq!="INQ" && nameq!="KOUQ" && nameq!="iSLIP" )
    {
        cout<<"Enter the correct queue type;"<<endl ;
        exit(0);
    }

    // a packet arrived at time t can transmit at the best t+2 since it shoul request and accept the connection
    for(int i=0 ; i<T ; i++)
    {

        if(i==0)
        TrafficGeneration(i);
        else if(i==1)
        {

            PacketSchedule(); 
            TrafficGeneration(i) ;
        }
        else
        {
            PacketTransmission(i);
            PacketSchedule(); 
            TrafficGeneration(i);
        }
      
        
    }

    FinalOutput();

    return 0 ;
}
