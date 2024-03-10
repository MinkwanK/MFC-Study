#include <iostream>
using namespace std;

class Node
{
	int data;
	Node* next;

public:
	Node(int _data)
	{
		data = _data;
		next = nullptr;
	}

	void setNext(Node* _next)
	{
		next = _next;
	}

	void setData(int _data)
	{
		data = _data;
	}

	int getData()
	{
		return data;
	}

	void showNodeInfo()
	{
		cout << "Node Data: " << data << endl;
	}

	Node* getNext()
	{
		return next;
	}
};


class SinglyLinkedList
{
	Node* head;
	Node* now;
public:

	SinglyLinkedList()
	{
		head = nullptr;
		now = nullptr;
	}
	SinglyLinkedList(Node* _head)
	{
		head = _head;
		now = _head;
	}

	void Insert(Node* _newNode)
	{
		
		if (head == nullptr)
		{
			head = _newNode;
			now = _newNode;
		}
		else
		{
			now->setNext(_newNode);
			now = _newNode;
		}
		cout << "��� ���� �Ϸ�!" << endl;
	
	}

	void Insert(int _data)
	{

		Node* _newNode = new Node(_data);
		if (head == nullptr)
		{
			head =_newNode;
			now = _newNode;
		}
		else
		{
			now->setNext(_newNode);
			now = _newNode;
		}
		cout << "��� ���� �Ϸ�! " << _newNode->getData() <<  endl;

	}

	//������ ����� �����͸� ����
	void Modify(int _data)
	{
		if (head == nullptr)
		{
			cout << "����Ʈ�� ��尡 �����ϴ�" << endl;
			return;
		}

		if (now != nullptr)
		{
			now->setData(_data);
			cout << "������ ����� ������: " << now->getData() << endl;
		}
	}

	void FInd(int _data)
	{
		if (head == nullptr)
		{
			cout << "����Ʈ�� ��尡 �����ϴ�" << endl;
			return;
		}
		Node* start = head;

		while (true)
		{
			cout << start->getData() << endl;
			if (start->getData() == _data)
			{
				cout << "ã�� �����Ͱ� �����մϴ�. " << start->getData()<< endl;
				return;
			}

			start = start->getNext();
			if (start == nullptr)
			{
				cout << "ã�� �����Ͱ� �����ϴ�." << start->getData()  << endl;
				return;
			}
		}
	}

	void Delete(int _data)
	{
		if (head == nullptr)
		{
			cout << "����Ʈ�� ��尡 �����ϴ�" << endl;
			return;
		}

		Node* start = head;

		while (true)
		{
			if (start->getData() == _data)
			{	
			
				//Beginning
				if (start == head)
				{
					Node* temp = head;
					head = head->getNext();
					start = head;
					delete(temp);
					cout << "���� �Ϸ� beginning" << endl;
				}	
				else
				{
					//end
					if (start->getNext() == nullptr)
					{
						start = nullptr;
						cout << "���� �Ϸ� End" << endl;
						return;
					}
					//Middle
					else
					{
						Node* prev = head;
						Node* next = start->getNext();

						while (true)
						{
							if (prev->getNext()->getData() == _data)
							{
								Node* temp = prev->getNext();
								prev->setNext(next);
								delete(temp);
								start = next;
								cout << "���� �Ϸ�" << endl;
								break;
							}
							prev = prev->getNext();
						}
					}
					
				}

			}

			if (start->getNext() == nullptr) 
			{
				cout << "delete ����" << endl;
				break;
			}
			start = start->getNext();
			
		}
	}

	void showListInfo()
	{
		if (head == nullptr)
		{
			cout << "�� ����Ʈ�Դϴ�" << endl;
		}
		Node* start = head;
		while (start != nullptr)
		{
			start->showNodeInfo();
			start = start->getNext();
		}
	}
};



int main()
{
	
	SinglyLinkedList* list = new SinglyLinkedList();
	//Node* node = new Node(1);
	//Node* node2 = new Node(2);

	for (int i = 1; i <= 10; i++)
	{
		list->Insert(i);
	}

	//list->showListInfo();
	//list->Modify(1);
	//list->FInd(9);

	list->Insert(4);
	list->Insert(4);
	list->Insert(4);
	list->Insert(4);
	list->Insert(4);
	list->Insert(15);
	list->Delete(4);

	list->showListInfo();

	

	return 0;
}


