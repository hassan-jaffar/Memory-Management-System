#include <iostream>
using namespace std;

template<class type>
class List {
private:
	struct Node {
	public:
		type data;
		Node* next;
		Node(type val, Node* nptr = 0)
		{
			data = val;
			next = nptr;
		}
	};
	Node* head;
	Node* tail;

	class ListIterator {
	private:
		friend class List;		//	imp
		Node* iptr;
	public:
		ListIterator(Node* ptr = NULL)
		{
			iptr = ptr;
		};
		//operator overloading for iterators
		ListIterator& operator++(int)		//post-increment
		{
			ListIterator old = *this;
			++(*this);
			return old;
		}
		ListIterator& operator++()		//pre-increment
		{
			if (iptr) iptr = iptr->next;
			return (*this);
		}
		bool operator==(const ListIterator& l)
		{
			return (iptr == l.iptr);
		}
		type& operator*()
		{
			return iptr->data;
		}
		bool operator!=(const ListIterator& l)
		{
			return (iptr != l.iptr);
		}
	};
public:
	typedef ListIterator Iterator;
	Iterator begin()
	{
		Iterator I(head->next);
		return I;
	}
	Iterator end()
	{
		Iterator I(tail);
		return I;
	}
	List()					//singly link list with dummy tail
	{
		head = new Node(0);
		tail = new Node(0);
		head->next = tail;
	}
	//~List()
	//{
	//	while (!isEmpty())
	//		deleteAtStart();
	//}

	void print() {
		if (!isEmpty())			//prints null if list empty
		{
			for (Node* temp = head->next; temp != tail; temp = temp->next)
			{
				if (temp->next == tail)			//does not print arrow head at tail
					cout << temp->data << endl << endl;
				else
					cout << temp->data << " -> ";
			}
		}
		else
			cout << "null\n\n";
	}

	void insertAtStart(type& val)
	{
		head->next = new Node(val, head->next);
	}

	void insertAtEnd(type& val)
	{
		Node* temp = head;
		while (temp->next != tail)
			temp = temp->next;
		temp->next = new Node(val, temp->next);
	}
	void insert(Iterator ite)		//inserts a node at the next of the given iterator
	{
		ite.iptr->next = new Node(0, ite.iptr->next);
	}
	void deleteThis(Iterator& ite)			//deletes the node pointed by the given iterator
	{
		Node* temp = head;
		while (temp->next != ite.iptr)		//loops from head till the previous of the node to be deleted
			temp = temp->next;

		Node* temp1 = temp->next;
		temp->next = temp->next->next;
		delete temp1;
	}
	void deleteAtStart()
	{
		if (!isEmpty())
		{
			Node* temp = head->next;
			head->next = head->next->next;
			//cout << "deleting " << temp->data << endl;
			delete temp;
		}
	}
	void deleteAtEnd()
	{
		if (!isEmpty())
		{
			Node* temp = head->next;
			while (temp->next->next != tail)
				temp = temp->next;
			Node* temp1 = temp->next;
			temp->next = temp->next->next;
			//cout << "deleting " << temp->data << endl;
			delete temp1;
		}
	}
	bool isEmpty()		//validation checks
	{
		if (head->next == tail)
			return true;
		else
			return false;
	}
};

class Block {
private:
	int startByteID;
	int totalBytes;
public:
	Block(int start = 0, int total = 0)			//overloaded constructor
	{
		startByteID = start;
		totalBytes = total;
	}
	//getters and setters
	int getStart()
	{
		return startByteID;
	}
	int getTotal()
	{
		return totalBytes;
	}
	void setStart(int start)
	{
		startByteID = start;
	}
	void setTotal(int total)
	{
		totalBytes = total;
	}
	//seperate print function for blocks
	void print()
	{
		cout << "Block: \tStartByteID: " << startByteID << "\tTotalBytes: " << totalBytes << endl;
	}
};

class Program {
private:
	char* ID;
	int size;
	List<Block> blocks;
public:
	Program(char* id = 0, int s = 0)			//overloaded constructor
	{
		ID = id;
		size = s;
	}
	void print()
	{
		cout << "Program: \tID: " << ID << "\tSize: " << size << endl;
		List<Block>::Iterator ite;
		for (ite = blocks.begin(); ite != blocks.end(); ite++)
		{
			cout << "\t";
			(*ite).print();
		}
		cout << endl;
	}
	//setters and getters
	void setSize(int s)
	{
		size = s;
	}
	int getSize()
	{
		return size;
	}
	List<Block>& returnBlocks()
	{
		return blocks;
	}
	const char* getID()
	{
		return ID;
	}
	void addBlockToProgram(Block& temp)			//inserts the new block at the end of the list of progs
	{
		blocks.insertAtEnd(temp);
	}
	bool operator==(const char* rhs)		//operator overloading to check if the prog matches the search ID
	{
		int length = strlen(ID);
		if (length == strlen(rhs))			//equality only possible if lengths are equal
		{
			for (int i = 0; i < length; i++)
			{
				if (ID[i] != rhs[i])		//exits at the very first mismatch
					return false;
			}
			return true;
		}
		return false;
	}
};

class MemoryManagementSystem {
private:
	List<Block> pool;
	List<Program> progs;
	int sizeOfMemory;
	bool strategy;
public:
	MemoryManagementSystem(int size, bool strategy_)
	{
		sizeOfMemory = size * 1000;
		strategy = strategy_;
		Block temp(0, sizeOfMemory);
		pool.insertAtStart(temp);		//whole memory inserted as block in pool
	}
	void addBlockToPool(int start, int total)			//adds block returned from progs to pool
	{
		List<Block>::Iterator ite;
		for (ite = pool.begin(); ite != pool.end(); ite++)
		{
			if ((start + total) < (*ite).getStart())		//checks for the position to keep sorted
			{
				pool.insert(ite);
				ite++;
				(*ite).setStart(start);
				(*ite).setTotal(total);
			}
			else if ((start + total) == (*ite).getStart())		//checks if merging is possible
			{
				//set the block with startbyte and add both total bytes and delete the second one
				(*ite).setStart(start);
				(*ite).setTotal((*ite).getTotal() + total);
				pool.deleteThis(ite);
			}
		}
	}
	bool getMem(char* id, int s)
	{
		int start = 0;
		bool available = false;
		if (strategy == true)				//conditions to get memory from pool in case of first fit strategy
		{
			List<Block>::Iterator it;
			for (it = pool.begin(); it != pool.end(); it++)
			{
				if ((*it).getTotal() >= s)
				{
					(*it).setTotal((*it).getTotal() - s);
					start = (*it).getStart();
					(*it).setStart((*it).getStart() + s);
					available = true;
					break;
				}
			}
		}
		else
		{										//conditions to get memory from pool in case of first fit strategy
			List<Block>::Iterator it;
			List<Block>::Iterator it2;
			int difference = sizeOfMemory;
			bool set = false;
			for (it = pool.begin(); it != pool.end(); it++)
			{
				if ((*it).getTotal() >= s)
				{
					if (((*it).getTotal() - s) <= difference)
					{
						difference = (*it).getTotal() - s;
						it2 = it;
						set = true;
					}
				}
			}
			if (set == true)					//get mem if memory is available in pool
			{
				(*it2).setTotal((*it2).getTotal() - s);
				start = (*it2).getStart();
				(*it2).setStart((*it2).getStart() + s);
				available = true;
			}
		}
		if (available == true)				//if pool gives memory, assign it  to progs
		{
			List<Program>::Iterator ite;
			Block bl(start, s);
			for (ite = progs.begin(); ite != progs.end(); ite++)
			{
				if ((*ite) == id)					//in case prog ID already exists
				{
					(*ite).addBlockToProgram(bl);
					(*ite).setSize((*ite).getSize() + s);
					return true;
				}
			}
			Program temp(id, s);				//in case new program is to be made
			progs.insertAtEnd(temp);
			temp.addBlockToProgram(bl);
			//(temp.returnBlocks()).insertAtEnd(bl);
			return true;
		}

		return false;
	}
	bool deleteProgram(const char* id)
	{
		List<Program>::Iterator ite;
		for (ite = progs.begin(); ite != progs.end(); ite++)
		{
			if ((*ite) == id)
			{
				List<Block>::Iterator it;
				for (it = (*ite).returnBlocks().begin(); it != (*ite).returnBlocks().end(); it++)	//returns all the blocks
				{																			//given to progs back to pool
					addBlockToPool((*it).getStart(), (*it).getTotal());
				}
				List<Program>::Iterator lit;
				lit = progs.begin();
				while (lit != ite)
					lit++;
				progs.deleteThis(lit);			//delete the program
				return true;
			}
		}
		return false;
	}
	void printMemoryPool()
	{
		if (!pool.isEmpty())				//checks if empty
		{
			List<Block>::Iterator ite;
			for (ite = pool.begin(); ite != pool.end(); ite++)
				(*ite).print();
		}
		else
			cout << "null\n";
	}
	void printProgs()
	{
		if (!progs.isEmpty())			//checks if empty
		{
			List<Program>::Iterator ite;
			for (ite = progs.begin(); ite != progs.end(); ite++)
				(*ite).print();
		}
		else
			cout << "null\n";
	}
};

int main()
{
	bool strategy = false;
	bool run = true;
	int RAM = 0;
	int input = 0;

	cout << "Enter True (1) for First Come Strategy && False (0) for Best Fit Strategy" << endl;
	cin >> strategy;
	cout << endl;

	cout << "Enter RAM size" << endl;
	cin >> RAM;

	MemoryManagementSystem object(RAM, strategy);
	object.addBlockToPool(0, RAM);

	while (run == true)
	{
		cout << endl;
		cout << "Press 1 to enter a program" << endl;
		cout << "Press 2 to delete a program" << endl;
		cout << "Press 3 to view the Pool (Memory of free blocks)" << endl;
		cout << "Press 4 to view the Programs in Memory" << endl;
		cout << "Press 5 to exit the program" << endl;
		cout << endl;

		cin >> input;

		if (input == 1)
		{
			char* programId = new char[8];
			//int programId = 0;
			int memoryRequired = 0;
			cout << "Enter Program ID" << endl;
			cin >> programId;
			cout << "Enter memory required for the program" << endl;
			cin >> memoryRequired;
			if (object.getMem(programId, memoryRequired))
			{
				cout << "Memory succesfully allocated" << endl;
			}
			else
			{
				cout << "Memory not available" << endl;
			}
		}
		else if (input == 2)
		{
			char* programId = new char[8];
			//int programId = 0;
			cout << "Enter Program ID which you wish to delete for eg P1" << endl;
			cin >> programId;
			if (object.deleteProgram(programId))
			{
				cout << "Memory succesfully deallocated" << endl;
			}
			else
			{
				cout << "Memory can't be deallocated. Program does not exist" << endl;
			}
		}
		else if (input == 3)
		{
			object.printMemoryPool();
		}
		else if (input == 4)
		{
			object.printProgs();
		}
		else if (input == 5)
		{
			run = false;
			cout << "Program exited." << endl;
		}
		else
		{
			cout << "Invalid Input. Enter again" << endl;
		}
	}

	return 0;
}