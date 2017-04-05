#include "Master.h"





std::list<Master::ORDER> Master::Orders;
//Find order and delete it from the list
bool Master::FindOrder(Master::ORDER order)
{
	for (Master::ORDER o : Orders)
	{
		if (o == order)
		{
			Orders.remove(o);
			return true;
		}
	}

	return false;
}

Master::Master()
{
	Orders.push_front(Master::ORDER::EXPLORE);
}

void Master::Update()
{

}

Master::~Master()
{
}
