#include<iostream>

using namespace std;

struct AdjListNode;

struct AdjListHead
{
	string id;
	AdjListHead* nextHead;
	AdjListNode* next;
};

struct AdjListNode
{
	string id, angkotId;
	AdjListNode* next;
	AdjListHead* graphNode;
	double distance;
};

struct Edge
{
	string source, destination, angkotId;
	double distance;
};

struct Route
{
	string route;
	
	struct LList
	{
		struct Node
		{
			string id;
			Node* next;
		}*head, *tail;
		int n;
		LList()
		{
			head = tail = NULL;
			n = 0;
		}
		
		void addLast(string id)
		{
			if(head == NULL)
			{
				head = tail = new Node{id,NULL};
			}
			else
			{
				tail->next = new Node{id,NULL};
				tail = tail->next;
			}
			n++;
		}
		
		bool idExists(string id)
		{
			Node* p = head;
			if(p == NULL) return false;
			do
			{
				if(p->id == id) return true;
				p = p->next;
			}while(p!=NULL);
			return false;
		}
	}visited, angkoted;
	
	double distance;
	
	Route(string start = "")
	{
		route = start;
		distance = 0;
		if(start != "") visited.addLast(start);
	}
	
	Route(const Route& r)
	{
		route = r.route;
		distance = r.distance;
		LList::Node* p = r.visited.head;
		while(p!=NULL)
		{
			visited.addLast(p->id);
			p = p->next;
		}
		p = r.angkoted.head;
		while(p!=NULL)
		{
			angkoted.addLast(p->id);
			p = p->next;
		}
	}
	
	~Route()
	{
		LList::Node* p = visited.head;
		while(p!=NULL)
		{
			LList::Node* q = p;
			p = p->next;
			delete q;
		}
		p = angkoted.head;
		while(p!=NULL)
		{
			LList::Node* q = p;
			p = p->next;
			delete q;
		}
		visited.head = visited.tail = angkoted.head = angkoted.tail = NULL;
	}
};


struct Graph
{
	AdjListHead* start;
	
	Graph(){start = NULL;}
	
	void addEdge(string source, string destination, string angkot, double distance)
	{
		if(start == NULL)
		{
			AdjListHead* destHead = new AdjListHead{destination, NULL, NULL};
			start = new AdjListHead{source, destHead, NULL};
			
			start->next = new AdjListNode{destination, angkot, NULL, destHead, distance};
//			destHead->next = new AdjListNode{source, angkot, NULL, start, distance};
		}
		else
		{
			AdjListHead* srcHead = start;
			AdjListNode* destNode;
			while(srcHead->nextHead!=NULL)
			{
				if(srcHead->id == source) break;
				srcHead = srcHead->nextHead;
			}
			if(srcHead->id == source)
			{
				if(srcHead->next == NULL)
				{
					srcHead->next = new AdjListNode{destination, angkot, NULL, NULL, distance};
					destNode = srcHead->next;
				}
				else
				{
					destNode = srcHead->next;
					while(destNode->next!=NULL) destNode = destNode->next;
					destNode->next = new AdjListNode{destination, angkot, NULL, NULL, distance};
					destNode = destNode->next;
				}
			}
			else
			{
				srcHead->nextHead = new AdjListHead{source, NULL, NULL};
				srcHead = srcHead->nextHead;
				srcHead->next = new AdjListNode{destination, angkot, NULL, NULL, distance};
				destNode = srcHead->next;
			}
			
			AdjListHead* destHead = start;
//			AdjListNode* srcNode;
			while(destHead->nextHead!=NULL)
			{
				if(destHead->id == destination) break;
				destHead = destHead->nextHead;
			}
			if(destHead->id != destination)
			{
				destHead->nextHead = new AdjListHead{destination, NULL, NULL};
				destHead = destHead->nextHead;
//				srcNode = new AdjListNode{source, angkot, NULL, NULL, distance};
//				destHead->next = srcNode;
			}
//			srcNode->graphNode = srcHead;
			destNode->graphNode = destHead;
		}
	}
	
	void addEdge(Edge edge) {addEdge(edge.source, edge.destination, edge.angkotId, edge.distance);}
	
	void printRoute(string start, string end)
	{
		AdjListHead* p = this->start;
		while(p->nextHead != NULL)
		{
			if(p->id == start) break;
//			cout << (p->next == NULL) << '\n';
			p = p->nextHead;
		}
		if(p->id == start)
		{
			Route* routes[100];
			Route* route = new Route(start);
			int maxIndex = routeTraversalHead(p, end, route, routes);
			delete route;
			if(maxIndex == -1)
			{
				cout << "Tidak Ada Rute dari " << start << " ke " << end << '\n';
				return;
			}
//			cout << maxIndex;
			mergeSort(routes, 0, maxIndex);
//			cout << routes[0]->angkoted.n << ' ' << routes[1]->angkoted.n << '\n';
			cout << "Rute Rekomendasi:\n\n" << routes[0]->route << "\nJarak: " << routes[0]->distance << "km\n" << routes[0]->angkoted.n-1 << " Kali Pindah Angkot\n";
			
			cout << "\nTampilkan Semua Rute?(y/n) --> " << maxIndex+1 << " Rute\n";
			char pilihan{};
			while(pilihan != 'y' && pilihan != 'n')
			{
				cout << ">>";
				cin >> pilihan;
				cin.ignore();
			}
			cout << '\n';
			if(pilihan == 'y')
			{
				for(int i=0; i<=maxIndex; i++)
				{
					cout << i+1 << ".\t" <<  "Jarak = " << routes[i]->distance << "km\t" << routes[i]->angkoted.n-1 << " Kali Pindah Angkot" << '\t' << routes[i]->route << '\n';
				}
				cout << '\n';
			}
		}
		else cout << start << " Tidak Ditemukan\n";
	}
	
	int routeTraversalHead(AdjListHead* headNode, string end, Route* route, Route* routes[100])
	{
		static int index{-1}; //cout << index << ' ';
		if(route->visited.n == 1) index = -1;
		if(index >= 99) return index;
//		cout << headNode->id << '\t' << route->route << '\n';
		if(headNode->id == end)
		{
			routes[++index] = route;
			return index;
		}
		
		AdjListNode* p = headNode->next;
		while(p!=NULL)
		{
//			cout << p->angkotId << route->angkoted.tail->id;
			if(!route->visited.idExists(p->id) && ((route->angkoted.tail!=NULL && p->angkotId == route->angkoted.tail->id) || !route->angkoted.idExists(p->angkotId)))
			{
				Route* tmp = new Route(*route);
				routeTraversalNode(p, end, tmp, routes);
				delete tmp;
			}
			p = p->next;
		}
		return index;
	}
	
	void routeTraversalNode(AdjListNode* node, string end, Route* route, Route* routes[100])
	{
		route->visited.addLast(node->id);
		if(route->angkoted.tail==NULL || node->angkotId != route->angkoted.tail->id)route->angkoted.addLast(node->angkotId);
		route->route += " -" + node->angkotId + "-> " + node->id;
		route->distance += node->distance;
		Route* tmp = new Route(*route);
		routeTraversalHead(node->graphNode, end, tmp, routes);
//		delete tmp;
	}
	
	void merge(Route* routes[100], int l, int m, int r)
	{
		int n1{l};
		int n2{m+1};
		Route** tmp = new Route*[r-l+1];
		for(int i{0}; i<=r-l; i++)
		{
			if(n1 > m) tmp[i] = routes[n2++];
			else if(n2 > r) tmp[i] = routes[n1++];
			else
			{
//				cout << routes[n1]->angkoted.n << ' ' << routes[n2]->angkoted.n << '\n';
//				cout << n2;
//		cout << l << ' ' << m << ' ' << r << '\n';
				if(routes[n1]->distance < routes[n2]->distance || (routes[n1]->distance == routes[n2]->distance && routes[n1]->angkoted.n <= routes[n2]->angkoted.n))
				{
					tmp[i] = routes[n1++];
				}
				else 
				{
					tmp[i] = routes[n2++];
				}
			}
		}
		for(int i{0}; i<=r-l; i++)
		{
			routes[l+i] = tmp[i];
		}
	}

	void mergeSort(Route* routes[100], int l, int r)
	{
		if(l >= r) return;
		int m{(l+(r-1))/2};
		mergeSort(routes, l, m);
		mergeSort(routes, m+1, r);
		merge(routes, l, m, r);
//		cout << l << ' ' << r << '\n';
	}
	
	void printAdjList()
	{
		AdjListHead* p = start;
		while(p!=NULL)
		{
			cout << p->id;
			AdjListNode* q = p->next;
			while(q!=NULL)
			{
				cout << " -" << q->angkotId << '-' << q->distance << "-> " << q->id;
				q = q->next;
			}
			cout << '\n';
			p = p->nextHead;
		}
		cout << '\n';
	}
	
	void printAllGraphNode()
	{
		AdjListHead* p = start;
		while(p!=NULL)
		{
			cout << p->id << '\n';
			p = p->nextHead;
		}
		cout << '\n';
	}
};

bool menu(Graph& g)
{
	cout << "\nMENU\n1. Rute\n2. Daftar Nama Jalanan\n3. Info\n4. Keluar\n";
	char pilihan{};
	while(pilihan != '1' && pilihan != '2' && pilihan != '3' && pilihan != '4')
	{
		cout << ">>";
		cin >> pilihan; cin.ignore();
	}
	
	switch(pilihan)
	{
		case '1':
		{
			cout << "\nDari Mana Nih?\n>>";
			string asal; getline(cin, asal);
			
			cout << "\nMau Ke Mana Nih?\n>>";
			string tujuan; getline(cin, tujuan);
			cout << '\n';
			
			g.printRoute(asal, tujuan);
		} break;
		case '2': 
		{
			cout << "\nDAFTAR NAMA JALANAN\n\n";
			g.printAllGraphNode();
			cout << "Mohon mengisikan nama jalanan sama persis dengan yang disediakan di sini\n";
		} break;
		case '3':
		{
			cout << "\nINFO\n\n";
			cout << "Format Rute:\n";
			cout << "[Nama Jalanan] -[Kode Angkot]-> [Nama Jalanan]\n\n";
			cout << "PERHATIAN\n";
			cout << "1. Tidak semua jalur angkot Malang ada di program ini\n";
			cout << "2. Tidak semua jalanan yang dilalui setiap jalur angkot ada di program ini\n";
			cout << "3. Rute tidak akan melewati satu jalanan lebih dari satu kali\n";
			cout << "4. Rute tidak akan mengambil kode angkot yang sama jika pernah mengambilnya sebelumnya\n";
			cout << "5. Akan dicari maksimal 100 rute\n";
			cout << "6. Jarak tempuh yang diberikan rute bisa jadi tidak akurat\n";
			cout << "7. Nama jalanan yang diisi harus sama persis dengan yang disediakan di bagian Daftar Nama Jalanan\n\n";
			cout << "SUMBER\n";
			cout << "https://dishub.malangkota.go.id/transportasi/jalur-angkutan-kota-malang-2/\n";
			cout << "maps.google.co.id\n";
		} break;
		case '4': 
		{
			cout << "\nKELUAR";
			return false;
		} break;
	}
	return true;
}

int main()
{
	Graph malang;
	Edge edges[]{
		{"Terminal Arjosari", "Jl. W.R. Supratman", "AL", 7.9}, {"Jl. W.R. Supratman", "Jl. Patimura", "AL", 1.1}, {"Jl. Patimura", "Jl. Trunojoyo", "AL", 0.55}, {"Jl. Trunojoyo", "Jl. Tugu", "AL", 0.5}, {"Jl. Tugu", "Jl. Semeru", "AL", 1}, {"Jl. Semeru", "Jl. Ijen", "AL", 0.8}, {"Jl. Ijen", "Jl. Veteran", "AL", 3.2}, {"Jl. Veteran", "Terminal Landungsari", "AL", 5.3},
		{"Terminal Landungsari", "Jl. Veteran", "AL", 5.1}, {"Jl. Veteran", "Jl. Ijen", "AL", 1.9}, {"Jl. Ijen", "Jl. Semeru", "AL", 1.2}, {"Jl. Semeru", "Jl. Tugu", "AL", 1}, {"Jl. Tugu", "Jl. Trunojoyo", "AL", 450}, {"Jl. Trunojoyo", "Jl. Patimura", "AL", 0.55}, {"Jl. Patimura", "Jl. W.R. Supratman", "AL", 1.1}, {"Jl. W.R. Supratman", "Terminal Arjosari", "AL", 6.1},
		{"Terminal Arjosari", "Jl. W.R. Supratman", "ADL", 6.1}, {"Jl. W.R. Supratman", "Jl. Tugu", "ADL", 2.7}, {"Jl. Tugu", "Jl. Semeru", "ADL", 1}, {"Jl. Semeru", "Jl. Ijen", "ADL", 0.8}, {"Jl. Ijen", "Jl. Mayjend Panjaitan", "ADL", 3.2}, {"Jl. Mayjend Panjaitan", "Terminal Landungsari", "ADL", 4.8},
		{"Terminal Landungsari", "Jl. Mayjend Panjaitan", "ADL", 4.8}, {"Jl. Mayjend Panjaitan", "Jl. Ijen", "ADL", 1.9}, {"Jl. Ijen", "Jl. Semeru", "ADL", 1.2}, {"Jl. Semeru", "Jl. Tugu", "ADL", 1}, {"Jl. Tugu", "Jl. Trunojoyo", "ADL", 0.45}, {"Jl. Trunojoyo", "Jl. Dr. Cipto", "ADL", 1}, {"Jl. Dr. Cipto", "Jl. W.R. Supratman", "ADL", 0.6}, {"Jl. W.R. Supratman", "Terminal Arjosari", "ADL", 6.1},
		{"Terminal Landungsari", "Jl. Veteran", "GL/HL", 5.1}, {"Jl. Veteran", "Jl. Ijen", "GL/HL", 1.9}, {"Jl. Ijen", "Jl. Merdeka Utara", "GL/HL", 2},
		{"Jl. Merdeka Utara", "Jl. Tugu", "GA/HA", 0.85}, {"Jl. Tugu", "Jl. Pajajaran", "GA/HA", 0.45}, {"Jl. Pajajaran", "Jl. Trunojoyo", "GA/HA", 0.14}, {"Jl. Trunojoyo", "Jl. Dr. Cipto", "GA/HA", 0.85}, {"Jl. Dr. Cipto", "Jl. W.R. Supratman", "GA/HA", 0.6}, {"Jl. W.R. Supratman", "Terminal Arjosari", "GA/HA", 6.1},
		{"Terminal Arjosari", "Jl. Merdeka Utara", "AG/AH", 8.1}, {"Jl. Merdeka Utara", "Jl. Kol. Sugiono", "AG/AH", 2}, {"Jl. Kol. Sugiono", "Terminal Gadang", "AG/AH", 3.2},
		{"Terminal Gadang", "Jl. Kol. Sugiono", "LDG/LDH", 3.2}, {"Jl. Kol. Sugiono", "Jl. Kawi", "LDG/LDH", 3.1}, {"Jl. Kawi", "Jl. Semeru", "LDG/LDH", 0.45}, {"Jl. Semeru", "Jl. Ijen", "LDG/LDH", 1.3}, {"Jl. Ijen", "Jl. Veteran", "LDG/LDH", 1.6}, {"Jl. Veteran", "Jl. Mayjend Panjaitan", "LDG/LDH", 1.1}, {"Jl. Mayjend Panjaitan", "Terminal Landungsari", "LDG/LDH", 4.8}
	};
	
	for(unsigned i=0; i < (sizeof(edges)/sizeof(edges[0])); i++) malang.addEdge(edges[i]);
	
	// malang.printAdjList();
	
	cout << "---------------------------------------------ASAM---------------------------------------\n";
	cout << "------------------------------------Asisten Angkot Malang-------------------------------\n";
	cout << "\nCukup isikan nama jalanan asal dan tujuan, akan kami berikan cara ke sana melalui angkot\n";
	
	while(menu(malang));
	
	return 0;
}