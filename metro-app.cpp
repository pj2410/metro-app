#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <cstring>
#include<bits/stdc++.h>
using namespace std;

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> neighbours;
    };

    static unordered_map<string, Vertex> vtces;

    Graph_M()
    {
        vtces.clear();
    }

    int numVetex()
    {
        return vtces.size();
    }

    bool containsVertex(string vname)
    {
        return vtces.count(vname) > 0;
    }

    void addVertex(string vname)
    {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(string vname)
    {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (auto it = vtx.neighbours.begin(); it != vtx.neighbours.end(); it++)
        {
            keys.push_back(it->first);
        }

        for (string key : keys)
        {
            Vertex nbrVtx = vtces[key];
            nbrVtx.neighbours.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges()
    {
        int count = 0;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            Vertex vtx = it->second;
            count += vtx.neighbours.size();
        }

        return count / 2;
    }

    bool containsEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) == 0)
        {
            return false;
        }

        return true;
    }

    void addEdge(string vname1, string vname2, int value)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) > 0)
        {
            return;
        }

        vtces[vname1].neighbours[vname2] = value;
        vtces[vname2].neighbours[vname1] = value;
    }

    void removeEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].neighbours.count(vname2) == 0)
        {
            return;
        }

        vtces[vname1].neighbours.erase(vname2);
        vtces[vname2].neighbours.erase(vname1);
    }

    void display_Map()
    {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << endl;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            string str = key + " =>" + "\n";
            Vertex vtx = it->second;
            for (auto it2 = vtx.neighbours.begin(); it2 != vtx.neighbours.end(); it2++)
            {
                string nbr = it2->first;
                str += "\t" + nbr + "\t";
                if (nbr.length() < 16)
                    str += "\t";
                if (nbr.length() < 8)
                    str += "\t";
                str += to_string(it2->second) + "\n";
            }
            cout << str << endl;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << endl;
    }

    void display_Stations()
    {
        cout << endl;
        cout << "*" << endl;
        cout << endl;
        int i = 1;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            cout << i << ". " << key << endl;
            i++;
        }
        cout << endl;
        cout << "*" << endl;
        cout << endl;
    }

    bool hasPath(string vname1, string vname2, unordered_map<string, bool> &processed)
    {
        if (containsEdge(vname1, vname2))
        {
            return true;
        }

        processed[vname1] = true;

        Vertex vtx = vtces[vname1];
        for (auto it = vtx.neighbours.begin(); it != vtx.neighbours.end(); it++)
        {
            string nbr = it->first;
            if (!processed.count(nbr))
            {
                if (hasPath(nbr, vname2, processed))
                {
                    return true;
                }
            }
        }

        return false;
    }

    class DijkstraPair
    {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair &other) const
        {
            return cost > other.cost;
        }
    };

    int dijkstra(string src, string des, bool nan)
    {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> pq;

        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            DijkstraPair np;
            np.vname = key;
            np.cost = INT_MAX;

            if (key == src)
            {
                np.cost = 0;
                np.psf = key;
            }

            pq.push(np);
            map[key] = np;
        }

        while (!pq.empty())
        {
            DijkstraPair rp = pq.top();
            pq.pop();

            if (rp.vname == des)
            {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            Vertex v = vtces[rp.vname];
            for (auto it = v.neighbours.begin(); it != v.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (map.count(nbr))
                {
                    int oc = map[nbr].cost;
                    Vertex k = vtces[rp.vname];
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * k.neighbours[nbr];
                    else
                        nc = rp.cost + k.neighbours[nbr];

                    if (nc < oc)
                    {
                        DijkstraPair gp = map[nbr];
                        gp.psf = rp.psf + nbr;
                        gp.cost = nc;

                        pq.push(gp);
                    }
                }
            }
        }
        return val;
    }

    class Pair
    {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string Get_Minimum_Distance(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_dis;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.neighbours[nbr];
                    stack.push_front(np);
                }
            }
        }
        ans = ans + to_string(min);
        return ans;
    }

    string Get_Minimum_Time(string src, string dst)
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_front(sp);

        while (!stack.empty())
        {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname))
            {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst)
            {
                int temp = rp.min_time;
                if (temp < min)
                {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto it = rpvtx.neighbours.begin(); it != rpvtx.neighbours.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.neighbours[nbr];
                    stack.push_front(np);
                }
            }
        }
        double minutes = ceil((double)min / 60);
        ans = ans + to_string(minutes);
        return ans;
    }

    vector<string> get_Interchanges(string str)
    {
        vector<string> arr;
        string res[100];
        int count = 0;
        char *temp = strtok((char *)str.c_str(), "  ");
        while (temp != NULL)
        {
            res[count++] = temp;
            temp = strtok(NULL, "  ");
        }
        arr.push_back(res[0]);

        int c=0;
        for (int i = 1; i < count - 1; i++)
        {
            int index = res[i].find('~');
            string s = res[i].substr(index + 1);

            if (s.length() == 2)
            {
                string prev = res[i - 1].substr(res[i - 1].find('~') + 1);
                string next = res[i + 1].substr(res[i + 1].find('~') + 1);

                if (prev == next)
                {
                    arr.push_back(res[i]);
                }
                else
                {
                    arr.push_back(res[i] + " ==> " + res[i + 1]);
                    i++;
                    c++;
                }
            }
            else
            {
                arr.push_back(res[i]);
            }
        }
        arr.push_back(res[count - 1]);
        arr.push_back(to_string(c) );
        return arr;
    }

    vector<int> Get_Minimum_Fare(string src,string dst,int x)
    {
        int stations=0;
        int fare=0;
        //distance-wise


        if(x==7)
        {
            stations=get_Interchanges(Get_Minimum_Distance(src, dst)).size()-1;
        }

        else
        {
            stations=get_Interchanges(Get_Minimum_Time(src, dst)).size()-1;
        }

        if(stations>0 && stations<=3)
        fare=10;

        else if(stations>3 && stations<=7)
        fare=20;

        else if(stations>7 && stations<=11)
        fare=30;

        else if(stations>11 && stations<=20)
        fare=40;

        else
        fare=40+ (stations-20)*10;

        return {stations,fare};
    }


    static void Create_Metro_Map(Graph_M &g)
    {
      g.addVertex("Noida_Sector_62~B");
        g.addVertex("Botanical_Garden~B");
        g.addVertex("Yamuna_Bank~B");
        g.addVertex("Rajiv_Chowk~BY");
        g.addVertex("Vaishali~B");
        g.addVertex("Moti_Nagar~B");
        g.addVertex("Janak_Puri_West~BO");
        g.addVertex("Dwarka_Sector_21~B");
        g.addVertex("Huda_City_Center~Y");
        g.addVertex("Saket~Y");
        g.addVertex("Vishwavidyalaya~Y");
        g.addVertex("Chandni_Chowk~Y");
        g.addVertex("New_Delhi~YO");
        g.addVertex("AIIMS~Y");
        g.addVertex("Shivaji_Stadium~O");
        g.addVertex("DDS_Campus~O");
        g.addVertex("IGI_Airport~O");
        g.addVertex("Rajouri_Garden~BP");
        g.addVertex("Netaji_Subhash_Place~PR");
        g.addVertex("Punjabi_Bagh_West~P");

        g.addEdge("Noida_Sector_62~B", "Botanical_Garden~B", 8);
        g.addEdge("Botanical_Garden~B", "Yamuna_Bank~B", 10);
        g.addEdge("Yamuna_Bank~B", "Vaishali~B", 8);
        g.addEdge("Yamuna_Bank~B", "Rajiv_Chowk~BY", 6);
        g.addEdge("Rajiv_Chowk~BY", "Moti_Nagar~B", 9);
        g.addEdge("Moti_Nagar~B", "Janak_Puri_West~BO", 7);
        g.addEdge("Janak_Puri_West~BO", "Dwarka_Sector_21~B", 6);
        g.addEdge("Huda_City_Center~Y", "Saket~Y", 15);
        g.addEdge("Saket~Y", "AIIMS~Y", 6);
        g.addEdge("AIIMS~Y", "Rajiv_Chowk~BY", 7);
        g.addEdge("Rajiv_Chowk~BY", "New_Delhi~YO", 1);
        g.addEdge("New_Delhi~YO", "Chandni_Chowk~Y", 2);
        g.addEdge("Chandni_Chowk~Y", "Vishwavidyalaya~Y", 5);
        g.addEdge("New_Delhi~YO", "Shivaji_Stadium~O", 2);
        g.addEdge("Shivaji_Stadium~O", "DDS_Campus~O", 7);
        g.addEdge("DDS_Campus~O", "IGI_Airport~O", 8);
        g.addEdge("Moti_Nagar~B", "Rajouri_Garden~BP", 2);
        g.addEdge("Punjabi_Bagh_West~P", "Rajouri_Garden~BP", 2);
        g.addEdge("Punjabi_Bagh_West~P", "Netaji_Subhash_Place~PR", 3);

    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

string *printCodelist()
{
    cout << "\nList of station along with their codes:" << endl;
    unordered_map<string, Graph_M::Vertex>::iterator it;
    int i = 1, j = 0, m = 1;
    string temp = "";
    string *codes = new string[Graph_M::vtces.size()];
    char c;
    for (it = Graph_M::vtces.begin(); it != Graph_M::vtces.end(); it++)
    {
        string key = it->first;
        codes[i - 1] = "";
        j = 0;
        while (j < key.length())
        {
            temp = key[j];
            c = temp[0];
            while (c > 47 && c < 58)
            {
                codes[i - 1] += c;
                j++;
                c = key[j];
            }
            if ((c < 48 || c > 57) && c < 123)
                codes[i - 1] += c;
            j++;
        }
        if (codes[i - 1].length() < 2)
            codes[i - 1] += toupper(key[1]);

        cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            cout << "\t";
        if (key.length() < (14 - m))
            cout << "\t";
        if (key.length() < (6 - m))
            cout << "\t";
        cout << codes[i - 1] << endl;
        i++;
        if (i == pow(10, m))
            m++;
    }
    return codes;
}

int main()
{
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\n\n\n\t\t\t\t\t\t\t\t\tWELCOME TO THE METRO APP" << endl;

    while (true)
    {
        cout<<"\n\n\t*********************************************************************************************************************************\n";
        cout<<"\t*                                                                                                                               *\n";
        cout<<"\t*                                                                                                                               *\n";
        cout<<"\t*                                                   ~LIST OF ACTIONS                                                            *\n";
        cout<<"\t*         1. LIST ALL THE STATIONS IN THE MAP                                                                                   *\n";
        cout<<"\t*         2. SHOW THE METRO MAP                                                                                                 *\n";
        cout<<"\t*         3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                             *\n";
        cout<<"\t*         4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                                        *\n";
        cout<<"\t*         5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                        *\n";
        cout<<"\t*         6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION                            *\n";
        cout<<"\t*         7. GET FARE FOR SHORTEST PATH(DISTANCE WISE)                                                                          *\n";
        cout<<"\t*         8. GET FARE FOR SHORTEST PATH(TIME WISE)                                                                              *\n";
        cout<<"\t*         9. EXIT THE MENU                                                                                                      *\n";
        cout<<"\t*                                                                                                                               *\n";
        cout<<"\t*                                                                                                                               *\n";
        cout<<"\t*                                                                                                                               *\n";
        cout<<"\t*********************************************************************************************************************************\n";

        cout << "\n\n";
        int choice = -1;
        cout<<"ENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 9) :";
        cin >> choice;

        if (choice == 9)
        {break;}


        switch (choice)
        {
            case 1:
                g.display_Stations();
                break;

            case 2:
                g.display_Map();
                break;

            case 3:
            {
                string *keys = printCodelist();
                string st1, st2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, st1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, st2);

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                    cout << "\nTHE INPUTS ARE INVALID" << endl;
                else
                    cout << "\nSHORTEST DISTANCE: "<< g.dijkstra(st1, st2, false) << "KM" << endl;
                break;
            }

            case 4:
            {
                string *keys = printCodelist();
                string sat1, sat2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                cout << "\nSHORTEST TIME: " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES" << endl;
                break;
            }

            case 5:
            {
                string *keys = printCodelist();
                string s1, s2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, s1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout<<endl<<endl;
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len-2]<< endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len -1] << endl;
                    cout << endl<< endl<<"THE PATH IS AS FOLLOWS:\n\n";
                    for (int i = 0; i <len-2; i++)
                    {cout << str[i] << endl;}
                }
                break;
            }

            case 6:
            {
                string *keys = printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else
                {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout<<endl<<endl;
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len-2] << " MINUTES" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len-1] << endl;
                    cout << endl<< endl<<"THE PATH IS AS FOLLOWS:\n";
                    for (int i = 0; i < len-2; i++)
                    {cout << str[i] << endl;}
                    cout << endl;
                }
                break;
            }

            case 7:
            {
                string *keys = printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;

                else
                {
                    cout<<"\nNUMBER OF STATIONS IN SHORTEST DISTANCE :"<<g.Get_Minimum_Fare(ss1,ss2,7)[0];
                    cout<<"\nCALCULATED FARE :Rs"<<g.Get_Minimum_Fare(ss1,ss2,7)[1];
                }

                break;

            }

            case 8:
            {
                string *keys = printCodelist();
                string ss1, ss2;
                cout << "\nENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                cout << "THE INPUTS ARE INVALID" << endl;


                else
                {
                    cout<<"\nNUMBER OF STATIONS IN SHORTEST TIME :"<<g.Get_Minimum_Fare(ss1,ss2,8)[0];
                    cout<<"\nCALCULATED FARE : Rs"<<g.Get_Minimum_Fare(ss1,ss2,7)[1];
                }


                break;

            }

            default:
            {
                cout << "Invalid choice" << endl;
                break;
            }
        }
    }

    return 0;
}
