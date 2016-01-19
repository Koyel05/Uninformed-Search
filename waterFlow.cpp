#include <iostream>
#include<list>
#include<fstream>
#include <sstream>
#include <string>
#include<vector>
#include<queue>
#include<algorithm>
#include<stack>
using namespace std;
struct pipes
{
    string pStart;
    string pEnd;
    int pLength;
    vector<int> offperiods;
};
struct sortPipes_length
{
    inline bool operator() (const pipes& struct1, const pipes& struct2)
    {
        return (struct1.pLength < struct2.pLength);
    }
};
struct sortPipes_name
{
    inline bool operator() (const pipes& struct1, const pipes& struct2)
    {
        return (struct1.pEnd < struct2.pEnd);
    }
};
vector<string> split(string str, char delimiter)
 {
  vector<string> data;
  istringstream iss(str);
  string s;
  while(getline(iss, s, delimiter)) {
    data.push_back(s);
  }
  return data;
}
class Graph
{
    vector<string> nodes;
    list<pipes> *adj;
    public:
        ~Graph(){delete adj;}
    void addNodes(string node)
    {
        nodes.push_back(node);
    }

    void addEdge(pipes p)
    {
        int index=getNodeIndex(p.pStart);
        adj[index].push_back(p);
    }
    void createList()
    {
        adj = new list<pipes>[nodes.size()];
    }
    int getNodeIndex(string n)
	{
		for(int i=0;i<nodes.size();i++)
		{
			if(n==nodes[i])
			return i;
		}
		return -1;
	}
  void bfs(string source,vector<string> destinations,int start_time,int &l,ofstream &outputFile)
    {

    int sourceNo=getNodeIndex(source);
    bool *visited = new bool[nodes.size()];
    for(int i = 0; i < nodes.size(); i++)
        visited[i] = false;

    // Create a queue for BFS
    queue<string> q;
    visited[sourceNo] = true;
    l=0;
    if(find(destinations.begin(),destinations.end(),source)!=destinations.end())
        { int time=(l+start_time)>=24 ? (l+start_time)%24 : (l+start_time);
        outputFile<<source<<" "<<time;
        return;
        }
    q.push(source);
    q.push("marker");
    list<pipes>::iterator i;
    int checkGoal=0;
    while(q.size()>1)
    {
       string n = q.front();
       int index=getNodeIndex(n);
        q.pop();

        if(n=="marker")
        {
            l++;
            q.push("marker");
            continue;
        }
        vector<string> children;
        for(i = adj[index].begin(); i != adj[index].end(); ++i)
        {
            children.push_back(i->pEnd);
        }
        sort(children.begin(),children.end());

        for(vector<string>::iterator it=children.begin();it!=children.end();++it)
        {

            if(!visited[getNodeIndex(*it)])
            {
                visited[getNodeIndex(*it)] = true;
                //string current=string(1,char(*it));
                if(find(destinations.begin(),destinations.end(),*it)!=destinations.end())
                    { int time=(l+start_time+1)>=24 ? (l+start_time+1)%24 : (l+start_time+1);
                        outputFile<<*it<<" "<<time;
                    checkGoal=1;
                    return;
                    }

                    q.push(*it);
            }
        }
    }
    if (checkGoal==0)
    outputFile<<"None";
  }

  void dfs(string source,vector<string> destinations,int start_time,int &l,ofstream &outputFile)
  {
      l=0;
    int sourceNo=getNodeIndex(source);
    bool *visited = new bool[nodes.size()];
    for(int i = 0; i < nodes.size(); i++)
        visited[i] = false;

    stack<string> st;
    st.push(source);

    list<pipes>::iterator i;
    while(!st.empty())
    {
       string n = st.top();
       int index=getNodeIndex(n);
        st.pop();
        if(find(destinations.begin(),destinations.end(),n)!=destinations.end())
            { int time=(l+start_time)>=24 ? (l+start_time)%24 : (l+start_time);
                outputFile<<n<<" "<<time;
                return;}
        visited[index] = true;

        if(n=="marker")
        {
        l--;
        continue;
        }
        vector<string> children;
        for(i = adj[index].begin(); i != adj[index].end(); ++i)
        {
            children.push_back(i->pEnd);
        }
        if(children.size()>0)
        {
            st.push("marker");
            l++;
            sort(children.begin(),children.end(),greater<string>());
        }

        for(vector<string>::iterator it=children.begin();it!=children.end();++it)
        {
            if(!visited[getNodeIndex(*it)])
                   {
                      st.push(*it);
                   }
        }
    }
    outputFile<<"None";
  }

  void UCS(string source,vector<string> destinations,int start_time,int &l,ofstream &outputFile)
  {
    bool *visited = new bool[nodes.size()];
    for(int i = 0; i < nodes.size(); i++)
        visited[i] = false;

    vector<pipes> open;
    pipes sourceNode;
    sourceNode.pStart="";
    sourceNode.pEnd=source;
    sourceNode.pLength=0+start_time;
    open.push_back(sourceNode);
    list<pipes>::iterator i;
    while(!open.empty())
    {
       pipes n = open.front();
       //cout<<n.pEnd<<n.pLength<<endl;
       int index=getNodeIndex(n.pEnd);
       open.erase(open.begin()+0);
       if(find(destinations.begin(),destinations.end(),n.pEnd)!=destinations.end())
          //return n;
          { n.pLength=n.pLength>=24 ? n.pLength%24 : n.pLength;
              outputFile<<n.pEnd<<" "<<n.pLength;return;}

        vector<pipes> children;
        for(i = adj[index].begin(); i != adj[index].end(); ++i)
        {
            children.push_back(*i);
        }
        while(!children.empty())
        {
            pipes ch=children.front();
            //cout<<"child:"<<ch.pLength<<endl;
            children.erase(children.begin());
            //1st case
            vector<pipes>::iterator it;
            int flag=0;pipes curr;
            for(it=open.begin();it!=open.end();++it)
            {
                curr=*it;
                if(curr.pEnd==ch.pEnd)
                {
                    flag=1;
                    break;
                }
            }

            if(flag==0 && !visited[getNodeIndex(ch.pEnd)])
                {   if(find(ch.offperiods.begin(),ch.offperiods.end(),(n.pLength%24))==ch.offperiods.end())
                    {ch.pLength+=n.pLength;
                    open.push_back(ch);
                    }
                }
            else if(flag==1)
                if(ch.pLength+n.pLength < (*it).pLength)
                {
                if(find(ch.offperiods.begin(),ch.offperiods.end(),(n.pLength%24))==ch.offperiods.end())
                  {open.erase(it);
                  ch.pLength+=n.pLength;
                  open.push_back(ch);
                  }

                }
        }
        visited[index] = true;
        sort(open.begin(),open.end(),sortPipes_name());
        sort(open.begin(),open.end(),sortPipes_length());

    }
    outputFile<<"None";
  }
};


int main(int argc,char* argv[])
{

    int cases;
    string algo;
    string source;
    int no_pipes;
    int start_time;
    ofstream outputFile("output.txt");
    ifstream infile;
    infile.open(argv[2]);
    if(infile.is_open())
    {
        string line;
    getline( infile, line );
    istringstream iss(line);
    iss>>cases;
     while(cases>0)
      {

        Graph *obj = new Graph();
        int level=0;
        vector<string> destinations;
        vector<string> middle;
        getline( infile, algo );
        getline( infile, source );
        obj->addNodes(source);

        getline( infile, line );
        istringstream iss1(line);
        string s;
        while (getline(iss1, s, ' '))
            {
                destinations.push_back(s);
                obj->addNodes(s);
            }


        getline( infile, line );
        istringstream iss2(line);
        while (getline(iss2, s, ' '))
          {
            middle.push_back(s);
            obj->addNodes(s);
          }

        getline( infile, line );
        istringstream iss3(line);
        iss3>>no_pipes;
        obj->createList();
        while(no_pipes>0)
        {
          pipes p;
          int no_of_off;
          getline( infile, line );
          vector<string> pipe_info = split(line, ' ');
          p.pStart = pipe_info[0];
          p.pEnd = pipe_info[1];
          p.pLength = atoi(pipe_info[2].c_str());
          no_of_off = atoi(pipe_info[3].c_str());
          for(int j=4;j<no_of_off+4;j++)
          {
              vector<string> offrange=split(pipe_info[j],'-');
              int lb,ub;
              istringstream(offrange[0])>>lb;
              istringstream(offrange[1])>>ub;
              for(int k=lb;k<=ub;k++)
                p.offperiods.push_back(k);
          }
          //getline(iss, p.pStart, ' ');
          //getline(iss, p.pEnd, ' ');

          //getline(iss, s, ' ');
          //istringstream ( s ) >> p.pLength;
          //getline(iss, no_of_off, ' ');
          //while(no_)
            obj->addEdge(p);
          no_pipes--;
        }
        getline(infile,line);
        istringstream iss4(line);
        iss4>>start_time;

        string result;
        if(algo=="BFS")
            obj->bfs(source,destinations,start_time,level,outputFile);
        else if(algo=="DFS")
           obj->dfs(source,destinations,start_time,level,outputFile);
        else
         if(algo=="UCS")
            obj->UCS(source,destinations,start_time,level,outputFile);

        if(cases>1)
            outputFile<<endl;
        getline(infile,line);

        cases--;
      }
        infile.close();
        outputFile.close();
    }
    else
        cout<<"File could not be opened";

    return 0;
}
