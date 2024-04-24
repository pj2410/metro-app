#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <stack>
#include <cmath>
#include <cstring>
using namespace std;

class Graph_M
{
public:
    class Vertex
    {
    public:
        unordered_map<string, int> nbrs;
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
        for (auto it = vtx.nbrs.begin(); it != vtx.nbrs.end(); it++)
        {
            keys.push_back(it->first);
        }

        for (string key : keys)
        {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges()
    {
        int count = 0;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            Vertex vtx = it->second;
            count += vtx.nbrs.size();
        }

        return count / 2;
    }

    bool containsEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0)
        {
            return false;
        }

        return true;
    }

    void addEdge(string vname1, string vname2, int value)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) > 0)
        {
            return;
        }

        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    void removeEdge(string vname1, string vname2)
    {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0)
        {
            return;
        }

        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
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
            for (auto it2 = vtx.nbrs.begin(); it2 != vtx.nbrs.end(); it2++)
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
        cout << "***********************************************************************" << endl;
        cout << endl;
        int i = 1;
        for (auto it = vtces.begin(); it != vtces.end(); it++)
        {
            string key = it->first;
            cout << i << ". " << key << endl;
            i++;
        }
        cout << endl;
        cout << "***********************************************************************" << endl;
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
        for (auto it = vtx.nbrs.begin(); it != vtx.nbrs.end(); it++)
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
            for (auto it = v.nbrs.begin(); it != v.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (map.count(nbr))
                {
                    int oc = map[nbr].cost;
                    Vertex k = vtces[rp.vname];
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * k.nbrs[nbr];
                    else
                        nc = rp.cost + k.nbrs[nbr];

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
            for (auto it = rpvtx.nbrs.begin(); it != rpvtx.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
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
            for (auto it = rpvtx.nbrs.begin(); it != rpvtx.nbrs.end(); it++)
            {
                string nbr = it->first;
                if (!processed.count(nbr))
                {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.nbrs[nbr];
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
            res[count] = temp;
            count++;
            temp = strtok(NULL, "  ");
        }
        arr.push_back(res[0]);
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
                    count++;
                }
            }
            else
            {
                arr.push_back(res[i]);
            }
        }
        arr.push_back(to_string(count - 2));
        arr.push_back(res[count - 1]);
        return arr;
    }

    static void Create_Metro_Map(Graph_M &g)
    {
        g.addVertex("Noida Sector 62~B");
        g.addVertex("Botanical Garden~B");
        g.addVertex("Yamuna Bank~B");
        g.addVertex("Rajiv Chowk~BY");
        g.addVertex("Vaishali~B");
        g.addVertex("Moti Nagar~B");
        g.addVertex("Janak Puri West~BO");
        g.addVertex("Dwarka Sector 21~B");
        g.addVertex("Huda City Center~Y");
        g.addVertex("Saket~Y");
        g.addVertex("Vishwavidyalaya~Y");
        g.addVertex("Chandni Chowk~Y");
        g.addVertex("New Delhi~YO");
        g.addVertex("AIIMS~Y");
        g.addVertex("Shivaji Stadium~O");
        g.addVertex("DDS Campus~O");
        g.addVertex("IGI Airport~O");
        g.addVertex("Rajouri Garden~BP");
        g.addVertex("Netaji Subhash Place~PR");
        g.addVertex("Punjabi Bagh West~P");

        g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
        g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
        g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
        g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
        g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
        g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
        g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
        g.addEdge("Huda City Center~Y", "Saket~Y", 15);
        g.addEdge("Saket~Y", "AIIMS~Y", 6);
        g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
        g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
        g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
        g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
        g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
        g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
        g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
        g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
    }
};

unordered_map<string, Graph_M::Vertex> Graph_M::vtces;

string *printCodelist()
{
    cout << "List of station along with their codes:" << endl;
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

    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****" << endl;

    while (true)
    {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "7. EXIT THE MENU" << endl;
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";
        int choice = -1;
        cin >> choice;
        cout << "\n***********************************************************\n";
        if (choice == 7)
        {
            break;
        }
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
            cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
            cout << "ENTER YOUR CHOICE:";
            int ch;
            cin >> ch;
            int j;

            string st1 = "", st2 = "";
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            if (ch == 1)
            {
                int a, b;
                cin >> a >> b;
                st1 = keys[a - 1];
                st2 = keys[b - 1];
            }
            else if (ch == 2)
            {
                string a, b;
                cin >> a >> b;
                for (j = 0; j < Graph_M::vtces.size(); j++)
                {
                    if (a == keys[j])
                        break;
                }
                st1 = keys[j];
                for (j = 0; j < Graph_M::vtces.size(); j++)
                {
                    if (b == keys[j])
                        break;
                }
                st2 = keys[j];
            }
            else if (ch == 3)
            {
                cin.ignore();
                getline(cin, st1);
                getline(cin, st2);
            }
            else
            {
                cout << "Invalid choice" << endl;
                return 0;
            }

            unordered_map<string, bool> processed;
            if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
                cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM" << endl;
            break;
        }

        case 4:
        {
            string sat1, sat2;
            cout << "ENTER THE SOURCE STATION: ";
            cin.ignore();
            getline(cin, sat1);
            cout << "ENTER THE DESTINATION STATION: ";
            getline(cin, sat2);

            unordered_map<string, bool> processed1;
            cout << "SHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES" << endl;
            break;
        }

        case 5:
        {
            string s1, s2;
            cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
            cin.ignore();
            getline(cin, s1);
            getline(cin, s2);

            unordered_map<string, bool> processed2;
            if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                cout << "THE INPUTS ARE INVALID" << endl;
            else
            {
                vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                int len = str.size();
                cout << "SOURCE STATION : " << s1 << endl;
                cout << "SOURCE STATION : " << s2 << endl;
                cout << "DISTANCE : " << str[len - 1] << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                cout << "~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << endl;
                for (int i = 1; i < len - 3; i++)
                {
                    cout << str[i] << endl;
                }
                cout << str[len - 3] << "   ==>    END";
                cout << "\n~~~~~~~~~~~~~" << endl;
            }
            break;
        }

        case 6:
        {
            string ss1, ss2;
            cout << "ENTER THE SOURCE STATION: ";
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
                cout << "SOURCE STATION : " << ss1 << endl;
                cout << "DESTINATION STATION : " << ss2 << endl;
                cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                cout << "~~~~~~~~~~~~~" << endl;
                cout << "START  ==>  " << str[0] << endl;
                for (int i = 1; i < len - 3; i++)
                {
                    cout << str[i] << endl;
                }
                cout << str[len - 3] << "   ==>    END";
                cout << "\n~~~~~~~~~~~~~" << endl;
            }
            break;
        }

        default:
            cout << "Invalid choice" << endl;
            break;
        }
    }

    return 0;
}



